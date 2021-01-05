#include "instr_map.h"

#include <base/print_format.h>
#include <stdio.h>

static const byte parity_flag_table[0x100] = {
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
};

byte fetch_operand_value_byte(struct cpu *cpu, struct operand *operand) {
  assert(operand->size == os_8);

  switch (operand->type) {
    case ot_register:
      return cpu->regs.byte[operand->data.as_register.reg_8];

    case ot_immediate:
      return operand->data.as_immediate.immediate_8;

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
    case ot_direct: {
      u32 address = flatten_address(segment_offset(cpu->segs[operand->data.as_direct.seg_reg],
                                                   operand->data.as_direct.address));
      byte low = bus_fetch_byte(cpu->bus, address);
      byte high = bus_fetch_byte(cpu->bus, address + 1);
      return low + (high << 8);
    }

    case ot_register:
      return cpu->regs.word[operand->data.as_register.reg_16];

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
    case ot_indirect: {
      u16 segment = cpu->segs[operand->data.as_indirect.seg_reg];
      u16 offset = 0;

      switch (operand->data.as_indirect.encoding) {
        case ime_bx_si:
          offset = cpu->regs.word[BX] + cpu->regs.word[SI];
          break;

        case ime_bx_di:
          offset = cpu->regs.word[BX] + cpu->regs.word[DI];
          break;

        case ime_bp_si:
          offset = cpu->regs.word[BP] + cpu->regs.word[SI];
          break;

        case ime_bp_di:
          offset = cpu->regs.word[BP] + cpu->regs.word[DI];
          break;

        case ime_si:
          offset = cpu->regs.word[SI];
          break;

        case ime_di:
          offset = cpu->regs.word[DI];
          break;

        case ime_bp:
          offset = cpu->regs.word[BP];
          break;

        case ime_bx:
          offset = cpu->regs.word[BX];
          break;

        default:
          assert(0);
          break;
      }

      u32 address = flatten_address(segment_offset(segment, offset));

      bus_store_byte(cpu->bus, address, value & 0xff);
      bus_store_byte(cpu->bus, address + 1, value >> 8);
      break;
    }

    case ot_register:
      cpu->regs.word[operand->data.as_register.reg_16] = value;
      break;

    case ot_segment_register:
      cpu->segs[operand->data.as_segment_register.reg] = value;
      break;

    default:
      assert(0);
      break;
  }
}

void flags_sign_zero_parity_byte(union flags *flags, byte value) {
  flags->zero = (!value) ? 1 : 0;
  flags->sign = (value & 0x80) ? 1 : 0;
  flags->parity = parity_flag_table[value];
}

void flags_sign_zero_parity_word(union flags *flags, word value) {
  flags->zero = (!value) ? 1 : 0;
  flags->sign = (value & 0x8000) ? 1 : 0;
  flags->parity = parity_flag_table[value & 0xff];
}

void flags_sub_byte(union flags *flags, byte left, byte right) {
  word result = (word)left - (word)right;

  flags_sign_zero_parity_byte(flags, result);

  flags->carry = (result & 0xff00) ? 1 : 0;
  flags->overflow = ((result ^ left) & (left ^ right) & 0x80) ? 1 : 0;
  flags->adjust = ((left ^ right ^ result) & 0x10) ? 1 : 0;
}

void flags_sub_word(union flags *flags, word left, word right) {
  dword result = (dword)left - (dword)right;

  flags_sign_zero_parity_word(flags, result);

  flags->carry = (result & 0xffff0000) ? 1 : 0;
  flags->overflow = ((result ^ left) & (left ^ right) & 0x8000) ? 1 : 0;
  flags->adjust = ((left ^ right ^ result) & 0x10) ? 1 : 0;
}

#define flags_add_byte flags_sub_byte
#define flags_add_word flags_sub_word

#define FLAGS_LOG(SIZE)                                                                            \
  void flags_log_##SIZE(union flags *flags, SIZE value) {                                          \
    flags_sign_zero_parity_##SIZE(flags, value);                                                   \
                                                                                                   \
    flags->carry = 0;                                                                              \
    flags->overflow = 0;                                                                           \
  }

FLAGS_LOG(byte)
FLAGS_LOG(word)

#undef FLAGS_LOG

static void push_word(struct cpu *cpu, word value) {
  cpu->regs.word[SP] -= sizeof(word);
  u32 addr = flatten_address(segment_offset(cpu->segs[SS], cpu->regs.word[SP]));
  bus_store_word(cpu->bus, addr, value);
}

static word pop(struct cpu *cpu) {
  u32 addr = flatten_address(segment_offset(cpu->segs[SS], cpu->regs.word[SP]));
  word value = bus_fetch_word(cpu->bus, addr);
  cpu->regs.word[SP] += sizeof(word);
  return value;
}

/* ---------------------------------------------------------------------------------------------- */

void exec_add(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_add);

  switch (instruction->destination.size) {
    case os_8: {
      byte d = fetch_operand_value_byte(cpu, &instruction->destination);
      byte s = fetch_operand_value_byte(cpu, &instruction->source);
      flags_add_byte(&cpu->flags, d, s);
      store_operand_value_byte(cpu, &instruction->destination, d + s);
      break;
    }

    case os_16: {
      word d = fetch_operand_value_word(cpu, &instruction->destination);
      word s = fetch_operand_value_word(cpu, &instruction->source);
      flags_add_word(&cpu->flags, d, s);
      store_operand_value_word(cpu, &instruction->destination, d + s);
      break;
    }

    default:
      assert(0);
      break;
  }
}

void exec_call(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_call);

  i16 offset = instruction->destination.data.as_jump.offset;
  push_word(cpu, cpu->ip);
  cpu->ip += offset;
}

void exec_cld(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_cld);

  cpu->flags.direction = 0;
}

void exec_cli(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_cli);

  cpu->flags.interrupt = 0;
}

void exec_cmp(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_cmp);
  assert(instruction->destination.size == instruction->source.size);

  switch (instruction->destination.size) {
    case os_8: {
      byte left = fetch_operand_value_byte(cpu, &instruction->destination);
      byte right = fetch_operand_value_byte(cpu, &instruction->source);
      flags_sub_byte(&cpu->flags, left, right);
      break;
    }

    case os_16:
      break;

    default:
      assert(0);
      break;
  }
}

void exec_dec(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_dec);

#define OP(SIZE)                                                                                   \
  do {                                                                                             \
    SIZE left = fetch_operand_value_##SIZE(cpu, &instruction->destination);                        \
    SIZE right = 1;                                                                                \
                                                                                                   \
    SIZE result = left - right;                                                                    \
                                                                                                   \
    byte carry_temp = cpu->flags.carry;                                                            \
    flags_sub_##SIZE(&cpu->flags, left, right);                                                    \
    cpu->flags.carry = carry_temp;                                                                 \
                                                                                                   \
    store_operand_value_##SIZE(cpu, &instruction->destination, result);                            \
  } while (0)

  switch (instruction->destination.size) {
    case os_8: {
      OP(byte);
      break;
    }

    case os_16: {
      OP(word);
      break;
    }

    default:
      assert(0);
      break;
  }

#undef OP
}

void exec_div(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_div);

  // TODO: Handle error cases

#define OP(SIZE, UPSIZE, OVERFLOW)                                                                 \
  do {                                                                                             \
    SIZE left = fetch_operand_value_##SIZE(cpu, &instruction->destination);                        \
    SIZE right = fetch_operand_value_##SIZE(cpu, &instruction->source);                            \
                                                                                                   \
    if (!right || (((UPSIZE)left / (UPSIZE)right) > (OVERFLOW))) {                                 \
    } else {                                                                                       \
      store_operand_value_##SIZE(cpu, &instruction->destination, left / right);                    \
    }                                                                                              \
  } while (0)

  switch (instruction->destination.size) {
    case os_8: {
      OP(byte, word, 0xff);
      break;
    }

    case os_16: {
      OP(word, dword, 0xffff);
      break;
    }

    default:
      assert(0);
      break;
  }

#undef OP
}

void exec_inc(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_inc);

#define OP(SIZE)                                                                                   \
  do {                                                                                             \
    SIZE left = fetch_operand_value_##SIZE(cpu, &instruction->destination);                        \
    SIZE right = 1;                                                                                \
                                                                                                   \
    SIZE result = left + right;                                                                    \
                                                                                                   \
    byte carry_temp = cpu->flags.carry;                                                            \
    flags_add_##SIZE(&cpu->flags, left, right);                                                    \
    cpu->flags.carry = carry_temp;                                                                 \
                                                                                                   \
    store_operand_value_##SIZE(cpu, &instruction->destination, result);                            \
  } while (0)

  switch (instruction->destination.size) {
    case os_8: {
      OP(byte);
      break;
    }

    case os_16: {
      OP(word);
      break;
    }

    default:
      assert(0);
      break;
  }

#undef OP
}

void exec_int(struct cpu *cpu, struct instruction *instruction) {
  UNUSED(cpu);
  assert(instruction->type == it_int);

  printf("## int " HEX_8 "\n\n", instruction->destination.data.as_immediate.immediate_8);
}

void exec_jmp(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_jmp);

  switch (instruction->destination.type) {
    case ot_far_jump:
      cpu->segs[CS] = instruction->destination.data.as_far_jump.segment;
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

void exec_jcxz(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_jcxz);
  assert(instruction->destination.type == ot_jump);

  i16 offset = instruction->destination.data.as_jump.offset;
  if (cpu->regs.word[CX] == 0) {
    cpu->ip += offset;
  }
}

void exec_jump_conditional(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->destination.size == os_8);

  i16 offset = instruction->destination.data.as_jump.offset;

  switch (instruction->type) {
    case it_jo:
      if (cpu->flags.overflow) {
        cpu->ip += offset;
      }
      break;

    case it_jno:
      if (!cpu->flags.overflow) {
        cpu->ip += offset;
      }
      break;

    case it_jb:
      if (cpu->flags.carry) {
        cpu->ip += offset;
      }
      break;

    case it_jnb:
      if (!cpu->flags.carry) {
        cpu->ip += offset;
      }
      break;

    case it_jz:
      if (cpu->flags.zero) {
        cpu->ip += offset;
      }
      break;

    case it_jnz:
      if (!cpu->flags.zero) {
        cpu->ip += offset;
      }
      break;

    case it_jbe:
      if (cpu->flags.carry || cpu->flags.zero) {
        cpu->ip += offset;
      }
      break;

    case it_jnbe:
      if (!cpu->flags.carry && !cpu->flags.zero) {
        cpu->ip += offset;
      }
      break;

    case it_js:
      if (cpu->flags.sign) {
        cpu->ip += offset;
      }
      break;

    case it_jns:
      if (!cpu->flags.sign) {
        cpu->ip += offset;
      }
      break;

    case it_jp:
      if (cpu->flags.parity) {
        cpu->ip += offset;
      }
      break;

    case it_jnp:
      if (!cpu->flags.parity) {
        cpu->ip += offset;
      }
      break;

    case it_jl:
      if (cpu->flags.sign != cpu->flags.overflow) {
        cpu->ip += offset;
      }
      break;

    case it_jnl:
      if (cpu->flags.sign == cpu->flags.overflow) {
        cpu->ip += offset;
      }
      break;

    case it_jle:
      if ((cpu->flags.sign != cpu->flags.overflow) || cpu->flags.zero) {
        cpu->ip += offset;
      }
      break;

    case it_jnle:
      if (!cpu->flags.zero && (cpu->flags.sign == cpu->flags.overflow)) {
        cpu->ip += offset;
      }
      break;

    default:
      assert(0);
      break;
  }
}

void exec_mov(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_mov);

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

void exec_out(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_out);

  word address = 0;
  if (instruction->destination.type == ot_register && instruction->destination.size == os_16 &&
      instruction->destination.data.as_register.reg_16 == DX) {
    address = cpu->regs.word[DX];
  } else if (instruction->destination.type == ot_immediate &&
             instruction->destination.size == os_8) {
    address = instruction->destination.data.as_immediate.immediate_8;
  } else {
    assert(0);
    return;
  }

  assert(instruction->source.type == ot_register);

  switch (instruction->source.size) {
    case os_8:
      ports_out(cpu->ports, address, fetch_operand_value_byte(cpu, &instruction->source));
      break;

    default:
      assert(0);
      break;
  }
}

void exec_push(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_push);

  switch (instruction->destination.size) {
    case os_8: {
      byte value = fetch_operand_value_byte(cpu, &instruction->destination);
      push_word(cpu, value);
      break;
    }

    case os_16: {
      word value = fetch_operand_value_word(cpu, &instruction->destination);
      push_word(cpu, value);
      break;
    }

    default:
      assert(0);
      break;
  }
}

void exec_ret(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_ret);

  cpu->ip = pop(cpu);
}

void exec_scas(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_scas);
  assert(instruction->destination.type == ot_register);
  assert(instruction->source.type == ot_es_di);

  if (instruction->rep_mode != rm_none && cpu->regs.word[CX] == 0) {
    return;
  }

  u32 es_di = flatten_address(segment_offset(cpu->segs[ES], cpu->regs.word[DI]));

  switch (instruction->destination.size) {
    case os_8: {
      byte left = cpu->regs.byte[AL];
      byte right = bus_fetch_byte(cpu->bus, es_di);
      flags_sub_byte(&cpu->flags, left, right);
      break;
    }

    case os_16: {
      word left = cpu->regs.word[AX];
      word right = bus_fetch_word(cpu->bus, es_di);
      flags_sub_word(&cpu->flags, left, right);
      break;
    }

    default:
      assert(0);
      break;
  }

  if (cpu->flags.direction) {
    cpu->regs.word[DI] -= 1;
  } else {
    cpu->regs.word[DI] += 1;
  }

  if (instruction->rep_mode != rm_none) {
    cpu->regs.word[CX] -= 1;
  }

  if (instruction->rep_mode == rm_rep && !cpu->flags.zero) {
    return;
  } else if (instruction->rep_mode == rm_repne && cpu->flags.zero) {
    return;
  }

  assert(instruction->instruction_size);
  // Move the IP back to where this instruction started so that it gets repeated.
  cpu->ip -= instruction->instruction_size;
}

void exec_stc(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_stc);

  cpu->flags.carry = 1;
}

void exec_std(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_std);

  cpu->flags.direction = 1;
}

void exec_sti(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_sti);

  cpu->flags.interrupt = 1;
}

void exec_stos(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_stos);
  assert(instruction->destination.type == ot_es_di);
  assert(instruction->source.type == ot_register);

  if (instruction->rep_mode != rm_none && cpu->regs.word[CX] == 0) {
    return;
  }

  u32 flat = flatten_address(segment_offset(cpu->segs[ES], cpu->regs.word[DI]));

  u8 size_in_bytes;

  switch (instruction->destination.size) {
    case os_8:
      bus_store_byte(cpu->bus, flat, cpu->regs.byte[AL]);
      size_in_bytes = 1;
      break;

    case os_16:
      bus_store_byte(cpu->bus, flat, cpu->regs.byte[AL]);
      bus_store_byte(cpu->bus, flat + 1, cpu->regs.byte[AH]);
      size_in_bytes = 2;
      break;

    default:
      assert(0);
      break;
  }

  if (cpu->flags.direction) {
    cpu->regs.word[DI] -= size_in_bytes;
  } else {
    cpu->regs.word[DI] += size_in_bytes;
  }

  if (instruction->rep_mode != rm_none) {
    cpu->regs.word[CX] -= 1;
  }

  assert(instruction->instruction_size);
  // Move the IP back to where this instruction started so that it gets repeated.
  cpu->ip -= instruction->instruction_size;
}

void exec_xor(struct cpu *cpu, struct instruction *instruction) {
  assert(instruction->type == it_xor);
  assert(instruction->destination.size == instruction->source.size);

#define OP(SIZE)                                                                                   \
  do {                                                                                             \
    SIZE left = fetch_operand_value_##SIZE(cpu, &instruction->destination);                        \
    SIZE right = fetch_operand_value_##SIZE(cpu, &instruction->source);                            \
    SIZE result = left ^ right;                                                                    \
    flags_log_word(&cpu->flags, result);                                                           \
    store_operand_value_##SIZE(cpu, &instruction->destination, result);                            \
  } while (0)

  switch (instruction->destination.size) {
    case os_8: {
      OP(byte);
      break;
    }

    case os_16: {
      OP(word);
      break;
    }

    default:
      assert(0);
      break;
  }

#undef OP
}

struct instr_mapping instr_map[] = {
    {it_aaa, 0},                     //
    {it_aad, 0},                     //
    {it_aam, 0},                     //
    {it_aas, 0},                     //
    {it_adc, 0},                     //
    {it_add, exec_add},              //
    {it_and, 0},                     //
    {it_arpl, 0},                    //
    {it_bound, 0},                   //
    {it_call, exec_call},            //
    {it_callf, 0},                   //
    {it_cbw, 0},                     //
    {it_clc, 0},                     //
    {it_cld, exec_cld},              //
    {it_cli, exec_cli},              //
    {it_cmc, 0},                     //
    {it_cmp, exec_cmp},              //
    {it_cmps, 0},                    //
    {it_cwd, 0},                     //
    {it_daa, 0},                     //
    {it_das, 0},                     //
    {it_dec, exec_dec},              //
    {it_div, exec_div},              //
    {it_enter, 0},                   //
    {it_fwait, 0},                   //
    {it_hlt, 0},                     //
    {it_idiv, 0},                    //
    {it_imul, 0},                    //
    {it_in, 0},                      //
    {it_inc, exec_inc},              //
    {it_ins, 0},                     //
    {it_int, exec_int},              //
    {it_int1, 0},                    //
    {it_int3, 0},                    //
    {it_into, 0},                    //
    {it_iret, 0},                    //
    {it_jb, exec_jump_conditional},  //
    {it_jbe, 0},                     //
    {it_jcxz, exec_jcxz},            //
    {it_jl, 0},                      //
    {it_jle, 0},                     //
    {it_jmp, exec_jmp},              //
    {it_jnb, 0},                     //
    {it_jnbe, 0},                    //
    {it_jnl, 0},                     //
    {it_jnle, 0},                    //
    {it_jno, 0},                     //
    {it_jnp, 0},                     //
    {it_jns, 0},                     //
    {it_jnz, exec_jump_conditional}, //
    {it_jo, 0},                      //
    {it_jp, 0},                      //
    {it_js, 0},                      //
    {it_jz, 0},                      //
    {it_lahf, 0},                    //
    {it_lds, 0},                     //
    {it_lea, 0},                     //
    {it_leave, 0},                   //
    {it_les, 0},                     //
    {it_lods, 0},                    //
    {it_loop, 0},                    //
    {it_loope, 0},                   //
    {it_loopne, 0},                  //
    {it_mov, exec_mov},              //
    {it_movs, 0},                    //
    {it_mul, 0},                     //
    {it_neg, 0},                     //
    {it_not, 0},                     //
    {it_or, 0},                      //
    {it_out, exec_out},              //
    {it_outs, 0},                    //
    {it_pop, 0},                     //
    {it_popa, 0},                    //
    {it_popf, 0},                    //
    {it_push, exec_push},            //
    {it_pusha, 0},                   //
    {it_pushf, 0},                   //
    {it_rcl, 0},                     //
    {it_rcr, 0},                     //
    {it_ret, exec_ret},              //
    {it_retf, 0},                    //
    {it_rol, 0},                     //
    {it_ror, 0},                     //
    {it_sahf, 0},                    //
    {it_salc, 0},                    //
    {it_sar, 0},                     //
    {it_sbb, 0},                     //
    {it_scas, exec_scas},            //
    {it_shl, 0},                     //
    {it_shr, 0},                     //
    {it_stc, exec_stc},              //
    {it_std, exec_std},              //
    {it_sti, exec_sti},              //
    {it_stos, exec_stos},            //
    {it_sub, 0},                     //
    {it_test, 0},                    //
    {it_xchg, 0},                    //
    {it_xlat, 0},                    //
    {it_xor, exec_xor},              //
};
