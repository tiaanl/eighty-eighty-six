#include "decoder.h"
#include "cpu.h"

#include <assert.h>
#include <string.h>

enum mod_reg_rm_mod {
  MOD_INDIRECT = 0b00,
  MOD_BYTE = 0b01,
  MOD_DOUBLE_WORD = 0b10,
  MOD_REGISTER = 0b11,
};

#if 0
struct mod_reg_rm {
  enum mod_reg_rm_mod mod;
  enum register_encoding reg;
  enum indirect_register_encoding rm;
};
#else
struct mod_reg_rm {
  u8 mod;
  u8 reg;
  u8 rm;
};
#endif

static struct mod_reg_rm decode_mod_reg_rm(u8 b) {
  struct mod_reg_rm result;

  result.mod = (b & 0b11000000) >> 6;
  result.reg = (b & 0b00111000) >> 3;
  result.rm = (b & 0b00000111) >> 0;

  return result;
}

static int decode_instruction_no_operands(struct op_code_mapping *mapping, const u8 *buffer,
                                          unsigned buffer_size, struct instruction *instruction) {
  instruction->type = mapping->instruction_type;
  instruction->destination.mode = OPERAND_MODE_NONE;
  instruction->source.mode = OPERAND_MODE_NONE;

  return 1;
}

static int decode_instruction_no_mod_rm(struct op_code_mapping *mapping, const u8 *buffer,
                                        unsigned buffer_size, struct instruction *instruction) {
  assert(buffer[0] != 0x0f);

  u8 op_code = buffer[0];

  // op_code
  int instruction_size = 1;

  instruction->type = mapping->instruction_type;

  switch (mapping->destination_operand_type) {
    case REG_AL: {
      instruction->destination.mode = OPERAND_MODE_REGISTER;
      instruction->destination.size = OPERAND_SIZE_8;
      instruction->destination.reg = REG_AL_AX;
      break;
    }

    case REG_AX: {
      instruction->destination.mode = OPERAND_MODE_REGISTER;
      instruction->destination.size = OPERAND_SIZE_16;
      instruction->destination.reg = REG_AL_AX;
      break;
    }

    case REG_8: {
      u8 reg = op_code & 0b111;
      instruction->destination.mode = OPERAND_MODE_REGISTER;
      instruction->destination.size = OPERAND_SIZE_8;
      instruction->destination.reg = reg;
      break;
    }

    case REG_16: {
      u8 reg = op_code & 0b111;
      instruction->destination.mode = OPERAND_MODE_REGISTER;
      instruction->destination.size = OPERAND_SIZE_16;
      instruction->destination.reg = reg;
      break;
    }

    case JMP_8:
      instruction->destination.mode = OPERAND_MODE_DISPLACEMENT_8;
      instruction->destination.size = OPERAND_SIZE_8;
      instruction->destination.disp8 = (i8)buffer[1];
      instruction_size += 1;
      break;

    case JMP_16:
      instruction->destination.mode = OPERAND_MODE_DISPLACEMENT_16;
      instruction->destination.size = OPERAND_SIZE_16;
      instruction->destination.disp16 = (i16)(buffer[1] + (buffer[2] << 8));
      instruction_size += 2;
      break;

    default:
      assert(0);
  }

  switch (mapping->source_operand_type) {
    case OPERAND_NONE:
      instruction->source.mode = OPERAND_MODE_NONE;
      break;

    case IMM_8:
      instruction->source.mode = OPERAND_MODE_IMMEDIATE;
      instruction->source.size = OPERAND_SIZE_8;
      instruction->source.immediate8 = buffer[1];
      instruction_size += 1;
      break;

    case IMM_16:
      instruction->source.mode = OPERAND_MODE_IMMEDIATE;
      instruction->source.size = OPERAND_SIZE_16;
      instruction->source.immediate16 = buffer[1] + (buffer[2] << 8);
      instruction_size += 2;
      break;

    default:
      assert(0);
  }

  return instruction_size;
}

static int decode_reg_operand(struct mod_reg_rm mrm, enum operand_size operand_size,
                              const u8 *buffer, struct operand *operand) {
  operand->mode = OPERAND_MODE_REGISTER;
  operand->size = operand_size;
  operand->reg = mrm.reg;

  return 0;
}

static int decode_mem_operand(struct mod_reg_rm mrm, enum operand_size operand_size,
                              const u8 *buffer, struct operand *operand) {
  operand->size = operand_size;

  switch ((enum mod_reg_rm_mod)mrm.mod) {
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

static int decode_imm_operand(struct mod_reg_rm mrm, enum operand_size operand_size,
                              const u8 *buffer, struct operand *operand) {
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

static int decode_operand(enum operand_type type, struct mod_reg_rm mrm, const u8 *buffer,
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

static int decode_instruction_with_mod_rm(struct op_code_mapping *mapping, const u8 *buffer,
                                          unsigned buffer_size, struct instruction *instruction) {
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

// clang-format off
struct op_code_mapping op_code_table[] = {
    /* 00 */ {ADD, MEM_8,  REG_8,  decode_instruction_with_mod_rm},
    /* 01 */ {ADD, MEM_16, REG_16, decode_instruction_with_mod_rm},
    /* 02 */ {ADD, REG_8,  MEM_8,  decode_instruction_with_mod_rm},
    /* 03 */ {ADD, REG_16, MEM_16, decode_instruction_with_mod_rm},
    /* 04 */ {ADD, REG_AL, IMM_8,  decode_instruction_no_mod_rm},
    /* 05 */ {ADD, REG_AX, IMM_16, decode_instruction_no_mod_rm},
    /* 06 */ {NOP},
    /* 07 */ {NOP},
    /* 08 */ {NOP},
    /* 09 */ {NOP},
    /* 0a */ {NOP},
    /* 0b */ {NOP},
    /* 0c */ {NOP},
    /* 0d */ {NOP},
    /* 0e */ {NOP},
    /* 0f */ {NOP},

    /* 10 */ {NOP},
    /* 11 */ {NOP},
    /* 12 */ {NOP},
    /* 13 */ {NOP},
    /* 14 */ {NOP},
    /* 15 */ {NOP},
    /* 16 */ {NOP},
    /* 17 */ {NOP},
    /* 18 */ {NOP},
    /* 19 */ {NOP},
    /* 1a */ {NOP},
    /* 1b */ {NOP},
    /* 1c */ {NOP},
    /* 1d */ {NOP},
    /* 1e */ {NOP},
    /* 1f */ {NOP},

    /* 20 */ {NOP},
    /* 21 */ {NOP},
    /* 22 */ {NOP},
    /* 23 */ {NOP},
    /* 24 */ {NOP},
    /* 25 */ {NOP},
    /* 26 */ {NOP},
    /* 27 */ {NOP},
    /* 28 */ {NOP},
    /* 29 */ {NOP},
    /* 2a */ {NOP},
    /* 2b */ {NOP},
    /* 2c */ {NOP},
    /* 2d */ {NOP},
    /* 2e */ {NOP},
    /* 2f */ {NOP},

    /* 30 */ {NOP},
    /* 31 */ {NOP},
    /* 32 */ {NOP},
    /* 33 */ {NOP},
    /* 34 */ {NOP},
    /* 35 */ {NOP},
    /* 36 */ {NOP},
    /* 37 */ {NOP},
    /* 38 */ {NOP},
    /* 39 */ {NOP},
    /* 3a */ {NOP},
    /* 3b */ {NOP},
    /* 3c */ {NOP},
    /* 3d */ {NOP},
    /* 3e */ {NOP},
    /* 3f */ {NOP},

    /* 40 */ {INC, REG_16, OPERAND_NONE, decode_instruction_no_mod_rm},
    /* 41 */ {INC, REG_16, OPERAND_NONE, decode_instruction_no_mod_rm},
    /* 42 */ {INC, REG_16, OPERAND_NONE, decode_instruction_no_mod_rm},
    /* 43 */ {INC, REG_16, OPERAND_NONE, decode_instruction_no_mod_rm},
    /* 44 */ {INC, REG_16, OPERAND_NONE, decode_instruction_no_mod_rm},
    /* 45 */ {INC, REG_16, OPERAND_NONE, decode_instruction_no_mod_rm},
    /* 46 */ {INC, REG_16, OPERAND_NONE, decode_instruction_no_mod_rm},
    /* 47 */ {INC, REG_16, OPERAND_NONE, decode_instruction_no_mod_rm},
    /* 48 */ {NOP},
    /* 49 */ {NOP},
    /* 4a */ {NOP},
    /* 4b */ {NOP},
    /* 4c */ {NOP},
    /* 4d */ {NOP},
    /* 4e */ {NOP},
    /* 4f */ {NOP},

    /* 50 */ {NOP},
    /* 51 */ {NOP},
    /* 52 */ {NOP},
    /* 53 */ {NOP},
    /* 54 */ {NOP},
    /* 55 */ {NOP},
    /* 56 */ {NOP},
    /* 57 */ {NOP},
    /* 58 */ {NOP},
    /* 59 */ {NOP},
    /* 5a */ {NOP},
    /* 5b */ {NOP},
    /* 5c */ {NOP},
    /* 5d */ {NOP},
    /* 5e */ {NOP},
    /* 5f */ {NOP},

    /* 60 */ {NOP},
    /* 61 */ {NOP},
    /* 62 */ {NOP},
    /* 63 */ {NOP},
    /* 64 */ {NOP},
    /* 65 */ {NOP},
    /* 66 */ {NOP},
    /* 67 */ {NOP},
    /* 68 */ {NOP},
    /* 69 */ {NOP},
    /* 6a */ {NOP},
    /* 6b */ {NOP},
    /* 6c */ {NOP},
    /* 6d */ {NOP},
    /* 6e */ {NOP},
    /* 6f */ {NOP},

    /* 70 */ {NOP},
    /* 71 */ {NOP},
    /* 72 */ {NOP},
    /* 73 */ {NOP},
    /* 74 */ {JE, JMP_8, OPERAND_NONE, decode_instruction_no_mod_rm},
    /* 75 */ {NOP},
    /* 76 */ {NOP},
    /* 77 */ {NOP},
    /* 78 */ {NOP},
    /* 79 */ {NOP},
    /* 7a */ {NOP},
    /* 7b */ {NOP},
    /* 7c */ {NOP},
    /* 7d */ {NOP},
    /* 7e */ {NOP},
    /* 7f */ {NOP},

    /* 80 */ {ADD, MEM_8,  IMM_8,  decode_instruction_with_mod_rm},
    /* 81 */ {ADD, MEM_16, IMM_16, decode_instruction_with_mod_rm},
    /* 82 */ {NOP},
    /* 83 */ {NOP},
    /* 84 */ {NOP},
    /* 85 */ {NOP},
    /* 86 */ {NOP},
    /* 87 */ {NOP},
    /* 88 */ {MOV, MEM_8,  REG_8,  decode_instruction_with_mod_rm},
    /* 89 */ {MOV, MEM_16, REG_16, decode_instruction_with_mod_rm},
    /* 8a */ {MOV, REG_8,  MEM_8,  decode_instruction_with_mod_rm},
    /* 8b */ {MOV, REG_16, MEM_16, decode_instruction_with_mod_rm},
    /* 8c */ {NOP},
    /* 8d */ {NOP},
    /* 8e */ {NOP},
    /* 8f */ {NOP},

    /* 90 */ {NOP},
    /* 91 */ {NOP},
    /* 92 */ {NOP},
    /* 93 */ {NOP},
    /* 94 */ {NOP},
    /* 95 */ {NOP},
    /* 96 */ {NOP},
    /* 97 */ {NOP},
    /* 98 */ {NOP},
    /* 99 */ {NOP},
    /* 9a */ {NOP},
    /* 9b */ {NOP},
    /* 9c */ {NOP},
    /* 9d */ {NOP},
    /* 9e */ {NOP},
    /* 9f */ {NOP},

    /* a0 */ {NOP},
    /* a1 */ {NOP},
    /* a2 */ {NOP},
    /* a3 */ {NOP},
    /* a4 */ {NOP},
    /* a5 */ {NOP},
    /* a6 */ {NOP},
    /* a7 */ {NOP},
    /* a8 */ {NOP},
    /* a9 */ {NOP},
    /* aa */ {NOP},
    /* ab */ {NOP},
    /* ac */ {NOP},
    /* ad */ {NOP},
    /* ae */ {NOP},
    /* af */ {NOP},

    /* b0 */ {MOV, REG_8,  IMM_8,  decode_instruction_no_mod_rm},
    /* b1 */ {MOV, REG_8,  IMM_8,  decode_instruction_no_mod_rm},
    /* b2 */ {MOV, REG_8,  IMM_8,  decode_instruction_no_mod_rm},
    /* b3 */ {MOV, REG_8,  IMM_8,  decode_instruction_no_mod_rm},
    /* b4 */ {MOV, REG_8,  IMM_8,  decode_instruction_no_mod_rm},
    /* b5 */ {MOV, REG_8,  IMM_8,  decode_instruction_no_mod_rm},
    /* b6 */ {MOV, REG_8,  IMM_8,  decode_instruction_no_mod_rm},
    /* b7 */ {MOV, REG_8,  IMM_8,  decode_instruction_no_mod_rm},
    /* b8 */ {MOV, REG_16, IMM_16, decode_instruction_no_mod_rm},
    /* b9 */ {MOV, REG_16, IMM_16, decode_instruction_no_mod_rm},
    /* ba */ {MOV, REG_16, IMM_16, decode_instruction_no_mod_rm},
    /* bb */ {MOV, REG_16, IMM_16, decode_instruction_no_mod_rm},
    /* bc */ {MOV, REG_16, IMM_16, decode_instruction_no_mod_rm},
    /* bd */ {MOV, REG_16, IMM_16, decode_instruction_no_mod_rm},
    /* be */ {MOV, REG_16, IMM_16, decode_instruction_no_mod_rm},
    /* bf */ {MOV, REG_16, IMM_16, decode_instruction_no_mod_rm},

    /* c0 */ {NOP},
    /* c1 */ {NOP},
    /* c2 */ {NOP},
    /* c3 */ {NOP},
    /* c4 */ {NOP},
    /* c5 */ {NOP},
    /* c6 */ {NOP},
    /* c7 */ {NOP},
    /* c8 */ {NOP},
    /* c9 */ {NOP},
    /* ca */ {NOP},
    /* cb */ {NOP},
    /* cc */ {NOP},
    /* cd */ {NOP},
    /* ce */ {NOP},
    /* cf */ {NOP},

    /* d0 */ {NOP},
    /* d1 */ {NOP},
    /* d2 */ {NOP},
    /* d3 */ {NOP},
    /* d4 */ {NOP},
    /* d5 */ {NOP},
    /* d6 */ {NOP},
    /* d7 */ {NOP},
    /* d8 */ {NOP},
    /* d9 */ {NOP},
    /* da */ {NOP},
    /* db */ {NOP},
    /* dc */ {NOP},
    /* dd */ {NOP},
    /* de */ {NOP},
    /* df */ {NOP},

    /* e0 */ {NOP},
    /* e1 */ {NOP},
    /* e2 */ {NOP},
    /* e3 */ {NOP},
    /* e4 */ {NOP},
    /* e5 */ {NOP},
    /* e6 */ {NOP},
    /* e7 */ {NOP},
    /* e8 */ {NOP},
    /* e9 */ {NOP},
    /* ea */ {NOP},
    /* eb */ {NOP},
    /* ec */ {NOP},
    /* ed */ {NOP},
    /* ee */ {NOP},
    /* ef */ {NOP},

    /* f0 */ {NOP},
    /* f1 */ {NOP},
    /* f2 */ {NOP},
    /* f3 */ {NOP},
    /* f4 */ {HLT, OPERAND_NONE, OPERAND_NONE, decode_instruction_no_operands},
    /* f5 */ {NOP},
    /* f6 */ {TEST, MEM_8, IMM_8, decode_instruction_with_mod_rm},
    /* f7 */ {TEST, MEM_16, IMM_16, decode_instruction_with_mod_rm},
    /* f8 */ {NOP},
    /* f9 */ {NOP},
    /* fa */ {NOP},
    /* fb */ {NOP},
    /* fc */ {NOP},
    /* fd */ {NOP},
    /* fe */ {NOP},
    /* ff */ {NOP},
};
// clang-format on

// clang-format off
struct op_code_mapping op_code_table_0f[] = {
    /* 00 */ {NOP},
    /* 01 */ {NOP},
    /* 02 */ {NOP},
    /* 03 */ {NOP},
    /* 04 */ {NOP},
    /* 05 */ {NOP},
    /* 06 */ {NOP},
    /* 07 */ {NOP},
    /* 08 */ {NOP},
    /* 09 */ {NOP},
    /* 0a */ {NOP},
    /* 0b */ {NOP},
    /* 0c */ {NOP},
    /* 0d */ {NOP},
    /* 0e */ {NOP},
    /* 0f */ {NOP},

    /* 10 */ {NOP},
    /* 11 */ {NOP},
    /* 12 */ {NOP},
    /* 13 */ {NOP},
    /* 14 */ {NOP},
    /* 15 */ {NOP},
    /* 16 */ {NOP},
    /* 17 */ {NOP},
    /* 18 */ {NOP},
    /* 19 */ {NOP},
    /* 1a */ {NOP},
    /* 1b */ {NOP},
    /* 1c */ {NOP},
    /* 1d */ {NOP},
    /* 1e */ {NOP},
    /* 1f */ {NOP},

    /* 20 */ {NOP},
    /* 21 */ {NOP},
    /* 22 */ {NOP},
    /* 23 */ {NOP},
    /* 24 */ {NOP},
    /* 25 */ {NOP},
    /* 26 */ {NOP},
    /* 27 */ {NOP},
    /* 28 */ {NOP},
    /* 29 */ {NOP},
    /* 2a */ {NOP},
    /* 2b */ {NOP},
    /* 2c */ {NOP},
    /* 2d */ {NOP},
    /* 2e */ {NOP},
    /* 2f */ {NOP},

    /* 30 */ {NOP},
    /* 31 */ {NOP},
    /* 32 */ {NOP},
    /* 33 */ {NOP},
    /* 34 */ {NOP},
    /* 35 */ {NOP},
    /* 36 */ {NOP},
    /* 37 */ {NOP},
    /* 38 */ {NOP},
    /* 39 */ {NOP},
    /* 3a */ {NOP},
    /* 3b */ {NOP},
    /* 3c */ {NOP},
    /* 3d */ {NOP},
    /* 3e */ {NOP},
    /* 3f */ {NOP},

    /* 40 */ {NOP},
    /* 41 */ {NOP},
    /* 42 */ {NOP},
    /* 43 */ {NOP},
    /* 44 */ {NOP},
    /* 45 */ {NOP},
    /* 46 */ {NOP},
    /* 47 */ {NOP},
    /* 48 */ {NOP},
    /* 49 */ {NOP},
    /* 4a */ {NOP},
    /* 4b */ {NOP},
    /* 4c */ {NOP},
    /* 4d */ {NOP},
    /* 4e */ {NOP},
    /* 4f */ {NOP},

    /* 50 */ {NOP},
    /* 51 */ {NOP},
    /* 52 */ {NOP},
    /* 53 */ {NOP},
    /* 54 */ {NOP},
    /* 55 */ {NOP},
    /* 56 */ {NOP},
    /* 57 */ {NOP},
    /* 58 */ {NOP},
    /* 59 */ {NOP},
    /* 5a */ {NOP},
    /* 5b */ {NOP},
    /* 5c */ {NOP},
    /* 5d */ {NOP},
    /* 5e */ {NOP},
    /* 5f */ {NOP},

    /* 60 */ {NOP},
    /* 61 */ {NOP},
    /* 62 */ {NOP},
    /* 63 */ {NOP},
    /* 64 */ {NOP},
    /* 65 */ {NOP},
    /* 66 */ {NOP},
    /* 67 */ {NOP},
    /* 68 */ {NOP},
    /* 69 */ {NOP},
    /* 6a */ {NOP},
    /* 6b */ {NOP},
    /* 6c */ {NOP},
    /* 6d */ {NOP},
    /* 6e */ {NOP},
    /* 6f */ {NOP},

    /* 70 */ {NOP},
    /* 71 */ {NOP},
    /* 72 */ {NOP},
    /* 73 */ {NOP},
    /* 74 */ {NOP},
    /* 75 */ {NOP},
    /* 76 */ {NOP},
    /* 77 */ {NOP},
    /* 78 */ {NOP},
    /* 79 */ {NOP},
    /* 7a */ {NOP},
    /* 7b */ {NOP},
    /* 7c */ {NOP},
    /* 7d */ {NOP},
    /* 7e */ {NOP},
    /* 7f */ {NOP},

    /* 80 */ {NOP},
    /* 81 */ {NOP},
    /* 82 */ {NOP},
    /* 83 */ {NOP},
    /* 84 */ {JE, JMP_16, OPERAND_NONE, decode_instruction_no_mod_rm},
    /* 85 */ {NOP},
    /* 86 */ {NOP},
    /* 87 */ {NOP},
    /* 88 */ {NOP},
    /* 89 */ {NOP},
    /* 8a */ {NOP},
    /* 8b */ {NOP},
    /* 8c */ {NOP},
    /* 8d */ {NOP},
    /* 8e */ {NOP},
    /* 8f */ {NOP},

    /* 90 */ {NOP},
    /* 91 */ {NOP},
    /* 92 */ {NOP},
    /* 93 */ {NOP},
    /* 94 */ {NOP},
    /* 95 */ {NOP},
    /* 96 */ {NOP},
    /* 97 */ {NOP},
    /* 98 */ {NOP},
    /* 99 */ {NOP},
    /* 9a */ {NOP},
    /* 9b */ {NOP},
    /* 9c */ {NOP},
    /* 9d */ {NOP},
    /* 9e */ {NOP},
    /* 9f */ {NOP},

    /* a0 */ {NOP},
    /* a1 */ {NOP},
    /* a2 */ {NOP},
    /* a3 */ {NOP},
    /* a4 */ {NOP},
    /* a5 */ {NOP},
    /* a6 */ {NOP},
    /* a7 */ {NOP},
    /* a8 */ {NOP},
    /* a9 */ {NOP},
    /* aa */ {NOP},
    /* ab */ {NOP},
    /* ac */ {NOP},
    /* ad */ {NOP},
    /* ae */ {NOP},
    /* af */ {NOP},

    /* b0 */ {NOP},
    /* b1 */ {NOP},
    /* b2 */ {NOP},
    /* b3 */ {NOP},
    /* b4 */ {NOP},
    /* b5 */ {NOP},
    /* b6 */ {NOP},
    /* b7 */ {NOP},
    /* b8 */ {NOP},
    /* b9 */ {NOP},
    /* ba */ {NOP},
    /* bb */ {NOP},
    /* bc */ {NOP},
    /* bd */ {NOP},
    /* be */ {NOP},
    /* bf */ {NOP},

    /* c0 */ {NOP},
    /* c1 */ {NOP},
    /* c2 */ {NOP},
    /* c3 */ {NOP},
    /* c4 */ {NOP},
    /* c5 */ {NOP},
    /* c6 */ {NOP},
    /* c7 */ {NOP},
    /* c8 */ {NOP},
    /* c9 */ {NOP},
    /* ca */ {NOP},
    /* cb */ {NOP},
    /* cc */ {NOP},
    /* cd */ {NOP},
    /* ce */ {NOP},
    /* cf */ {NOP},

    /* d0 */ {NOP},
    /* d1 */ {NOP},
    /* d2 */ {NOP},
    /* d3 */ {NOP},
    /* d4 */ {NOP},
    /* d5 */ {NOP},
    /* d6 */ {NOP},
    /* d7 */ {NOP},
    /* d8 */ {NOP},
    /* d9 */ {NOP},
    /* da */ {NOP},
    /* db */ {NOP},
    /* dc */ {NOP},
    /* dd */ {NOP},
    /* de */ {NOP},
    /* df */ {NOP},

    /* e0 */ {NOP},
    /* e1 */ {NOP},
    /* e2 */ {NOP},
    /* e3 */ {NOP},
    /* e4 */ {NOP},
    /* e5 */ {NOP},
    /* e6 */ {NOP},
    /* e7 */ {NOP},
    /* e8 */ {NOP},
    /* e9 */ {NOP},
    /* ea */ {NOP},
    /* eb */ {NOP},
    /* ec */ {NOP},
    /* ed */ {NOP},
    /* ee */ {NOP},
    /* ef */ {NOP},

    /* f0 */ {NOP},
    /* f1 */ {NOP},
    /* f2 */ {NOP},
    /* f3 */ {NOP},
    /* f4 */ {NOP},
    /* f5 */ {NOP},
    /* f6 */ {NOP},
    /* f7 */ {NOP},
    /* f8 */ {NOP},
    /* f9 */ {NOP},
    /* fa */ {NOP},
    /* fb */ {NOP},
    /* fc */ {NOP},
    /* fd */ {NOP},
    /* fe */ {NOP},
    /* ff */ {NOP},
};
// clang-format on

int decode_instruction(const u8 *buffer, unsigned buffer_size, struct instruction *instruction) {
  assert(buffer_size > 0);

  int instruction_size = 0;

  u8 op_code = buffer[0];
  struct op_code_mapping *mapping;
  if (op_code != 0x0f) {
    mapping = &op_code_table[op_code];
  } else {
    instruction_size += 1;
    buffer += 1;
    op_code = buffer[0];
    mapping = &op_code_table_0f[op_code];
  }

  if (mapping->instruction_type == NOP) {
    return -1;
  }

  if (mapping->decoder_func != 0) {
    memset(instruction, 0, sizeof(struct instruction));
    instruction_size += mapping->decoder_func(mapping, buffer, buffer_size, instruction);
    return instruction_size;
  } else {
    return -2;
  }
}
