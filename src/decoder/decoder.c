#include "decoder.h"

#include "../address.h"
#include "../mod_rm.h"
#include "../print_format.h"
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

static inline enum register_8 encoding_to_register_8(enum mod_rm_reg encoding) {
  static const enum register_8 mapping[] = {
      AL, CL, DL, BL, AH, CH, DH, BH,
  };

  return mapping[encoding];
}

static inline enum register_16 encoding_to_register_16(enum mod_rm_reg encoding) {
  // 16-bit registers are equal to their encoding.
  return (enum register_16)encoding;
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

int decode_mem_operand(struct mod_rm mrm, enum operand_size size, struct input_stream *reader,
                       struct operand *result) {
  result->size = size;

  switch (mrm.mod) {
    case mod_rm_mod_indirect: {
      enum indirect_encoding indirect_encoding = (enum indirect_encoding)mrm.reg_mem.as_mem;
      if (indirect_encoding == ie_bp) {
        result->type = ot_direct;
        result->data.as_direct.address = input_stream_fetch_u16(reader);
        return 2;
      } else {
        result->type = ot_indirect;
        result->data.as_indirect.encoding = indirect_encoding;
        return 0;
      }
    }

    case mod_rm_mod_byte: {
      result->type = ot_displacement;
      result->data.as_indirect.encoding = (enum indirect_encoding)mrm.reg_mem.as_mem;
      result->data.as_displacement.displacement = (i16)input_stream_fetch_i8(reader);
      return 1;
    }

    case mod_rm_mod_word: {
      result->type = ot_displacement;
      result->data.as_indirect.encoding = (enum indirect_encoding)mrm.reg_mem.as_mem;
      result->data.as_displacement.displacement = input_stream_fetch_i16(reader);
      return 2;
    }

    case mod_rm_mod_register: {
      result->type = ot_register;
      switch (size) {
        case os_8:
          result->data.as_register.reg_8 = encoding_to_register_8(mrm.reg_mem.as_reg);
          return 0;

        case os_16:
          result->data.as_register.reg_16 = encoding_to_register_16(mrm.reg_mem.as_reg);
          return 0;

        default:
          break;
      }
    }

    default:
      break;
  }

  assert(0);
  return 0;
}

void decode_operand_common(enum decode_type decode_type, struct input_stream *reader,
                           struct operand *result) {
  switch (decode_type) {
    case DT_IMM_8: {
      result->type = ot_immediate;
      result->size = os_8;
      result->data.as_immediate.immediate_8 = input_stream_fetch_u8(reader);
      break;
    }

    case DT_IMM_16: {
      result->type = ot_immediate;
      result->size = os_16;
      result->data.as_immediate.immediate_16 = input_stream_fetch_u16(reader);
      break;
    }

    case DT_JMP_8: {
      result->type = ot_jump;
      result->size = os_8;
      result->data.as_jump.offset = (i16)input_stream_fetch_i8(reader);
      break;
    }

    case DT_JMP_16: {
      result->type = ot_jump;
      result->size = os_16;
      result->data.as_jump.offset = input_stream_fetch_i16(reader);
      break;
    }

    case DT_NONE:
      result->type = ot_none;
      break;

    default:
      printf("Invalid operand decode type\n");
      assert(0);
      break;
  }
}

void decode_operand(u8 op_code, enum decode_type decode_type, struct input_stream *reader,
                    struct operand *result) {
  switch (decode_type) {
    case DT_OP_CODE_REG_8: {
      result->type = ot_register;
      result->size = os_8;
      result->data.as_register.reg_8 = encoding_to_register_8(MOD_RM_REG_MEM(op_code));
      break;
    }

    case DT_OP_CODE_REG_16: {
      result->type = ot_register;
      result->size = os_16;
      result->data.as_register.reg_16 = encoding_to_register_16(MOD_RM_REG_MEM(op_code));
      break;
    }

    case DT_AL: {
      result->type = ot_register;
      result->size = os_8;
      result->data.as_register.reg_8 = AL;
      break;
    }

    case DT_AX: {
      result->type = ot_register;
      result->size = os_16;
      result->data.as_register.reg_16 = AX;
      break;
    }

    case DT_DX: {
      result->type = ot_register;
      result->size = os_16;
      result->data.as_register.reg_16 = DX;
      break;
    }

    case DT_SEGMENT_REG: { // Duplicated
      result->type = ot_segment_register;
      result->size = 16;
      result->data.as_segment_register.reg = encoding_to_segment_register(MOD_RM_REG(op_code));
      break;
    }

    case DT_SEG_DIRECT: {
      result->type = ot_direct_with_segment;
      result->size = 16;
      u16 offset = input_stream_fetch_u16(reader);
      u16 segment = input_stream_fetch_u16(reader);
      result->data.as_direct_with_segment.address = segment_offset(segment, offset);
      break;
    }

    default:
      decode_operand_common(decode_type, reader, result);
      break;
  }
}

void decode_operand_with_mod_rm(enum decode_type decode_type, struct input_stream *reader,
                                struct operand *result) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(reader));

  switch (decode_type) {
    case DT_MOD_RM_REG_8: {
      result->type = ot_register;
      result->size = os_8;
      result->data.as_register.reg_8 = encoding_to_register_8(mrm.reg_mem.as_reg);
      break;
    }

    case DT_MOD_RM_REG_16: {
      result->type = ot_register;
      result->size = os_16;
      result->data.as_register.reg_16 = encoding_to_register_16(mrm.reg_mem.as_reg);
      break;
    }

    case DT_MOD_RM_RM_8: {
      decode_mem_operand(mrm, os_8, reader, result);
      break;
    }

    case DT_MOD_RM_RM_16: {
      decode_mem_operand(mrm, os_16, reader, result);
      break;
    }

    case DT_SEGMENT_REG: { // Duplicated
      result->type = ot_segment_register;
      result->size = 16;
      result->data.as_segment_register.reg = encoding_to_segment_register(mrm.reg);
      break;
    }

    default:
      decode_operand_common(decode_type, reader, result);
      break;
  }
}

struct instruction_reader_writer {
  struct input_stream *original;
  struct output_stream instruction_stream;
};

u8 instruction_reader_writer_fetch_u8(void *context, u32 position) {
  struct instruction_reader_writer *irw = context;

  u8 value = input_stream_fetch_u8(irw->original);
  output_stream_store_u8(&irw->instruction_stream, value);

  return value;
}

void decode_instruction(struct input_stream *reader, struct instruction *instruction) {
#if !defined(NDEBUG)
  struct input_stream wrapper;
  struct instruction_reader_writer irw;
  irw.original = reader;
  output_stream_init(&irw.instruction_stream, &instruction->buffer,
                     output_stream_flat_memory_store);

  input_stream_init(&wrapper, &irw, instruction_reader_writer_fetch_u8);

  reader = &wrapper;
#endif

  u8 op_code = input_stream_fetch_u8(reader);

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

      default:
        assert(0);
        break;
    }

    op_code = input_stream_fetch_u8(reader);
  }

  struct op_code_mapping *mapping = &op_code_table[op_code];

  instruction->type = mapping->instruction_type;

  if (mapping->flags & DF_HAS_MOD_RM) {
    decode_operand_with_mod_rm(mapping->destination_type, reader, &instruction->destination);
    decode_operand_with_mod_rm(mapping->source_type, reader, &instruction->source);
    decode_operand_with_mod_rm(mapping->third_type, reader, &instruction->third);
  } else {
    decode_operand(op_code, mapping->destination_type, reader, &instruction->destination);
    decode_operand(op_code, mapping->source_type, reader, &instruction->source);
    decode_operand(op_code, mapping->third_type, reader, &instruction->third);
  }

#if !defined(NDEBUG)
  instruction->instruction_size = irw.instruction_stream.position;
#endif
}
