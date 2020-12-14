#ifndef DECODER_OP_CODE_TABLE_H_
#define DECODER_OP_CODE_TABLE_H_

#include "../instruction.h"

enum operand_type {
  OPERAND_NONE,
  REG_AL,
  REG_AX,
  REG_SEG,
  MEM_8,
  MEM_16,
  REG_8,
  REG_16,
  IMM_8,
  IMM_16,
  JMP_8,
  JMP_16,
};

struct op_code_mapping {
  enum instruction_type instruction_type;
  enum operand_type destination_operand_type;
  enum operand_type source_operand_type;
  enum operand_type third;
  int (*decoder_func)(struct op_code_mapping *, const u8 *, unsigned, struct instruction *);
};

extern struct op_code_mapping op_code_table[];

#endif // DECODER_OP_CODE_TABLE_H_
