#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "address.h"
#include "platform.h"
#include "registers.h"

#include <string.h>

enum operand_type {
  ot_displacement,
  ot_indirect,
  ot_direct,
  ot_direct_with_segment,
  ot_register,
  ot_immediate,
  ot_jump,
  ot_segment_register,

  ot_none,
};

enum operand_size {
  os_8,
  os_16,
};

enum indirect_encoding {
  ie_bx_si,
  ie_bx_di,
  ie_bp_si,
  ie_bp_di,
  ie_si,
  ie_di,
  ie_bp,
  ie_bx,
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

struct operand_direct_with_segment {
  struct address address;
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
    struct operand_direct_with_segment as_direct_with_segment;
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

const char *instruction_type_to_string(enum instruction_type instruction_type);

#endif // INSTRUCTION_H_
