#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "platform.h"
#include "registers.h"

#include <string.h>

enum operand_type {
  OT_DISPLACEMENT,
  OT_INDIRECT,
  OT_DIRECT,
  OT_REGISTER,
  OT_IMMEDIATE,
  OT_JUMP,
  OT_SEGMENT_REGISTER,

  OT_NONE,
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

struct operand_jump {
  i16 offset;
};

struct operand_segment_register {
  enum segment_register reg;
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
    struct operand_jump as_jump;
    struct operand_segment_register as_segment_register;
  };
};

enum instruction_type {
  INVALID,
  NOOP,

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
  struct operand third;

#if !defined(NDEBUG)
  u8 buffer[16];
  u8 instruction_size;
#endif
};

#endif // INSTRUCTION_H_
