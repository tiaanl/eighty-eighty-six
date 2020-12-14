#ifndef MOD_REG_RM_H_
#define MOD_REG_RM_H_

#include "../platform.h"

#include <assert.h>

enum mrrm_mod {
  MRRM_MOD_INDIRECT,
  MRRM_MOD_BYTE,
  MRRM_MOD_DOUBLE_WORD,
  MRRM_MOD_REGISTER,

  MRRM_MOD_COUNT
};

enum mrrm_reg {
  MRRM_REG_AL_AX,
  MRRM_REG_CL_CX,
  MRRM_REG_DL_DX,
  MRRM_REG_BL_BX,
  MRRM_REG_AH_SP,
  MRRM_REG_CH_BP,
  MRRM_REG_DH_SI,
  MRRM_REG_BH_DI,

  MRRM_REG_COUNT,
};

enum mrrm_rm {
  MRRM_RM_BX_SI,
  MRRM_RM_BX_DI,
  MRRM_RM_BP_SI,
  MRRM_RM_BP_DI,
  MRRM_RM_SI,
  MRRM_RM_DI,
  MRRM_RM_BP,
  MRRM_RM_BX,

  MRRM_RM_COUNT,
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
  assert(result < MRRM_MOD_COUNT);
  return result;
}

static inline enum mrrm_reg decode_mrrm_reg(u8 byte) {
  enum mrrm_reg result = byte >> 0x03 & 0x07;
  assert(result < MRRM_REG_COUNT);
  return result;
}

static inline enum mrrm_rm decode_mrrm_rm(u8 byte) {
  enum mrrm_rm result = byte & 0x07;
  assert(result < MRRM_RM_COUNT);
  return result;
}

static inline enum mrrm_reg decode_mrrm_reg_from_rm(u8 byte) {
  enum mrrm_reg result = byte & 0x07;
  assert(result < MRRM_REG_COUNT);
  return result;
}

static inline struct mrrm decode_mrrm(u8 byte) {
  struct mrrm result;

  result.mod = decode_mrrm_mod(byte);
  result.reg = decode_mrrm_reg(byte);
  switch (result.mod) {
    case MRRM_MOD_INDIRECT:
    case MRRM_MOD_BYTE:
    case MRRM_MOD_DOUBLE_WORD:
      result.rm_rm = decode_mrrm_rm(byte);
      break;

    case MRRM_MOD_REGISTER:
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
