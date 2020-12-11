#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "platform.h"

enum operand_size {
  OPERAND_SIZE_8 = 0b0,
  OPERAND_SIZE_16 = 0b1,
};

enum operand_mode {
  // MOD bits on modR/M
  OPERAND_MODE_INDIRECT = 0b000,
  OPERAND_MODE_DISPLACEMENT_8 = 0b001,
  OPERAND_MODE_DISPLACEMENT_16 = 0b010,
  OPERAND_MODE_REGISTER = 0b011,

  // Special when MOD == INDIRECT and R/M == 0b101.
  OPERAND_MODE_DIRECT = 0b100,

  // When an immediate value was passed.
  OPERAND_MODE_IMMEDIATE = 0b101,

  OPERAND_MODE_NONE = 0xff,
};

enum indirect_register_encoding {
  INDIRECT_REG_BX_SI,
  INDIRECT_REG_BX_DI,
  INDIRECT_REG_BP_SI,
  INDIRECT_REG_BP_DI,
  INDIRECT_REG_SI,
  INDIRECT_REG_DI,
  INDIRECT_REG_BP,
  INDIRECT_REG_BX,
};

enum register_encoding {
  REG_AL_AX = 0b000,
  REG_CL_CX = 0b001,
  REG_DL_DX = 0b010,
  REG_BL_BX = 0b011,
  REG_AH_SP = 0b100,
  REG_CH_BP = 0b101,
  REG_DH_SI = 0b110,
  REG_BH_DI = 0b111,
};

struct operand {
  enum operand_mode mode;
  enum operand_size size;
  union {
    enum indirect_register_encoding indirect_reg;
    enum register_encoding reg;
  };
  union {
    i8 disp8;
    i16 disp16;
    u8 immediate8;
    u16 immediate16;
  };
};

enum instruction_type {
  ADD,
  HLT,
  INC,
  JE,
  LOOP,
  MOV,
  NOP,
  TEST,
};

struct instruction {
  enum instruction_type type;
  struct operand destination;
  struct operand source;
};

#endif // INSTRUCTION_H_
