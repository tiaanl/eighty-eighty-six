#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include "instruction.h"

struct cpu;

enum operand_type {
  REG_AL,
  REG_AX,
  MEM_8,
  MEM_16,
  REG_8,
  REG_16,
  IMM_8,
  IMM_16,
};

struct op_code_mapping {
  enum instruction_type instruction_type;
  enum operand_type destination_operand_type;
  enum operand_type source_operand_type;
  int (*decoder_func)(const u8 *, unsigned, struct instruction *);
};

extern struct op_code_mapping op_code_table[];

const char *instruction_type_to_string(enum instruction_type instruction_type);

#endif // INSTRUCTIONS_H_
