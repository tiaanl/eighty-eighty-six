#include "decoder_funcs.h"

static void operand_from_mod_rm_reg(enum operand_size size, enum mod_rm_reg reg,
                                    struct operand *operand) {
  operand->type = ot_register;
  operand->size = size;
  operand->data.as_register.reg_8 = (enum register_8)reg;
}

void operand_from_mod_rm_reg_mem(struct input_stream *stream, enum operand_size size,
                                 enum mod_rm_mod mod, union mod_rm_reg_mem reg_mem,
                                 struct operand *operand) {
  switch (mod) {
    case mod_rm_mod_indirect:
      if (reg_mem.as_mem == mod_rm_mem_bp) {
        operand->type = ot_direct;
        operand->size = size;
        operand->data.as_direct.address = input_stream_fetch_u16(stream);
      } else {
        operand->type = ot_indirect;
        operand->size = size;
        operand->data.as_indirect.encoding = reg_mem.as_mem;
      }
      break;

    case mod_rm_mod_byte:
      operand->type = ot_displacement;
      operand->size = size;
      operand->data.as_displacement.encoding = reg_mem.as_mem;
      operand->data.as_displacement.displacement = (i16)input_stream_fetch_i8(stream);
      break;

    case mod_rm_mod_word:
      operand->type = ot_displacement;
      operand->size = size;
      operand->data.as_displacement.encoding = reg_mem.as_mem;
      operand->data.as_displacement.displacement = input_stream_fetch_i16(stream);
      break;

    case mod_rm_mod_register:
      operand_from_mod_rm_reg(size, reg_mem.as_reg, operand);
      break;

    default:
      assert(0);
      break;
  }
}

void decode_Eb_Gb(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg_mem(stream, os_8, mrm.mod, mrm.reg_mem, &instruction->destination);
  operand_from_mod_rm_reg(os_8, mrm.reg, &instruction->source);
}

void decode_Ew_Gw(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg_mem(stream, os_16, mrm.mod, mrm.reg_mem, &instruction->destination);
  operand_from_mod_rm_reg(os_16, mrm.reg, &instruction->source);
}

void decode_Gb_Eb(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg(os_8, mrm.reg, &instruction->destination);
  operand_from_mod_rm_reg_mem(stream, os_8, mrm.mod, mrm.reg_mem, &instruction->source);
}

void decode_Gw_Ew(struct input_stream *stream, struct instruction *instruction) {
  struct mod_rm mrm = decode_mod_rm(input_stream_fetch_u8(stream));

  operand_from_mod_rm_reg(os_16, mrm.reg, &instruction->destination);
  operand_from_mod_rm_reg_mem(stream, os_16, mrm.mod, mrm.reg_mem, &instruction->source);
}
