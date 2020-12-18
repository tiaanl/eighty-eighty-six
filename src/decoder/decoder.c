#include "decoder.h"

#include "../address.h"
#include "../print_format.h"
#include "mod_reg_rm.h"
#include "op_code_table.h"

#include <assert.h>
#include <stdio.h>

typedef void (*prefix_func)(u8, struct instruction *);

static void dump_buffer(const u8 *buffer, unsigned buffer_size) {
  for (unsigned i = 0; i < buffer_size; ++i) {
    printf(HEX_8 " ", buffer[i]);
  }
  printf("\n");
}

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
    {0x26, prefix_segment_override},
    {0x2e, prefix_segment_override},
    {0x36, prefix_segment_override},
    {0x3e, prefix_segment_override},
};

static inline enum register_8 encoding_to_register_8(u8 encoding) {
  static const enum register_8 mapping[] = {
      AL, CL, DL, BL, AH, CH, DH, BH,
  };

  return mapping[encoding];
}

static inline enum register_16 encoding_to_register_16(u8 encoding) {
  static const enum register_16 mapping[] = {
      AX, CX, DX, BX, SP, BP, SI, DI,
  };

  return mapping[encoding];
}

static inline enum segment_register encoding_to_segment_register(u8 encoding) {
  static const enum segment_register mapping[] = {ES, CS, SS, DS};

  return mapping[encoding];
}

struct decode_context {
  const u8 *buffer;
  const unsigned buffer_size;
  u8 flags;
  unsigned data_read;
};

int decode_mem_operand(enum operand_size size, const u8 *buffer, unsigned buffer_size,
                       unsigned data_offset, struct operand *result) {
  result->size = size;
  u8 mod_rm_byte = buffer[1];

  enum mrrm_mod mod = decode_mrrm_mod(mod_rm_byte);

  switch (mod) {
    case MRRM_MOD_INDIRECT: {
      enum indirect_encoding indirect_encoding = mod_rm_byte & 0x07;
      if (indirect_encoding == IE_BP) {
        result->type = OT_DIRECT;
        result->as_direct.address = read_u16(buffer + data_offset, buffer_size - data_offset);
        return 2;
      } else {
        result->type = OT_INDIRECT;
        result->as_indirect.encoding = indirect_encoding;
        return 0;
      }
    }

    case MRRM_MOD_BYTE: {
      result->type = OT_DISPLACEMENT;
      result->as_indirect.encoding = mod_rm_byte & 0x07;
      result->as_displacement.displacement =
          (i16)read_i8(buffer + data_offset, buffer_size - data_offset);
      return 1;
    }

    case MRRM_MOD_DOUBLE_WORD: {
      result->type = OT_DISPLACEMENT;
      result->as_indirect.encoding = mod_rm_byte & 0x07;
      result->as_displacement.displacement =
          read_i16(buffer + data_offset, buffer_size - data_offset);
      return 2;
    }

    case MRRM_MOD_REGISTER: {
      result->type = OT_REGISTER;
      switch (size) {
        case OS_8:
          result->as_register.reg_8 = encoding_to_register_8(decode_mrrm_reg_from_rm(mod_rm_byte));
          return 0;

        case OS_16:
          result->as_register.reg_16 =
              encoding_to_register_16(decode_mrrm_reg_from_rm(mod_rm_byte));
          return 0;

        default:
          break;
      }
    }

    default:
      break;
  }

  assert(0);
}

int decode_operand_common(enum decode_type decode_type, const u8 *buffer, unsigned buffer_size,
                          unsigned data_offset, struct operand *result) {
  switch (decode_type) {
    case DT_IMM_8: {
      result->type = OT_IMMEDIATE;
      result->size = OS_8;
      result->as_immediate.immediate_8 = read_u8(buffer + data_offset, buffer_size - data_offset);
      return 1;
    }

    case DT_IMM_16: {
      result->type = OT_IMMEDIATE;
      result->size = OS_16;
      result->as_immediate.immediate_16 = read_u16(buffer + data_offset, buffer_size - data_offset);
      return 2;
    }

    case DT_JMP_8: {
      result->type = OT_JUMP;
      result->size = OS_8;
      result->as_jump.offset = (i16)read_i8(buffer + data_offset, buffer_size - data_offset);
      return 1;
    }

    case DT_JMP_16: {
      result->type = OT_JUMP;
      result->size = OS_16;
      result->as_jump.offset = read_i16(buffer + data_offset, buffer_size - data_offset);
      return 2;
    }

    case DT_NONE:
      result->type = OT_NONE;
      return 0;

    default:
      printf("Invalid operand decode type\n");
      dump_buffer(buffer, buffer_size);
      assert(0);
  }
}

int decode_operand(enum decode_type decode_type, const u8 *buffer, unsigned buffer_size,
                   unsigned data_offset, struct operand *result) {
  switch (decode_type) {
    case DT_OP_CODE_REG_8: {
      result->type = OT_REGISTER;
      result->size = OS_8;
      result->as_register.reg_8 = encoding_to_register_8(buffer[0] & 0x07);
      return 0;
    }

    case DT_OP_CODE_REG_16: {
      result->type = OT_REGISTER;
      result->size = OS_16;
      result->as_register.reg_16 = encoding_to_register_16(buffer[0] & 0x07);
      return 0;
    }

    case DT_AL: {
      result->type = OT_REGISTER;
      result->size = OS_8;
      result->as_register.reg_8 = AL;
      return 0;
    }

    case DT_AX: {
      result->type = OT_REGISTER;
      result->size = OS_16;
      result->as_register.reg_16 = AX;
      return 0;
    }

    case DT_DX: {
      result->type = OT_REGISTER;
      result->size = OS_16;
      result->as_register.reg_16 = DX;
      return 0;
    }

    case DT_SEGMENT_REG: { // Duplicated
      result->type = OT_SEGMENT_REGISTER;
      result->size = 16;
      result->as_segment_register.reg = encoding_to_segment_register(buffer[0] >> 3 & 0x07);
      return 0;
    }

    case DT_SEG_DIRECT: {
      result->type = OT_DIRECT_WITH_SEGMENT;
      result->size = 16;
      u16 offset = read_u16(buffer + data_offset, buffer_size - data_offset);
      u16 segment = read_u16(buffer + data_offset + 2, buffer_size - data_offset - 2);
      result->as_direct_with_segment.address = segment_offset(segment, offset);
      return 0;
    }

    default:
      return decode_operand_common(decode_type, buffer, buffer_size, data_offset, result);
  }
}

int decode_operand_with_mod_rm(enum decode_type decode_type, const u8 *buffer, unsigned buffer_size,
                               unsigned data_offset, struct operand *result) {
  u8 mod_rm_byte = buffer[1];

  switch (decode_type) {
    case DT_MOD_RM_REG_8: {
      result->type = OT_REGISTER;
      result->size = OS_8;
      result->as_register.reg_8 = encoding_to_register_8(mod_rm_byte >> 3 & 0x07);
      return 0;
    }

    case DT_MOD_RM_REG_16: {
      result->type = OT_REGISTER;
      result->size = OS_16;
      result->as_register.reg_16 = encoding_to_register_16(mod_rm_byte >> 3 & 0x07);
      return 0;
    }

    case DT_MOD_RM_RM_8: {
      return decode_mem_operand(OS_8, buffer, buffer_size, data_offset, result);
    }

    case DT_MOD_RM_RM_16: {
      return decode_mem_operand(OS_16, buffer, buffer_size, data_offset, result);
    }

    case DT_SEGMENT_REG: { // Duplicated
      result->type = OT_SEGMENT_REGISTER;
      result->size = 16;
      result->as_segment_register.reg = encoding_to_segment_register(buffer[1] >> 3 & 0x07);
      return 0;
    }

    default:
      return decode_operand_common(decode_type, buffer, buffer_size, data_offset, result);
  }
}

int decode_instruction(const u8 *buffer, unsigned buffer_size, struct instruction *instruction) {
  assert(buffer_size > 0);

  unsigned data_offset = 1;
  u8 op_code = buffer[0];

  instruction->segment_register = DS;
  if (op_code == 0x2e || op_code == 0x26 || op_code == 0x3e || op_code == 0x36) {
    switch (op_code) {
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
    }

    buffer++;
    buffer_size--;
    // data_offset++;
    op_code = buffer[0];
  }

  struct op_code_mapping *mapping = &op_code_table[op_code];

  instruction->type = mapping->instruction_type;

  if (mapping->flags & DF_HAS_MOD_RM) {
    data_offset++;
    data_offset += decode_operand_with_mod_rm(mapping->destination_type, buffer, buffer_size,
                                              data_offset, &instruction->destination);
    data_offset += decode_operand_with_mod_rm(mapping->source_type, buffer, buffer_size,
                                              data_offset, &instruction->source);
    data_offset += decode_operand_with_mod_rm(mapping->third_type, buffer, buffer_size, data_offset,
                                              &instruction->third);
  } else {
    data_offset += decode_operand(mapping->destination_type, buffer, buffer_size, data_offset,
                                  &instruction->destination);
    data_offset += decode_operand(mapping->source_type, buffer, buffer_size, data_offset,
                                  &instruction->source);
    data_offset +=
        decode_operand(mapping->third_type, buffer, buffer_size, data_offset, &instruction->third);
  }
  return data_offset;

  return -1;
}
