#include "bus.h"
#include "cpu.h"
#include "decoder/mod_reg_rm.h"
#include "print_format.h"

#include <stdio.h>

#define MOD_REG_RM_MOD(x) ((x) >> 0x06)
#define MOD_REG_RM_REG(x) ((x) >> 0x03 & 0x07)
#define MOD_REG_RM_RM(x) ((x) >> 0x07)

static const u8 parity[0x100] = {
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
};

static u8 cpu_fetch_8(struct cpu *cpu) {
  u8 result = bus_fetch(cpu->bus, segment_offset(cpu->segments[CS], cpu->ip));
  cpu->ip += 1;
  return result;
}

static u16 cpu_fetch_16(struct cpu *cpu) {
  u8 low = bus_fetch(cpu->bus, segment_offset(cpu->segments[CS], cpu->ip));
  cpu->ip += 1;
  u8 high = bus_fetch(cpu->bus, segment_offset(cpu->segments[CS], cpu->ip));
  cpu->ip += 1;
  return low + (high << 8);
}

struct mod_reg_rm {
  u8 mod;
  u8 reg;
  u8 rm;
  i16 disp;
};

struct mod_reg_rm fetch_mod_reg_rm(struct cpu *cpu) {
  struct mod_reg_rm result;

  u8 mrr_byte = cpu_fetch_8(cpu);

  result.mod = MOD_REG_RM_MOD(mrr_byte);
  result.reg = MOD_REG_RM_REG(mrr_byte);
  result.rm = MOD_REG_RM_RM(mrr_byte);

  switch (result.mod) {
    case mrrm_mod_indirect:
      if (result.rm == 0x06) {
        result.disp = cpu_fetch_16(cpu);
      } else {
        result.disp = 0;
      }
      break;

    case mrrm_mod_word:
      result.disp = cpu_fetch_16(cpu);
      break;

    case mrrm_mod_byte:
      result.disp = cpu_fetch_8(cpu);
      break;

    default:
      result.disp = 0;
      break;
  }

  return result;
}

static u16 calc_mod_reg_rm_addr(struct cpu *cpu, struct mod_reg_rm mrr) {
  if (mrr.mod == mrrm_mod_indirect && mrr.rm == 0x06) {
    return mrr.disp;
  }

  switch (mrr.rm) {
    case mrrm_rm_bx_si:
      return cpu->reg_16[BX] + cpu->reg_16[SI] + mrr.disp;

    case mrrm_rm_bx_di:
      return cpu->reg_16[BX] + cpu->reg_16[DI] + mrr.disp;

    case mrrm_rm_bp_si:
      return cpu->reg_16[BP] + cpu->reg_16[SI] + mrr.disp;

    case mrrm_rm_bp_di:
      return cpu->reg_16[BP] + cpu->reg_16[DI] + mrr.disp;

    case mrrm_rm_si:
      return cpu->reg_16[SI] + mrr.disp;

    case mrrm_rm_di:
      return cpu->reg_16[DI] + mrr.disp;

    case mrrm_rm_bp:
      return cpu->reg_16[BP] + mrr.disp;

    case mrrm_rm_bx:
      return cpu->reg_16[BX] + mrr.disp;

    default:
      assert(0);
  }
}

static u8 read_mod_reg_rm_8(struct cpu *cpu, struct mod_reg_rm mrr, enum segment_register seg_reg) {
  printf("reading mod r/m address from ");

  u16 addr = 0;

  switch (mrr.mod) {
    case mrrm_mod_indirect:
    case mrrm_mod_byte:
    case mrrm_mod_word: {
      addr = calc_mod_reg_rm_addr(cpu, mrr);
      return bus_fetch(cpu->bus, segment_offset(cpu->segments[seg_reg], addr));
    }

    case mrrm_mod_register: {
      printf("register %s\n", register_8_to_string(mrr.rm));
      return cpu->reg_8[mrr.rm];
    }

    default:
      assert(0);
  }
}

static void write_mod_reg_rm_8(struct cpu *cpu, struct mod_reg_rm mrr,
                               enum segment_register seg_reg, u8 value) {
  switch ((enum mrrm_mod)mrr.mod) {
    case mrrm_mod_indirect:
    case mrrm_mod_byte:
    case mrrm_mod_word: {
      u16 addr = calc_mod_reg_rm_addr(cpu, mrr);
      bus_store(cpu->bus, segment_offset(cpu->segments[seg_reg], addr), value);
      break;
    }

    case mrrm_mod_register:
      cpu->reg_8[mrr.rm] = value;
      break;

    default:
      assert(0);
  }
}

static void cpu_flags_sign_zero_parity_8(struct cpu *cpu, u8 value) {
  cpu_set_flag_value(cpu, fl_zf, !value);
  cpu_set_flag_value(cpu, fl_sf, value & 0x80);

  cpu_set_flag_value(cpu, fl_pf, parity[value]);
};

void op_group_2_8(struct cpu *cpu, struct mod_reg_rm mrr, u8 value, u16 count) {
  u16 v = value;
  u16 shift;

  switch (mrr.reg) {
    case 0:
      printf("rol");

      for (shift = 1; shift <= count; shift++) {
        if (v & 0x80) {
          cpu_set_flag(cpu, fl_cf);
        } else {
          cpu_clear_flag(cpu, fl_cf);
        }

        v = v << 1;
        v = v | cpu_flag_is_set(cpu, fl_cf);
      }

      if (count == 1) {
        // of = cf ^ ( (s >> 7) & 1);
        if ((v & 0x80) && cpu_flag_is_set(cpu, fl_cf)) {
          cpu_set_flag(cpu, fl_of);
        } else {
          cpu_clear_flag(cpu, fl_of);
        }
      } else {
        cpu_clear_flag(cpu, fl_of);
      }
      break;

    case 1:
      printf("ror");
      break;

    case 2:
      printf("rcl");
      break;

    case 3:
      printf("rcr");
      break;

    case 4:
    case 6:
      printf("shl");
      break;

    case 5: {
      cpu_set_flag_value(cpu, fl_of, (count == 1) && (v & 0x80));

      u8 cf = cpu_flag_is_set(cpu, fl_cf);
      for (shift = 0; shift <= count; ++shift) {
        cf = v & 1;
        v = v >> 1;
      }
      cpu_set_flag_value(cpu, fl_cf, cf);

      cpu_flags_sign_zero_parity_8(cpu, (u8)v);
      break;
    }

    case 7:
      printf("sar");
      break;
  }
}

#if defined(SIMPLE_CPU)
void cpu_run(struct cpu *cpu) {
  // print_registers(cpu);

  int running = 1;
  while (running) {
    printf(HEX_16 ":" HEX_16 "  ", cpu->segments[CS], cpu->ip);
    for (unsigned i = 0; i < 8; ++i) {
      printf("%02x ", bus_fetch(cpu->bus, segment_offset(cpu->segments[CS], cpu->ip + i)));
    }
    printf("\n");

    u8 op_code = cpu_fetch_8(cpu);

    // Set up the segment register for this instruction.
    enum segment_register segment_register = DS;

    // TODO: Handle segment register overrides.

    switch (op_code) {
      case 0x70: {
        // jo Jb
        i8 offset = (i8)cpu_fetch_8(cpu);
        if (cpu_flag_is_set(cpu, fl_of)) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x71: {
        // jno Jb
        i8 offset = (i8)cpu_fetch_8(cpu);
        if (!cpu_flag_is_set(cpu, fl_of)) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x72: {
        // jb Jb
        i8 offset = (i8)cpu_fetch_8(cpu);
        if (cpu_flag_is_set(cpu, fl_cf)) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x73: {
        // jnb Jb
        i8 offset = (i8)cpu_fetch_8(cpu);
        if (!cpu_flag_is_set(cpu, fl_cf)) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x74: {
        // jz Jb
        i8 offset = (i8)cpu_fetch_8(cpu);
        if (cpu_flag_is_set(cpu, fl_zf)) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x75: {
        // jnz Jb
        i8 offset = (i8)cpu_fetch_8(cpu);
        if (!cpu_flag_is_set(cpu, fl_zf)) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x76: {
        // jbe Jb
        i8 offset = (i8)cpu_fetch_8(cpu);
        if (cpu_flag_is_set(cpu, fl_cf) || cpu_flag_is_set(cpu, fl_zf)) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x77: {
        // jnbe Jb
        i8 offset = (i8)cpu_fetch_8(cpu);
        if (!cpu_flag_is_set(cpu, fl_cf) && !cpu_flag_is_set(cpu, fl_zf)) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x78: {
        // js Jb
        i8 offset = (i8)cpu_fetch_8(cpu);
        if (cpu_flag_is_set(cpu, fl_sf)) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x79: {
        // jns
        i8 offset = (i8)cpu_fetch_8(cpu);
        if (!cpu_flag_is_set(cpu, fl_sf)) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x7a: {
        // jp Jb
        i8 offset = (i8)cpu_fetch_8(cpu);
        if (cpu_flag_is_set(cpu, fl_pf)) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x7b: {
        // jnp Jb
        i8 offset = (i8)cpu_fetch_8(cpu);
        if (!cpu_flag_is_set(cpu, fl_pf)) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x7c: {
        // jl Jb
        i8 offset = (i8)cpu_fetch_8(cpu);
        if (cpu_flag_is_set(cpu, fl_sf) != cpu_flag_is_set(cpu, fl_of)) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x7d: {
        // jnl Jb
        i8 offset = (i8)cpu_fetch_8(cpu);
        if (cpu_flag_is_set(cpu, fl_sf) == cpu_flag_is_set(cpu, fl_of)) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x7e: {
        // jle Jb
        i8 offset = (i8)cpu_fetch_8(cpu);
        if ((cpu_flag_is_set(cpu, fl_sf) != cpu_flag_is_set(cpu, fl_of)) ||
            cpu_flag_is_set(cpu, fl_zf)) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x7f: {
        // jnle Jb
        i8 offset = (i8)cpu_fetch_8(cpu);
        if (!cpu_flag_is_set(cpu, fl_zf) &&
            (cpu_flag_is_set(cpu, fl_sf) == cpu_flag_is_set(cpu, fl_of))) {
          cpu->ip += offset;
        }
        break;
      }

      case 0x9e: {
        // sahf
        cpu->flags = cpu->flags | cpu->reg_8[AH];
        break;
      }

      case 0x9f: {
        // sahf
        cpu->reg_8[AH] = cpu->flags & 0xff;
        break;
      }

      case 0xb0: {
        // mov AL Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->reg_8[AL] = value;
        break;
      }

      case 0xb1: {
        // mov CL Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->reg_8[CL] = value;
        break;
      }

      case 0xb2: {
        // mov DL Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->reg_8[DL] = value;
        break;
      }

      case 0xb3: {
        // mov BL Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->reg_8[BL] = value;
        break;
      }

      case 0xb4: {
        // mov AH Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->reg_8[AH] = value;
        break;
      }

      case 0xb5: {
        // mov CH Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->reg_8[CH] = value;
        break;
      }

      case 0xb6: {
        // mov DH Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->reg_8[DH] = value;
        break;
      }

      case 0xb7: {
        // mov BH Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->reg_8[BH] = value;
        break;
      }

      case 0xb8: {
        // mov AX Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->reg_16[AX] = value;
        break;
      }

      case 0xb9: {
        // mov CX Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->reg_16[CX] = value;
        break;
      }

      case 0xba: {
        // mov DX Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->reg_16[DX] = value;
        break;
      }

      case 0xbb: {
        // mov BX Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->reg_16[BX] = value;
        break;
      }

      case 0xbc: {
        // mov BP Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->reg_16[BP] = value;
        break;
      }

      case 0xbd: {
        // mov SP Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->reg_16[SP] = value;
        break;
      }

      case 0xbe: {
        // mov SI Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->reg_16[SI] = value;
        break;
      }

      case 0xbf: {
        // mov DI Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->reg_16[DI] = value;
        break;
      }

      case 0xd2: {
        struct mod_reg_rm mrr = fetch_mod_reg_rm(cpu);
        u8 value = read_mod_reg_rm_8(cpu, mrr, segment_register);

        op_group_2_8(cpu, mrr, value, cpu->reg_8[CL]);

        write_mod_reg_rm_8(cpu, mrr, segment_register, value);
        return;
        break;
      }

      case 0xea: {
        // jmp Ap
        u16 new_ip = cpu_fetch_16(cpu);
        u16 new_cs = cpu_fetch_16(cpu);

        cpu->segments[CS] = new_cs;
        cpu->ip = new_ip;
        break;
      }

      case 0xfa: {
        // cli
        cpu_clear_flag(cpu, fl_if);
        break;
      }

      default: {
        printf("Invalid op_code: " HEX_8 "\n", op_code);
        running = 0;
        break;
      }
    }
  }
}
#endif // defined(SIMPLE_CPU)
