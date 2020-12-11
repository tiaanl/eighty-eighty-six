#include "cpu.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "bus.h"
#include "decoder.h"
#include "disassembler.h"

#define REG_HI(x) ((x) >> 8)
#define REG_LO(x) ((x)&0xff)

static void print_registers(struct registers *registers) {
  printf("ax: " HEX_16 ", bx: " HEX_16 ", cx: " HEX_16 ", dx: " HEX_16 ", ip: " HEX_16 "\n",
         registers->ax, registers->bx, registers->cx, registers->dx, registers->ip);
  printf("sp: " HEX_16 ", bp: " HEX_16 ", si: " HEX_16 ", di: " HEX_16 ", fl: " HEX_16 "\n",
         registers->sp, registers->bp, registers->si, registers->di, registers->flags);
}

struct address get_operand_indirect_address(struct operand *operand, struct cpu *cpu) {
  struct address result;

  switch (operand->indirect_reg) {
    case INDIRECT_REG_BX_SI:
    case INDIRECT_REG_BX_DI:
    case INDIRECT_REG_BP_SI:
    case INDIRECT_REG_BP_DI:
      assert(0);
      break;

    case INDIRECT_REG_SI:
      result = segment_offset(cpu->registers.ds, cpu->registers.si);
      break;

    case INDIRECT_REG_DI:
      result = segment_offset(cpu->registers.ds, cpu->registers.di);
      break;

    case INDIRECT_REG_BP:
      result = segment_offset(cpu->registers.ds, cpu->registers.bp);
      break;

    case INDIRECT_REG_BX:
      result = segment_offset(cpu->registers.ds, cpu->registers.bx);
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

static u16 get_operand_register_value_8(struct operand *operand, struct cpu *cpu) {
  switch (operand->reg) {
    case REG_AL_AX:
      return REG_LO(cpu->registers.ax);

    case REG_CL_CX:
      return REG_LO(cpu->registers.cx);

    case REG_DL_DX:
      return REG_LO(cpu->registers.dx);

    case REG_BL_BX:
      return REG_LO(cpu->registers.bx);

    case REG_AH_SP:
      return REG_HI(cpu->registers.ax);

    case REG_CH_BP:
      return REG_HI(cpu->registers.cx);

    case REG_DH_SI:
      return REG_HI(cpu->registers.dx);

    case REG_BH_DI:
      return REG_HI(cpu->registers.bx);

    default:
      assert(0);
  }
}

static u16 get_operand_register_value_16(struct operand *operand, struct cpu *cpu) {
  switch (operand->reg) {
    case REG_AL_AX:
      return cpu->registers.ax;

    case REG_CL_CX:
      return cpu->registers.cx;

    case REG_DL_DX:
      return cpu->registers.dx;

    case REG_BL_BX:
      return cpu->registers.bx;

    case REG_AH_SP:
      return cpu->registers.sp;

    case REG_CH_BP:
      return cpu->registers.bp;

    case REG_DH_SI:
      return cpu->registers.si;

    case REG_BH_DI:
      return cpu->registers.di;

    default:
      assert(0);
  }
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
}

static void set_operand_indirect_value_8(struct operand *operand, struct cpu *cpu, u8 value) {
  u16 addr;

  switch (operand->indirect_reg) {
    case INDIRECT_REG_BX_SI:
    case INDIRECT_REG_BX_DI:
    case INDIRECT_REG_BP_SI:
    case INDIRECT_REG_BP_DI:
      assert(0);
      break;

    case INDIRECT_REG_SI:
      addr = cpu->registers.si;
      break;

    case INDIRECT_REG_DI:
      addr = cpu->registers.di;
      break;

    case INDIRECT_REG_BP:
      addr = cpu->registers.bp;
      break;

    case INDIRECT_REG_BX:
      addr = cpu->registers.bx;
      break;
  }

  bus_store(cpu->bus, segment_offset(0, addr), value);
}

static void set_operand_indirect_value_16(struct operand *operand, struct cpu *cpu, u16 value) {
  u16 addr;

  switch (operand->indirect_reg) {
    case INDIRECT_REG_BX_SI:
    case INDIRECT_REG_BX_DI:
    case INDIRECT_REG_BP_SI:
    case INDIRECT_REG_BP_DI:
      assert(0);
      break;

    case INDIRECT_REG_SI:
      addr = cpu->registers.si;
      break;

    case INDIRECT_REG_DI:
      addr = cpu->registers.di;
      break;

    case INDIRECT_REG_BP:
      addr = cpu->registers.bp;
      break;

    case INDIRECT_REG_BX:
      addr = cpu->registers.bx;
      break;
  }

  bus_store(cpu->bus, segment_offset(0, addr), value >> 8);
  bus_store(cpu->bus, segment_offset(0, addr + 1), value & 0xff);
}

static void set_operand_register_value_8(struct operand *operand, struct cpu *cpu, u8 value) {
  switch (operand->reg) {
    case REG_AL_AX:
      cpu->registers.ax = (cpu->registers.ax & 0xff00) + value;
      break;

    case REG_CL_CX:
      cpu->registers.cx = (cpu->registers.cx & 0xff00) + value;
      break;

    case REG_DL_DX:
      cpu->registers.dx = (cpu->registers.dx & 0xff00) + value;
      break;

    case REG_BL_BX:
      cpu->registers.bx = (cpu->registers.bx & 0xff00) + value;
      break;

    case REG_AH_SP:
      cpu->registers.ax = (cpu->registers.ax & 0x00ff) + (value << 8);
      break;

    case REG_CH_BP:
      cpu->registers.cx = (cpu->registers.cx & 0x00ff) + (value << 8);
      break;

    case REG_DH_SI:
      cpu->registers.dx = (cpu->registers.dx & 0x00ff) + (value << 8);
      break;

    case REG_BH_DI:
      cpu->registers.bx = (cpu->registers.bx & 0x00ff) + (value << 8);
      break;
  }
}

static void set_operand_register_value_16(struct operand *operand, struct cpu *cpu, u16 value) {
  switch (operand->reg) {
    case REG_AL_AX:
      cpu->registers.ax = value;
      break;

    case REG_CL_CX:
      cpu->registers.cx = value;
      break;

    case REG_DL_DX:
      cpu->registers.dx = value;
      break;

    case REG_BL_BX:
      cpu->registers.bx = value;
      break;

    case REG_AH_SP:
      cpu->registers.sp = value;
      break;

    case REG_CH_BP:
      cpu->registers.bp = value;
      break;

    case REG_DH_SI:
      cpu->registers.si = value;
      break;

    case REG_BH_DI:
      cpu->registers.di = value;
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
      u8 cl = REG_LO(cpu->registers.cx);
      if (cl) {
        cpu->registers.cx = (cpu->registers.cx & 0xff00) + cl - 1;
        cpu->registers.ip += instruction->destination.disp8;
      }
      break;
    }

    case OPERAND_SIZE_16: {
      u8 cx = cpu->registers.cx;
      if (cx) {
        cpu->registers.cx -= 1;
        cpu->registers.ip += instruction->destination.disp16;
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
        cpu->registers.ip += instruction->destination.disp8;
        break;

      case OPERAND_SIZE_16:
        cpu->registers.ip += instruction->destination.disp16;
        break;

      default:
        assert(0);
    }
  }
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

    default:
      assert(0);
  }
}

void cpu_init(struct cpu *cpu, struct bus *bus) {
  cpu->registers.ax = 0;
  cpu->registers.bx = 0;
  cpu->registers.cx = 0;
  cpu->registers.dx = 0;

  cpu->registers.ip = 0;
  cpu->registers.si = 0;
  cpu->registers.di = 0;
  cpu->registers.bp = 0;
  cpu->registers.sp = 0;
  cpu->registers.flags = 0;

  cpu->registers.cs = 0;
  cpu->registers.ds = 0;
  cpu->registers.ss = 0;
  cpu->registers.es = 0;

  cpu->bus = bus;
}

void cpu_run(struct cpu *cpu) {
  print_registers(&cpu->registers);

  while (1) {
    u8 buffer[16];
    memset(buffer, 0, sizeof(buffer));
    unsigned buffer_size = cpu_prefetch(cpu, buffer, sizeof(buffer));

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

    disassemble_addr(&instruction, cpu->registers.ip);

    cpu->registers.ip += instruction_size;

    if (instruction.type == HLT) {
      break;
    }

    fflush(stdout);
    interpret_instruction(cpu, &instruction);

    print_registers(&cpu->registers);
  }
}

u8 cpu_peek_u8(struct cpu *cpu, int offset) {
  return bus_fetch(cpu->bus, segment_offset(0, cpu->registers.ip + offset));
}

i8 cpu_peek_i8(struct cpu *cpu, int offset) {
  return (i8)cpu_peek_u8(cpu, offset);
}

u16 cpu_peek_u16(struct cpu *cpu, int offset) {
  u16 addr = cpu->registers.ip + offset;
  return bus_fetch(cpu->bus, segment_offset(0, addr)) +
         (bus_fetch(cpu->bus, segment_offset(0, addr + 1)) << 8);
}

i16 cpu_peek_i16(struct cpu *cpu, int offset) {
  return (i16)cpu_peek_u16(cpu, offset);
}

u8 cpu_fetch_u8(struct cpu *cpu) {
  u8 value = cpu_peek_u8(cpu, 0);
  cpu_advance_ip(cpu, 1);
  return value;
}

i8 cpu_fetch_i8(struct cpu *cpu) {
  i8 value = cpu_peek_i8(cpu, 0);
  cpu_advance_ip(cpu, 1);
  return value;
}

u16 cpu_fetch_u16(struct cpu *cpu) {
  u16 value = cpu_peek_u16(cpu, 0);
  cpu_advance_ip(cpu, 2);
  return value;
}

i16 cpu_fetch_i16(struct cpu *cpu) {
  i16 value = cpu_peek_i16(cpu, 0);
  cpu_advance_ip(cpu, 2);
  return value;
}

u16 cpu_advance_ip(struct cpu *cpu, u16 count) {
  cpu->registers.ip += count;
  return cpu->registers.ip;
}

unsigned cpu_prefetch(struct cpu *cpu, u8 *buffer, unsigned size) {
  unsigned i = 0;
  for (; i < size; ++i) {
    buffer[i] = cpu_peek_u8(cpu, i);
  }

  return i;
}

u8 cpu_flag_is_set(struct cpu *cpu, enum flags flag) {
  return (cpu->registers.flags & flag) == flag;
}

void cpu_set_flag(struct cpu *cpu, enum flags flag, u8 value) {
  if (!value) {
    cpu->registers.flags &= ~flag;
  } else {
    cpu->registers.flags |= flag;
  }
}
