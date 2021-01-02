#ifndef DECODER_OP_CODE_TABLE_H_
#define DECODER_OP_CODE_TABLE_H_

#include "decoder/instruction.h"
#include "decoder_context.h"

#include <base/streams.h>

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

  // Operand has direct memory address, including a segment register, e.g. mov 0xf000:0xfff0
  DT_SEG_DIRECT,

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

enum addressing_method {
  // None
  am_Xx,

  // General registers (8-bit)
  am_Rb,
  am_AL,

  // General registers (16-bit)
  am_Rw,
  am_AX,
  am_DX,

  // Segment registers
  am_ES,
  am_CS,
  am_SS,
  am_DS,

  // The immediate value 1
  am_1,

  // Addressing methods
  am_Ap,
  am_Eb,
  am_Ew,
  am_Fw,
  am_Gb,
  am_Gw,
  am_Ib,
  am_Iw,
  am_Jb,
  am_Jw,
  am_M,
  am_Ma,
  am_Ob,
  am_Ow,
  am_Sw,
  am_Xb,
  am_Xw,
  am_Yb,
  am_Yw,
};

typedef void (*decode_func)(struct decoder_context *decoder_context,
                            struct instruction *instruction);

struct op_code_mapping {
  enum instruction_type instruction_type;
  enum addressing_method tmp1;
  enum addressing_method tmp2;
  enum addressing_method tmp3;
  decode_func decode_func;
  struct op_code_mapping *group_table;
};

extern struct op_code_mapping op_code_table[];
extern struct op_code_mapping op_code_table_group_f7[];

#endif // DECODER_OP_CODE_TABLE_H_
