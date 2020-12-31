
#include "decoder_funcs.h"

#include "mod_rm.h"

#include <stdbool.h>

static void operand_from_mod_rm_reg(struct operand *operand, enum operand_size size,
                                    enum mod_rm_reg reg) {
  static enum register_8 reg_8_mapping[] = {
      AL, CL, DL, BL, AH, CH, DH, BH,
  };

  // This mapping is identical to the registers enum, but its here for completeness sake.
  static enum register_16 reg_16_mapping[] = {
      AX, CX, DX, BX, SP, BP, SI, DI,
  };

  operand->type = ot_register;
  operand->size = size;
  switch (size) {
    case os_8:
      operand->data.as_register.reg_8 = reg_8_mapping[reg];
      break;

    case os_16:
      operand->data.as_register.reg_16 = reg_16_mapping[reg];
      break;

    default:
      assert(0);
      break;
  }
}

#if defined(TESTING)

#endif

static void operand_from_mod_rm_reg_mem(struct operand *operand, enum operand_size size,
                                        struct input_stream *stream, enum mod_rm_mod mod,
                                        union mod_rm_reg_mem reg_mem, enum segment_register seg_reg,
                                        bool mem_only) {
  if (mem_only && mod == mod_rm_mod_register) {
    assert(0);
    return;
  }

  switch (mod) {
    case mod_rm_mod_indirect:
      if (reg_mem.as_mem == mod_rm_mem_bp) {
        operand->type = ot_direct;
        operand->size = size;
        operand->data.as_direct.seg_reg = seg_reg;
        operand->data.as_direct.address = input_stream_fetch_u16(stream);
      } else {
        operand->type = ot_indirect;
        operand->size = size;
        operand->data.as_indirect.seg_reg = seg_reg;
        operand->data.as_indirect.encoding = (enum indirect_memory_encoding)reg_mem.as_mem;
      }
      break;

    case mod_rm_mod_byte:
      operand->type = ot_displacement;
      operand->size = size;
      operand->data.as_displacement.seg_reg = seg_reg;
      operand->data.as_displacement.encoding = (enum indirect_memory_encoding)reg_mem.as_mem;
      operand->data.as_displacement.displacement = (i16)input_stream_fetch_i8(stream);
      break;

    case mod_rm_mod_word:
      operand->type = ot_displacement;
      operand->size = size;
      operand->data.as_displacement.seg_reg = seg_reg;
      operand->data.as_displacement.encoding = (enum indirect_memory_encoding)reg_mem.as_mem;
      operand->data.as_displacement.displacement = input_stream_fetch_i16(stream);
      break;

    case mod_rm_mod_register:
      operand_from_mod_rm_reg(operand, size, reg_mem.as_reg);
      break;

    default:
      assert(0);
      break;
  }
}

static void operand_from_register_8(struct operand *operand, enum operand_size size,
                                    enum register_8 reg) {
  operand->type = ot_register;
  operand->size = size;
  operand->data.as_register.reg_8 = reg;
}

static void operand_from_register_16(struct operand *operand, enum operand_size size,
                                     enum register_16 reg) {
  operand->type = ot_register;
  operand->size = size;
  operand->data.as_register.reg_16 = reg;
}

static void operand_from_immediate(struct operand *operand, enum operand_size size,
                                   struct input_stream *stream) {
  operand->type = ot_immediate;
  operand->size = size;
  switch (size) {
    case os_8:
      operand->data.as_immediate.immediate_8 = input_stream_fetch_u8(stream);
      break;

    case os_16:
      operand->data.as_immediate.immediate_16 = input_stream_fetch_u16(stream);
      break;

    default:
      assert(0);
      break;
  }
}

void operand_from_segment_register(struct operand *operand, enum segment_register seg_reg) {
  operand->type = ot_segment_register;
  operand->data.as_segment_register.reg = seg_reg;
}

void operand_none(struct operand *operand) {
  operand->type = ot_none;
  operand->size = os_none;
}

void operand_from_jump(struct operand *operand, enum operand_size size,
                       struct input_stream *stream) {
  operand->type = ot_jump;
  operand->size = size;
  switch (operand->size) {
    case os_8:
      operand->data.as_jump.offset = (i16)input_stream_fetch_i8(stream);
      break;

    case os_16:
      operand->data.as_jump.offset = input_stream_fetch_i16(stream);
      break;

    default:
      assert(0);
  }
}

void operand_from_far_jump(struct operand *operand, struct input_stream *stream) {
  operand->type = ot_far_jump;
  operand->size = os_16;

  operand->data.as_far_jump.offset = input_stream_fetch_u16(stream);
  operand->data.as_far_jump.segment = input_stream_fetch_u16(stream);
}

void operand_from_mod_rm_seg_reg(struct operand *operand, enum operand_size size,
                                 enum mod_rm_reg reg) {
  operand->type = ot_segment_register;
  operand->size = size;
  operand->data.as_segment_register.reg = (enum segment_register)reg;
}

void operand_from_flags_register(struct operand *operand) {
  operand->type = ot_flags;
  operand->size = os_none;
}

void operand_from_offset(struct operand *operand, enum operand_size size,
                         struct input_stream *stream, enum segment_register seg_reg) {
  operand->type = ot_offset;
  operand->size = size;

  operand->data.as_offset.seg_reg = seg_reg;

  switch (size) {
    case os_8:
      operand->data.as_offset.offset = (i16)input_stream_fetch_i8(stream);
      break;

    case os_16:
      operand->data.as_offset.offset = input_stream_fetch_i16(stream);
      break;

    default:
      assert(0);
      break;
  }
}

void operand_from_constant(struct operand *operand, enum operand_size size, u16 constant) {
  operand->type = ot_immediate;
  operand->size = size;
  switch (size) {
    case os_8:
      operand->data.as_immediate.immediate_8 = constant & 0xff;
      break;

    case os_16:
      operand->data.as_immediate.immediate_16 = constant;
      break;

    default:
      assert(0);
      break;
  }
}

void operand_from_ds_si(struct operand *operand, enum operand_size size) {
  operand->type = ot_ds_si;
  operand->size = size;
}

void operand_from_es_di(struct operand *operand, enum operand_size size) {
  operand->type = ot_es_di;
  operand->size = size;
}

/* ============================================================================================== */

void decode_Xx_Xx_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_none(&instruction->destination);
  operand_none(&instruction->source);
  operand_none(&instruction->third);
}

void decode_Eb_Gb_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg_mem(&instruction->destination, os_8, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_from_mod_rm_reg(&instruction->source, os_8, mrm.reg);
  operand_none(&instruction->third);
}

void decode_Ew_Gw_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg_mem(&instruction->destination, os_16, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_from_mod_rm_reg(&instruction->source, os_16, mrm.reg);
  operand_none(&instruction->third);
}

void decode_Gb_Eb_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg(&instruction->destination, os_8, mrm.reg);
  operand_from_mod_rm_reg_mem(&instruction->source, os_8, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_none(&instruction->third);
}

void decode_Gw_Ew_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg(&instruction->destination, os_16, mrm.reg);
  operand_from_mod_rm_reg_mem(&instruction->source, os_16, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_none(&instruction->third);
}

void decode_AL_Ib_Xx(struct input_stream *stream, struct instruction *instruction) {
  operand_from_register_8(&instruction->destination, os_8, AL);
  operand_from_immediate(&instruction->source, os_8, stream);
  operand_none(&instruction->third);
}

void decode_AX_Iw_Xx(struct input_stream *stream, struct instruction *instruction) {
  operand_from_register_16(&instruction->destination, os_16, AX);
  operand_from_immediate(&instruction->source, os_16, stream);
  operand_none(&instruction->third);
}

void decode_ES_Xx_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_segment_register(&instruction->destination, ES);
  operand_none(&instruction->source);
  operand_none(&instruction->third);
}

void decode_CS_Xx_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_segment_register(&instruction->destination, CS);
  operand_none(&instruction->source);
  operand_none(&instruction->third);
}

void decode_SS_Xx_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_segment_register(&instruction->destination, SS);
  operand_none(&instruction->source);
  operand_none(&instruction->third);
}

void decode_DS_Xx_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_segment_register(&instruction->destination, DS);
  operand_none(&instruction->source);
  operand_none(&instruction->third);
}

void decode_Ib_Xx_Xx(struct input_stream *stream, struct instruction *instruction) {
  operand_from_immediate(&instruction->destination, os_8, stream);
  operand_none(&instruction->source);
  operand_none(&instruction->third);
}

void decode_Iw_Xx_Xx(struct input_stream *stream, struct instruction *instruction) {
  operand_from_immediate(&instruction->destination, os_16, stream);
  operand_none(&instruction->source);
  operand_none(&instruction->third);
}

void decode_Jb_Xx_Xx(struct input_stream *stream, struct instruction *instruction) {
  operand_from_jump(&instruction->destination, os_8, stream);
  operand_none(&instruction->source);
  operand_none(&instruction->third);
}

void decode_Gw_Ma_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg(&instruction->destination, os_16, mrm.reg);
  operand_from_mod_rm_reg_mem(&instruction->source, os_16, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, true);
  operand_none(&instruction->third);
}

void decode_Gw_Ew_Ib(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg(&instruction->destination, os_16, mrm.reg);
  operand_from_mod_rm_reg_mem(&instruction->source, os_16, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_from_immediate(&instruction->third, os_8, stream);
}

void decode_Gw_Ew_Iw(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg(&instruction->destination, os_16, mrm.reg);
  operand_from_mod_rm_reg_mem(&instruction->source, os_16, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_from_immediate(&instruction->third, os_16, stream);
}

void decode_Yb_DX_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_es_di(&instruction->destination, os_8);
  operand_from_register_16(&instruction->source, os_8, DX);
  operand_none(&instruction->third);
}

void decode_Yw_DX_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_es_di(&instruction->destination, os_16);
  operand_from_register_16(&instruction->source, os_8, DX);
  operand_none(&instruction->third);
}

void decode_DX_Xb_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_register_16(&instruction->destination, os_8, DX);
  operand_from_register_16(&instruction->source, os_8, SI);
  operand_none(&instruction->third);
}

void decode_DX_Xw_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_register_16(&instruction->destination, os_16, DX);
  operand_from_ds_si(&instruction->source, os_16);
  operand_none(&instruction->third);
}

void decode_Eb_Ib_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg_mem(&instruction->destination, os_8, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_from_immediate(&instruction->source, os_8, stream);
  operand_none(&instruction->third);
}

void decode_Ew_Iw_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg_mem(&instruction->destination, os_16, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_from_immediate(&instruction->source, os_16, stream);
  operand_none(&instruction->third);
}

void decode_Ew_Ib_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg_mem(&instruction->destination, os_16, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_from_immediate(&instruction->source, os_8, stream);
  operand_none(&instruction->third);
}

void decode_Ew_Sw_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg_mem(&instruction->destination, os_16, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_from_mod_rm_seg_reg(&instruction->source, os_16, mrm.reg);
  operand_none(&instruction->third);
}

void decode_Gw_M_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg(&instruction->destination, os_16, mrm.reg);
  operand_from_mod_rm_reg_mem(&instruction->source, os_16, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, true);
  operand_none(&instruction->third);
}

void decode_Sw_Ew_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_seg_reg(&instruction->destination, os_16, mrm.reg);
  operand_from_mod_rm_reg_mem(&instruction->source, os_16, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_none(&instruction->third);
}

void decode_Eb_Xx_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg_mem(&instruction->destination, os_8, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_none(&instruction->source);
  operand_none(&instruction->third);
}

void decode_Ew_Xx_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg_mem(&instruction->destination, os_16, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_none(&instruction->source);
  operand_none(&instruction->third);
}

void decode_AX_DX_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_register_16(&instruction->destination, os_16, AX);
  operand_from_register_16(&instruction->source, os_16, DX);
  operand_none(&instruction->third);
}

void decode_AL_Eb_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_register_8(&instruction->destination, os_8, AL);
  operand_from_mod_rm_reg_mem(&instruction->source, os_8, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_none(&instruction->third);
}

void decode_AX_Ew_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_register_16(&instruction->destination, os_16, AX);
  operand_from_mod_rm_reg_mem(&instruction->source, os_16, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_none(&instruction->third);
}

void decode_Ap_Xx_Xx(struct input_stream *stream, struct instruction *instruction) {
  operand_from_far_jump(&instruction->destination, stream);
  operand_none(&instruction->source);
  operand_none(&instruction->third);
}

void decode_Fw_Xx_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_flags_register(&instruction->destination);
  operand_none(&instruction->source);
  operand_none(&instruction->third);
}

void decode_AL_Ob_Xx(struct input_stream *stream, struct instruction *instruction) {
  operand_from_register_8(&instruction->destination, os_8, AL);
  operand_from_offset(&instruction->source, os_8, stream, instruction->segment_register);
  operand_none(&instruction->third);
}

void decode_AX_Ow_Xx(struct input_stream *stream, struct instruction *instruction) {
  operand_from_register_16(&instruction->destination, os_16, AX);
  operand_from_offset(&instruction->source, os_16, stream, instruction->segment_register);
  operand_none(&instruction->third);
}

void decode_Ob_AL_Xx(struct input_stream *stream, struct instruction *instruction) {
  operand_from_offset(&instruction->destination, os_8, stream, instruction->segment_register);
  operand_from_register_8(&instruction->source, os_8, AL);
  operand_none(&instruction->third);
}

void decode_Ow_AX_Xx(struct input_stream *stream, struct instruction *instruction) {
  operand_from_offset(&instruction->destination, os_16, stream, instruction->segment_register);
  operand_from_register_16(&instruction->source, os_16, AX);
  operand_none(&instruction->third);
}

void decode_Xb_Yb_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_ds_si(&instruction->destination, os_8);
  operand_from_es_di(&instruction->source, os_8);
  operand_none(&instruction->third);
}

void decode_Xw_Yw_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_ds_si(&instruction->destination, os_16);
  operand_from_es_di(&instruction->source, os_16);
  operand_none(&instruction->third);
}

void decode_Yb_AL_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_es_di(&instruction->destination, os_8);
  operand_from_register_8(&instruction->source, os_8, AL);
  operand_none(&instruction->third);
}

void decode_Yw_AX_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_es_di(&instruction->destination, os_16);
  operand_from_register_16(&instruction->source, os_16, AX);
  operand_none(&instruction->third);
}

void decode_AL_Xb_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_register_8(&instruction->destination, os_8, AL);
  operand_from_ds_si(&instruction->source, os_8);
  operand_none(&instruction->third);
}

void decode_AX_Xw_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_register_16(&instruction->destination, os_16, AX);
  operand_from_ds_si(&instruction->source, os_16);
  operand_none(&instruction->third);
}

void decode_AL_Yb_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_register_8(&instruction->destination, os_8, AL);
  operand_from_es_di(&instruction->source, os_8);
  operand_none(&instruction->third);
}

void decode_AX_Yw_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_register_16(&instruction->destination, os_16, AX);
  operand_from_es_di(&instruction->source, os_16);
  operand_none(&instruction->third);
}

void decode_Iw_Ib_Xx(struct input_stream *stream, struct instruction *instruction) {
  operand_from_immediate(&instruction->destination, os_8, stream);
  operand_from_immediate(&instruction->source, os_16, stream);
  operand_none(&instruction->third);
}

void decode_Eb_1_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg_mem(&instruction->destination, os_8, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_from_constant(&instruction->source, os_8, 1);
  operand_none(&instruction->third);
}

void decode_Ew_1_Xx(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg_mem(&instruction->destination, os_16, stream, mrm.mod, mrm.reg_mem,
                              instruction->segment_register, false);
  operand_none(&instruction->third);
}

void decode_AX_Ib_Xx(struct input_stream *stream, struct instruction *instruction) {
  operand_from_register_16(&instruction->destination, os_16, AX);
  operand_from_immediate(&instruction->source, os_8, stream);
  operand_none(&instruction->third);
}

void decode_Ib_AL_Xx(struct input_stream *stream, struct instruction *instruction) {
  operand_from_immediate(&instruction->destination, os_8, stream);
  operand_from_register_8(&instruction->source, os_8, AL);
  operand_none(&instruction->third);
}

void decode_Ib_AX_Xx(struct input_stream *stream, struct instruction *instruction) {
  operand_from_immediate(&instruction->destination, os_8, stream);
  operand_from_register_16(&instruction->source, os_16, AX);
  operand_none(&instruction->third);
}

void decode_Jw_Xx_Xx(struct input_stream *stream, struct instruction *instruction) {
  operand_from_jump(&instruction->destination, os_16, stream);
  operand_none(&instruction->source);
  operand_none(&instruction->third);
}

void decode_AL_DX_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_register_8(&instruction->destination, os_8, AL);
  operand_from_register_16(&instruction->source, os_16, DX);
  operand_none(&instruction->third);
}

void decode_DX_AL_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_register_16(&instruction->destination, os_16, DX);
  operand_from_register_8(&instruction->source, os_8, AL);
  operand_none(&instruction->third);
}

void decode_DX_AX_Xx(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  operand_from_register_16(&instruction->destination, os_16, DX);
  operand_from_register_16(&instruction->source, os_16, AX);
  operand_none(&instruction->third);
}

void decode_Rw_Xx_Xx(struct input_stream *stream, struct instruction *instruction) {
  u8 op_code = input_stream_peek_u8(stream, -1);

  operand_from_mod_rm_reg(&instruction->destination, os_16, op_code & 0x07);
  operand_none(&instruction->source);
  operand_none(&instruction->third);
}

void decode_Rb_Ib_Xx(struct input_stream *stream, struct instruction *instruction) {
  u8 op_code = input_stream_peek_u8(stream, -1);

  operand_from_mod_rm_reg(&instruction->destination, os_8, op_code & 0x07);
  operand_from_immediate(&instruction->source, os_8, stream);
  operand_none(&instruction->third);
}

void decode_Rw_Iw_Xx(struct input_stream *stream, struct instruction *instruction) {
  u8 op_code = input_stream_peek_u8(stream, -1);

  operand_from_mod_rm_reg(&instruction->destination, os_16, op_code & 0x07);
  operand_from_immediate(&instruction->source, os_16, stream);
  operand_none(&instruction->third);
}

void decode_AX_Rw_Xx(struct input_stream *stream, struct instruction *instruction) {
  u8 op_code = input_stream_peek_u8(stream, -1);

  operand_from_register_16(&instruction->destination, os_16, AX);
  operand_from_mod_rm_reg(&instruction->source, os_16, op_code & 0x07);
  operand_none(&instruction->third);
}

void decode_rep_prefix(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  instruction->rep_prefix = rp_rep;
}

void decode_repne_prefix(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  instruction->rep_prefix = rp_repne;
}

void decode_seg_reg_override_cs(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  instruction->segment_register = CS;
}

void decode_seg_reg_override_es(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  instruction->segment_register = ES;
}

void decode_seg_reg_override_ds(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  instruction->segment_register = DS;
}

void decode_seg_reg_override_ss(struct input_stream *stream, struct instruction *instruction) {
  UNUSED(stream);

  instruction->segment_register = SS;
}
