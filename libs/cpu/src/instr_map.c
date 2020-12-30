#include "instr_map.h"

#include <base/print_format.h>
#include <stdio.h>

byte fetch_operand_value_byte(struct cpu *cpu, struct operand *operand) {
  assert(operand->size == os_8);

  switch (operand->type) {
    default:
      assert(0);
      break;
  }
}

void store_operand_value_byte(struct cpu *cpu, struct operand *operand, byte value) {
  assert(operand->size == os_8);

  switch (operand->type) {
    case ot_register:
      cpu->regs.byte[operand->data.as_register.reg_8] = value;
      break;

    default:
      assert(0);
      break;
  }
}

word fetch_operand_value_word(struct cpu *cpu, struct operand *operand) {
  assert(operand->size == os_16);

  switch (operand->type) {
    case ot_immediate:
      return operand->data.as_immediate.immediate_16;

    default:
      assert(0);
      break;
  }
}

void store_operand_value_word(struct cpu *cpu, struct operand *operand, word value) {
  assert(operand->size == os_16);

  switch (operand->type) {
    case ot_register:
      cpu->regs.word[operand->data.as_register.reg_16] = value;
      break;

    default:
      assert(0);
      break;
  }
}

/* ---------------------------------------------------------------------------------------------- */

void exec_int(struct cpu *cpu, struct instruction *instruction) {
  UNUSED(cpu);

  printf("## int " HEX_8 "\n\n", instruction->destination.data.as_immediate.immediate_8);
}

void exec_jmp(struct cpu *cpu, struct instruction *instruction) {
  switch (instruction->destination.type) {
    case ot_far_jump:
      cpu->segment_16[CS] = instruction->destination.data.as_far_jump.segment;
      cpu->ip = instruction->destination.data.as_far_jump.offset;
      break;

    case ot_jump:
      cpu->ip += instruction->destination.data.as_jump.offset;
      break;

    default:
      assert(0);
      break;
  }
}

void exec_mov(struct cpu *cpu, struct instruction *instruction) {
  switch (instruction->destination.size) {
    case os_8: {
      byte source = fetch_operand_value_byte(cpu, &instruction->source);
      store_operand_value_byte(cpu, &instruction->destination, source);
      break;
    }

    case os_16: {
      word source = fetch_operand_value_word(cpu, &instruction->source);
      store_operand_value_word(cpu, &instruction->destination, source);
      break;
    }

    default:
      assert(0);
      break;
  }
}

struct instr_mapping instr_map[] = {
    {it_aaa, 0},        //
    {it_aad, 0},        //
    {it_aam, 0},        //
    {it_aas, 0},        //
    {it_adc, 0},        //
    {it_add, 0},        //
    {it_and, 0},        //
    {it_arpl, 0},       //
    {it_bound, 0},      //
    {it_call, 0},       //
    {it_callf, 0},      //
    {it_cbw, 0},        //
    {it_clc, 0},        //
    {it_cld, 0},        //
    {it_cli, 0},        //
    {it_cmc, 0},        //
    {it_cmp, 0},        //
    {it_cmps, 0},       //
    {it_cwd, 0},        //
    {it_daa, 0},        //
    {it_das, 0},        //
    {it_dec, 0},        //
    {it_div, 0},        //
    {it_enter, 0},      //
    {it_fwait, 0},      //
    {it_hlt, 0},        //
    {it_idiv, 0},       //
    {it_imul, 0},       //
    {it_in, 0},         //
    {it_inc, 0},        //
    {it_ins, 0},        //
    {it_int, exec_int}, //
    {it_int1, 0},       //
    {it_int3, 0},       //
    {it_into, 0},       //
    {it_iret, 0},       //
    {it_jb, 0},         //
    {it_jbe, 0},        //
    {it_jcxz, 0},       //
    {it_jl, 0},         //
    {it_jle, 0},        //
    {it_jmp, exec_jmp}, //
    {it_jnb, 0},        //
    {it_jnbe, 0},       //
    {it_jnl, 0},        //
    {it_jnle, 0},       //
    {it_jno, 0},        //
    {it_jnp, 0},        //
    {it_jns, 0},        //
    {it_jnz, 0},        //
    {it_jo, 0},         //
    {it_jp, 0},         //
    {it_js, 0},         //
    {it_jz, 0},         //
    {it_lahf, 0},       //
    {it_lds, 0},        //
    {it_lea, 0},        //
    {it_leave, 0},      //
    {it_les, 0},        //
    {it_lods, 0},       //
    {it_loop, 0},       //
    {it_loope, 0},      //
    {it_loopne, 0},     //
    {it_mov, exec_mov}, //
    {it_movs, 0},       //
    {it_mul, 0},        //
    {it_neg, 0},        //
    {it_not, 0},        //
    {it_or, 0},         //
    {it_out, 0},        //
    {it_outs, 0},       //
    {it_pop, 0},        //
    {it_popa, 0},       //
    {it_popf, 0},       //
    {it_push, 0},       //
    {it_pusha, 0},      //
    {it_pushf, 0},      //
    {it_rcl, 0},        //
    {it_rcr, 0},        //
    {it_ret, 0},        //
    {it_retf, 0},       //
    {it_rol, 0},        //
    {it_ror, 0},        //
    {it_sahf, 0},       //
    {it_salc, 0},       //
    {it_sar, 0},        //
    {it_sbb, 0},        //
    {it_scas, 0},       //
    {it_shl, 0},        //
    {it_shr, 0},        //
    {it_stc, 0},        //
    {it_std, 0},        //
    {it_sti, 0},        //
    {it_stos, 0},       //
    {it_sub, 0},        //
    {it_test, 0},       //
    {it_xchg, 0},       //
    {it_xlat, 0},       //
    {it_xor, 0},        //
};
