void decoder_tests(void);
void mod_rm_tests(void);

#include "decoder/mod_reg_rm.h"
#include "platform.h"
#include "registers.h"

#include <string.h>

enum decode_type {
  DT_REG_8,
  DT_REG_16,
  DT_DST_8,
  DT_DST_16,
  DT_MEM_8,
  DT_MEM_16,
  DT_IMM_8,
  DT_IMM_16,
};

enum operand_type {
  OT_DISPLACEMENT,
  OT_INDIRECT,
  OT_DIRECT,
  OT_REGISTER,
  OT_IMMEDIATE,
};

enum operand_size {
  OS_8,
  OS_16,
};

enum indirect_encoding {
  IE_BX_SI,
  IE_BX_DI,
  IE_BP_SI,
  IE_BP_DI,
  IE_SI,
  IE_DI,
  IE_BP,
  IE_BX,
};

struct operand_displacement {
  enum indirect_encoding encoding;
  i16 displacement;
};

struct operand_indirect {
  enum indirect_encoding encoding;
};

struct operand_direct {
  u16 address;
};

struct operand_register {
  union {
    enum register_8 reg_8;
    enum register_16 reg_16;
  };
};

struct operand_immediate {
  union {
    u8 immediate_8;
    u16 immediate_16;
  };
};

struct operand {
  enum operand_type type;
  enum operand_size size;

  union {
    struct operand_displacement as_displacement;
    struct operand_indirect as_indirect;
    struct operand_direct as_direct;
    struct operand_register as_register;
    struct operand_immediate as_immediate;
  };
};

static inline enum register_8 encoding_to_register_8(u8 encoding) {
  static const enum register_8 mapping[] = {
      AL, CL, DL, BL, AH, CH, DH, BH,
  };

  return mapping[encoding];
}

static inline enum register_16 encoding_to_register_16(u8 encoding) {
  static const enum register_16 mapping[] = {
      AX, CX, DX, BX, SP, BP, SI, DI,
  };

  return mapping[encoding];
}

int decode_mem_operand(enum operand_size size, u8 operand_byte, const u8 *extra,
                       unsigned int extra_size, struct operand *result) {
  result->size = size;

  enum mrrm_mod mod = decode_mrrm_mod(operand_byte);
  enum indirect_encoding indirect_encoding = operand_byte & 0x07;

  switch (mod) {
    case MRRM_MOD_INDIRECT: {
      if (indirect_encoding == IE_BP) {
        result->type = OT_DIRECT;
        result->as_direct.address = extra ? read_u16(extra, extra_size) : 0;
        return 2;
      } else {
        result->type = OT_INDIRECT;
        result->as_indirect.encoding = indirect_encoding;
        return 0;
      }
    }

    case MRRM_MOD_BYTE: {
      result->type = OT_DISPLACEMENT;
      result->as_indirect.encoding = indirect_encoding;
      result->as_displacement.displacement = extra ? (i16)read_i8(extra, extra_size) : 0;
      return 1;
    }

    case MRRM_MOD_DOUBLE_WORD: {
      result->type = OT_DISPLACEMENT;
      result->as_indirect.encoding = indirect_encoding;
      result->as_displacement.displacement = extra ? read_i16(extra, extra_size) : 0;
      return 2;
    }

    case MRRM_MOD_REGISTER: {
      result->type = OT_REGISTER;
      switch (size) {
        case OS_8:
          result->as_register.reg_8 = encoding_to_register_8(decode_mrrm_reg_from_rm(operand_byte));
          return 0;

        case OS_16:
          result->as_register.reg_16 =
              encoding_to_register_16(decode_mrrm_reg_from_rm(operand_byte));
          return 0;

        default:
          break;
      }
    }

    default:
      break;
  }

  assert(0);
}

int decode_operand(enum decode_type decode_type, u8 operand_byte, const u8 *extra,
                   unsigned extra_size, struct operand *result) {
  switch (decode_type) {
    case DT_REG_8: {
      result->type = OT_REGISTER;
      result->size = OS_8;
      result->as_register.reg_8 = encoding_to_register_8(decode_mrrm_reg(operand_byte));
      return 0;
    }

    case DT_REG_16: {
      result->type = OT_REGISTER;
      result->size = OS_16;
      result->as_register.reg_16 = encoding_to_register_16(decode_mrrm_reg(operand_byte));
      return 0;
    }

    case DT_MEM_8:
      return decode_mem_operand(OS_8, operand_byte, extra, extra_size, result);

    case DT_MEM_16:
      return decode_mem_operand(OS_16, operand_byte, extra, extra_size, result);

    case DT_IMM_8: {
      result->type = OT_IMMEDIATE;
      result->size = OS_8;
      result->as_immediate.immediate_8 = read_u8(extra, extra_size);
      return 1;
    }

    case DT_IMM_16: {
      result->type = OT_IMMEDIATE;
      result->size = OS_16;
      result->as_immediate.immediate_16 = read_u16(extra, extra_size);
      return 2;
    }

    case DT_DST_8:
      result->type = OT_REGISTER;
      result->size = OS_8;
      result->as_register.reg_8 = encoding_to_register_8(operand_byte & 0x07);
      return 0;

    case DT_DST_16:
      result->type = OT_REGISTER;
      result->size = OS_16;
      result->as_register.reg_16 = encoding_to_register_16(operand_byte & 0x07);
      return 0;

    default:
      assert(0);
  }

  return 0;
}

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

int main(void) {
  // decoder_tests();
  // mod_rm_tests();

  test_1();
  test_2();
  test_3();
  test_4();

  return 0;
}
