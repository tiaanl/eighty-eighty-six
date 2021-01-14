#ifndef DECODER_OP_CODE_TABLE_H_
#define DECODER_OP_CODE_TABLE_H_

#include "decoder_context.h"

#include <base/streams.h>
#include <instructions/instructions.h>

enum op_code_type {
  oct_invalid,
  oct_instruction,
  oct_prefix,
  oct_group,
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
  enum op_code_type op_code_type;
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
