#include "cpu.h"

#include "bus.h"
#include "decoder/decoder.h"
#include "disassembler.h"
#include "print_format.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

const char *register_8_string_map[] = {
    "al", "cl", "dl", "bl", // low bytes
    "ah", "ch", "dh", "bh", // hi bytes
};

const char *register_16_string_map[] = {
    "ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "ip", "flags",
};

const char *segment_register_string_map[] = {
    "es",
    "cs",
    "ss",
    "ds",
};

const char *register_8_to_string(enum register_8 reg) {
  assert(reg < REGISTER_8_COUNT);
  return register_8_string_map[reg];
}

const char *register_16_to_string(enum register_16 reg) {
  assert(reg < REGISTER_16_COUNT);
  return register_16_string_map[reg];
}

const char *segment_register_to_string(enum segment_register reg) {
  assert(reg < SEGMENT_REGISTER_COUNT);
  return segment_register_string_map[reg];
}

#define REG_HI(x) ((x) >> 8)
#define REG_LO(x) ((x)&0xff)

static void print_registers(struct cpu *cpu) {
  printf("ax: " HEX_16 ", bx: " HEX_16 ", cx: " HEX_16 ", dx: " HEX_16 ", ip: " HEX_16 "\n",
         cpu->reg_16[AX], cpu->reg_16[BX], cpu->reg_16[CX], cpu->reg_16[DX], cpu->ip);
  printf("si: " HEX_16 ", di: " HEX_16 ", bp: " HEX_16 ", sp: " HEX_16 ", fl: " HEX_16 "\n",
         cpu->reg_16[SI], cpu->reg_16[DI], cpu->reg_16[BP], cpu->reg_16[SP], cpu->flags);
}

struct address get_operand_indirect_address(struct operand *operand, struct cpu *cpu) {
  struct address result;

  switch (operand->reg_16) {
    case MRRM_RM_BX_SI:
      result = segment_offset(cpu_get_register_16(cpu, BX), cpu_get_register_16(cpu, SI));
      break;

    case MRRM_RM_BX_DI:
      result = segment_offset(cpu_get_register_16(cpu, BX), cpu_get_register_16(cpu, DI));
      break;

    case MRRM_RM_BP_SI:
      result = segment_offset(cpu_get_register_16(cpu, BP), cpu_get_register_16(cpu, SI));
      break;

    case MRRM_RM_BP_DI:
      result = segment_offset(cpu_get_register_16(cpu, BP), cpu_get_register_16(cpu, DI));
      break;

    case MRRM_RM_SI:
      result = segment_offset(cpu_get_segment(cpu, DS), cpu_get_register_16(cpu, SI));
      break;

    case MRRM_RM_DI:
      result = segment_offset(cpu_get_segment(cpu, DS), cpu_get_register_16(cpu, DI));
      break;

    case MRRM_RM_BP:
      result = segment_offset(cpu_get_segment(cpu, DS), cpu_get_register_16(cpu, BP));
      break;

    case MRRM_RM_BX:
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
  return cpu->reg_8[operand->reg_8];
}

static u16 get_operand_register_value_16(struct operand *operand, struct cpu *cpu) {
  return cpu->reg_16[operand->reg_16];
}

static u8 get_operand_value_8(struct operand *operand, struct cpu *cpu) {
  switch (operand->mode) {
    case OPERAND_MODE_INDIRECT:
      return get_operand_indirect_value_8(operand, cpu);

    case OPERAND_MODE_DISPLACEMENT_8:
      assert(0);
      break;

    case OPERAND_MODE_DISPLACEMENT_16:
      assert(0);
      break;

    case OPERAND_MODE_REGISTER:
      return get_operand_register_value_8(operand, cpu);

    case OPERAND_MODE_DIRECT:
      return bus_fetch(cpu->bus, segment_offset(0, operand->disp16));

    case OPERAND_MODE_IMMEDIATE:
      return operand->immediate8;

    case OPERAND_MODE_NONE:
    default:
      assert(0);
  }

  return 0;
}

static u16 get_operand_value_16(struct operand *operand, struct cpu *cpu) {
  switch (operand->mode) {
    case OPERAND_MODE_INDIRECT:
      return get_operand_indirect_value_16(operand, cpu);

    case OPERAND_MODE_DISPLACEMENT_8:
      assert(0);
      break;

    case OPERAND_MODE_DISPLACEMENT_16:
      assert(0);
      break;

    case OPERAND_MODE_REGISTER:
      return get_operand_register_value_16(operand, cpu);

    case OPERAND_MODE_DIRECT:
      return bus_fetch(cpu->bus, segment_offset(0, operand->disp16)) +
             (bus_fetch(cpu->bus, segment_offset(0, operand->disp16 + 1)) << 8);

    case OPERAND_MODE_IMMEDIATE:
      return operand->immediate16;

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
  cpu->reg_8[operand->reg_8] = value;
}

static void set_operand_register_value_16(struct operand *operand, struct cpu *cpu, u16 value) {
  cpu->reg_16[operand->reg_16] = value;
}

static void set_operand_value_8(struct operand *operand, struct cpu *cpu, u8 value) {
  switch (operand->mode) {
    case OPERAND_MODE_INDIRECT:
      set_operand_indirect_value_8(operand, cpu, value);
      break;

    case OPERAND_MODE_DISPLACEMENT_8:
      assert(0);
      break;

    case OPERAND_MODE_DISPLACEMENT_16:
      assert(0);
      break;

    case OPERAND_MODE_REGISTER:
      set_operand_register_value_8(operand, cpu, value);
      break;

    case OPERAND_MODE_DIRECT:
      bus_store(cpu->bus, segment_offset(0, operand->disp16), value);
      break;

    case OPERAND_MODE_IMMEDIATE:
    case OPERAND_MODE_NONE:
    default:
      assert(0);
      break;
  }
}

static void set_operand_value_16(struct operand *operand, struct cpu *cpu, u16 value) {
  switch (operand->mode) {
    case OPERAND_MODE_INDIRECT:
      set_operand_indirect_value_16(operand, cpu, value);
      break;

    case OPERAND_MODE_DISPLACEMENT_8:
      assert(0);
      break;

    case OPERAND_MODE_DISPLACEMENT_16:
      assert(0);
      break;

    case OPERAND_MODE_REGISTER:
      set_operand_register_value_16(operand, cpu, value);
      break;

    case OPERAND_MODE_DIRECT:
      bus_store(cpu->bus, segment_offset(0, operand->disp16), value >> 8);
      bus_store(cpu->bus, segment_offset(0, operand->disp16 + 1), value & 0xff);
      break;

    case OPERAND_MODE_IMMEDIATE:
    case OPERAND_MODE_NONE:
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
    case OPERAND_SIZE_8:
      value8 = get_operand_value_8(&instruction->destination, cpu);
      break;

    case OPERAND_SIZE_16:
      value16 = get_operand_value_16(&instruction->destination, cpu);
      break;

    default:
      assert(0);
  }

  switch (instruction->source.size) {
    case OPERAND_SIZE_8:
      value8 += get_operand_value_8(&instruction->source, cpu);
      break;

    case OPERAND_SIZE_16:
      value16 += get_operand_value_16(&instruction->source, cpu);
      break;

    default:
      assert(0);
  }

  switch (instruction->destination.size) {
    case OPERAND_SIZE_8:
      set_operand_value_8(&instruction->destination, cpu, value8);
      break;

    case OPERAND_SIZE_16:
      set_operand_value_16(&instruction->destination, cpu, value16);
      break;

    default:
      assert(0);
  }
}

static void interpret_inc(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == INC);

  switch (instruction->destination.size) {
    case OPERAND_SIZE_8: {
      switch (instruction->destination.mode) {
        case OPERAND_MODE_REGISTER: {
          u8 value = get_operand_value_8(&instruction->destination, cpu);
          set_operand_value_8(&instruction->destination, cpu, value + 1);
          break;
        }

        default:
          assert(0);
      }
      break;
    }

    case OPERAND_SIZE_16: {
      switch (instruction->destination.mode) {
        case OPERAND_MODE_REGISTER: {
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
    case OPERAND_SIZE_8:
      value8 = get_operand_value_8(&instruction->source, cpu);
      break;

    case OPERAND_SIZE_16:
      value16 = get_operand_value_16(&instruction->source, cpu);
      break;

    default:
      assert(0);
  }

  switch (instruction->destination.size) {
    case OPERAND_SIZE_8:
      set_operand_value_8(&instruction->destination, cpu, value8);
      break;

    case OPERAND_SIZE_16:
      set_operand_value_16(&instruction->destination, cpu, value16);
      break;

    default:
      assert(0);
  }
}

static void interpret_test(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == TEST);

  cpu_set_flag(cpu, FLAG_CF, 0);
  cpu_set_flag(cpu, FLAG_OF, 0);

  switch (instruction->destination.size) {
    case OPERAND_SIZE_8: {
      u8 destination = get_operand_value_8(&instruction->destination, cpu);
      u8 source = get_operand_value_8(&instruction->source, cpu);
      u8 r = destination & source;

      cpu_set_flag(cpu, FLAG_SF, r & 0xff);
      cpu_set_flag(cpu, FLAG_ZF, !r);

      // TODO: More flags are affected by TEST
      break;
    }

    case OPERAND_SIZE_16: {
      u16 destination = get_operand_value_16(&instruction->destination, cpu);
      u16 source = get_operand_value_16(&instruction->source, cpu);
      u16 r = destination & source;

      cpu_set_flag(cpu, FLAG_SF, r & 0xffff);
      cpu_set_flag(cpu, FLAG_ZF, !r);
      break;
    }

    default:
      assert(0);
  }
}

static void interpret_loop(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == LOOP);

  switch (instruction->destination.size) {
    case OPERAND_SIZE_8: {
      u8 cl = cpu_get_register_8(cpu, CL) - 1;
      cpu_set_register_8(cpu, CL, cl);
      if (cl) {
        cpu->ip += instruction->destination.disp8;
      }
      break;
    }

    case OPERAND_SIZE_16: {
      u8 cx = cpu_get_register_16(cpu, CX);
      if (cx) {
        cpu_set_register_16(cpu, CX, cx - 1);
        cpu->ip += instruction->destination.disp16;
      }
      break;
    }

    default:
      assert(0);
  }
}

static void interpret_jz(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == JZ);

  if (cpu_flag_is_set(cpu, FLAG_ZF)) {
    switch (instruction->destination.size) {
      case OPERAND_SIZE_8:
        cpu->ip += instruction->destination.disp8;
        break;

      case OPERAND_SIZE_16:
        cpu->ip += instruction->destination.disp16;
        break;

      default:
        assert(0);
    }
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

static void interpret_instruction(struct cpu *cpu, struct instruction *instruction) {
  switch (instruction->type) {
    case ADD:
      interpret_add(cpu, instruction);
      break;

    case HLT:
      break;

    case INC:
      interpret_inc(cpu, instruction);
      break;

    case JZ:
      interpret_jz(cpu, instruction);
      break;

    case LOOP:
      interpret_loop(cpu, instruction);
      break;

    case MOV:
      interpret_mov(cpu, instruction);
      break;

    case NOP:
      break;

    case TEST:
      interpret_test(cpu, instruction);
      break;

    case PUSH:
      interpret_push(cpu, instruction);
      break;

    case POP:
      interpret_pop(cpu, instruction);
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
}

void cpu_run(struct cpu *cpu) {
  // print_registers(cpu->registers);

  while (1) {
    u8 buffer[16];
    memset(buffer, 0, sizeof(buffer));
    unsigned buffer_size = cpu_prefetch(cpu, buffer, sizeof(buffer));

    struct instruction instruction;
    memset(&instruction, 0, sizeof(struct instruction));
    int instruction_size = decode_instruction(buffer, buffer_size, &instruction);

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

    u16 ip = cpu->ip;
    disassemble_addr(&instruction, ip);

    cpu->ip += instruction_size;

    if (instruction.type == HLT) {
      printf("HALT HALT HALT!\n");
      break;
    }

    //    fflush(stdout);
    //    interpret_instruction(cpu, &instruction);
    //
    //    print_registers(cpu->registers);
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

u8 cpu_flag_is_set(struct cpu *cpu, enum flags flag) {
  return (cpu->flags & flag) == flag;
}

void cpu_set_flag(struct cpu *cpu, enum flags flag, u8 value) {
  if (!value) {
    cpu->flags &= ~flag;
  } else {
    cpu->flags |= flag;
  }
}
