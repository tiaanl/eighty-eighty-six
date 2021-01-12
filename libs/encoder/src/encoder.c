#include "encoder/encoder.h"

#include <assert.h>

static int encode_aaa(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_aaa);

  return 0;
}

static int encode_aad(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_aad);

  return 0;
}

static int encode_aam(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_aam);

  return 0;
}

static int encode_aas(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_aas);

  return 0;
}

static int encode_adc(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_adc);

  return 0;
}

static int encode_add(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_add);

  return 0;
}

static int encode_and(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_and);

  return 0;
}

static int encode_arpl(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_arpl);

  return 0;
}

static int encode_bound(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_bound);

  return 0;
}

static int encode_call(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_call);

  return 0;
}

static int encode_callf(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_callf);

  return 0;
}

static int encode_cbw(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_cbw);

  return 0;
}
static int encode_clc(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_clc);

  return 0;
}

static int encode_cld(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_cld);

  return 0;
}

static int encode_cli(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_cli);

  return 0;
}

static int encode_cmc(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_cmc);

  return 0;
}

static int encode_cmp(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_cmp);

  return 0;
}

static int encode_cmps(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_cmps);

  return 0;
}

static int encode_cwd(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_cwd);

  return 0;
}

static int encode_daa(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_daa);

  return 0;
}

static int encode_das(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_das);

  return 0;
}

static int encode_dec(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_dec);

  return 0;
}

static int encode_div(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_div);

  return 0;
}

static int encode_enter(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_enter);

  return 0;
}

static int encode_fwait(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_fwait);

  return 0;
}

static int encode_hlt(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_hlt);

  return 0;
}

static int encode_idiv(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_idiv);

  return 0;
}

static int encode_imul(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_imul);

  return 0;
}

static int encode_in(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_in);

  return 0;
}

static int encode_inc(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_inc);

  return 0;
}

static int encode_ins(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_ins);

  return 0;
}

static int encode_int(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_int);

  return 0;
}

static int encode_int1(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_int1);

  return 0;
}

static int encode_int3(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_int3);

  return 0;
}

static int encode_into(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_into);

  return 0;
}

static int encode_iret(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_iret);

  return 0;
}

static int encode_jb(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jb);

  return 0;
}

static int encode_jbe(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jbe);

  return 0;
}

static int encode_jcxz(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jcxz);

  return 0;
}

static int encode_jl(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jl);

  return 0;
}

static int encode_jle(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jle);

  return 0;
}

static int encode_jmp(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jmp);

  return 0;
}

static int encode_jnb(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jnb);

  return 0;
}

static int encode_jnbe(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jnbe);

  return 0;
}

static int encode_jnl(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jnl);

  return 0;
}

static int encode_jnle(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jnle);

  return 0;
}

static int encode_jno(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jno);

  return 0;
}

static int encode_jnp(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jnp);

  return 0;
}

static int encode_jns(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jns);

  return 0;
}

static int encode_jnz(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jnz);

  return 0;
}

static int encode_jo(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jo);

  return 0;
}

static int encode_jp(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jp);

  return 0;
}

static int encode_js(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_js);

  return 0;
}

static int encode_jz(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_jz);

  return 0;
}

static int encode_lahf(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_lahf);

  return 0;
}

static int encode_lds(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_lds);

  return 0;
}

static int encode_lea(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_lea);

  return 0;
}

static int encode_leave(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_leave);

  return 0;
}

static int encode_les(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_les);

  return 0;
}

static int encode_lods(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_lods);

  return 0;
}

static int encode_loop(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_loop);

  return 0;
}

static int encode_loope(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_loope);

  return 0;
}

static int encode_loopne(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_loopne);

  return 0;
}

static int encode_mov(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_mov);

  assert(instruction->destination.size == instruction->source.size);

  if (instruction->destination.type == ot_register) {
    if (instruction->source.type == ot_immediate) {
      byte op_code = 0xb0;
      switch (instruction->destination.size) {
        case os_8: {
          buffer[0] = op_code | instruction->destination.data.as_register.reg_8;
          buffer[1] = instruction->source.data.as_immediate.immediate_8;
          return 2;
        }

        case os_16: {
          buffer[0] = op_code | instruction->destination.data.as_register.reg_16;
          buffer[1] = instruction->source.data.as_immediate.immediate_16;
          buffer[2] = instruction->source.data.as_immediate.immediate_16 >> 8;
          return 3;
        }

        default:
          assert(0);
          break;
      }
    }
  }

  return 0;
}

static int encode_movs(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_movs);

  return 0;
}

static int encode_mul(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_mul);

  return 0;
}

static int encode_neg(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_neg);

  return 0;
}

static int encode_not(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_not);

  return 0;
}

static int encode_or(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_or);

  return 0;
}

static int encode_out(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_out);

  return 0;
}

static int encode_outs(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_outs);

  return 0;
}

static int encode_pop(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_pop);

  return 0;
}

static int encode_popa(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_popa);

  return 0;
}

static int encode_popf(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_popf);

  return 0;
}

static int encode_push(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_push);

  return 0;
}

static int encode_pusha(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_pusha);

  return 0;
}

static int encode_pushf(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_pushf);

  return 0;
}

static int encode_rcl(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_rcl);

  return 0;
}

static int encode_rcr(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_rcr);

  return 0;
}

static int encode_ret(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_ret);

  return 0;
}

static int encode_retf(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_retf);

  return 0;
}

static int encode_rol(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_rol);

  return 0;
}

static int encode_ror(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_ror);

  return 0;
}

static int encode_sahf(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_sahf);

  return 0;
}

static int encode_salc(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_salc);

  return 0;
}

static int encode_sar(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_sar);

  return 0;
}

static int encode_sbb(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_sbb);

  return 0;
}

static int encode_scas(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_scas);

  return 0;
}

static int encode_shl(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_shl);

  return 0;
}

static int encode_shr(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_shr);

  return 0;
}

static int encode_stc(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_stc);

  return 0;
}

static int encode_std(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_std);

  return 0;
}

static int encode_sti(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_sti);

  return 0;
}

static int encode_stos(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_stos);

  return 0;
}

static int encode_sub(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_sub);

  return 0;
}

static int encode_test(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_test);

  return 0;
}

static int encode_xchg(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_xchg);

  return 0;
}

static int encode_xlat(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_xlat);

  return 0;
}

static int encode_xor(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  assert(instruction->type == it_xor);

  return 0;
}

typedef int (*encode_func)(const struct instruction *instruction, u8 *buffer, usize buffer_size);

static const encode_func encode_funcs[] = {
    encode_aaa,  encode_aad,   encode_aam,   encode_aas,   encode_adc,  encode_add,   encode_and,
    encode_arpl, encode_bound, encode_call,  encode_callf, encode_cbw,  encode_clc,   encode_cld,
    encode_cli,  encode_cmc,   encode_cmp,   encode_cmps,  encode_cwd,  encode_daa,   encode_das,
    encode_dec,  encode_div,   encode_enter, encode_fwait, encode_hlt,  encode_idiv,  encode_imul,
    encode_in,   encode_inc,   encode_ins,   encode_int,   encode_int1, encode_int3,  encode_into,
    encode_iret, encode_jb,    encode_jbe,   encode_jcxz,  encode_jl,   encode_jle,   encode_jmp,
    encode_jnb,  encode_jnbe,  encode_jnl,   encode_jnle,  encode_jno,  encode_jnp,   encode_jns,
    encode_jnz,  encode_jo,    encode_jp,    encode_js,    encode_jz,   encode_lahf,  encode_lds,
    encode_lea,  encode_leave, encode_les,   encode_lods,  encode_loop, encode_loope, encode_loopne,
    encode_mov,  encode_movs,  encode_mul,   encode_neg,   encode_not,  encode_or,    encode_out,
    encode_outs, encode_pop,   encode_popa,  encode_popf,  encode_push, encode_pusha, encode_pushf,
    encode_rcl,  encode_rcr,   encode_ret,   encode_retf,  encode_rol,  encode_ror,   encode_sahf,
    encode_salc, encode_sar,   encode_sbb,   encode_scas,  encode_shl,  encode_shr,   encode_stc,
    encode_std,  encode_sti,   encode_stos,  encode_sub,   encode_test, encode_xchg,  encode_xlat,
    encode_xor,
};

int encode(const struct instruction *instruction, u8 *buffer, usize buffer_size) {
  encode_func func = encode_funcs[instruction->type];
  return func(instruction, buffer, buffer_size);
}
