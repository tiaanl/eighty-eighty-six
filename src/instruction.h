#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "address.h"
#include "mod_rm.h"
#include "platform.h"
#include "registers.h"

#include <string.h>

enum operand_type {
  ot_none,
  ot_displacement,
  ot_indirect,
  ot_direct,
  ot_direct_with_segment,
  ot_register,
  ot_immediate,
  ot_jump,
  ot_segment_register,
};

enum operand_size {
  os_8,
  os_16,
};

struct operand_displacement {
  enum mod_rm_mem encoding;
  i16 displacement;
};

struct operand_indirect {
  enum mod_rm_mem encoding;
};

struct operand_direct {
  u16 address;
};

struct operand_direct_with_segment {
  struct address address;
};

union operand_register {
  enum register_8 reg_8;
  enum register_16 reg_16;
};

union operand_immediate {
  u8 immediate_8;
  u16 immediate_16;
};

struct operand_jump {
  i16 offset;
};

struct operand_segment_register {
  enum segment_register reg;
};

union operand_data {
  struct operand_displacement as_displacement;
  struct operand_indirect as_indirect;
  struct operand_direct as_direct;
  struct operand_direct_with_segment as_direct_with_segment;
  union operand_register as_register;
  union operand_immediate as_immediate;
  struct operand_jump as_jump;
  struct operand_segment_register as_segment_register;
};

struct operand {
  enum operand_type type;
  enum operand_size size;
  union operand_data data;
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
  CMPSB,
  CMPSW,
  INSB,
  INSW,
  LODSB,
  LODSW,
  MOVSB,
  MOVSW,
  OUTSB,
  OUTSW,
  SCASB,
  SCASW,
  STOSB,
  STOSW,
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

void instruction_init(struct instruction *instruction);

const char *instruction_type_to_string(enum instruction_type instruction_type);

#endif // INSTRUCTION_H_
