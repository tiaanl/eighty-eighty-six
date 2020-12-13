#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "platform.h"
#include "registers.h"

#include <string.h>

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

  // Points to a segment register.
  OPERAND_MODE_SEGMENT_REGISTER = 0b110,

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

enum segment_register_encoding {
  SEG_REG_ES = 0b00,
  SEG_REG_CS = 0b01,
  SEG_REG_SS = 0b10,
  SEG_REG_DS = 0b11,
};

struct operand {
  enum operand_mode mode;
  enum operand_size size;
  union {
    enum indirect_register_encoding indirect_reg;
    enum register_encoding reg;
    enum segment_register_encoding segment_reg;
  };
  union {
    i8 disp8;
    i16 disp16;
    u8 immediate8;
    u16 immediate16;
  };
};

enum instruction_type {
  AAA,
  AAD,
  AAM,
  AAS,
  ADC,
  ADD,
  AND,
  ARPL,
  BOUND,
  CALL,
  CALLF,
  CBW,
  CLC,
  CLD,
  CLI,
  CMC,
  CMP,
  CWD,
  DAA,
  DAS,
  DEC,
  ENTER,
  FWAIT,
  HLT,
  IMUL,
  IN,
  INC,
  INT,
  INT1,
  INT3,
  INTO,
  IRET,
  JAE,
  JB,
  JBE,
  JCXZ,
  JL,
  JLE,
  JMP,
  JMPF,
  JNB,
  JNBE,
  JNL,
  JNLE,
  JNO,
  JNP,
  JNS,
  JNZ,
  JO,
  JP,
  JS,
  JZ,
  LAHF,
  LDS,
  LEA,
  LEAVE,
  LES,
  LOOP,
  LOOPE,
  LOOPNE,
  MOV,
  NOP,
  OR,
  OUT,
  POP,
  POPA,
  POPF,
  PUSH,
  PUSHA,
  PUSHF,
  REP_CMPSB,
  REP_CMPSW,
  REP_INSB,
  REP_INSW,
  REP_LODSB,
  REP_LODSW,
  REP_MOVSB,
  REP_MOVSW,
  REP_OUTSB,
  REP_OUTSW,
  REP_SCASB,
  REP_SCASW,
  REP_STOSB,
  REP_STOSW,
  RET,
  RETF,
  SAHF,
  SALC,
  SAR,
  SBB,
  STC,
  STD,
  STI,
  SUB,
  TEST,
  XCHG,
  XLAT,
  XOR,
};

struct instruction {
  enum instruction_type type;
  enum segment_register segment_register;
  struct operand destination;
  struct operand source;
};

#endif // INSTRUCTION_H_
