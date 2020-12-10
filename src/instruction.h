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
};

struct operand {
  enum operand_mode mode;
  union {
    u8 disp8;
    u16 disp16;
    u8 reg;
    u16 addr;
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
