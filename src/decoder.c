#include "decoder.h"
#include "cpu.h"

#include <assert.h>
#include <string.h>

int decode_instruction(struct cpu *cpu, struct instruction *instruction) {
  u8 buffer[16];
  memset(buffer, 0, sizeof(buffer));
  if (cpu_prefetch(cpu, buffer, sizeof(buffer)) < 1) {
    return -1;
  }

  u8 op_code = buffer[0];

  struct op_code_mapping *mapping = &op_code_table[op_code];

  instruction->type = mapping->instruction_type;

  switch (mapping->destination_operand_type) {
    case REG_AL: {
      instruction->destination.mode = OPERAND_MODE_REGISTER;
      instruction->destination.size = OPERAND_SIZE_8;
      instruction->destination.reg = REG_AL_AX;
      return 2;
    }

    case REG_AX: {
      instruction->destination.mode = OPERAND_MODE_REGISTER;
      instruction->destination.size = OPERAND_SIZE_16;
      instruction->destination.reg = REG_AL_AX;
      return 2;
    }

    case REG_8: {
      u8 reg = op_code >> 3 & 0b111;
      instruction->destination.mode = OPERAND_MODE_REGISTER;
      instruction->destination.size = OPERAND_SIZE_8;
      instruction->destination.reg = reg;
      break;
    }

    case REG_16: {
      u8 reg = op_code >> 3 & 0b111;
      instruction->destination.mode = OPERAND_MODE_REGISTER;
      instruction->destination.size = OPERAND_SIZE_16;
      instruction->destination.reg = reg;
      break;
    }

    case IMM_8:
      break;

    case IMM_16:
      break;

    default:
      assert(0);
  }

  return 3;
}

enum mod_rm_mod {
  MOD_INDIRECT = 0b00,
  MOD_BYTE = 0b01,
  MOD_DOUBLE_WORD = 0b10,
  MOD_REGISTER = 0b11,
};

struct mod_reg_rm {
  u8 mod;
  u8 reg;
  u8 rm;
};

struct mod_reg_rm decode_mod_reg_rm(u8 b) {
  struct mod_reg_rm result;

  result.mod = (b & 0b11000000) >> 6;
  result.reg = (b & 0b00111000) >> 3;
  result.rm = (b & 0b00000111) >> 0;

  return result;
}

int decode_reg_operand(struct mod_reg_rm mrm, enum operand_size operand_size, const u8 *buffer,
                       struct operand *operand) {
  operand->mode = OPERAND_MODE_REGISTER;
  operand->size = operand_size;
  operand->reg = mrm.reg;

  return 0;
}

int decode_mem_operand(struct mod_reg_rm mrm, enum operand_size operand_size, const u8 *buffer,
                       struct operand *operand) {
  operand->size = operand_size;

  switch ((enum mod_rm_mod)mrm.mod) {
    case MOD_INDIRECT:
      operand->mode = OPERAND_MODE_INDIRECT;
      // Special case on 16-bit platforms.
      if (mrm.rm == 0b110) {
        operand->mode = OPERAND_MODE_DIRECT;
        operand->disp16 = (i16)(buffer[2] + (buffer[3] << 8));
        return 2;
      } else {
        operand->indirect_reg = mrm.rm;
        return 0;
      }

    case MOD_BYTE:
      operand->mode = OPERAND_MODE_DISPLACEMENT_8;
      operand->disp8 = (i8)buffer[2];
      return 1;

    case MOD_DOUBLE_WORD:
      operand->mode = OPERAND_MODE_DISPLACEMENT_16;
      operand->disp16 = (i16)(buffer[2] + (buffer[3] << 8));
      return 2;

    case MOD_REGISTER:
      operand->mode = OPERAND_MODE_REGISTER;
      operand->reg = mrm.rm;
      return 0;
  }

  assert(0);
}

int decode_imm_operand(struct mod_reg_rm mrm, enum operand_size operand_size, const u8 *buffer,
                       struct operand *operand) {
  operand->mode = OPERAND_MODE_IMMEDIATE;
  operand->size = operand_size;
  operand->reg = mrm.rm;

  switch (operand_size) {
    case OPERAND_SIZE_8:
      operand->immediate8 = buffer[2];
      return 1;

    case OPERAND_SIZE_16:
      operand->immediate16 = buffer[2] + (buffer[3] << 8);
      return 2;

    default:
      assert(0);
  }
}

int decode_operand(enum operand_type type, struct mod_reg_rm mrm, const u8 *buffer,
                   struct operand *operand) {
  switch (type) {
    case MEM_8:
      return decode_mem_operand(mrm, OPERAND_SIZE_8, buffer, operand);

    case MEM_16:
      return decode_mem_operand(mrm, OPERAND_SIZE_16, buffer, operand);

    case REG_8:
      return decode_reg_operand(mrm, OPERAND_SIZE_8, buffer, operand);

    case REG_16:
      return decode_reg_operand(mrm, OPERAND_SIZE_16, buffer, operand);

    case IMM_8:
      return decode_imm_operand(mrm, OPERAND_SIZE_8, buffer, operand);

    case IMM_16:
      return decode_imm_operand(mrm, OPERAND_SIZE_16, buffer, operand);

    default:
      assert(0);
  }
}

int decode_instruction_with_mod_rm(struct cpu *cpu, struct instruction *instruction) {
  u8 buffer[16];
  memset(buffer, 0, sizeof(buffer));
  if (cpu_prefetch(cpu, buffer, sizeof(buffer)) < 1) {
    return -1;
  }

  u8 op_code = buffer[0];

  struct op_code_mapping *mapping = &op_code_table[op_code];

  struct mod_reg_rm mrm = decode_mod_reg_rm(buffer[1]);

  instruction->type = mapping->instruction_type;

  // op_code + mod_rm
  int instruction_size = 2;

  instruction_size +=
      decode_operand(mapping->destination_operand_type, mrm, buffer, &instruction->destination);
  instruction_size +=
      decode_operand(mapping->source_operand_type, mrm, buffer, &instruction->source);

  return instruction_size;
}
