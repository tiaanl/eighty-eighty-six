#include "cpu.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "bus.h"
#include "decoder.h"
#include "disassembler.h"

#define MNEMONIC "%-6s"
#define HEX_8 "0x%02x"
#define HEX_16 "0x%04x"

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

void print_mod_rm_reg(enum register_encoding reg, int is_16_bit) {
  // printf("(%d) ", reg);

  switch (reg) {
    case REG_AL_AX:
      printf(!is_16_bit ? "al" : "ax");
      break;

    case REG_CL_CX:
      printf(!is_16_bit ? "cl" : "cx");
      break;

    case REG_DL_DX:
      printf(!is_16_bit ? "dl" : "dx");
      break;

    case REG_BL_BX:
      printf(!is_16_bit ? "bl" : "bx");
      break;

    case REG_AH_SP:
      printf(!is_16_bit ? "ah" : "sp");
      break;

    case REG_CH_BP:
      printf(!is_16_bit ? "ch" : "bp");
      break;

    case REG_DH_SI:
      printf(!is_16_bit ? "dh" : "si");
      break;

    case REG_BH_DI:
      printf(!is_16_bit ? "bh" : "di");
      break;

    default:
      break;
  }
}

void cpu_run(struct cpu *cpu) {
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
      // Could not decode the instruction.
      break;
    }

    disassemble_addr(&instruction, cpu->registers.ip);

    cpu->registers.ip += instruction_size;
  }
}

const char *mod_rm_reg_to_string(enum register_encoding encoding, enum operand_size size) {
  static const char *reg_values[2][8] = {{"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"},
                                         {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"}};

  return reg_values[size][encoding];
}

const char *mod_rm_indirect_rm_to_string(enum indirect_register_encoding x) {
  switch (x) {
    case INDIRECT_REG_BX_SI:
      return "bx:si";

    case INDIRECT_REG_BX_DI:
      return "bx:di";

    case INDIRECT_REG_BP_SI:
      return "bp:si";

    case INDIRECT_REG_BP_DI:
      return "bp:di";

    case INDIRECT_REG_SI:
      return "si";

    case INDIRECT_REG_DI:
      return "di";

    case INDIRECT_REG_BP:
      return "bp";

    case INDIRECT_REG_BX:
      return "bx";
  }

  assert(0);
  return "??";
}

u8 cpu_peek_u8(struct cpu *cpu, int offset) {
  return bus_fetch(cpu->bus, cpu->registers.ip + offset);
}

i8 cpu_peek_i8(struct cpu *cpu, int offset) {
  return (i8)cpu_peek_u8(cpu, offset);
}

u16 cpu_peek_u16(struct cpu *cpu, int offset) {
  u16 addr = cpu->registers.ip + offset;
  return bus_fetch(cpu->bus, addr) + (bus_fetch(cpu->bus, addr + 1) << 8);
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
