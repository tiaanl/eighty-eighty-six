#ifndef MOD_REG_RM_H_
#define MOD_REG_RM_H_

#include "../platform.h"

#include <assert.h>

enum mrrm_mod {
  mrrm_mod_indirect,
  mrrm_mod_byte,
  mrrm_mod_word,
  mrrm_mod_register,

  mrrm_mod_count
};

enum mrrm_reg {
  mrrm_reg_al_ax,
  mrrm_reg_cl_cx,
  mrrm_reg_dl_dx,
  mrrm_reg_bl_bx,
  mrrm_reg_ah_sp,
  mrrm_reg_ch_bp,
  mrrm_reg_dh_si,
  mrrm_reg_bh_di,

  mrrm_reg_count,
};

enum mrrm_rm {
  mrrm_rm_bx_si,
  mrrm_rm_bx_di,
  mrrm_rm_bp_si,
  mrrm_rm_bp_di,
  mrrm_rm_si,
  mrrm_rm_di,
  mrrm_rm_bp,
  mrrm_rm_bx,

  mrrm_rm_count,
};

struct mrrm {
  enum mrrm_mod mod;
  enum mrrm_reg reg;
  union {
    enum mrrm_rm rm_rm;
    enum mrrm_reg rm_reg;
    u8 rm_byte;
  };
};

static inline enum mrrm_mod decode_mrrm_mod(u8 byte) {
  enum mrrm_mod result = byte >> 0x06;
  assert(result < mrrm_mod_count);
  return result;
}

static inline enum mrrm_reg decode_mrrm_reg(u8 byte) {
  enum mrrm_reg result = byte >> 0x03 & 0x07;
  assert(result < mrrm_reg_count);
  return result;
}

static inline enum mrrm_rm decode_mrrm_rm(u8 byte) {
  enum mrrm_rm result = byte & 0x07;
  assert(result < mrrm_rm_count);
  return result;
}

static inline enum mrrm_reg decode_mrrm_reg_from_rm(u8 byte) {
  enum mrrm_reg result = byte & 0x07;
  assert(result < mrrm_reg_count);
  return result;
}

static inline struct mrrm decode_mrrm(u8 byte) {
  struct mrrm result;

  result.mod = decode_mrrm_mod(byte);
  result.reg = decode_mrrm_reg(byte);
  switch (result.mod) {
    case mrrm_mod_indirect:
    case mrrm_mod_byte:
    case mrrm_mod_word:
      result.rm_rm = decode_mrrm_rm(byte);
      break;

    case mrrm_mod_register:
      result.rm_reg = decode_mrrm_reg_from_rm(byte);
      break;

    default:
      assert(0);
  }

  return result;
}

static inline u8 encode_mrrm(enum mrrm_mod mod, enum mrrm_reg reg, enum mrrm_rm rm) {
  return ((u8)mod << 0x06) | ((u8)reg << 3) | ((u8)rm);
}

#endif // MOD_REG_RM_H_
