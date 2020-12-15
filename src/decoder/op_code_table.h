#ifndef DECODER_OP_CODE_TABLE_H_
#define DECODER_OP_CODE_TABLE_H_

#include "../instruction.h"

enum decode_type {
  DT_NONE,
  DT_REG_AL,
  DT_REG_AX,
  DT_REG_8,
  DT_REG_16,
  DT_DST_8,
  DT_DST_16,
  DT_MEM_8,
  DT_MEM_16,
  DT_IMM_8,
  DT_IMM_16,
  DT_JMP_8,
  DT_JMP_16,
  DT_SEG_REG,
};

enum decode_flags {
  DF_HAS_MOD_RM = 0x01,
};

struct op_code_mapping {
  enum instruction_type instruction_type;
  enum decode_type destination_type;
  enum decode_type source_type;
  enum decode_type third_type;
  u8 flags;
};

extern struct op_code_mapping op_code_table[];

#endif // DECODER_OP_CODE_TABLE_H_
