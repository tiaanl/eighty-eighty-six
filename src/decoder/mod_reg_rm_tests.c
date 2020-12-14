#include "mod_reg_rm.h"

#include <assert.h>

void test_decode_mod_reg_rm(void) {
  struct mrrm r1 = decode_mrrm(0x00);
  assert(r1.mod == 0x00);
  assert(r1.reg == 0x00);
  assert(r1.rm == 0x00);
}

void mod_rm_tests(void) {
  test_decode_mod_reg_rm();
}
