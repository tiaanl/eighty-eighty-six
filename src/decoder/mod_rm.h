#ifndef DECODER_MOD_RM_H_
#define DECODER_MOD_RM_H_

#include "../instruction.h"

enum mod_reg_rm_mod {
  MOD_INDIRECT = 0b00,
  MOD_BYTE = 0b01,
  MOD_DOUBLE_WORD = 0b10,
  MOD_REGISTER = 0b11,
};

struct mod_reg_rm {
  enum mod_reg_rm_mod mod;
  enum register_encoding reg;
  enum indirect_register_encoding rm;
};

inline struct mod_reg_rm decode_mod_reg_rm(u8 b) {
  struct mod_reg_rm result;

  result.mod = (b & 0b11000000) >> 6;
  result.reg = (b & 0b00111000) >> 3;
  result.rm = (b & 0b00000111) >> 0;

  return result;
}

#endif // DECODER_MOD_RM_H_
