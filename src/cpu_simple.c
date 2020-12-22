#include "bus.h"
#include "cpu.h"
#include "mod_rm.h"
#include "print_format.h"

#if defined(TESTING)
#include "testing.h"
#endif

#include <stdio.h>

// Map of 8-bit parity flags. sum of set bits are equal.
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

struct flags_map {
  u8 carry;     // 0
  u8 parity;    // 2
  u8 adjust;    // 4
  u8 zero;      // 6
  u8 sign;      // 7
  u8 trap;      // 8
  u8 interrupt; // 9
  u8 direction; // 10
  u8 overflow;  // 11
};

static inline u16 encode_flags(struct flags_map flags) {
  // `f02a` is all reserved flags set to 1.
  return 0xf02a |                  //
         flags.carry |             //
         (flags.parity << 2) |     //
         (flags.adjust << 4) |     //
         (flags.zero << 6) |       //
         (flags.sign << 7) |       //
         (flags.trap << 8) |       //
         (flags.interrupt << 9) |  //
         (flags.direction << 10) | //
         (flags.overflow << 11);
}

static inline struct flags_map decode_flags(u16 flags) {
  struct flags_map result;

  result.carry = flags & 1;
  result.parity = (flags >> 2) & 1;
  result.adjust = (flags >> 4) & 1;
  result.zero = (flags >> 6) & 1;
  result.sign = (flags >> 7) & 1;
  result.trap = (flags >> 8) & 1;
  result.interrupt = (flags >> 9) & 1;
  result.direction = (flags >> 10) & 1;
  result.overflow = (flags >> 11) & 1;

  return result;
}

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

struct mod_rm fetch_mod_rm(struct cpu *cpu) {
  u8 mrr_byte = cpu_fetch_8(cpu);

  struct mod_rm result = decode_mod_rm(mrr_byte);

  switch (result.mod) {
    case mod_rm_mod_indirect:
      if (result.reg_mem.as_mem == mod_rm_mem_bp) {
        result.displacement.as_address = cpu_fetch_16(cpu);
      } else {
        result.displacement.as_offset = 0;
      }
      break;

    case mod_rm_mod_word:
      result.displacement.as_offset = cpu_fetch_16(cpu);
      break;

    case mod_rm_mod_byte:
      result.displacement.as_offset = cpu_fetch_8(cpu);
      break;

    default:
      result.displacement.as_offset = 0;
      break;
  }

  return result;
}

static u16 calc_mod_reg_rm_addr(struct cpu *cpu, struct mod_rm mrm) {
  // Can not calculate the address for a register.
  assert(mrm.mod != mod_rm_mod_register);

  // Indirect addressing on register BP is handled as a 16-bit displacement.
  if (mrm.mod == mod_rm_mod_indirect && mrm.reg_mem.as_mem == mod_rm_mem_bp) {
    return mrm.displacement.as_address;
  }

#if !defined(NDEBUG)
  // Indirect mode should not have a displacement, except for the case above.
  if (mrm.mod == mod_rm_mod_indirect) {
    assert(mrm.displacement.as_offset == 0);
  }

  // Check the bounds for the 8-bit displacement.
  if (mrm.mod == mod_rm_mod_byte) {
    assert((mrm.displacement.as_offset & 0xff00) == 0);
  }
#endif

  switch (mrm.reg_mem.as_mem) {
    case mod_rm_mem_bx_si:
      return cpu->regs.reg_16[BX] + cpu->regs.reg_16[SI] + mrm.displacement.as_offset;

    case mod_rm_mem_bx_di:
      return cpu->regs.reg_16[BX] + cpu->regs.reg_16[DI] + mrm.displacement.as_offset;

    case mod_rm_mem_bp_si:
      return cpu->regs.reg_16[BP] + cpu->regs.reg_16[SI] + mrm.displacement.as_offset;

    case mod_rm_mem_bp_di:
      return cpu->regs.reg_16[BP] + cpu->regs.reg_16[DI] + mrm.displacement.as_offset;

    case mod_rm_mem_si:
      return cpu->regs.reg_16[SI] + mrm.displacement.as_offset;

    case mod_rm_mem_di:
      return cpu->regs.reg_16[DI] + mrm.displacement.as_offset;

    case mod_rm_mem_bp:
      return cpu->regs.reg_16[BP] + mrm.displacement.as_offset;

    case mod_rm_mem_bx:
      return cpu->regs.reg_16[BX] + mrm.displacement.as_offset;

    default:
      assert(0);
      return 0;
  }
}

static u8 read_mod_reg_rm_8(struct cpu *cpu, struct mod_rm mrm, enum segment_register seg_reg) {
  printf("reading mod r/m address from ");

  switch (mrm.mod) {
    case mod_rm_mod_indirect:
    case mod_rm_mod_byte:
    case mod_rm_mod_word: {
      u16 addr = calc_mod_reg_rm_addr(cpu, mrm);
      return bus_fetch(cpu->bus, segment_offset(cpu->segments[seg_reg], addr));
    }

    case mod_rm_mod_register: {
      printf("register %s\n", register_8_to_string(mrm.reg_mem.as_reg));
      // TODO: This needs a mapping!!
      return cpu->regs.reg_8[mrm.reg_mem.as_reg];
    }

    default:
      assert(0);
      return 0;
  }
}

static void write_mod_reg_rm_8(struct cpu *cpu, struct mod_rm mrm, enum segment_register seg_reg,
                               u8 value) {
  switch (mrm.mod) {
    case mod_rm_mod_indirect:
    case mod_rm_mod_byte:
    case mod_rm_mod_word: {
      u16 addr = calc_mod_reg_rm_addr(cpu, mrm);
      bus_store(cpu->bus, segment_offset(cpu->segments[seg_reg], addr), value);
      break;
    }

    case mod_rm_mod_register:
      // TODO: This needs a mapping!!
      cpu->regs.reg_8[mrm.reg_mem.as_reg] = value;
      break;

    default:
      assert(0);
  }
}

static void update_flags_sign_zero_parity_8(struct flags_map *flags, u8 value) {
  flags->sign = (value & 0x80) != 0;
  flags->zero = !value;
  flags->parity = parity[value];
}

enum instruction_group_2_type {
  instruction_group_2_type_rol,
  instruction_group_2_type_ror,
  instruction_group_2_type_rcl,
  instruction_group_2_type_rcr,
  instruction_group_2_type_shl_1,
  instruction_group_2_type_shr,
  instruction_group_2_type_shl_2,
  instruction_group_2_type_sar,
};

static u8 instruction_rol_8(u8 value, u8 count, struct flags_map *flags) {
  u16 value_16 = value;
  u8 carry = 0;
  while (count > 0) {
    carry = (value_16 & 0x80) ? 1 : 0;
    value_16 = (value_16 << 1) + carry;
    --count;
  }
  flags->carry = carry;
  flags->overflow = (count == 1) ? (value_16 & 0x80) && carry : 0;

  return value_16 & 0xff;
}

static u8 instruction_shr_8(u8 value, u8 count, struct flags_map *flags) {
  u16 value_16 = value;

  flags->overflow = (count == 1) && (value_16 & 0x80);

  for (u16 shift = 0; shift <= count; ++shift) {
    flags->carry = value_16 & 1;
    value_16 >>= 1;
  }

  update_flags_sign_zero_parity_8(flags, (u8)value_16);

  return value_16 & 0xff;
}

static u8 instruction_group_2_8(enum instruction_group_2_type type, struct flags_map *flags, u8 value,
                         u8 count) {
  switch (type) {
    case instruction_group_2_type_rol:
      return instruction_rol_8(value, count, flags);

    case instruction_group_2_type_shr: {
      return instruction_shr_8(value, count, flags);
    }

    default:
      assert(0);
  }

  return 0;
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
        cpu->flags = cpu->flags | cpu->regs.reg_8[AH];
        break;
      }

      case 0x9f: {
        // sahf
        cpu->regs.reg_8[AH] = cpu->flags & 0xff;
        break;
      }

      case 0xb0: {
        // mov AL Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->regs.reg_8[AL] = value;
        break;
      }

      case 0xb1: {
        // mov CL Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->regs.reg_8[CL] = value;
        break;
      }

      case 0xb2: {
        // mov DL Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->regs.reg_8[DL] = value;
        break;
      }

      case 0xb3: {
        // mov BL Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->regs.reg_8[BL] = value;
        break;
      }

      case 0xb4: {
        // mov AH Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->regs.reg_8[AH] = value;
        break;
      }

      case 0xb5: {
        // mov CH Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->regs.reg_8[CH] = value;
        break;
      }

      case 0xb6: {
        // mov DH Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->regs.reg_8[DH] = value;
        break;
      }

      case 0xb7: {
        // mov BH Ib
        u8 value = cpu_fetch_8(cpu);
        cpu->regs.reg_8[BH] = value;
        break;
      }

      case 0xb8: {
        // mov AX Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->regs.reg_16[AX] = value;
        break;
      }

      case 0xb9: {
        // mov CX Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->regs.reg_16[CX] = value;
        break;
      }

      case 0xba: {
        // mov DX Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->regs.reg_16[DX] = value;
        break;
      }

      case 0xbb: {
        // mov BX Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->regs.reg_16[BX] = value;
        break;
      }

      case 0xbc: {
        // mov BP Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->regs.reg_16[BP] = value;
        break;
      }

      case 0xbd: {
        // mov SP Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->regs.reg_16[SP] = value;
        break;
      }

      case 0xbe: {
        // mov SI Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->regs.reg_16[SI] = value;
        break;
      }

      case 0xbf: {
        // mov DI Ib
        u16 value = cpu_fetch_16(cpu);
        cpu->regs.reg_16[DI] = value;
        break;
      }

      case 0xd2: {
        struct mod_rm mrm = fetch_mod_rm(cpu);
        u8 value = read_mod_reg_rm_8(cpu, mrm, segment_register);

        struct flags_map flags = decode_flags(cpu->flags);
        instruction_group_2_8(mrm.reg, &flags, value, cpu->regs.reg_8[CL]);

        write_mod_reg_rm_8(cpu, mrm, segment_register, value);
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

#if defined(TESTING)
void test_fetch_mod_reg_rm(void) {
  static struct {
    u8 buffer[6];
    struct mod_rm result;
  } tests[] = {
      {{0x00, 0x00, 0xf0},
       {mod_rm_mod_indirect, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0}}},
      {{0x01, 0x01, 0xf1},
       {mod_rm_mod_indirect, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0}}},
      {{0x02, 0x02, 0xf2},
       {mod_rm_mod_indirect, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0}}},
      {{0x03, 0x03, 0xf3},
       {mod_rm_mod_indirect, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0}}},
      {{0x04, 0x04, 0xf4},
       {mod_rm_mod_indirect, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_si}, {.as_offset = 0}}},
      {{0x05, 0x05, 0xf5},
       {mod_rm_mod_indirect, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_di}, {.as_offset = 0}}},
      {{0x06, 0x06, 0xf6},
       {mod_rm_mod_indirect, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bp}, {.as_address = 0xf606}}},
      {{0x07, 0x07, 0xf7},
       {mod_rm_mod_indirect, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0}}},
      {{0x08, 0x08, 0xf8},
       {mod_rm_mod_indirect, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0}}},
      {{0x09, 0x09, 0xf9},
       {mod_rm_mod_indirect, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0}}},
      {{0x0a, 0x0a, 0xfa},
       {mod_rm_mod_indirect, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0}}},
      {{0x0b, 0x0b, 0xfb},
       {mod_rm_mod_indirect, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0}}},
      {{0x0c, 0x0c, 0xfc},
       {mod_rm_mod_indirect, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_si}, {.as_offset = 0}}},
      {{0x0d, 0x0d, 0xfd},
       {mod_rm_mod_indirect, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_di}, {.as_offset = 0}}},
      {{0x0e, 0x0e, 0xfe},
       {mod_rm_mod_indirect, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bp}, {.as_address = 0xfe0e}}},
      {{0x0f, 0x0f, 0xff},
       {mod_rm_mod_indirect, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0}}},

      {{0x10, 0x10, 0x00},
       {mod_rm_mod_indirect, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0}}},
      {{0x11, 0x11, 0x01},
       {mod_rm_mod_indirect, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0}}},
      {{0x12, 0x12, 0x02},
       {mod_rm_mod_indirect, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0}}},
      {{0x13, 0x13, 0x03},
       {mod_rm_mod_indirect, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0}}},
      {{0x14, 0x14, 0x04},
       {mod_rm_mod_indirect, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_si}, {.as_offset = 0}}},
      {{0x15, 0x15, 0x05},
       {mod_rm_mod_indirect, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_di}, {.as_offset = 0}}},
      {{0x16, 0x16, 0x06},
       {mod_rm_mod_indirect, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bp}, {.as_address = 0x0616}}},
      {{0x17, 0x17, 0x07},
       {mod_rm_mod_indirect, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0}}},
      {{0x18, 0x18, 0x08},
       {mod_rm_mod_indirect, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0}}},
      {{0x19, 0x19, 0x09},
       {mod_rm_mod_indirect, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0}}},
      {{0x1a, 0x1a, 0x0a},
       {mod_rm_mod_indirect, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0}}},
      {{0x1b, 0x1b, 0x0b},
       {mod_rm_mod_indirect, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0}}},
      {{0x1c, 0x1c, 0x0c},
       {mod_rm_mod_indirect, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_si}, {.as_offset = 0}}},
      {{0x1d, 0x1d, 0x0d},
       {mod_rm_mod_indirect, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_di}, {.as_offset = 0}}},
      {{0x1e, 0x1e, 0x0e},
       {mod_rm_mod_indirect, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bp}, {.as_address = 0x0e1e}}},
      {{0x1f, 0x1f, 0x0f},
       {mod_rm_mod_indirect, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0}}},

      {{0x20, 0x20, 0x10},
       {mod_rm_mod_indirect, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0}}},
      {{0x21, 0x21, 0x11},
       {mod_rm_mod_indirect, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0}}},
      {{0x22, 0x22, 0x12},
       {mod_rm_mod_indirect, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0}}},
      {{0x23, 0x23, 0x13},
       {mod_rm_mod_indirect, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0}}},
      {{0x24, 0x24, 0x14},
       {mod_rm_mod_indirect, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_si}, {.as_offset = 0}}},
      {{0x25, 0x25, 0x15},
       {mod_rm_mod_indirect, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_di}, {.as_offset = 0}}},
      {{0x26, 0x26, 0x16},
       {mod_rm_mod_indirect, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bp}, {.as_address = 0x1626}}},
      {{0x27, 0x27, 0x17},
       {mod_rm_mod_indirect, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0}}},
      {{0x28, 0x28, 0x18},
       {mod_rm_mod_indirect, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0}}},
      {{0x29, 0x29, 0x19},
       {mod_rm_mod_indirect, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0}}},
      {{0x2a, 0x2a, 0x1a},
       {mod_rm_mod_indirect, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0}}},
      {{0x2b, 0x2b, 0x1b},
       {mod_rm_mod_indirect, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0}}},
      {{0x2c, 0x2c, 0x1c},
       {mod_rm_mod_indirect, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_si}, {.as_offset = 0}}},
      {{0x2d, 0x2d, 0x1d},
       {mod_rm_mod_indirect, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_di}, {.as_offset = 0}}},
      {{0x2e, 0x2e, 0x1e},
       {mod_rm_mod_indirect, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bp}, {.as_address = 0x1e2e}}},
      {{0x2f, 0x2f, 0x1f},
       {mod_rm_mod_indirect, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0}}},

      {{0x30, 0x30, 0x20},
       {mod_rm_mod_indirect, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0}}},
      {{0x31, 0x31, 0x21},
       {mod_rm_mod_indirect, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0}}},
      {{0x32, 0x32, 0x22},
       {mod_rm_mod_indirect, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0}}},
      {{0x33, 0x33, 0x23},
       {mod_rm_mod_indirect, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0}}},
      {{0x34, 0x34, 0x24},
       {mod_rm_mod_indirect, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_si}, {.as_offset = 0}}},
      {{0x35, 0x35, 0x25},
       {mod_rm_mod_indirect, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_di}, {.as_offset = 0}}},
      {{0x36, 0x36, 0x26},
       {mod_rm_mod_indirect, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bp}, {.as_address = 0x2636}}},
      {{0x37, 0x37, 0x27},
       {mod_rm_mod_indirect, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0}}},
      {{0x38, 0x38, 0x28},
       {mod_rm_mod_indirect, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0}}},
      {{0x39, 0x39, 0x29},
       {mod_rm_mod_indirect, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0}}},
      {{0x3a, 0x3a, 0x2a},
       {mod_rm_mod_indirect, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0}}},
      {{0x3b, 0x3b, 0x2b},
       {mod_rm_mod_indirect, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0}}},
      {{0x3c, 0x3c, 0x2c},
       {mod_rm_mod_indirect, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_si}, {.as_offset = 0}}},
      {{0x3d, 0x3d, 0x2d},
       {mod_rm_mod_indirect, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_di}, {.as_offset = 0}}},
      {{0x3e, 0x3e, 0x2e},
       {mod_rm_mod_indirect, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bp}, {.as_address = 0x2e3e}}},
      {{0x3f, 0x3f, 0x2f},
       {mod_rm_mod_indirect, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0}}},

      {{0x40, 0x40, 0x30},
       {mod_rm_mod_byte, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x40}}},
      {{0x41, 0x41, 0x31},
       {mod_rm_mod_byte, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x41}}},
      {{0x42, 0x42, 0x32},
       {mod_rm_mod_byte, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x42}}},
      {{0x43, 0x43, 0x33},
       {mod_rm_mod_byte, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x43}}},
      {{0x44, 0x44, 0x34},
       {mod_rm_mod_byte, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x44}}},
      {{0x45, 0x45, 0x35},
       {mod_rm_mod_byte, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x45}}},
      {{0x46, 0x46, 0x36},
       {mod_rm_mod_byte, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x46}}},
      {{0x47, 0x47, 0x37},
       {mod_rm_mod_byte, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x47}}},
      {{0x48, 0x48, 0x38},
       {mod_rm_mod_byte, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x48}}},
      {{0x49, 0x49, 0x39},
       {mod_rm_mod_byte, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x49}}},
      {{0x4a, 0x4a, 0x3a},
       {mod_rm_mod_byte, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x4a}}},
      {{0x4b, 0x4b, 0x3b},
       {mod_rm_mod_byte, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x4b}}},
      {{0x4c, 0x4c, 0x3c},
       {mod_rm_mod_byte, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x4c}}},
      {{0x4d, 0x4d, 0x3d},
       {mod_rm_mod_byte, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x4d}}},
      {{0x4e, 0x4e, 0x3e},
       {mod_rm_mod_byte, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x4e}}},
      {{0x4f, 0x4f, 0x3f},
       {mod_rm_mod_byte, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x4f}}},

      {{0x50, 0x50, 0x40},
       {mod_rm_mod_byte, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x50}}},
      {{0x51, 0x51, 0x41},
       {mod_rm_mod_byte, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x51}}},
      {{0x52, 0x52, 0x42},
       {mod_rm_mod_byte, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x52}}},
      {{0x53, 0x53, 0x43},
       {mod_rm_mod_byte, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x53}}},
      {{0x54, 0x54, 0x44},
       {mod_rm_mod_byte, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x54}}},
      {{0x55, 0x55, 0x45},
       {mod_rm_mod_byte, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x55}}},
      {{0x56, 0x56, 0x46},
       {mod_rm_mod_byte, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x56}}},
      {{0x57, 0x57, 0x47},
       {mod_rm_mod_byte, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x57}}},
      {{0x58, 0x58, 0x48},
       {mod_rm_mod_byte, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x58}}},
      {{0x59, 0x59, 0x49},
       {mod_rm_mod_byte, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x59}}},
      {{0x5a, 0x5a, 0x4a},
       {mod_rm_mod_byte, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x5a}}},
      {{0x5b, 0x5b, 0x4b},
       {mod_rm_mod_byte, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x5b}}},
      {{0x5c, 0x5c, 0x4c},
       {mod_rm_mod_byte, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x5c}}},
      {{0x5d, 0x5d, 0x4d},
       {mod_rm_mod_byte, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x5d}}},
      {{0x5e, 0x5e, 0x4e},
       {mod_rm_mod_byte, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x5e}}},
      {{0x5f, 0x5f, 0x4f},
       {mod_rm_mod_byte, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x5f}}},

      {{0x60, 0x60, 0x50},
       {mod_rm_mod_byte, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x60}}},
      {{0x61, 0x61, 0x51},
       {mod_rm_mod_byte, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x61}}},
      {{0x62, 0x62, 0x52},
       {mod_rm_mod_byte, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x62}}},
      {{0x63, 0x63, 0x53},
       {mod_rm_mod_byte, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x63}}},
      {{0x64, 0x64, 0x54},
       {mod_rm_mod_byte, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x64}}},
      {{0x65, 0x65, 0x55},
       {mod_rm_mod_byte, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x65}}},
      {{0x66, 0x66, 0x56},
       {mod_rm_mod_byte, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x66}}},
      {{0x67, 0x67, 0x57},
       {mod_rm_mod_byte, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x67}}},
      {{0x68, 0x68, 0x58},
       {mod_rm_mod_byte, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x68}}},
      {{0x69, 0x69, 0x59},
       {mod_rm_mod_byte, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x69}}},
      {{0x6a, 0x6a, 0x5a},
       {mod_rm_mod_byte, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x6a}}},
      {{0x6b, 0x6b, 0x5b},
       {mod_rm_mod_byte, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x6b}}},
      {{0x6c, 0x6c, 0x5c},
       {mod_rm_mod_byte, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x6c}}},
      {{0x6d, 0x6d, 0x5d},
       {mod_rm_mod_byte, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x6d}}},
      {{0x6e, 0x6e, 0x5e},
       {mod_rm_mod_byte, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x6e}}},
      {{0x6f, 0x6f, 0x5f},
       {mod_rm_mod_byte, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x6f}}},

      {{0x70, 0x70, 0x60},
       {mod_rm_mod_byte, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x70}}},
      {{0x71, 0x71, 0x61},
       {mod_rm_mod_byte, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x71}}},
      {{0x72, 0x72, 0x62},
       {mod_rm_mod_byte, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x72}}},
      {{0x73, 0x73, 0x63},
       {mod_rm_mod_byte, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x73}}},
      {{0x74, 0x74, 0x64},
       {mod_rm_mod_byte, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x74}}},
      {{0x75, 0x75, 0x65},
       {mod_rm_mod_byte, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x75}}},
      {{0x76, 0x76, 0x66},
       {mod_rm_mod_byte, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x76}}},
      {{0x77, 0x77, 0x67},
       {mod_rm_mod_byte, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x77}}},
      {{0x78, 0x78, 0x68},
       {mod_rm_mod_byte, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x78}}},
      {{0x79, 0x79, 0x69},
       {mod_rm_mod_byte, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x79}}},
      {{0x7a, 0x7a, 0x6a},
       {mod_rm_mod_byte, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x7a}}},
      {{0x7b, 0x7b, 0x6b},
       {mod_rm_mod_byte, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x7b}}},
      {{0x7c, 0x7c, 0x6c},
       {mod_rm_mod_byte, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x7c}}},
      {{0x7d, 0x7d, 0x6d},
       {mod_rm_mod_byte, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x7d}}},
      {{0x7e, 0x7e, 0x6e},
       {mod_rm_mod_byte, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x7e}}},
      {{0x7f, 0x7f, 0x6f},
       {mod_rm_mod_byte, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x7f}}},

      {{0x80, 0x80, 0x70},
       {mod_rm_mod_word, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x7080}}},
      {{0x81, 0x81, 0x71},
       {mod_rm_mod_word, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x7181}}},
      {{0x82, 0x82, 0x72},
       {mod_rm_mod_word, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x7282}}},
      {{0x83, 0x83, 0x73},
       {mod_rm_mod_word, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x7383}}},
      {{0x84, 0x84, 0x74},
       {mod_rm_mod_word, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x7484}}},
      {{0x85, 0x85, 0x75},
       {mod_rm_mod_word, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x7585}}},
      {{0x86, 0x86, 0x76},
       {mod_rm_mod_word, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x7686}}},
      {{0x87, 0x87, 0x77},
       {mod_rm_mod_word, mod_rm_reg_al_ax, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x7787}}},
      {{0x88, 0x88, 0x78},
       {mod_rm_mod_word, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x7888}}},
      {{0x89, 0x89, 0x79},
       {mod_rm_mod_word, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x7989}}},
      {{0x8a, 0x8a, 0x7a},
       {mod_rm_mod_word, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x7a8a}}},
      {{0x8b, 0x8b, 0x7b},
       {mod_rm_mod_word, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x7b8b}}},
      {{0x8c, 0x8c, 0x7c},
       {mod_rm_mod_word, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x7c8c}}},
      {{0x8d, 0x8d, 0x7d},
       {mod_rm_mod_word, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x7d8d}}},
      {{0x8e, 0x8e, 0x7e},
       {mod_rm_mod_word, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x7e8e}}},
      {{0x8f, 0x8f, 0x7f},
       {mod_rm_mod_word, mod_rm_reg_cl_cx, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x7f8f}}},

      {{0x90, 0x90, 0x80},
       {mod_rm_mod_word, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x8090}}},
      {{0x91, 0x91, 0x81},
       {mod_rm_mod_word, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x8191}}},
      {{0x92, 0x92, 0x82},
       {mod_rm_mod_word, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x8292}}},
      {{0x93, 0x93, 0x83},
       {mod_rm_mod_word, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x8393}}},
      {{0x94, 0x94, 0x84},
       {mod_rm_mod_word, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x8494}}},
      {{0x95, 0x95, 0x85},
       {mod_rm_mod_word, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x8595}}},
      {{0x96, 0x96, 0x86},
       {mod_rm_mod_word, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x8696}}},
      {{0x97, 0x97, 0x87},
       {mod_rm_mod_word, mod_rm_reg_dl_dx, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x8797}}},
      {{0x98, 0x98, 0x88},
       {mod_rm_mod_word, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x8898}}},
      {{0x99, 0x99, 0x89},
       {mod_rm_mod_word, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x8999}}},
      {{0x9a, 0x9a, 0x8a},
       {mod_rm_mod_word, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x8a9a}}},
      {{0x9b, 0x9b, 0x8b},
       {mod_rm_mod_word, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x8b9b}}},
      {{0x9c, 0x9c, 0x8c},
       {mod_rm_mod_word, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x8c9c}}},
      {{0x9d, 0x9d, 0x8d},
       {mod_rm_mod_word, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x8d9d}}},
      {{0x9e, 0x9e, 0x8e},
       {mod_rm_mod_word, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x8e9e}}},
      {{0x9f, 0x9f, 0x8f},
       {mod_rm_mod_word, mod_rm_reg_bl_bx, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x8f9f}}},

      {{0xa0, 0xa0, 0x90},
       {mod_rm_mod_word, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x90a0}}},
      {{0xa1, 0xa1, 0x91},
       {mod_rm_mod_word, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x91a1}}},
      {{0xa2, 0xa2, 0x92},
       {mod_rm_mod_word, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x92a2}}},
      {{0xa3, 0xa3, 0x93},
       {mod_rm_mod_word, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x93a3}}},
      {{0xa4, 0xa4, 0x94},
       {mod_rm_mod_word, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x94a4}}},
      {{0xa5, 0xa5, 0x95},
       {mod_rm_mod_word, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x95a5}}},
      {{0xa6, 0xa6, 0x96},
       {mod_rm_mod_word, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x96a6}}},
      {{0xa7, 0xa7, 0x97},
       {mod_rm_mod_word, mod_rm_reg_ah_sp, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x97a7}}},
      {{0xa8, 0xa8, 0x98},
       {mod_rm_mod_word, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x98a8}}},
      {{0xa9, 0xa9, 0x99},
       {mod_rm_mod_word, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x99a9}}},
      {{0xaa, 0xaa, 0x9a},
       {mod_rm_mod_word, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x9aaa}}},
      {{0xab, 0xab, 0x9b},
       {mod_rm_mod_word, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x9bab}}},
      {{0xac, 0xac, 0x9c},
       {mod_rm_mod_word, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x9cac}}},
      {{0xad, 0xad, 0x9d},
       {mod_rm_mod_word, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x9dad}}},
      {{0xae, 0xae, 0x9e},
       {mod_rm_mod_word, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x9eae}}},
      {{0xaf, 0xaf, 0x9f},
       {mod_rm_mod_word, mod_rm_reg_ch_bp, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x9faf}}},

      {{0xb0, 0xb0, 0xa0},
       {mod_rm_mod_word, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0xa0b0}}},
      {{0xb1, 0xb1, 0xa1},
       {mod_rm_mod_word, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0xa1b1}}},
      {{0xb2, 0xb2, 0xa2},
       {mod_rm_mod_word, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0xa2b2}}},
      {{0xb3, 0xb3, 0xa3},
       {mod_rm_mod_word, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0xa3b3}}},
      {{0xb4, 0xb4, 0xa4},
       {mod_rm_mod_word, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_si}, {.as_offset = 0xa4b4}}},
      {{0xb5, 0xb5, 0xa5},
       {mod_rm_mod_word, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_di}, {.as_offset = 0xa5b5}}},
      {{0xb6, 0xb6, 0xa6},
       {mod_rm_mod_word, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0xa6b6}}},
      {{0xb7, 0xb7, 0xa7},
       {mod_rm_mod_word, mod_rm_reg_dh_si, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0xa7b7}}},
      {{0xb8, 0xb8, 0xa8},
       {mod_rm_mod_word, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0xa8b8}}},
      {{0xb9, 0xb9, 0xa9},
       {mod_rm_mod_word, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0xa9b9}}},
      {{0xba, 0xba, 0xaa},
       {mod_rm_mod_word, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0xaaba}}},
      {{0xbb, 0xbb, 0xab},
       {mod_rm_mod_word, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0xabbb}}},
      {{0xbc, 0xbc, 0xac},
       {mod_rm_mod_word, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_si}, {.as_offset = 0xacbc}}},
      {{0xbd, 0xbd, 0xad},
       {mod_rm_mod_word, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_di}, {.as_offset = 0xadbd}}},
      {{0xbe, 0xbe, 0xae},
       {mod_rm_mod_word, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0xaebe}}},
      {{0xbf, 0xbf, 0xaf},
       {mod_rm_mod_word, mod_rm_reg_bh_di, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0xafbf}}},

      {{0xc0, 0xc0, 0xb0},
       {mod_rm_mod_register, mod_rm_reg_al_ax, {.as_reg = mod_rm_reg_al_ax}, {.as_offset = 0}}},
      {{0xc1, 0xc1, 0xb1},
       {mod_rm_mod_register, mod_rm_reg_al_ax, {.as_reg = mod_rm_reg_cl_cx}, {.as_offset = 0}}},
      {{0xc2, 0xc2, 0xb2},
       {mod_rm_mod_register, mod_rm_reg_al_ax, {.as_reg = mod_rm_reg_dl_dx}, {.as_offset = 0}}},
      {{0xc3, 0xc3, 0xb3},
       {mod_rm_mod_register, mod_rm_reg_al_ax, {.as_reg = mod_rm_reg_bl_bx}, {.as_offset = 0}}},
      {{0xc4, 0xc4, 0xb4},
       {mod_rm_mod_register, mod_rm_reg_al_ax, {.as_reg = mod_rm_reg_ah_sp}, {.as_offset = 0}}},
      {{0xc5, 0xc5, 0xb5},
       {mod_rm_mod_register, mod_rm_reg_al_ax, {.as_reg = mod_rm_reg_ch_bp}, {.as_offset = 0}}},
      {{0xc6, 0xc6, 0xb6},
       {mod_rm_mod_register, mod_rm_reg_al_ax, {.as_reg = mod_rm_reg_dh_si}, {.as_offset = 0}}},
      {{0xc7, 0xc7, 0xb7},
       {mod_rm_mod_register, mod_rm_reg_al_ax, {.as_reg = mod_rm_reg_bh_di}, {.as_offset = 0}}},
      {{0xc8, 0xc8, 0xb8},
       {mod_rm_mod_register, mod_rm_reg_cl_cx, {.as_reg = mod_rm_reg_al_ax}, {.as_offset = 0}}},
      {{0xc9, 0xc9, 0xb9},
       {mod_rm_mod_register, mod_rm_reg_cl_cx, {.as_reg = mod_rm_reg_cl_cx}, {.as_offset = 0}}},
      {{0xca, 0xca, 0xba},
       {mod_rm_mod_register, mod_rm_reg_cl_cx, {.as_reg = mod_rm_reg_dl_dx}, {.as_offset = 0}}},
      {{0xcb, 0xcb, 0xbb},
       {mod_rm_mod_register, mod_rm_reg_cl_cx, {.as_reg = mod_rm_reg_bl_bx}, {.as_offset = 0}}},
      {{0xcc, 0xcc, 0xbc},
       {mod_rm_mod_register, mod_rm_reg_cl_cx, {.as_reg = mod_rm_reg_ah_sp}, {.as_offset = 0}}},
      {{0xcd, 0xcd, 0xbd},
       {mod_rm_mod_register, mod_rm_reg_cl_cx, {.as_reg = mod_rm_reg_ch_bp}, {.as_offset = 0}}},
      {{0xce, 0xce, 0xbe},
       {mod_rm_mod_register, mod_rm_reg_cl_cx, {.as_reg = mod_rm_reg_dh_si}, {.as_offset = 0}}},
      {{0xcf, 0xcf, 0xbf},
       {mod_rm_mod_register, mod_rm_reg_cl_cx, {.as_reg = mod_rm_reg_bh_di}, {.as_offset = 0}}},

      {{0xd0, 0xd0, 0xc0},
       {mod_rm_mod_register, mod_rm_reg_dl_dx, {.as_reg = mod_rm_reg_al_ax}, {.as_offset = 0}}},
      {{0xd1, 0xd1, 0xc1},
       {mod_rm_mod_register, mod_rm_reg_dl_dx, {.as_reg = mod_rm_reg_cl_cx}, {.as_offset = 0}}},
      {{0xd2, 0xd2, 0xc2},
       {mod_rm_mod_register, mod_rm_reg_dl_dx, {.as_reg = mod_rm_reg_dl_dx}, {.as_offset = 0}}},
      {{0xd3, 0xd3, 0xc3},
       {mod_rm_mod_register, mod_rm_reg_dl_dx, {.as_reg = mod_rm_reg_bl_bx}, {.as_offset = 0}}},
      {{0xd4, 0xd4, 0xc4},
       {mod_rm_mod_register, mod_rm_reg_dl_dx, {.as_reg = mod_rm_reg_ah_sp}, {.as_offset = 0}}},
      {{0xd5, 0xd5, 0xc5},
       {mod_rm_mod_register, mod_rm_reg_dl_dx, {.as_reg = mod_rm_reg_ch_bp}, {.as_offset = 0}}},
      {{0xd6, 0xd6, 0xc6},
       {mod_rm_mod_register, mod_rm_reg_dl_dx, {.as_reg = mod_rm_reg_dh_si}, {.as_offset = 0}}},
      {{0xd7, 0xd7, 0xc7},
       {mod_rm_mod_register, mod_rm_reg_dl_dx, {.as_reg = mod_rm_reg_bh_di}, {.as_offset = 0}}},
      {{0xd8, 0xd8, 0xc8},
       {mod_rm_mod_register, mod_rm_reg_bl_bx, {.as_reg = mod_rm_reg_al_ax}, {.as_offset = 0}}},
      {{0xd9, 0xd9, 0xc9},
       {mod_rm_mod_register, mod_rm_reg_bl_bx, {.as_reg = mod_rm_reg_cl_cx}, {.as_offset = 0}}},
      {{0xda, 0xda, 0xca},
       {mod_rm_mod_register, mod_rm_reg_bl_bx, {.as_reg = mod_rm_reg_dl_dx}, {.as_offset = 0}}},
      {{0xdb, 0xdb, 0xcb},
       {mod_rm_mod_register, mod_rm_reg_bl_bx, {.as_reg = mod_rm_reg_bl_bx}, {.as_offset = 0}}},
      {{0xdc, 0xdc, 0xcc},
       {mod_rm_mod_register, mod_rm_reg_bl_bx, {.as_reg = mod_rm_reg_ah_sp}, {.as_offset = 0}}},
      {{0xdd, 0xdd, 0xcd},
       {mod_rm_mod_register, mod_rm_reg_bl_bx, {.as_reg = mod_rm_reg_ch_bp}, {.as_offset = 0}}},
      {{0xde, 0xde, 0xce},
       {mod_rm_mod_register, mod_rm_reg_bl_bx, {.as_reg = mod_rm_reg_dh_si}, {.as_offset = 0}}},
      {{0xdf, 0xdf, 0xcf},
       {mod_rm_mod_register, mod_rm_reg_bl_bx, {.as_reg = mod_rm_reg_bh_di}, {.as_offset = 0}}},

      {{0xe0, 0xe0, 0xd0},
       {mod_rm_mod_register, mod_rm_reg_ah_sp, {.as_reg = mod_rm_reg_al_ax}, {.as_offset = 0}}},
      {{0xe1, 0xe1, 0xd1},
       {mod_rm_mod_register, mod_rm_reg_ah_sp, {.as_reg = mod_rm_reg_cl_cx}, {.as_offset = 0}}},
      {{0xe2, 0xe2, 0xd2},
       {mod_rm_mod_register, mod_rm_reg_ah_sp, {.as_reg = mod_rm_reg_dl_dx}, {.as_offset = 0}}},
      {{0xe3, 0xe3, 0xd3},
       {mod_rm_mod_register, mod_rm_reg_ah_sp, {.as_reg = mod_rm_reg_bl_bx}, {.as_offset = 0}}},
      {{0xe4, 0xe4, 0xd4},
       {mod_rm_mod_register, mod_rm_reg_ah_sp, {.as_reg = mod_rm_reg_ah_sp}, {.as_offset = 0}}},
      {{0xe5, 0xe5, 0xd5},
       {mod_rm_mod_register, mod_rm_reg_ah_sp, {.as_reg = mod_rm_reg_ch_bp}, {.as_offset = 0}}},
      {{0xe6, 0xe6, 0xd6},
       {mod_rm_mod_register, mod_rm_reg_ah_sp, {.as_reg = mod_rm_reg_dh_si}, {.as_offset = 0}}},
      {{0xe7, 0xe7, 0xd7},
       {mod_rm_mod_register, mod_rm_reg_ah_sp, {.as_reg = mod_rm_reg_bh_di}, {.as_offset = 0}}},
      {{0xe8, 0xe8, 0xd8},
       {mod_rm_mod_register, mod_rm_reg_ch_bp, {.as_reg = mod_rm_reg_al_ax}, {.as_offset = 0}}},
      {{0xe9, 0xe9, 0xd9},
       {mod_rm_mod_register, mod_rm_reg_ch_bp, {.as_reg = mod_rm_reg_cl_cx}, {.as_offset = 0}}},
      {{0xea, 0xea, 0xda},
       {mod_rm_mod_register, mod_rm_reg_ch_bp, {.as_reg = mod_rm_reg_dl_dx}, {.as_offset = 0}}},
      {{0xeb, 0xeb, 0xdb},
       {mod_rm_mod_register, mod_rm_reg_ch_bp, {.as_reg = mod_rm_reg_bl_bx}, {.as_offset = 0}}},
      {{0xec, 0xec, 0xdc},
       {mod_rm_mod_register, mod_rm_reg_ch_bp, {.as_reg = mod_rm_reg_ah_sp}, {.as_offset = 0}}},
      {{0xed, 0xed, 0xdd},
       {mod_rm_mod_register, mod_rm_reg_ch_bp, {.as_reg = mod_rm_reg_ch_bp}, {.as_offset = 0}}},
      {{0xee, 0xee, 0xde},
       {mod_rm_mod_register, mod_rm_reg_ch_bp, {.as_reg = mod_rm_reg_dh_si}, {.as_offset = 0}}},
      {{0xef, 0xef, 0xdf},
       {mod_rm_mod_register, mod_rm_reg_ch_bp, {.as_reg = mod_rm_reg_bh_di}, {.as_offset = 0}}},

      {{0xf0, 0xf0, 0xe0},
       {mod_rm_mod_register, mod_rm_reg_dh_si, {.as_reg = mod_rm_reg_al_ax}, {.as_offset = 0}}},
      {{0xf1, 0xf1, 0xe1},
       {mod_rm_mod_register, mod_rm_reg_dh_si, {.as_reg = mod_rm_reg_cl_cx}, {.as_offset = 0}}},
      {{0xf2, 0xf2, 0xe2},
       {mod_rm_mod_register, mod_rm_reg_dh_si, {.as_reg = mod_rm_reg_dl_dx}, {.as_offset = 0}}},
      {{0xf3, 0xf3, 0xe3},
       {mod_rm_mod_register, mod_rm_reg_dh_si, {.as_reg = mod_rm_reg_bl_bx}, {.as_offset = 0}}},
      {{0xf4, 0xf4, 0xe4},
       {mod_rm_mod_register, mod_rm_reg_dh_si, {.as_reg = mod_rm_reg_ah_sp}, {.as_offset = 0}}},
      {{0xf5, 0xf5, 0xe5},
       {mod_rm_mod_register, mod_rm_reg_dh_si, {.as_reg = mod_rm_reg_ch_bp}, {.as_offset = 0}}},
      {{0xf6, 0xf6, 0xe6},
       {mod_rm_mod_register, mod_rm_reg_dh_si, {.as_reg = mod_rm_reg_dh_si}, {.as_offset = 0}}},
      {{0xf7, 0xf7, 0xe7},
       {mod_rm_mod_register, mod_rm_reg_dh_si, {.as_reg = mod_rm_reg_bh_di}, {.as_offset = 0}}},
      {{0xf8, 0xf8, 0xe8},
       {mod_rm_mod_register, mod_rm_reg_bh_di, {.as_reg = mod_rm_reg_al_ax}, {.as_offset = 0}}},
      {{0xf9, 0xf9, 0xe9},
       {mod_rm_mod_register, mod_rm_reg_bh_di, {.as_reg = mod_rm_reg_cl_cx}, {.as_offset = 0}}},
      {{0xfa, 0xfa, 0xea},
       {mod_rm_mod_register, mod_rm_reg_bh_di, {.as_reg = mod_rm_reg_dl_dx}, {.as_offset = 0}}},
      {{0xfb, 0xfb, 0xeb},
       {mod_rm_mod_register, mod_rm_reg_bh_di, {.as_reg = mod_rm_reg_bl_bx}, {.as_offset = 0}}},
      {{0xfc, 0xfc, 0xec},
       {mod_rm_mod_register, mod_rm_reg_bh_di, {.as_reg = mod_rm_reg_ah_sp}, {.as_offset = 0}}},
      {{0xfd, 0xfd, 0xed},
       {mod_rm_mod_register, mod_rm_reg_bh_di, {.as_reg = mod_rm_reg_ch_bp}, {.as_offset = 0}}},
      {{0xfe, 0xfe, 0xee},
       {mod_rm_mod_register, mod_rm_reg_bh_di, {.as_reg = mod_rm_reg_dh_si}, {.as_offset = 0}}},
      {{0xff, 0xff, 0xef},
       {mod_rm_mod_register, mod_rm_reg_bh_di, {.as_reg = mod_rm_reg_bh_di}, {.as_offset = 0}}},
  };

  for (unsigned i = 0; i < ARRAY_SIZE(tests); ++i) {
    struct bus bus;
    struct cpu cpu;

    testing_dummy_cpu_init(&cpu, &bus, tests[i].buffer, 6);

    struct mod_rm mrm = fetch_mod_rm(&cpu);
    EXPECT_U8_EQ(tests[i].result.mod, mrm.mod);
    EXPECT_U8_EQ(tests[i].result.reg, mrm.reg);
    EXPECT_U8_EQ(tests[i].result.reg_mem.as_mem, mrm.reg_mem.as_mem);
    EXPECT_I16_EQ(tests[i].result.displacement.as_offset, mrm.displacement.as_offset);

    testing_dummy_cpu_destroy(&bus);
  }
}

void test_calc_mod_reg_rm_addr(void) {
  static struct {
    struct mod_rm input;
    u16 result;
  } tests[] = {
      {{mod_rm_mod_indirect, 0, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x0000}}, 0x1214},
      {{mod_rm_mod_indirect, 0, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x0000}}, 0x1416},
      {{mod_rm_mod_indirect, 0, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x0000}}, 0x1416},
      {{mod_rm_mod_indirect, 0, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x0000}}, 0x1618},
      {{mod_rm_mod_indirect, 0, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x0000}}, 0x0c0d},
      {{mod_rm_mod_indirect, 0, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x0000}}, 0x0e0f},
      {{mod_rm_mod_indirect, 0, {.as_mem = mod_rm_mem_bp}, {.as_address = 0x1234}}, 0x1234},
      {{mod_rm_mod_indirect, 0, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x0000}}, 0x0607},

      {{mod_rm_mod_byte, 0, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x10}}, 0x1224},
      {{mod_rm_mod_byte, 0, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x11}}, 0x1427},
      {{mod_rm_mod_byte, 0, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x12}}, 0x1428},
      {{mod_rm_mod_byte, 0, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x13}}, 0x162b},
      {{mod_rm_mod_byte, 0, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x14}}, 0x0c21},
      {{mod_rm_mod_byte, 0, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x15}}, 0x0e24},
      {{mod_rm_mod_byte, 0, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x16}}, 0x081f},
      {{mod_rm_mod_byte, 0, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x17}}, 0x061e},

      {{mod_rm_mod_word, 0, {.as_mem = mod_rm_mem_bx_si}, {.as_offset = 0x0001}}, 0x1215},
      {{mod_rm_mod_word, 0, {.as_mem = mod_rm_mem_bx_di}, {.as_offset = 0x0203}}, 0x1619},
      {{mod_rm_mod_word, 0, {.as_mem = mod_rm_mem_bp_si}, {.as_offset = 0x0405}}, 0x181b},
      {{mod_rm_mod_word, 0, {.as_mem = mod_rm_mem_bp_di}, {.as_offset = 0x0607}}, 0x1c1f},
      {{mod_rm_mod_word, 0, {.as_mem = mod_rm_mem_si}, {.as_offset = 0x0809}}, 0x1416},
      {{mod_rm_mod_word, 0, {.as_mem = mod_rm_mem_di}, {.as_offset = 0x0a0b}}, 0x181a},
      {{mod_rm_mod_word, 0, {.as_mem = mod_rm_mem_bp}, {.as_offset = 0x0c0d}}, 0x1416},
      {{mod_rm_mod_word, 0, {.as_mem = mod_rm_mem_bx}, {.as_offset = 0x0e0f}}, 0x1416},
  };

  for (unsigned i = 0; i < ARRAY_SIZE(tests); ++i) {
    struct cpu cpu;
    struct bus bus;

    u8 buffer[] = {0x00};
    testing_dummy_cpu_init(&cpu, &bus, buffer, sizeof(buffer));

    cpu.regs.reg_16[AX] = 0x0102;
    cpu.regs.reg_16[CX] = 0x0203;
    cpu.regs.reg_16[DX] = 0x0405;
    cpu.regs.reg_16[BX] = 0x0607;
    cpu.regs.reg_16[BP] = 0x0809;
    cpu.regs.reg_16[SP] = 0x0a0b;
    cpu.regs.reg_16[SI] = 0x0c0d;
    cpu.regs.reg_16[DI] = 0x0e0f;

    u16 addr = calc_mod_reg_rm_addr(&cpu, tests[i].input);
    EXPECT_U16_EQ(tests[i].result, addr);

    testing_dummy_cpu_destroy(&bus);
  }
}

void test_read_mod_reg_rm_8(void) {
  struct cpu cpu;
  struct bus bus;

  u8 buffer[] = {0x00};
  testing_dummy_cpu_init(&cpu, &bus, buffer, sizeof(buffer));

  cpu.regs.reg_16[DX] = 0x1234;
  cpu.regs.reg_16[SI] = 0x0000;
  cpu.segments[DS] = 0x0000;

  struct mod_rm mrm;
  mrm.mod = mod_rm_mod_register;
  mrm.reg = mod_rm_reg_bl_bx;
  mrm.reg_mem.as_reg = mod_rm_reg_dl_dx;

  u8 result = read_mod_reg_rm_8(&cpu, mrm, DS);
  EXPECT_U8_EQ(0x12, result);

  testing_dummy_cpu_destroy(&bus);
}

void test_instruction_rol_8(void) {
  static struct {
    u8 input;
    u8 result;
    u8 count;
    u8 carry;
    u8 overflow;
  } tests[] = {
      {0x00, 0x00, 1, 0, 0},
      {0x01, 0x02, 1, 0, 0},
      {0x80, 0x01, 1, 1, 0},
      {0xbf, 0x7f, 1, 1, 0},
  };
  // TODO: Test for overflow flag.

  for (unsigned i = 0; i < ARRAY_SIZE(tests); ++i) {
    struct flags_map flags = decode_flags(0x0);
    u8 result = instruction_rol_8(tests[i].input, tests[i].count, &flags);
    EXPECT_U8_EQ(result, tests[i].result);
    EXPECT_U8_EQ(flags.carry, tests[i].carry);
    EXPECT_U8_EQ(flags.overflow, tests[i].overflow);
  }
}

void test_update_flags_sign_zero_parity_8(void) {
  static struct {
    u8 input;
    u8 sign;
    u8 zero;
    u8 parity;
  } tests[] = {
      {0x00, 0, 1, 1},
      {-78, 1, 0, 1},
  };

  for (unsigned i = 0; i < ARRAY_SIZE(tests); ++i) {
    struct flags_map flags;
    memset(&flags, 0, sizeof(struct flags_map));
    update_flags_sign_zero_parity_8(&flags, tests[i].input);
    EXPECT_U8_EQ(tests[i].sign, flags.sign);
    EXPECT_U8_EQ(tests[i].zero, flags.zero);
    EXPECT_U8_EQ(tests[i].parity, flags.parity);
  }
}

void test_simple_cpu(void) {
  test_fetch_mod_reg_rm();
  test_calc_mod_reg_rm_addr();
  test_update_flags_sign_zero_parity_8();
  test_instruction_rol_8();
  test_read_mod_reg_rm_8();
}
#endif
