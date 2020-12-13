#include "decoder.h"

#include "mod_rm.h"
#include "op_code_table.h"

#include <assert.h>

typedef void (*prefix_func)(u8, struct instruction *);

void prefix_segment_override(u8 prefix, struct instruction *instruction) {
  switch (prefix) {
    case 0x2e:
      instruction->segment_register = CS;
      break;

    case 0x26:
      instruction->segment_register = ES;
      break;

    case 0x3e:
      instruction->segment_register = DS;
      break;

    case 0x36:
      instruction->segment_register = SS;
      break;

    default:
      assert(0);
  }
}

struct prefix_func_table {
  u8 prefix_code;
  prefix_func func;
} prefix_func_table[] = {
    {0x2e, prefix_segment_override},
    {0x36, prefix_segment_override},
};

int decode_instruction_no_operands(struct op_code_mapping *mapping, const u8 *buffer,
                                   unsigned buffer_size, struct instruction *instruction) {
  instruction->type = mapping->instruction_type;
  instruction->destination.mode = OPERAND_MODE_NONE;
  instruction->source.mode = OPERAND_MODE_NONE;

  return 1;
}

int decode_instruction_no_mod_rm(struct op_code_mapping *mapping, const u8 *buffer,
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

    case REG_SEG: {
      instruction->destination.mode = OPERAND_MODE_SEGMENT_REGISTER;
      instruction->destination.size = OPERAND_SIZE_16;
      instruction->destination.segment_reg = op_code >> 3 & 0b111;
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

    case IMM_8:
      instruction->destination.mode = OPERAND_MODE_IMMEDIATE;
      instruction->destination.size = OPERAND_SIZE_8;
      instruction->destination.immediate8 = buffer[1];
      instruction_size += 1;
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

    default:
      assert(0);
  }

  return -1;
}

static int decode_immediate_operand(struct mod_reg_rm mrm, enum operand_size operand_size,
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

  return -1;
}

static int decode_segment_register_operand(struct mod_reg_rm mrm, const u8 *buffer,
                                           struct operand *operand) {
  operand->mode = OPERAND_MODE_SEGMENT_REGISTER;
  operand->size = OPERAND_SIZE_16;
  operand->segment_reg = mrm.reg;

  return 0;
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
      return decode_immediate_operand(mrm, OPERAND_SIZE_8, buffer, operand);

    case IMM_16:
      return decode_immediate_operand(mrm, OPERAND_SIZE_16, buffer, operand);

    case REG_SEG:
      return decode_segment_register_operand(mrm, buffer, operand);

    default:
      assert(0);
  }

  return -1;
}

int decode_instruction_with_mod_rm(struct op_code_mapping *mapping, const u8 *buffer,
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

int decode_instruction(const u8 *buffer, unsigned buffer_size, struct instruction *instruction) {
  assert(buffer_size > 0);

  int instruction_size = 0;

  u8 op_code = buffer[0];

  // If the `op_code` equals one of the prefix codes, then we advance the buffer by one and call
  // the `decode_instruction` function again, leaving the current instruction state in tact.  Then
  // We return the instruction size of that call + 1 for the prefix byte we processed.
  for (unsigned i = 0; i < sizeof(prefix_func_table) / sizeof(struct prefix_func_table); ++i) {
    if (op_code == prefix_func_table[i].prefix_code) {
      prefix_func_table[i].func(op_code, instruction);
      return 1 + decode_instruction(buffer + 1, buffer_size - 1, instruction);
    }
  }

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
    return instruction_size + mapping->decoder_func(mapping, buffer, buffer_size, instruction);
  } else {
    return -2;
  }
}
