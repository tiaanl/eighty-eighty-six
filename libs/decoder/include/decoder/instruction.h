#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <base/platform.h>
#include <decoder/registers.h>
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
  ot_far_jump,
  ot_offset,
  ot_segment_register,
  ot_flags,
  ot_es_di,
  ot_ds_si,
};

enum operand_size {
  os_none, // Size is irrelevant
  os_8,    // 8-bit
  os_16,   // 16-bit
};

enum indirect_memory_encoding {
  ime_bx_si,
  ime_bx_di,
  ime_bp_si,
  ime_bp_di,
  ime_si,
  ime_di,
  ime_bp,
  ime_bx,
};

struct operand_displacement {
  enum indirect_memory_encoding encoding;
  i16 displacement;
};

struct operand_indirect {
  enum indirect_memory_encoding encoding;
};

struct operand_direct {
  u16 address;
};

struct operand_direct_with_segment {
  u16 segment;
  u16 offset;
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

struct operand_far_jump {
  u16 segment;
  u16 offset;
};

struct operand_offset {
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
  struct operand_far_jump as_far_jump;
  struct operand_offset as_offset;
  struct operand_segment_register as_segment_register;
};

struct operand {
  enum operand_type type;
  enum operand_size size;
  union operand_data data;
};

enum instruction_type {
  it_aaa,
  it_aad,
  it_aam,
  it_aas,
  it_adc,
  it_add,
  it_and,
  it_arpl,
  it_bound,
  it_call,
  it_callf,
  it_cbw,
  it_clc,
  it_cld,
  it_cli,
  it_cmc,
  it_cmp,
  it_cmps,
  it_cwd,
  it_daa,
  it_das,
  it_dec,
  it_div,
  it_enter,
  it_fwait,
  it_hlt,
  it_idiv,
  it_imul,
  it_in,
  it_inc,
  it_insb,
  it_insw,
  it_int,
  it_int1,
  it_int3,
  it_into,
  it_iret,
  it_jae,
  it_jb,
  it_jbe,
  it_jcxz,
  it_jl,
  it_jle,
  it_jmp,
  it_jnb,
  it_jnbe,
  it_jnl,
  it_jnle,
  it_jno,
  it_jnp,
  it_jns,
  it_jnz,
  it_jo,
  it_jp,
  it_js,
  it_jz,
  it_lahf,
  it_lds,
  it_lea,
  it_leave,
  it_les,
  it_lods,
  it_loop,
  it_loope,
  it_loopne,
  it_mov,
  it_movs,
  it_mul,
  it_neg,
  it_noop,
  it_not,
  it_or,
  it_out,
  it_outs,
  it_pop,
  it_popa,
  it_popf,
  it_push,
  it_pusha,
  it_pushf,
  it_rcl,
  it_rcr,
  it_ret,
  it_retf,
  it_rol,
  it_ror,
  it_sahf,
  it_salc,
  it_sar,
  it_sbb,
  it_scas,
  it_shl,
  it_shr,
  it_stc,
  it_std,
  it_sti,
  it_stos,
  it_sub,
  it_test,
  it_xchg,
  it_xlat,
  it_xor,

  it_prefix,
  it_group,
  it_invalid,

  instruction_type_count,
};

enum rep_prefix {
  rp_none,
  rp_rep,
  rp_repne,
};

struct instruction {
  enum instruction_type type;
  enum segment_register segment_register;
  enum rep_prefix rep_prefix;

  struct operand destination;
  struct operand source;
  struct operand third;

  u8 buffer[16];
  u8 instruction_size;
};

// typedef void (*execute_func)(struct cpu *cpu, struct instruction *instruction);
//
// struct instruction_table_mapping {
//  enum instruction_type instruction_type;
//};

void instruction_init(struct instruction *instruction);

const char *instruction_type_to_string(enum instruction_type instruction_type);

#endif // INSTRUCTION_H_
