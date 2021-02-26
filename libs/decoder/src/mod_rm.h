#ifndef MOD_RM_H_
#define MOD_RM_H_

#include <assert.h>
#include <base/platform.h>

#define MOD_RM_MOD(x) ((x) >> 0x06)
#define MOD_RM_REG(x) ((x) >> 0x03 & 0x07)
#define MOD_RM_REG_MEM(x) ((x)&0x07)

enum mod_rm_mod {
  mod_rm_mod_indirect,
  mod_rm_mod_byte,
  mod_rm_mod_word,
  mod_rm_mod_register,
};

enum mod_rm_reg {
  mod_rm_reg_al_ax,
  mod_rm_reg_cl_cx,
  mod_rm_reg_dl_dx,
  mod_rm_reg_bl_bx,
  mod_rm_reg_ah_sp,
  mod_rm_reg_ch_bp,
  mod_rm_reg_dh_si,
  mod_rm_reg_bh_di,
};

enum mod_rm_mem {
  mod_rm_mem_bx_si,
  mod_rm_mem_bx_di,
  mod_rm_mem_bp_si,
  mod_rm_mem_bp_di,
  mod_rm_mem_si,
  mod_rm_mem_di,
  mod_rm_mem_bp,
  mod_rm_mem_bx,
};

union mod_rm_reg_mem {
  enum mod_rm_reg as_reg;
  enum mod_rm_mem as_mem;
};

struct mod_rm {
  enum mod_rm_mod mod;
  enum mod_rm_reg reg;
  union mod_rm_reg_mem reg_mem;
};

static inline struct mod_rm decode_mod_rm(u8 byte) {
  struct mod_rm result;

  result.mod = MOD_RM_MOD(byte);
  result.reg = MOD_RM_REG(byte);

  // We just use the `as_mem` part because the 2 parts of the union occupy the same memory space.
  result.reg_mem.as_mem = MOD_RM_REG_MEM(byte);

  return result;
}

static inline u8 encode_mod_rm_values(enum mod_rm_mod mod, enum mod_rm_reg reg,
                                      union mod_rm_reg_mem rm) {
  assert(mod <= (1 << 2));
  assert(reg <= (1 << 3));
  assert(rm.as_mem <= (1 << 3));

  return (((u8)mod & 0x03) << 0x06) | (((u8)reg & 0x07) << 3) | ((u8)rm.as_mem & 0x07);
}

static inline u8 encode_mod_rm(struct mod_rm mod_rm) {
  return encode_mod_rm_values(mod_rm.mod, mod_rm.reg, mod_rm.reg_mem);
}

#endif // MOD_RM_H_
