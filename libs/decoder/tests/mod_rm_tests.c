#include "../src/mod_rm.h"

#include <testing/testing.h>

void test_decoders(void) {
  EXPECT_U8_EQ(MOD_RM_MOD(0xff), 0x03);
  EXPECT_U8_EQ(MOD_RM_REG(0xff), 0x07);
  EXPECT_U8_EQ(MOD_RM_REG_MEM(0xff), 0x07);
}

void test_decode_mod_rm(void) {
  struct mod_rm mrm = decode_mod_rm(0x00);
  EXPECT_U8_EQ(mrm.mod, mod_rm_mod_indirect);
  EXPECT_U8_EQ(mrm.reg, mod_rm_reg_al_ax);
  EXPECT_U8_EQ(mrm.reg_mem.as_mem, mod_rm_mem_bx_si);
}

void test_encode_mod_rm_values(void) {
  union mod_rm_reg_mem mem = {.as_reg = mod_rm_reg_dl_dx};
  u8 result = encode_mod_rm_values(mod_rm_mod_register, mod_rm_reg_cl_cx, mem);
  EXPECT_U8_EQ(result, 0x0ca);
}

void test_encode_mod_rm(void) {
  struct mod_rm mrm = {
      .mod = mod_rm_mod_indirect,
      .reg = mod_rm_reg_ch_bp,
      .reg_mem =
          {
              .as_mem = mod_rm_mem_bp_di,
          },
  };

  u8 result = encode_mod_rm(mrm);
  EXPECT_U8_EQ(result, 0x02b);
}

void mod_rm_tests(void) {
  test_decoders();
  test_decode_mod_rm();
  test_encode_mod_rm_values();
  test_encode_mod_rm();
}
