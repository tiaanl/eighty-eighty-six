void decoder_tests(void);
void mod_rm_tests(void);

#include "decoder/mod_reg_rm.h"
#include "platform.h"
#include "registers.h"

#include <string.h>

#if 0
void test_1(void) {
  // test bl, 0x07
  u8 buffer[] = {0xf6, 0xc3, 0x07};

  struct operand destination;
  memset(&destination, 0, sizeof(struct operand));

  int extra_destination_bytes = decode_operand(DT_MEM_8, buffer[1], buffer + 2, 1, &destination);

  assert(destination.type == OT_REGISTER);
  assert(destination.size == OS_8);
  assert(destination.as_register.reg_8 == BL);
  assert(extra_destination_bytes == 0);

  struct operand source;
  memset(&source, 0, sizeof(struct operand));

  int extra_source_bytes = decode_operand(DT_IMM_8, buffer[1], buffer + 2, 1, &source);

  assert(source.type == OT_IMMEDIATE);
  assert(source.size == OS_8);
  assert(source.as_immediate.immediate_8 == 0x07);
  assert(extra_source_bytes == 1);
}

void test_2(void) {
  // mov [bp+si], bl
  u8 buffer[] = {0x88, 0x1a};

  struct operand destination;
  memset(&destination, 0, sizeof(struct operand));

  int extra_destination_bytes = decode_operand(DT_MEM_8, buffer[1], 0, 0, &destination);

  assert(destination.type == OT_INDIRECT);
  assert(destination.size == OS_8);
  assert(destination.as_indirect.encoding == IE_BP_SI);
  assert(extra_destination_bytes == 0);

  struct operand source;
  memset(&source, 0, sizeof(struct operand));

  int extra_source_bytes = decode_operand(DT_REG_8, buffer[1], 0, 0, &source);

  assert(source.type == OT_REGISTER);
  assert(source.size == OS_8);
  assert(source.as_register.reg_8 == BL);
  assert(extra_source_bytes == 0);
}

void test_3(void) {
  // inc cx
  u8 buffer[] = {0x41};

  struct operand destination;
  memset(&destination, 0, sizeof(struct operand));

  int extra_destination_bytes = decode_operand(DT_DST_16, buffer[0], 0, 0, &destination);

  assert(destination.type == OT_REGISTER);
  assert(destination.size == OS_16);
  assert(destination.as_register.reg_16 == CX);
  assert(extra_destination_bytes == 0);
}

void test_4(void) {
  // add byte [bp+0x0102], 0x0242
  u8 buffer[] = {0x81, 0x86, 0x02, 0x01, 0x42, 0x02};

  struct operand destination;
  memset(&destination, 0, sizeof(struct operand));

  int extra_destination_bytes = decode_operand(DT_MEM_16, buffer[1], buffer + 2, 4, &destination);

  assert(destination.type == OT_DISPLACEMENT);
  assert(destination.size == OS_16);
  assert(destination.as_displacement.encoding == IE_BP);
  assert(destination.as_displacement.displacement == 0x0102);
  assert(extra_destination_bytes == 2);

  struct operand source;
  memset(&source, 0, sizeof(struct operand));

  int extra_source_bytes = decode_operand(DT_IMM_16, buffer[1], buffer + 4, 2, &source);

  assert(source.type == OT_IMMEDIATE);
  assert(source.size == OS_16);
  assert(source.as_immediate.immediate_16 == 0x0242);
  assert(extra_source_bytes == 2);
}
#endif // 0
int main(void) {
  // decoder_tests();
  // mod_rm_tests();

  //  test_1();
  //  test_2();
  //  test_3();
  //  test_4();

  return 0;
}
