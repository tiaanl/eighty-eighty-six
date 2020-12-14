#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "decoder/mod_reg_rm.h"
#include "platform.h"
#include "registers.h"

#include <string.h>

enum operand_size {
  OPERAND_SIZE_8 = 0x00,
  OPERAND_SIZE_16 = 0x01,
};

enum operand_mode {
  // MOD bits on modR/M
  OPERAND_MODE_INDIRECT,        // 0b00
  OPERAND_MODE_DISPLACEMENT_8,  // 0b01
  OPERAND_MODE_DISPLACEMENT_16, // 0b10
  OPERAND_MODE_REGISTER,        // 0b11

  // Special when MOD == INDIRECT and R/M == 0b101.
  OPERAND_MODE_DIRECT,

  // When an immediate value was passed.
  OPERAND_MODE_IMMEDIATE,

  // Points to a segment register.
  OPERAND_MODE_SEGMENT_REGISTER,

  OPERAND_MODE_NONE = 0xff,
};

enum segment_register_encoding {
  SEG_REG_ES, // 0b00
  SEG_REG_CS, // 0b01
  SEG_REG_SS, // 0b10
  SEG_REG_DS, // 0b11
};

struct operand {
  enum operand_mode mode;
  enum operand_size size;
  union {
    enum mrrm_rm indirect_reg;
    enum mrrm_reg reg;
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
