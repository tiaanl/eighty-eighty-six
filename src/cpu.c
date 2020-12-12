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

static void print_registers(u16 registers[]) {
  printf("ax: " HEX_16 ", bx: " HEX_16 ", cx: " HEX_16 ", dx: " HEX_16 ", ip: " HEX_16 "\n",
         registers[AX], registers[BX], registers[CX], registers[DX], registers[IP]);
  printf("si: " HEX_16 ", di: " HEX_16 ", bp: " HEX_16 ", sp: " HEX_16 ", fl: " HEX_16 "\n",
         registers[SI], registers[DI], registers[BP], registers[SP], registers[FLAGS]);
}

struct address get_operand_indirect_address(struct operand *operand, struct cpu *cpu) {
  struct address result;

  switch (operand->indirect_reg) {
    case INDIRECT_REG_BX_SI:
      result = segment_offset(cpu_get_register_16(cpu, BX), cpu_get_register_16(cpu, SI));
      break;

    case INDIRECT_REG_BX_DI:
      result = segment_offset(cpu_get_register_16(cpu, BX), cpu_get_register_16(cpu, DI));
      break;

    case INDIRECT_REG_BP_SI:
      result = segment_offset(cpu_get_register_16(cpu, BP), cpu_get_register_16(cpu, SI));
      break;

    case INDIRECT_REG_BP_DI:
      result = segment_offset(cpu_get_register_16(cpu, BP), cpu_get_register_16(cpu, DI));
      break;

    case INDIRECT_REG_SI:
      result = segment_offset(cpu_get_segment(cpu, DS), cpu_get_register_16(cpu, SI));
      break;

    case INDIRECT_REG_DI:
      result = segment_offset(cpu_get_segment(cpu, DS), cpu_get_register_16(cpu, DI));
      break;

    case INDIRECT_REG_BP:
      result = segment_offset(cpu_get_segment(cpu, DS), cpu_get_register_16(cpu, BP));
      break;

    case INDIRECT_REG_BX:
      result = segment_offset(cpu_get_segment(cpu, DS), cpu_get_register_16(cpu, BX));
      break;
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
  switch (operand->reg) {
    case REG_AL_AX:
      return cpu_get_register_8(cpu, AL);

    case REG_CL_CX:
      return cpu_get_register_8(cpu, CL);

    case REG_DL_DX:
      return cpu_get_register_8(cpu, DL);

    case REG_BL_BX:
      return cpu_get_register_8(cpu, BL);

    case REG_AH_SP:
      return cpu_get_register_8(cpu, AH);

    case REG_CH_BP:
      return cpu_get_register_8(cpu, CH);

    case REG_DH_SI:
      return cpu_get_register_8(cpu, DH);

    case REG_BH_DI:
      return cpu_get_register_8(cpu, BH);

    default:
      assert(0);
  }

  return 0;
}

static u16 get_operand_register_value_16(struct operand *operand, struct cpu *cpu) {
  switch (operand->reg) {
    case REG_AL_AX:
      return cpu_get_register_16(cpu, AX);

    case REG_CL_CX:
      return cpu_get_register_16(cpu, CX);

    case REG_DL_DX:
      return cpu_get_register_16(cpu, DX);

    case REG_BL_BX:
      return cpu_get_register_16(cpu, BX);

    case REG_AH_SP:
      return cpu_get_register_16(cpu, SP);

    case REG_CH_BP:
      return cpu_get_register_16(cpu, BP);

    case REG_DH_SI:
      return cpu_get_register_16(cpu, SI);

    case REG_BH_DI:
      return cpu_get_register_16(cpu, DI);

    default:
      assert(0);
  }

  return 0;
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

    case OPERAND_MODE_NONE:
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
  switch (operand->reg) {
    case REG_AL_AX:
      cpu_set_register_8(cpu, AL, value);
      break;

    case REG_CL_CX:
      cpu_set_register_8(cpu, CL, value);
      break;

    case REG_DL_DX:
      cpu_set_register_8(cpu, DL, value);
      break;

    case REG_BL_BX:
      cpu_set_register_8(cpu, BL, value);
      break;

    case REG_AH_SP:
      cpu_set_register_8(cpu, AH, value);
      break;

    case REG_CH_BP:
      cpu_set_register_8(cpu, CH, value);
      break;

    case REG_DH_SI:
      cpu_set_register_8(cpu, DH, value);
      break;

    case REG_BH_DI:
      cpu_set_register_8(cpu, BH, value);
      break;
  }
}

static void set_operand_register_value_16(struct operand *operand, struct cpu *cpu, u16 value) {
  switch (operand->reg) {
    case REG_AL_AX:
      cpu_set_register_16(cpu, AX, value);
      break;

    case REG_CL_CX:
      cpu_set_register_16(cpu, CX, value);
      break;

    case REG_DL_DX:
      cpu_set_register_16(cpu, DX, value);
      break;

    case REG_BL_BX:
      cpu_set_register_16(cpu, BX, value);
      break;

    case REG_AH_SP:
      cpu_set_register_16(cpu, SP, value);
      break;

    case REG_CH_BP:
      cpu_set_register_16(cpu, BP, value);
      break;

    case REG_DH_SI:
      cpu_set_register_16(cpu, SI, value);
      break;

    case REG_BH_DI:
      cpu_set_register_16(cpu, DI, value);
      break;
  }
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
        u16 ip = cpu_get_register_16(cpu, IP);
        cpu_set_register_16(cpu, IP, ip + instruction->destination.disp8);
      }
      break;
    }

    case OPERAND_SIZE_16: {
      u8 cx = cpu_get_register_16(cpu, CX);
      if (cx) {
        cpu_set_register_16(cpu, CX, cx - 1);
        u16 ip = cpu_get_register_16(cpu, IP);
        cpu_set_register_16(cpu, IP, ip + instruction->destination.disp16);
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
    u16 ip = cpu_get_register_16(cpu, IP);
    switch (instruction->destination.size) {
      case OPERAND_SIZE_8:
        cpu_set_register_16(cpu, IP, ip + instruction->destination.disp8);
        break;

      case OPERAND_SIZE_16:
        cpu_set_register_16(cpu, IP, ip + instruction->destination.disp16);
        break;

      default:
        assert(0);
    }
  }
}

static void interpret_push(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == PUSH);
}

static void interpret_pop(struct cpu *cpu, struct instruction *instruction) {
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
  memset(cpu->registers, 0, sizeof(cpu->registers));
  cpu->bus = bus;
}

void cpu_run(struct cpu *cpu) {
  print_registers(cpu->registers);

  while (1) {
    u8 buffer[16];
    memset(buffer, 0, sizeof(buffer));
    unsigned buffer_size = cpu_prefetch(cpu, buffer, sizeof(buffer));

    // Special hack to exit decoding.
    if (buffer[0] == 0xff) {
      break;
    }

    struct instruction instruction;
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

    u16 ip = cpu_get_register_16(cpu, IP);
    disassemble_addr(&instruction, ip);

    cpu_set_register_16(cpu, IP, ip + instruction_size);

    if (instruction.type == HLT) {
      break;
    }

    fflush(stdout);
    interpret_instruction(cpu, &instruction);

    print_registers(cpu->registers);
  }
}

unsigned cpu_prefetch(struct cpu *cpu, u8 *buffer, unsigned size) {
  u16 cs = cpu_get_segment(cpu, CS);
  u16 ip = cpu_get_register_16(cpu, IP);

  unsigned i = 0;
  for (; i < size; ++i) {
    buffer[i] = bus_fetch(cpu->bus, segment_offset(cs, ip + i));
  }

  return i;
}

u8 cpu_get_register_8(struct cpu *cpu, enum register_8 reg) {
  if (reg < 4) {
    return REG_LO(cpu->registers[reg]);
  } else {
    return REG_HI(cpu->registers[reg % 4]);
  }
}

void cpu_set_register_8(struct cpu *cpu, enum register_8 reg, u8 value) {
  if (reg < 4) {
    cpu->registers[reg] = (cpu->registers[reg] & 0xff00) + value;
  } else {
    cpu->registers[reg] = (cpu->registers[reg] & 0x00ff) + (value << 8);
  }
}

u16 cpu_get_register_16(struct cpu *cpu, enum register_16 reg) {
  return cpu->registers[reg];
}

void cpu_set_register_16(struct cpu *cpu, enum register_16 reg, u16 value) {
  cpu->registers[reg] = value;
}

u16 cpu_get_segment(struct cpu *cpu, enum segment_register reg) {
  return cpu->registers[REGISTER_16_COUNT + reg];
}

u8 cpu_flag_is_set(struct cpu *cpu, enum flags flag) {
  u8 flags = cpu_get_register_16(cpu, FLAGS);
  return (flags & flag) == flag;
}

void cpu_set_flag(struct cpu *cpu, enum flags flag, u8 value) {
  u16 flags = cpu_get_register_16(cpu, FLAGS);
  if (!value) {
    cpu_set_register_16(cpu, FLAGS, flags & ~flag);
  } else {
    cpu_set_register_16(cpu, FLAGS, flags | flag);
  }
}
