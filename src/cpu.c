#include "cpu.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "decoder.h"
#include "disassembler.h"

#define MNEMONIC "%-6s"
#define HEX_8 "0x%02x"
#define HEX_16 "0x%04x"

void cpu_init(struct cpu *cpu, const u8 *code, unsigned int code_len) {
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

  cpu->code = code;
  cpu->code_len = code_len;
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
    u16 ip = cpu->registers.ip;

    if (ip >= cpu->code_len) {
      return;
    }

    u8 op_code = cpu_peek_u8(cpu, 0);

    struct op_code_mapping *mapping = &op_code_table[op_code];
    if (mapping->instruction_type == NOP) {
      printf("Found NOP instruction.\n");
      return;
    }

    if (mapping->decoder_func != 0) {
      struct instruction instruction;
      memset(&instruction, 0, sizeof(struct instruction));
      int instruction_size = mapping->decoder_func(cpu, &instruction);
      disassemble(&instruction);
      cpu->registers.ip += instruction_size;
    } else {
      printf("No instruction handler.\n");
      return;
    }
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
  return cpu->code[cpu->registers.ip + offset];
}

i8 cpu_peek_i8(struct cpu *cpu, int offset) {
  return (i8)cpu->code[cpu->registers.ip + offset];
}

u16 cpu_peek_u16(struct cpu *cpu, int offset) {
  return cpu->code[cpu->registers.ip + offset] + (cpu->code[cpu->registers.ip + offset + 1] << 8);
}

i16 cpu_peek_i16(struct cpu *cpu, int offset) {
  return (i16)(cpu->code[cpu->registers.ip + offset] +
               (cpu->code[cpu->registers.ip + offset + 1] << 8));
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

void cpu_instruction_add_mem_reg(struct cpu *cpu, enum operand_size operand_size) {
#if 0
  struct mod_rm m = mod_rm(cpu->code[cpu->registers.ip + 1]);

  printf("%-6s", "add");

  switch (m.mod) {
    case MOD_INDIRECT: {
      if (m.rm == 0b110) {
        // `rm` == 6 is treated as a special absolute address.
        u16 addr = cpu->code[cpu->registers.ip + 2] + (cpu->code[cpu->registers.ip + 3] << 8);
        printf("[0x%04x]", addr);
        cpu->registers.ip += 4;
      } else {
        printf("[%s]", mod_rm_indirect_rm_to_string(m.rm));
        cpu->registers.ip += 2;
      }
      break;
    }

    case MOD_BYTE: {
      // TODO: negative 16-bit displacement is not calculated correctly.
      printf("[%s", mod_rm_indirect_rm_to_string(m.rm));
      i8 displacement = (i8)cpu->code[cpu->registers.ip + 2];
      printf(displacement > 0 ? "+%d]" : "%d]", displacement);
      cpu->registers.ip += 3;
      break;
    }

    case MOD_DOUBLE_WORD: {
      printf("[%s", mod_rm_indirect_rm_to_string(m.rm));
      u16 displacement = ((cpu->code[cpu->registers.ip + 2] << 8) | cpu->code[cpu->registers.ip + 3]);
      printf(displacement > 0 ? "+%d]" : "%d]", (i16)displacement);
      cpu->registers.ip += 4;
      break;
    }

    case MOD_REGISTER:
      printf("%s", mod_rm_reg_to_string(m.rm, operand_size));
      cpu->registers.ip += 2;
      break;
  }

  printf(", %s\n", mod_rm_reg_to_string(m.reg, operand_size));
#endif
}

void cpu_instruction_add_reg_mem(struct cpu *cpu, enum operand_size operand_size) {
#if 0
  struct mod_rm m = mod_rm(cpu->code[cpu->registers.ip + 1]);

  printf("%-6s%s, ", "add", mod_rm_reg_to_string(m.reg, operand_size));

  switch (m.mod) {
    case MOD_INDIRECT: {
      if (m.rm == 0b110) {
        // `rm` == 6 is treated as a special absolute address.
        u16 addr = cpu->code[cpu->registers.ip + 2] + (cpu->code[cpu->registers.ip + 3] << 8);
        printf("[0x%04x]", addr);
        cpu->registers.ip += 4;
      } else {
        printf("[%s]", mod_rm_indirect_rm_to_string(m.rm));
        cpu->registers.ip += 2;
      }
      break;
    }

    case MOD_BYTE: {
      // TODO: negative 16-bit displacement is not calculated correctly.
      printf("[%s", mod_rm_indirect_rm_to_string(m.rm));
      i8 displacement = (i8)cpu->code[cpu->registers.ip + 2];
      printf(displacement > 0 ? "+%d]" : "%d]", displacement);
      cpu->registers.ip += 3;
      break;
    }

    case MOD_DOUBLE_WORD: {
      printf("[%s", mod_rm_indirect_rm_to_string(m.rm));
      u16 displacement = ((cpu->code[cpu->registers.ip + 2] << 8) | cpu->code[cpu->registers.ip + 3]);
      printf(displacement > 0 ? "+%d]" : "%d]", (i16)displacement);
      cpu->registers.ip += 4;
      break;
    }

    case MOD_REGISTER:
      printf("%s", mod_rm_reg_to_string(m.rm, operand_size));
      cpu->registers.ip += 2;
      break;
  }

  printf("\n");
#endif
}

unsigned cpu_prefetch(struct cpu *cpu, u8 *buffer, unsigned size) {
  unsigned i = 0;
  for (; i < size && cpu->registers.ip + i < cpu->code_len; ++i) {
    buffer[i] = cpu_peek_u8(cpu, i);
  }

  return i;
}

void cpu_instruction_add_mem8_reg8(struct cpu *cpu) {
  cpu_instruction_add_mem_reg(cpu, OPERAND_SIZE_8);
}

void cpu_instruction_add_mem16_reg16(struct cpu *cpu) {
  cpu_instruction_add_mem_reg(cpu, OPERAND_SIZE_16);
}

void cpu_instruction_add_reg8_mem8(struct cpu *cpu) {
  cpu_instruction_add_reg_mem(cpu, OPERAND_SIZE_8);
}

void cpu_instruction_add_reg16_mem16(struct cpu *cpu) {
  cpu_instruction_add_reg_mem(cpu, OPERAND_SIZE_16);
}

void cpu_instruction_add_al_imm8(struct cpu *cpu) {
  printf("%-6sal, 0x%02x", "add", cpu->code[cpu->registers.ip + 1]);

  cpu->registers.ip += 2;
}

void cpu_instruction_add_ax_imm16(struct cpu *cpu) {
  u16 value = cpu->code[cpu->registers.ip + 1] + (cpu->code[cpu->registers.ip + 1] << 8);
  printf("%-6sax, 0x%04x", "add", value);

  cpu->registers.ip += 2;
}

void cpu_instruction_mov_memx_regx(struct cpu *cpu) {
#if 0
  u8 op_code = cpu_fetch_u8(cpu);
  struct mod_rm m = mod_rm(cpu_fetch_u8(cpu));

  u8 is_16_bit = op_code & 0b00000001;
  u8 addr_first = (op_code & 0b00000010) >> 1;

  printf(MNEMONIC, "mov");

  switch (m.mod) {
    case MOD_INDIRECT: {
      if (m.rm == 0b110) {
        // `rm` == 6 is treated as a special absolute address.
        u16 addr = cpu_fetch_u16(cpu);
        printf("[0x%04x]", addr);
      } else {
        printf("[%s]", mod_rm_indirect_rm_to_string(m.rm));
      }
      break;
    }

    case MOD_BYTE: {
      // TODO: negative 16-bit displacement is not calculated correctly.
      printf("[%s", mod_rm_indirect_rm_to_string(m.rm));
      i8 displacement = cpu_fetch_i8(cpu);
      printf(displacement > 0 ? "+%d]" : "%d]", displacement);
      break;
    }

    case MOD_DOUBLE_WORD: {
      printf("[%s", mod_rm_indirect_rm_to_string(m.rm));
      i16 displacement = cpu_fetch_i16(cpu);
      printf(displacement > 0 ? "+%d]" : "%d]", displacement);
      break;
    }

    case MOD_REGISTER:
      printf("%s", mod_rm_reg_to_string(m.rm, is_16_bit ? OPERAND_SIZE_16 : OPERAND_SIZE_8));
      break;
  }

  printf(", %s\n", mod_rm_reg_to_string(m.reg, is_16_bit ? OPERAND_SIZE_16 : OPERAND_SIZE_8));
#endif // 0
}

void cpu_instruction_mov_regx_immx(struct cpu *cpu) {
  u8 op_code = cpu_fetch_u8(cpu);

  u8 reg = op_code & 0b00000111;
  u8 is_16_bit = (op_code & 0b00001000) >> 3;

  if (is_16_bit) {
    u16 value = cpu_fetch_u16(cpu);
    printf("%-6s%s, " HEX_16 "\n", "mov", mod_rm_reg_to_string(reg, OPERAND_SIZE_16), value);
  } else {
    u8 value = cpu_fetch_u8(cpu);
    printf("%-6s%s, " HEX_8 "\n", "mov", mod_rm_reg_to_string(reg, OPERAND_SIZE_8), value);
  }
}
