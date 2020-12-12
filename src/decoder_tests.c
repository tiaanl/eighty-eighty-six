#include "decoder/decoder.h"
#include "platform.h"

#include <assert.h>

void assert_operand_indirect(struct operand *operand, enum operand_size operand_size,
                             enum indirect_register_encoding indirect_register_encoding) {
  assert(operand->mode == OPERAND_MODE_INDIRECT);
  assert(operand->size == operand_size);
  assert(operand->indirect_reg == indirect_register_encoding);
  assert(operand->disp16 == 0);
}

void assert_operand_reg(struct operand *operand, enum operand_size operand_size,
                        enum register_encoding register_encoding) {
  assert(operand->mode == OPERAND_MODE_REGISTER);
  assert(operand->size == operand_size);
  assert(operand->reg == register_encoding);
  assert(operand->disp16 == 0);
}

void assert_operand_immediate(struct operand *operand, enum operand_size operand_size,
                              u16 immediate) {
  assert(operand->mode == OPERAND_MODE_IMMEDIATE);
  assert(operand->size == operand_size);
  assert(operand->immediate16 == immediate);
}

void assert_operand_none(struct operand *operand) {
  assert(operand->mode == OPERAND_MODE_NONE);
}

#define ASSERT_OPERAND_JUMP_DISPLACEMENT(SIZE)                                                     \
  void assert_operand_jump_displacement_##SIZE(struct operand *operand, i##SIZE displacement) {    \
    assert(operand->mode == OPERAND_MODE_DISPLACEMENT_##SIZE);                                     \
    assert(operand->size == OPERAND_SIZE_##SIZE);                                                  \
    assert(operand->disp##SIZE == displacement);                                                   \
  }

ASSERT_OPERAND_JUMP_DISPLACEMENT(8)
ASSERT_OPERAND_JUMP_DISPLACEMENT(16)

#undef ASSERT_OPERAND_JUMP_DISPLACEMENT

void test_00(void) {
  // add [di], bl
  const u8 buffer[] = {0x00, 0x1d};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == ADD);
  assert_operand_indirect(&i.destination, OPERAND_SIZE_8, INDIRECT_REG_DI);
  assert_operand_reg(&i.source, OPERAND_SIZE_8, REG_BL_BX);
}

void test_01(void) {
  // add [di], bx
  const u8 buffer[] = {0x01, 0x1d};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == ADD);
  assert_operand_indirect(&i.destination, OPERAND_SIZE_16, INDIRECT_REG_DI);
  assert_operand_reg(&i.source, OPERAND_SIZE_16, REG_BL_BX);
}

void test_02(void) {
  // add bl, [di]
  const u8 buffer[] = {0x02, 0x1d};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == ADD);
  assert_operand_reg(&i.destination, OPERAND_SIZE_8, REG_BL_BX);
  assert_operand_indirect(&i.source, OPERAND_SIZE_8, INDIRECT_REG_DI);
}

void test_03(void) {
  // add bx, [di]
  const u8 buffer[] = {0x03, 0x1d};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == ADD);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_BL_BX);
  assert_operand_indirect(&i.source, OPERAND_SIZE_16, INDIRECT_REG_DI);
}

void test_04(void) {
  // add al, 8
  const u8 buffer[] = {0x04, 0x08};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == ADD);
  assert_operand_reg(&i.destination, OPERAND_SIZE_8, REG_AL_AX);
  assert_operand_immediate(&i.source, OPERAND_SIZE_8, 0x08);
}

void test_05(void) {
  // add ax, 0xffee
  const u8 buffer[] = {0x03, 0x1d};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == ADD);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_BL_BX);
  assert_operand_indirect(&i.source, OPERAND_SIZE_16, INDIRECT_REG_DI);
}

void test_40(void) {
  // inc ax
  const u8 buffer[] = {0x40};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 1);
  assert(i.type == INC);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_AL_AX);
  assert_operand_none(&i.source);
}

void test_41(void) {
  // inc cx
  const u8 buffer[] = {0x41};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 1);
  assert(i.type == INC);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_CL_CX);
  assert_operand_none(&i.source);
}

void test_42(void) {
  // inc dx
  const u8 buffer[] = {0x42};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 1);
  assert(i.type == INC);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_DL_DX);
  assert_operand_none(&i.source);
}

void test_43(void) {
  // inc bx
  const u8 buffer[] = {0x43};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 1);
  assert(i.type == INC);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_BL_BX);
  assert_operand_none(&i.source);
}

void test_44(void) {
  // inc sp
  const u8 buffer[] = {0x44};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 1);
  assert(i.type == INC);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_AH_SP);
  assert_operand_none(&i.source);
}

void test_45(void) {
  // inc bp
  const u8 buffer[] = {0x45};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 1);
  assert(i.type == INC);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_CH_BP);
  assert_operand_none(&i.source);
}

void test_46(void) {
  // inc si
  const u8 buffer[] = {0x46};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 1);
  assert(i.type == INC);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_DH_SI);
  assert_operand_none(&i.source);
}

void test_47(void) {
  // inc di
  const u8 buffer[] = {0x47};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 1);
  assert(i.type == INC);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_BH_DI);
  assert_operand_none(&i.source);
}

void test_80(void) {
  // add bl, 3
  const u8 buffer[] = {0x80, 0xc3, 0x03};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 3);
  assert(i.type == ADD);
  assert_operand_reg(&i.destination, OPERAND_SIZE_8, REG_BL_BX);
  assert_operand_immediate(&i.source, OPERAND_SIZE_8, 0x03);
}

void test_74(void) {
  // t:
  //   je, t
  const u8 buffer[] = {0x74, 0xfe};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == JZ);
  assert_operand_jump_displacement_8(&i.destination, -2);
  assert_operand_none(&i.source);
}

void test_81(void) {
  // add bx, 0x1007
  const u8 buffer[] = {0x81, 0xc3, 0x07, 0x10};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 4);
  assert(i.type == ADD);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_BL_BX);
  assert_operand_immediate(&i.source, OPERAND_SIZE_16, 0x1007);
}

void test_88(void) {
  // mov [di], al
  const u8 buffer[] = {0x88, 0x05};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == MOV);
  assert_operand_indirect(&i.destination, OPERAND_SIZE_8, INDIRECT_REG_DI);
  assert_operand_reg(&i.source, OPERAND_SIZE_8, REG_AL_AX);
}

void test_89(void) {
  // mov [di], ax
  const u8 buffer[] = {0x89, 0x05};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == MOV);
  assert_operand_indirect(&i.destination, OPERAND_SIZE_16, INDIRECT_REG_DI);
  assert_operand_reg(&i.source, OPERAND_SIZE_16, REG_AL_AX);
}

void test_8a(void) {
  // mov bl, [di]
  const u8 buffer[] = {0x8a, 0x1d};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_8, REG_BL_BX);
  assert_operand_indirect(&i.source, OPERAND_SIZE_8, INDIRECT_REG_DI);
}

void test_8b(void) {
  // mov cx, [di]
  const u8 buffer[] = {0x8b, 0x0d};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_CL_CX);
  assert_operand_indirect(&i.source, OPERAND_SIZE_16, INDIRECT_REG_DI);
}

void test_b0(void) {
  // mov al, 0xb0
  const u8 buffer[] = {0xb0, 0xb0};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_8, REG_AL_AX);
  assert_operand_immediate(&i.source, OPERAND_SIZE_8, 0xb0);
}

void test_b1(void) {
  // mov cl, 0xb1
  const u8 buffer[] = {0xb1, 0xb1};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_8, REG_CL_CX);
  assert_operand_immediate(&i.source, OPERAND_SIZE_8, 0xb1);
}

void test_b2(void) {
  // mov dl, 0xb2
  const u8 buffer[] = {0xb2, 0xb2};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_8, REG_DL_DX);
  assert_operand_immediate(&i.source, OPERAND_SIZE_8, 0xb2);
}

void test_b3(void) {
  // mov bl, 0xb3
  const u8 buffer[] = {0xb3, 0xb3};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_8, REG_BL_BX);
  assert_operand_immediate(&i.source, OPERAND_SIZE_8, 0xb3);
}

void test_b4(void) {
  // mov ah, 0xb4
  const u8 buffer[] = {0xb4, 0xb4};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_8, REG_AH_SP);
  assert_operand_immediate(&i.source, OPERAND_SIZE_8, 0xb4);
}

void test_b5(void) {
  // mov ch, 0xb5
  const u8 buffer[] = {0xb5, 0xb5};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_8, REG_CH_BP);
  assert_operand_immediate(&i.source, OPERAND_SIZE_8, 0xb5);
}

void test_b6(void) {
  // mov dh, 0xb6
  const u8 buffer[] = {0xb6, 0xb6};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_8, REG_DH_SI);
  assert_operand_immediate(&i.source, OPERAND_SIZE_8, 0xb6);
}

void test_b7(void) {
  // mov bh, 0xb7
  const u8 buffer[] = {0xb7, 0xb7};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_8, REG_BH_DI);
  assert_operand_immediate(&i.source, OPERAND_SIZE_8, 0xb7);
}

void test_b8(void) {
  // mov ax, 0xb8b8
  const u8 buffer[] = {0xb8, 0xb8, 0xb8};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 3);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_AL_AX);
  assert_operand_immediate(&i.source, OPERAND_SIZE_16, 0xb8b8);
}

void test_b9(void) {
  // mov cx, 0xb9b9
  const u8 buffer[] = {0xb9, 0xb9, 0xb9};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 3);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_CL_CX);
  assert_operand_immediate(&i.source, OPERAND_SIZE_16, 0xb9b9);
}

void test_ba(void) {
  // mov dx, 0xbaba
  const u8 buffer[] = {0xba, 0xba, 0xba};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 3);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_DL_DX);
  assert_operand_immediate(&i.source, OPERAND_SIZE_16, 0xbaba);
}

void test_bb(void) {
  // mov bx, 0xbbbb
  const u8 buffer[] = {0xbb, 0xbb, 0xbb};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 3);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_BL_BX);
  assert_operand_immediate(&i.source, OPERAND_SIZE_16, 0xbbbb);
}

void test_bc(void) {
  // mov sp, 0xbcbc
  const u8 buffer[] = {0xbc, 0xbc, 0xbc};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 3);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_AH_SP);
  assert_operand_immediate(&i.source, OPERAND_SIZE_16, 0xbcbc);
}

void test_bd(void) {
  // mov bp, 0xbdbd
  const u8 buffer[] = {0xbd, 0xbd, 0xbd};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 3);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_CH_BP);
  assert_operand_immediate(&i.source, OPERAND_SIZE_16, 0xbdbd);
}

void test_be(void) {
  // mov si, 0xbebe
  const u8 buffer[] = {0xbe, 0xbe, 0xbe};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 3);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_DH_SI);
  assert_operand_immediate(&i.source, OPERAND_SIZE_16, 0xbebe);
}

void test_bf(void) {
  // mov di, 0xbfbf
  const u8 buffer[] = {0xbf, 0xbf, 0xbf};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 3);
  assert(i.type == MOV);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_BH_DI);
  assert_operand_immediate(&i.source, OPERAND_SIZE_16, 0xbfbf);
}

void test_e2(void) {
  // loop -2
  const u8 buffer[] = {0xe2, 0xfc};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 2);
  assert(i.type == LOOP);
  assert_operand_jump_displacement_8(&i.destination, -4);
  assert_operand_none(&i.source);
}

void test_f6(void) {
  // test bl, 0x07
  const u8 buffer[] = {0xf6, 0xc3, 0x07};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 3);
  assert(i.type == TEST);
  assert_operand_reg(&i.destination, OPERAND_SIZE_8, REG_BL_BX);
  assert_operand_immediate(&i.source, OPERAND_SIZE_8, 0x07);
}

void test_f4(void) {
  // hlt
  const u8 buffer[] = {0xf4};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 1);
  assert(i.type == HLT);
  assert_operand_none(&i.destination);
  assert_operand_none(&i.source);
}

void test_f7(void) {
  // test bx, 0x0707
  const u8 buffer[] = {0xf7, 0xc3, 0x07, 0x07};

  struct instruction i;
  assert(decode_instruction(buffer, sizeof(buffer), &i) == 4);
  assert(i.type == TEST);
  assert_operand_reg(&i.destination, OPERAND_SIZE_16, REG_BL_BX);
  assert_operand_immediate(&i.source, OPERAND_SIZE_16, 0x0707);
}

#define NOP_TEST(OpCode)                                                                           \
  void test_##OpCode(void) {                                                                       \
    const u8 buffer[] = {0x##OpCode};                                                              \
    struct instruction i;                                                                          \
    assert(decode_instruction(buffer, sizeof(buffer), &i) == -1);                                  \
  }

// NOP_TEST(00)
// NOP_TEST(01)
// NOP_TEST(02)
// NOP_TEST(03)
// NOP_TEST(04)
// NOP_TEST(05)
NOP_TEST(06)
NOP_TEST(07)
NOP_TEST(08)
NOP_TEST(09)
NOP_TEST(0a)
NOP_TEST(0b)
NOP_TEST(0c)
NOP_TEST(0d)
NOP_TEST(0e)
NOP_TEST(0f)

NOP_TEST(10)
NOP_TEST(11)
NOP_TEST(12)
NOP_TEST(13)
NOP_TEST(14)
NOP_TEST(15)
NOP_TEST(16)
NOP_TEST(17)
NOP_TEST(18)
NOP_TEST(19)
NOP_TEST(1a)
NOP_TEST(1b)
NOP_TEST(1c)
NOP_TEST(1d)
NOP_TEST(1e)
NOP_TEST(1f)

NOP_TEST(20)
NOP_TEST(21)
NOP_TEST(22)
NOP_TEST(23)
NOP_TEST(24)
NOP_TEST(25)
NOP_TEST(26)
NOP_TEST(27)
NOP_TEST(28)
NOP_TEST(29)
NOP_TEST(2a)
NOP_TEST(2b)
NOP_TEST(2c)
NOP_TEST(2d)
NOP_TEST(2e)
NOP_TEST(2f)

NOP_TEST(30)
NOP_TEST(31)
NOP_TEST(32)
NOP_TEST(33)
NOP_TEST(34)
NOP_TEST(35)
NOP_TEST(36)
NOP_TEST(37)
NOP_TEST(38)
NOP_TEST(39)
NOP_TEST(3a)
NOP_TEST(3b)
NOP_TEST(3c)
NOP_TEST(3d)
NOP_TEST(3e)
NOP_TEST(3f)

// NOP_TEST(40)
// NOP_TEST(41)
// NOP_TEST(42)
// NOP_TEST(43)
// NOP_TEST(44)
// NOP_TEST(45)
// NOP_TEST(46)
// NOP_TEST(47)
NOP_TEST(48)
NOP_TEST(49)
NOP_TEST(4a)
NOP_TEST(4b)
NOP_TEST(4c)
NOP_TEST(4d)
NOP_TEST(4e)
NOP_TEST(4f)

NOP_TEST(50)
NOP_TEST(51)
NOP_TEST(52)
NOP_TEST(53)
NOP_TEST(54)
NOP_TEST(55)
NOP_TEST(56)
NOP_TEST(57)
NOP_TEST(58)
NOP_TEST(59)
NOP_TEST(5a)
NOP_TEST(5b)
NOP_TEST(5c)
NOP_TEST(5d)
NOP_TEST(5e)
NOP_TEST(5f)

NOP_TEST(60)
NOP_TEST(61)
NOP_TEST(62)
NOP_TEST(63)
NOP_TEST(64)
NOP_TEST(65)
NOP_TEST(66)
NOP_TEST(67)
NOP_TEST(68)
NOP_TEST(69)
NOP_TEST(6a)
NOP_TEST(6b)
NOP_TEST(6c)
NOP_TEST(6d)
NOP_TEST(6e)
NOP_TEST(6f)

NOP_TEST(70)
NOP_TEST(71)
NOP_TEST(72)
NOP_TEST(73)
// NOP_TEST(74)
NOP_TEST(75)
NOP_TEST(76)
NOP_TEST(77)
NOP_TEST(78)
NOP_TEST(79)
NOP_TEST(7a)
NOP_TEST(7b)
NOP_TEST(7c)
NOP_TEST(7d)
NOP_TEST(7e)
NOP_TEST(7f)

// NOP_TEST(80)
// NOP_TEST(81)
NOP_TEST(82)
NOP_TEST(83)
NOP_TEST(84)
NOP_TEST(85)
NOP_TEST(86)
NOP_TEST(87)
// NOP_TEST(88)
// NOP_TEST(89)
// NOP_TEST(8a)
// NOP_TEST(8b)
NOP_TEST(8c)
NOP_TEST(8d)
NOP_TEST(8e)
NOP_TEST(8f)

NOP_TEST(90)
NOP_TEST(91)
NOP_TEST(92)
NOP_TEST(93)
NOP_TEST(94)
NOP_TEST(95)
NOP_TEST(96)
NOP_TEST(97)
NOP_TEST(98)
NOP_TEST(99)
NOP_TEST(9a)
NOP_TEST(9b)
NOP_TEST(9c)
NOP_TEST(9d)
NOP_TEST(9e)
NOP_TEST(9f)

NOP_TEST(a0)
NOP_TEST(a1)
NOP_TEST(a2)
NOP_TEST(a3)
NOP_TEST(a4)
NOP_TEST(a5)
NOP_TEST(a6)
NOP_TEST(a7)
NOP_TEST(a8)
NOP_TEST(a9)
NOP_TEST(aa)
NOP_TEST(ab)
NOP_TEST(ac)
NOP_TEST(ad)
NOP_TEST(ae)
NOP_TEST(af)

// NOP_TEST(b0)
// NOP_TEST(b1)
// NOP_TEST(b2)
// NOP_TEST(b3)
// NOP_TEST(b4)
// NOP_TEST(b5)
// NOP_TEST(b6)
// NOP_TEST(b7)
// NOP_TEST(b8)
// NOP_TEST(b9)
// NOP_TEST(ba)
// NOP_TEST(bb)
// NOP_TEST(bc)
// NOP_TEST(bd)
// NOP_TEST(be)
// NOP_TEST(bf)

NOP_TEST(c0)
NOP_TEST(c1)
NOP_TEST(c2)
NOP_TEST(c3)
NOP_TEST(c4)
NOP_TEST(c5)
NOP_TEST(c6)
NOP_TEST(c7)
NOP_TEST(c8)
NOP_TEST(c9)
NOP_TEST(ca)
NOP_TEST(cb)
NOP_TEST(cc)
NOP_TEST(cd)
NOP_TEST(ce)
NOP_TEST(cf)

NOP_TEST(d0)
NOP_TEST(d1)
NOP_TEST(d2)
NOP_TEST(d3)
NOP_TEST(d4)
NOP_TEST(d5)
NOP_TEST(d6)
NOP_TEST(d7)
NOP_TEST(d8)
NOP_TEST(d9)
NOP_TEST(da)
NOP_TEST(db)
NOP_TEST(dc)
NOP_TEST(dd)
NOP_TEST(de)
NOP_TEST(df)

NOP_TEST(e0)
NOP_TEST(e1)
// NOP_TEST(e2)
NOP_TEST(e3)
NOP_TEST(e4)
NOP_TEST(e5)
NOP_TEST(e6)
NOP_TEST(e7)
NOP_TEST(e8)
NOP_TEST(e9)
NOP_TEST(ea)
NOP_TEST(eb)
NOP_TEST(ec)
NOP_TEST(ed)
NOP_TEST(ee)
NOP_TEST(ef)

NOP_TEST(f0)
NOP_TEST(f1)
NOP_TEST(f2)
NOP_TEST(f3)
// NOP_TEST(f4)
NOP_TEST(f5)
// NOP_TEST(f6)
// NOP_TEST(f7)
NOP_TEST(f8)
NOP_TEST(f9)
NOP_TEST(fa)
NOP_TEST(fb)
NOP_TEST(fc)
NOP_TEST(fd)
NOP_TEST(fe)
NOP_TEST(ff)

#undef NOP_TEST

int main() {
  test_00();
  test_01();
  test_02();
  test_03();
  test_04();
  test_05();
  test_06();
  test_07();
  test_08();
  test_09();
  test_0a();
  test_0b();
  test_0c();
  test_0d();
  test_0e();
  test_0f();

  test_10();
  test_11();
  test_12();
  test_13();
  test_14();
  test_15();
  test_16();
  test_17();
  test_18();
  test_19();
  test_1a();
  test_1b();
  test_1c();
  test_1d();
  test_1e();
  test_1f();

  test_20();
  test_21();
  test_22();
  test_23();
  test_24();
  test_25();
  test_26();
  test_27();
  test_28();
  test_29();
  test_2a();
  test_2b();
  test_2c();
  test_2d();
  test_2e();
  test_2f();

  test_30();
  test_31();
  test_32();
  test_33();
  test_34();
  test_35();
  test_36();
  test_37();
  test_38();
  test_39();
  test_3a();
  test_3b();
  test_3c();
  test_3d();
  test_3e();
  test_3f();

  test_40();
  test_41();
  test_42();
  test_43();
  test_44();
  test_45();
  test_46();
  test_47();
  test_48();
  test_49();
  test_4a();
  test_4b();
  test_4c();
  test_4d();
  test_4e();
  test_4f();

  test_50();
  test_51();
  test_52();
  test_53();
  test_54();
  test_55();
  test_56();
  test_57();
  test_58();
  test_59();
  test_5a();
  test_5b();
  test_5c();
  test_5d();
  test_5e();
  test_5f();

  test_60();
  test_61();
  test_62();
  test_63();
  test_64();
  test_65();
  test_66();
  test_67();
  test_68();
  test_69();
  test_6a();
  test_6b();
  test_6c();
  test_6d();
  test_6e();
  test_6f();

  test_70();
  test_71();
  test_72();
  test_73();
  test_74();
  test_75();
  test_76();
  test_77();
  test_78();
  test_79();
  test_7a();
  test_7b();
  test_7c();
  test_7d();
  test_7e();
  test_7f();

  test_80();
  test_81();
  test_82();
  test_83();
  test_84();
  test_85();
  test_86();
  test_87();
  test_88();
  test_89();
  test_8a();
  test_8b();
  test_8c();
  test_8d();
  test_8e();
  test_8f();

  test_90();
  test_91();
  test_92();
  test_93();
  test_94();
  test_95();
  test_96();
  test_97();
  test_98();
  test_99();
  test_9a();
  test_9b();
  test_9c();
  test_9d();
  test_9e();
  test_9f();

  test_a0();
  test_a1();
  test_a2();
  test_a3();
  test_a4();
  test_a5();
  test_a6();
  test_a7();
  test_a8();
  test_a9();
  test_aa();
  test_ab();
  test_ac();
  test_ad();
  test_ae();
  test_af();

  test_b0();
  test_b1();
  test_b2();
  test_b3();
  test_b4();
  test_b5();
  test_b6();
  test_b7();
  test_b8();
  test_b9();
  test_ba();
  test_bb();
  test_bc();
  test_bd();
  test_be();
  test_bf();

  test_c0();
  test_c1();
  test_c2();
  test_c3();
  test_c4();
  test_c5();
  test_c6();
  test_c7();
  test_c8();
  test_c9();
  test_ca();
  test_cb();
  test_cc();
  test_cd();
  test_ce();
  test_cf();

  test_d0();
  test_d1();
  test_d2();
  test_d3();
  test_d4();
  test_d5();
  test_d6();
  test_d7();
  test_d8();
  test_d9();
  test_da();
  test_db();
  test_dc();
  test_dd();
  test_de();
  test_df();

  test_e0();
  test_e1();
  test_e2();
  test_e3();
  test_e4();
  test_e5();
  test_e6();
  test_e7();
  test_e8();
  test_e9();
  test_ea();
  test_eb();
  test_ec();
  test_ed();
  test_ee();
  test_ef();

  test_f0();
  test_f1();
  test_f2();
  test_f3();
  test_f4();
  test_f5();
  test_f6();
  test_f7();
  test_f8();
  test_f9();
  test_fa();
  test_fb();
  test_fc();
  test_fd();
  test_fe();
  test_ff();

  return 0;
}
