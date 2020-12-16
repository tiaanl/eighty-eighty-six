#ifndef DECODER_OP_CODE_TABLE_H_
#define DECODER_OP_CODE_TABLE_H_

#include "../instruction.h"

enum decode_type {
  // No operand for this position.
  DT_NONE,

  // Use the mod r/m byte's `rm` field and decode depending on `mod` field.
  DT_MOD_RM_RM_8,
  DT_MOD_RM_RM_16,

  // Use the mod r/m byte's `reg` field and decode as register.
  DT_MOD_RM_REG_8,
  DT_MOD_RM_REG_16,

  // Get an immediate value from the data bytes.
  DT_IMM_8,
  DT_IMM_16,

  // Get a displacement offset from the data bytes.
  DT_JMP_8,
  DT_JMP_16,

  // Get a segment register encoding from the op code. (00111000)
  DT_SEGMENT_REG,

  // Specifically these registers.
  DT_AL,
  DT_AX,
  DT_DX,

  // Get a register encoding from the op_code. (00000111)
  DT_OP_CODE_REG_8,
  DT_OP_CODE_REG_16,

  // NOT SURE
  DT_Ob,
  DT_Ow,
  DT_Ap,
  DT_Ma,
  DT_Fw,
  DT_Xb,
  DT_Xw,
  DT_Yb,
  DT_Yw,
  DT_M,
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
