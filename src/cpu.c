#include "cpu.h"

#include "bus.h"
#include "decoder/decoder.h"
#include "disassembler.h"
#include "print_format.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

const char *register_8_to_string(enum register_8 reg) {
  static const char *mapping[] = {
      "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh",
  };

  assert(reg < REGISTER_8_COUNT);
  return mapping[reg];
}

const char *register_16_to_string(enum register_16 reg) {
  static const char *mapping[] = {
      "ax", "bx", "cx", "dx", "sp", "bp", "si", "di", "ip", "flags",
  };

  assert(reg < REGISTER_16_COUNT);
  return mapping[reg];
}

const char *segment_register_to_string(enum segment_register reg) {
  static const char *mapping[] = {
      "es",
      "cs",
      "ss",
      "ds",
  };

  assert(reg < SEGMENT_REGISTER_COUNT);
  return mapping[reg];
}

#define REG_HI(x) ((x) >> 8)
#define REG_LO(x) ((x)&0xff)

static void print_registers(struct cpu *cpu) {
  printf("ax: " HEX_16 ", bx: " HEX_16 ", cx: " HEX_16 ", dx: " HEX_16 ", ip: " HEX_16 "\n",
         cpu->reg_16[AX], cpu->reg_16[BX], cpu->reg_16[CX], cpu->reg_16[DX], cpu->ip);
  printf("si: " HEX_16 ", di: " HEX_16 ", bp: " HEX_16 ", sp: " HEX_16 ", fl: " HEX_16 "\n",
         cpu->reg_16[SI], cpu->reg_16[DI], cpu->reg_16[BP], cpu->reg_16[SP], cpu->flags);
}

static void set_flags_sign_zero_8(struct cpu *cpu, u8 value) {
  if (!value) {
    cpu_set_flag(cpu, FLAG_ZF);
  } else {
    cpu_clear_flag(cpu, FLAG_ZF);
  }

  if (value & 0x80) {
    cpu_set_flag(cpu, FLAG_SF);
  } else {
    cpu_clear_flag(cpu, FLAG_SF);
  }
}

static void set_flags_subtract_8(struct cpu *cpu, u8 destination, u8 source) {
  u16 val = (u16)destination - (u16)source;

  set_flags_sign_zero_8(cpu, val);

  if (val & 0xff00) {
    cpu_set_flag(cpu, FLAG_CF);
  } else {
    cpu_clear_flag(cpu, FLAG_CF);
  }

  if ((val ^ destination) & (destination ^ source) & 0x80) {
    cpu_set_flag(cpu, FLAG_OF);
  } else {
    cpu_clear_flag(cpu, FLAG_OF);
  }

  if ((destination ^ source ^ val) & 0x10) {
    cpu_set_flag(cpu, FLAG_AF);
  } else {
    cpu_clear_flag(cpu, FLAG_AF);
  }
}

struct address get_operand_indirect_address(struct operand *operand, struct cpu *cpu) {
  struct address result;

  switch (operand->as_indirect.encoding) {
    case IE_BX_SI:
      result = segment_offset(cpu_get_register_16(cpu, BX), cpu_get_register_16(cpu, SI));
      break;

    case IE_BX_DI:
      result = segment_offset(cpu_get_register_16(cpu, BX), cpu_get_register_16(cpu, DI));
      break;

    case IE_BP_SI:
      result = segment_offset(cpu_get_register_16(cpu, BP), cpu_get_register_16(cpu, SI));
      break;

    case IE_BP_DI:
      result = segment_offset(cpu_get_register_16(cpu, BP), cpu_get_register_16(cpu, DI));
      break;

    case IE_SI:
      result = segment_offset(cpu_get_segment(cpu, DS), cpu_get_register_16(cpu, SI));
      break;

    case IE_DI:
      result = segment_offset(cpu_get_segment(cpu, DS), cpu_get_register_16(cpu, DI));
      break;

    case IE_BP:
      result = segment_offset(cpu_get_segment(cpu, DS), cpu_get_register_16(cpu, BP));
      break;

    case IE_BX:
      result = segment_offset(cpu_get_segment(cpu, DS), cpu_get_register_16(cpu, BX));
      break;

    default:
      assert(0);
  }

  return result;
}

static u16 get_operand_indirect_value_16(struct operand *operand, struct cpu *cpu) {
  struct address address = get_operand_indirect_address(operand, cpu);

  return bus_fetch(cpu->bus, address) + (bus_fetch(cpu->bus, address_offset(address, 1)) << 8);
}

static u8 get_operand_indirect_value_8(struct operand *operand, struct cpu *cpu) {
  struct address address = get_operand_indirect_address(operand, cpu);

  return bus_fetch(cpu->bus, address);
}

static u8 get_operand_register_value_8(struct operand *operand, struct cpu *cpu) {
  return cpu->reg_8[operand->as_register.reg_8];
}

static u16 get_operand_register_value_16(struct operand *operand, struct cpu *cpu) {
  return cpu->reg_16[operand->as_register.reg_16];
}

static u8 get_operand_value_8(struct operand *operand, struct cpu *cpu) {
  switch (operand->type) {
    case OT_INDIRECT:
      return get_operand_indirect_value_8(operand, cpu);

    case OT_DISPLACEMENT:
      assert(0);
      break;

    case OT_REGISTER:
      return get_operand_register_value_8(operand, cpu);

    case OT_DIRECT:
      return bus_fetch(cpu->bus, segment_offset(0, operand->as_direct.address));

    case OT_IMMEDIATE:
      return operand->as_immediate.immediate_8;

    case OT_NONE:
    default:
      assert(0);
  }

  return 0;
}

static u16 get_operand_value_16(struct operand *operand, struct cpu *cpu) {
  switch (operand->type) {
    case OT_INDIRECT:
      return get_operand_indirect_value_16(operand, cpu);

    case OT_DISPLACEMENT:
      assert(0);
      break;

    case OT_REGISTER:
      return get_operand_register_value_16(operand, cpu);

    case OT_DIRECT:
      return bus_fetch(cpu->bus, segment_offset(0, operand->as_direct.address)) +
             (bus_fetch(cpu->bus, segment_offset(0, operand->as_direct.address + 1)) << 8);

    case OT_IMMEDIATE:
      return operand->as_immediate.immediate_16;

    default:
      assert(0);
  }

  return 0;
}

static void set_operand_indirect_value_8(struct operand *operand, struct cpu *cpu, u8 value) {
  struct address address = get_operand_indirect_address(operand, cpu);

  bus_store(cpu->bus, address, value);
}

static void set_operand_indirect_value_16(struct operand *operand, struct cpu *cpu, u16 value) {
  struct address address = get_operand_indirect_address(operand, cpu);

  bus_store(cpu->bus, address, value >> 8);
  bus_store(cpu->bus, address_offset(address, 1), value & 0xff);
}

static void set_operand_register_value_8(struct operand *operand, struct cpu *cpu, u8 value) {
  cpu->reg_8[operand->as_register.reg_8] = value;
}

static void set_operand_register_value_16(struct operand *operand, struct cpu *cpu, u16 value) {
  cpu->reg_16[operand->as_register.reg_16] = value;
}

static void set_operand_value_8(struct operand *operand, struct cpu *cpu, u8 value) {
  switch (operand->type) {
    case OT_INDIRECT:
      set_operand_indirect_value_8(operand, cpu, value);
      break;

    case OT_DISPLACEMENT:
      assert(0);
      break;

    case OT_REGISTER:
      set_operand_register_value_8(operand, cpu, value);
      break;

    case OT_DIRECT:
      bus_store(cpu->bus, segment_offset(0, operand->as_direct.address), value);
      break;

    case OT_IMMEDIATE:
    case OT_NONE:
    default:
      assert(0);
      break;
  }
}

static void set_operand_value_16(struct operand *operand, struct cpu *cpu, u16 value) {
  switch (operand->type) {
    case OT_INDIRECT:
      set_operand_indirect_value_16(operand, cpu, value);
      break;

    case OT_DISPLACEMENT:
      assert(0);
      break;

    case OT_REGISTER:
      set_operand_register_value_16(operand, cpu, value);
      break;

    case OT_DIRECT:
      bus_store(cpu->bus, segment_offset(0, operand->as_direct.address), value >> 8);
      bus_store(cpu->bus, segment_offset(0, operand->as_direct.address + 1), value & 0xff);
      break;

    case OT_IMMEDIATE:
    case OT_NONE:
    default:
      assert(0);
      break;
  }
}

static void interpret_add(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == ADD);

  u8 value8;
  u16 value16;

  switch (instruction->destination.size) {
    case OS_8:
      value8 = get_operand_value_8(&instruction->destination, cpu);
      break;

    case OS_16:
      value16 = get_operand_value_16(&instruction->destination, cpu);
      break;

    default:
      assert(0);
  }

  switch (instruction->source.size) {
    case OS_8:
      value8 += get_operand_value_8(&instruction->source, cpu);
      break;

    case OS_16:
      value16 += get_operand_value_16(&instruction->source, cpu);
      break;

    default:
      assert(0);
  }

  switch (instruction->destination.size) {
    case OS_8:
      set_operand_value_8(&instruction->destination, cpu, value8);
      break;

    case OS_16:
      set_operand_value_16(&instruction->destination, cpu, value16);
      break;

    default:
      assert(0);
  }
}

static void interpret_inc(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == INC);

  switch (instruction->destination.size) {
    case OS_8: {
      switch (instruction->destination.type) {
        case OT_REGISTER: {
          u8 value = get_operand_value_8(&instruction->destination, cpu);
          set_operand_value_8(&instruction->destination, cpu, value + 1);
          break;
        }

        default:
          assert(0);
      }
      break;
    }

    case OS_16: {
      switch (instruction->destination.type) {
        case OT_REGISTER: {
          u16 value = get_operand_value_16(&instruction->destination, cpu);
          set_operand_value_16(&instruction->destination, cpu, value + 1);
          break;
        }

        default:
          assert(0);
      }
      break;
    }
  }
}

static void interpret_mov(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == MOV);

  u8 value8;
  u16 value16;

  switch (instruction->source.size) {
    case OS_8:
      value8 = get_operand_value_8(&instruction->source, cpu);
      break;

    case OS_16:
      value16 = get_operand_value_16(&instruction->source, cpu);
      break;

    default:
      assert(0);
  }

  switch (instruction->destination.size) {
    case OS_8:
      set_operand_value_8(&instruction->destination, cpu, value8);
      break;

    case OS_16:
      set_operand_value_16(&instruction->destination, cpu, value16);
      break;

    default:
      assert(0);
  }
}

static void interpret_test(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == TEST);

  cpu_clear_flag(cpu, FLAG_CF);
  cpu_clear_flag(cpu, FLAG_OF);

  switch (instruction->destination.size) {
    case OS_8: {
      u8 destination = get_operand_value_8(&instruction->destination, cpu);
      u8 source = get_operand_value_8(&instruction->source, cpu);
      u8 r = destination & source;

      set_flag_value(cpu, FLAG_SF, r & 0xff);
      set_flag_value(cpu, FLAG_ZF, !r);

      // TODO: More flags are affected by TEST
      break;
    }

    case OS_16: {
      u16 destination = get_operand_value_16(&instruction->destination, cpu);
      u16 source = get_operand_value_16(&instruction->source, cpu);
      u16 r = destination & source;

      set_flag_value(cpu, FLAG_SF, r & 0xffff);
      set_flag_value(cpu, FLAG_ZF, !r);
      break;
    }

    default:
      assert(0);
  }
}

static void interpret_loop(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == LOOP);

  switch (instruction->destination.size) {
    case OS_8: {
      u8 cl = cpu_get_register_8(cpu, CL) - 1;
      cpu_set_register_8(cpu, CL, cl);
      if (cl) {
        cpu->ip += instruction->destination.as_jump.offset;
      }
      break;
    }

    case OS_16: {
      u8 cx = cpu_get_register_16(cpu, CX);
      if (cx) {
        cpu_set_register_16(cpu, CX, cx - 1);
        cpu->ip += instruction->destination.as_jump.offset;
      }
      break;
    }

    default:
      assert(0);
  }
}

static void interpret_push(struct cpu *cpu, struct instruction *instruction) {
  (void)cpu;

  assert(instruction->type == PUSH);
}

static void interpret_pop(struct cpu *cpu, struct instruction *instruction) {
  (void)cpu;

  assert(instruction->type == POP);
}

static void interpret_jmp(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == JMP);

  switch (instruction->destination.type) {
    case OT_DIRECT_WITH_SEGMENT: {
      struct address address = instruction->destination.as_direct_with_segment.address;
      cpu->segments[CS] = address.segment;
      cpu->ip = address.offset;
      break;
    }

    default:
      assert(0);
  }
}

static void interpret_instruction(struct cpu *cpu, struct instruction *instruction) {
  switch (instruction->type) {
    case ADD:
      interpret_add(cpu, instruction);
      break;

    case HLT:
      // Handle this somewhere else.
      assert(0);
      break;

    case INC:
      interpret_inc(cpu, instruction);
      break;

    case LOOP:
      interpret_loop(cpu, instruction);
      break;

    case MOV:
      interpret_mov(cpu, instruction);
      break;

    case TEST:
      interpret_test(cpu, instruction);
      break;

    case CMP: {
      if (instruction->destination.size == OS_8) {
        u8 destination = get_operand_value_8(&instruction->destination, cpu);
        u8 source = get_operand_value_8(&instruction->source, cpu);
        set_flags_subtract_8(cpu, destination, source);
      }
      break;
    }

    case PUSH:
      interpret_push(cpu, instruction);
      break;

    case POP:
      interpret_pop(cpu, instruction);
      break;

    case JMP:
      interpret_jmp(cpu, instruction);
      break;

    case JB: {
      assert(instruction->destination.type == OT_JUMP);
      if (cpu_flag_is_set(cpu, FLAG_CF)) {
        cpu->ip += instruction->destination.as_jump.offset;
      }
      break;
    }

    case JNB: {
      assert(instruction->destination.type == OT_JUMP);
      if (!cpu_flag_is_set(cpu, FLAG_CF)) {
        cpu->ip += instruction->destination.as_jump.offset;
      }
      break;
    }

    case JZ: {
      assert(instruction->destination.type == OT_JUMP);
      if (cpu_flag_is_set(cpu, FLAG_ZF)) {
        cpu->ip += instruction->destination.as_jump.offset;
      }
      break;
    }

    case JNZ: {
      assert(instruction->destination.type == OT_JUMP);
      if (!cpu_flag_is_set(cpu, FLAG_ZF)) {
        cpu->ip += instruction->destination.as_jump.offset;
      }
      break;
    }

    case JP: {
      assert(instruction->destination.type == OT_JUMP);
      if (cpu_flag_is_set(cpu, FLAG_PF)) {
        cpu->ip += instruction->destination.as_jump.offset;
      }
      break;
    }

    case JNP: {
      assert(instruction->destination.type == OT_JUMP);
      if (!cpu_flag_is_set(cpu, FLAG_PF)) {
        cpu->ip += instruction->destination.as_jump.offset;
      }
      break;
    }

    case JS: {
      assert(instruction->destination.type == OT_JUMP);
      if (cpu_flag_is_set(cpu, FLAG_SF)) {
        cpu->ip += instruction->destination.as_jump.offset;
      }
      break;
    }

    case JNS: {
      assert(instruction->destination.type == OT_JUMP);
      if (!cpu_flag_is_set(cpu, FLAG_SF)) {
        cpu->ip += instruction->destination.as_jump.offset;
      }
      break;
    }

    case CLI: {
      cpu_clear_flag(cpu, FLAG_IF);
      break;
    }

    case SAHF: {
      cpu->flags = (cpu->flags & 0xff00) | cpu->reg_8[AH];
      break;
    }

    case LAHF: {
      cpu->reg_8[AH] = cpu->flags & 0xff;
      break;
    }

    case NOOP:
      break;

    default:
      printf("Unsupported instruction (" HEX_8 ")\n", instruction->type);
      assert(0);
  }
}

void cpu_init(struct cpu *cpu, struct bus *bus) {
  memset(cpu->reg_16, 0, sizeof(cpu->reg_16));
  cpu->ip = 0;
  cpu->flags = 0;
  memset(cpu->segments, 0, sizeof(cpu->segments));

  cpu->bus = bus;

  cpu->segments[CS] = 0xffff;
  cpu->ip = 0x0000;
}

void hex_dump(const u8 *data, unsigned data_size) {
  for (unsigned i = 0; i < data_size; ++i) {
    printf("%02x", data[i]);
    unsigned mod = i % 0x10;
    if (mod == 0x0f) {
      printf("\n");
    } else if (mod == 0x07) {
      printf("  ");
    } else {
      printf(" ");
    }
  }
}

void cpu_run(struct cpu *cpu) {
  print_registers(cpu);
  printf("\n");

  while (1) {
    u8 buffer[16];
    memset(buffer, 0, sizeof(buffer));
    unsigned buffer_size = cpu_prefetch(cpu, buffer, sizeof(buffer));

    // hex_dump(buffer, 16);

    struct instruction instruction;
    memset(&instruction, 0, sizeof(struct instruction));

#if !defined(NDEBUG)
    memcpy(instruction.buffer, buffer, sizeof(buffer));
#endif

    int instruction_size = decode_instruction(buffer, buffer_size, &instruction);
    instruction.instruction_size = instruction_size;

    if (instruction_size < 0) {
      printf("Could not decode instructions [");
      for (unsigned i = 0; i < sizeof(buffer); ++i) {
        printf("%02x", buffer[i]);
        if (i < sizeof(buffer) - 1) {
          printf(" ");
        }
      }
      printf("]\n");
      break;
    }

    disassemble(&instruction, segment_offset(cpu->segments[CS], cpu->ip));

    cpu->ip += instruction_size;

    if (instruction.type == HLT) {
      printf("HLT");
      break;
    }

    // Interpret the instruction after adjusting the IP.

    fflush(stdout);
    interpret_instruction(cpu, &instruction);

    print_registers(cpu);
    printf("\n");
  }
}

unsigned cpu_prefetch(struct cpu *cpu, u8 *buffer, unsigned size) {
  u16 cs = cpu->segments[CS];
  u16 ip = cpu->ip;

  unsigned i = 0;
  for (; i < size; ++i) {
    buffer[i] = bus_fetch(cpu->bus, segment_offset(cs, ip + i));
  }

  return i;
}

u8 cpu_get_register_8(struct cpu *cpu, enum register_8 reg) {
  return cpu->reg_8[reg];
}

void cpu_set_register_8(struct cpu *cpu, enum register_8 reg, u8 value) {
  cpu->reg_8[reg] = value;
}

u16 cpu_get_register_16(struct cpu *cpu, enum register_16 reg) {
  return cpu->reg_16[reg];
}

void cpu_set_register_16(struct cpu *cpu, enum register_16 reg, u16 value) {
  cpu->reg_16[reg] = value;
}

u16 cpu_get_segment(struct cpu *cpu, enum segment_register reg) {
  return cpu->segments[reg];
}
