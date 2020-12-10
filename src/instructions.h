#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include "instruction.h"

struct cpu;

enum mod_rm_mod {
  MOD_INDIRECT = 0b00,
  MOD_BYTE = 0b01,
  MOD_DOUBLE_WORD = 0b10,
  MOD_REGISTER = 0b11,
};

enum mod_rm_reg {
  REG_AL_AX = 0b000,
  REG_CL_CX = 0b001,
  REG_DL_DX = 0b010,
  REG_BL_BX = 0b011,
  REG_AH_SP = 0b100,
  REG_CH_BP = 0b101,
  REG_DH_SI = 0b110,
  REG_BH_DI = 0b111,
};

enum mod_rm_rm {
  MOD_RM_RM_BX_SI,
  MOD_RM_RM_BX_DI,
  MOD_RM_RM_BP_SI,
  MOD_RM_RM_BP_DI,
  MOD_RM_RM_SI,
  MOD_RM_RM_DI,
  MOD_RM_RM_BP,
  MOD_RM_RM_BX,
};

enum operand_type {
  MEM_8,
  MEM_16,
  REG_8,
  REG_16,
  IMM_8,
  IMM_16,
};

struct instruction;

struct op_code_mapping {
  enum instruction_type instruction_type;
  enum operand_type destination_operand_type;
  enum operand_type source_operand_type;
  void (*instruction_func)(struct cpu *);
};

extern struct op_code_mapping op_code_table[];

const char *instruction_type_to_string(enum instruction_type instruction_type);

#endif // INSTRUCTIONS_H_
