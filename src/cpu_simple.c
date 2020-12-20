#include "bus.h"
#include "cpu.h"
#include "decoder/mod_reg_rm.h"
#include "print_format.h"
#include "testing.h"

#include <stdio.h>

#define MOD_REG_RM_MOD(x) ((x) >> 0x06)
#define MOD_REG_RM_REG(x) ((x) >> 0x03 & 0x07)
#define MOD_REG_RM_RM(x) ((x)&0x07)

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

#if defined(TESTING)
void test_fetch_mod_reg_rm(void) {
  static struct {
    u8 buffer[6];
    struct mod_reg_rm result;
  } tests[] = {
      {{0x00, 0x00, 0xf0}, {mrrm_mod_indirect, AX, mrrm_rm_bx_si, 0}},
      {{0x01, 0x01, 0xf1}, {mrrm_mod_indirect, AX, mrrm_rm_bx_di, 0}},
      {{0x02, 0x02, 0xf2}, {mrrm_mod_indirect, AX, mrrm_rm_bp_si, 0}},
      {{0x03, 0x03, 0xf3}, {mrrm_mod_indirect, AX, mrrm_rm_bp_di, 0}},
      {{0x04, 0x04, 0xf4}, {mrrm_mod_indirect, AX, mrrm_rm_si, 0}},
      {{0x05, 0x05, 0xf5}, {mrrm_mod_indirect, AX, mrrm_rm_di, 0}},
      {{0x06, 0x06, 0xf6}, {mrrm_mod_indirect, AX, mrrm_rm_bp, 0xf606}},
      {{0x07, 0x07, 0xf7}, {mrrm_mod_indirect, AX, mrrm_rm_bx, 0}},
      {{0x08, 0x08, 0xf8}, {mrrm_mod_indirect, CX, mrrm_rm_bx_si, 0}},
      {{0x09, 0x09, 0xf9}, {mrrm_mod_indirect, CX, mrrm_rm_bx_di, 0}},
      {{0x0a, 0x0a, 0xfa}, {mrrm_mod_indirect, CX, mrrm_rm_bp_si, 0}},
      {{0x0b, 0x0b, 0xfb}, {mrrm_mod_indirect, CX, mrrm_rm_bp_di, 0}},
      {{0x0c, 0x0c, 0xfc}, {mrrm_mod_indirect, CX, mrrm_rm_si, 0}},
      {{0x0d, 0x0d, 0xfd}, {mrrm_mod_indirect, CX, mrrm_rm_di, 0}},
      {{0x0e, 0x0e, 0xfe}, {mrrm_mod_indirect, CX, mrrm_rm_bp, 0xfe0e}},
      {{0x0f, 0x0f, 0xff}, {mrrm_mod_indirect, CX, mrrm_rm_bx, 0}},

      {{0x10, 0x10, 0x00}, {mrrm_mod_indirect, DX, mrrm_rm_bx_si, 0}},
      {{0x11, 0x11, 0x01}, {mrrm_mod_indirect, DX, mrrm_rm_bx_di, 0}},
      {{0x12, 0x12, 0x02}, {mrrm_mod_indirect, DX, mrrm_rm_bp_si, 0}},
      {{0x13, 0x13, 0x03}, {mrrm_mod_indirect, DX, mrrm_rm_bp_di, 0}},
      {{0x14, 0x14, 0x04}, {mrrm_mod_indirect, DX, mrrm_rm_si, 0}},
      {{0x15, 0x15, 0x05}, {mrrm_mod_indirect, DX, mrrm_rm_di, 0}},
      {{0x16, 0x16, 0x06}, {mrrm_mod_indirect, DX, mrrm_rm_bp, 0x0616}},
      {{0x17, 0x17, 0x07}, {mrrm_mod_indirect, DX, mrrm_rm_bx, 0}},
      {{0x18, 0x18, 0x08}, {mrrm_mod_indirect, BX, mrrm_rm_bx_si, 0}},
      {{0x19, 0x19, 0x09}, {mrrm_mod_indirect, BX, mrrm_rm_bx_di, 0}},
      {{0x1a, 0x1a, 0x0a}, {mrrm_mod_indirect, BX, mrrm_rm_bp_si, 0}},
      {{0x1b, 0x1b, 0x0b}, {mrrm_mod_indirect, BX, mrrm_rm_bp_di, 0}},
      {{0x1c, 0x1c, 0x0c}, {mrrm_mod_indirect, BX, mrrm_rm_si, 0}},
      {{0x1d, 0x1d, 0x0d}, {mrrm_mod_indirect, BX, mrrm_rm_di, 0}},
      {{0x1e, 0x1e, 0x0e}, {mrrm_mod_indirect, BX, mrrm_rm_bp, 0x0e1e}},
      {{0x1f, 0x1f, 0x0f}, {mrrm_mod_indirect, BX, mrrm_rm_bx, 0}},

      {{0x20, 0x20, 0x10}, {mrrm_mod_indirect, BP, mrrm_rm_bx_si, 0}},
      {{0x21, 0x21, 0x11}, {mrrm_mod_indirect, BP, mrrm_rm_bx_di, 0}},
      {{0x22, 0x22, 0x12}, {mrrm_mod_indirect, BP, mrrm_rm_bp_si, 0}},
      {{0x23, 0x23, 0x13}, {mrrm_mod_indirect, BP, mrrm_rm_bp_di, 0}},
      {{0x24, 0x24, 0x14}, {mrrm_mod_indirect, BP, mrrm_rm_si, 0}},
      {{0x25, 0x25, 0x15}, {mrrm_mod_indirect, BP, mrrm_rm_di, 0}},
      {{0x26, 0x26, 0x16}, {mrrm_mod_indirect, BP, mrrm_rm_bp, 0x1626}},
      {{0x27, 0x27, 0x17}, {mrrm_mod_indirect, BP, mrrm_rm_bx, 0}},
      {{0x28, 0x28, 0x18}, {mrrm_mod_indirect, SP, mrrm_rm_bx_si, 0}},
      {{0x29, 0x29, 0x19}, {mrrm_mod_indirect, SP, mrrm_rm_bx_di, 0}},
      {{0x2a, 0x2a, 0x1a}, {mrrm_mod_indirect, SP, mrrm_rm_bp_si, 0}},
      {{0x2b, 0x2b, 0x1b}, {mrrm_mod_indirect, SP, mrrm_rm_bp_di, 0}},
      {{0x2c, 0x2c, 0x1c}, {mrrm_mod_indirect, SP, mrrm_rm_si, 0}},
      {{0x2d, 0x2d, 0x1d}, {mrrm_mod_indirect, SP, mrrm_rm_di, 0}},
      {{0x2e, 0x2e, 0x1e}, {mrrm_mod_indirect, SP, mrrm_rm_bp, 0x1e2e}},
      {{0x2f, 0x2f, 0x1f}, {mrrm_mod_indirect, SP, mrrm_rm_bx, 0}},

      {{0x30, 0x30, 0x20}, {mrrm_mod_indirect, SI, mrrm_rm_bx_si, 0}},
      {{0x31, 0x31, 0x21}, {mrrm_mod_indirect, SI, mrrm_rm_bx_di, 0}},
      {{0x32, 0x32, 0x22}, {mrrm_mod_indirect, SI, mrrm_rm_bp_si, 0}},
      {{0x33, 0x33, 0x23}, {mrrm_mod_indirect, SI, mrrm_rm_bp_di, 0}},
      {{0x34, 0x34, 0x24}, {mrrm_mod_indirect, SI, mrrm_rm_si, 0}},
      {{0x35, 0x35, 0x25}, {mrrm_mod_indirect, SI, mrrm_rm_di, 0}},
      {{0x36, 0x36, 0x26}, {mrrm_mod_indirect, SI, mrrm_rm_bp, 0x2636}},
      {{0x37, 0x37, 0x27}, {mrrm_mod_indirect, SI, mrrm_rm_bx, 0}},
      {{0x38, 0x38, 0x28}, {mrrm_mod_indirect, DI, mrrm_rm_bx_si, 0}},
      {{0x39, 0x39, 0x29}, {mrrm_mod_indirect, DI, mrrm_rm_bx_di, 0}},
      {{0x3a, 0x3a, 0x2a}, {mrrm_mod_indirect, DI, mrrm_rm_bp_si, 0}},
      {{0x3b, 0x3b, 0x2b}, {mrrm_mod_indirect, DI, mrrm_rm_bp_di, 0}},
      {{0x3c, 0x3c, 0x2c}, {mrrm_mod_indirect, DI, mrrm_rm_si, 0}},
      {{0x3d, 0x3d, 0x2d}, {mrrm_mod_indirect, DI, mrrm_rm_di, 0}},
      {{0x3e, 0x3e, 0x2e}, {mrrm_mod_indirect, DI, mrrm_rm_bp, 0x2e3e}},
      {{0x3f, 0x3f, 0x2f}, {mrrm_mod_indirect, DI, mrrm_rm_bx, 0}},

      {{0x40, 0x40, 0x30}, {mrrm_mod_byte, AX, mrrm_rm_bx_si, 0x40}},
      {{0x41, 0x41, 0x31}, {mrrm_mod_byte, AX, mrrm_rm_bx_di, 0x41}},
      {{0x42, 0x42, 0x32}, {mrrm_mod_byte, AX, mrrm_rm_bp_si, 0x42}},
      {{0x43, 0x43, 0x33}, {mrrm_mod_byte, AX, mrrm_rm_bp_di, 0x43}},
      {{0x44, 0x44, 0x34}, {mrrm_mod_byte, AX, mrrm_rm_si, 0x44}},
      {{0x45, 0x45, 0x35}, {mrrm_mod_byte, AX, mrrm_rm_di, 0x45}},
      {{0x46, 0x46, 0x36}, {mrrm_mod_byte, AX, mrrm_rm_bp, 0x46}},
      {{0x47, 0x47, 0x37}, {mrrm_mod_byte, AX, mrrm_rm_bx, 0x47}},
      {{0x48, 0x48, 0x38}, {mrrm_mod_byte, CX, mrrm_rm_bx_si, 0x48}},
      {{0x49, 0x49, 0x39}, {mrrm_mod_byte, CX, mrrm_rm_bx_di, 0x49}},
      {{0x4a, 0x4a, 0x3a}, {mrrm_mod_byte, CX, mrrm_rm_bp_si, 0x4a}},
      {{0x4b, 0x4b, 0x3b}, {mrrm_mod_byte, CX, mrrm_rm_bp_di, 0x4b}},
      {{0x4c, 0x4c, 0x3c}, {mrrm_mod_byte, CX, mrrm_rm_si, 0x4c}},
      {{0x4d, 0x4d, 0x3d}, {mrrm_mod_byte, CX, mrrm_rm_di, 0x4d}},
      {{0x4e, 0x4e, 0x3e}, {mrrm_mod_byte, CX, mrrm_rm_bp, 0x4e}},
      {{0x4f, 0x4f, 0x3f}, {mrrm_mod_byte, CX, mrrm_rm_bx, 0x4f}},

      {{0x50, 0x50, 0x40}, {mrrm_mod_byte, DX, mrrm_rm_bx_si, 0x50}},
      {{0x51, 0x51, 0x41}, {mrrm_mod_byte, DX, mrrm_rm_bx_di, 0x51}},
      {{0x52, 0x52, 0x42}, {mrrm_mod_byte, DX, mrrm_rm_bp_si, 0x52}},
      {{0x53, 0x53, 0x43}, {mrrm_mod_byte, DX, mrrm_rm_bp_di, 0x53}},
      {{0x54, 0x54, 0x44}, {mrrm_mod_byte, DX, mrrm_rm_si, 0x54}},
      {{0x55, 0x55, 0x45}, {mrrm_mod_byte, DX, mrrm_rm_di, 0x55}},
      {{0x56, 0x56, 0x46}, {mrrm_mod_byte, DX, mrrm_rm_bp, 0x56}},
      {{0x57, 0x57, 0x47}, {mrrm_mod_byte, DX, mrrm_rm_bx, 0x57}},
      {{0x58, 0x58, 0x48}, {mrrm_mod_byte, BX, mrrm_rm_bx_si, 0x58}},
      {{0x59, 0x59, 0x49}, {mrrm_mod_byte, BX, mrrm_rm_bx_di, 0x59}},
      {{0x5a, 0x5a, 0x4a}, {mrrm_mod_byte, BX, mrrm_rm_bp_si, 0x5a}},
      {{0x5b, 0x5b, 0x4b}, {mrrm_mod_byte, BX, mrrm_rm_bp_di, 0x5b}},
      {{0x5c, 0x5c, 0x4c}, {mrrm_mod_byte, BX, mrrm_rm_si, 0x5c}},
      {{0x5d, 0x5d, 0x4d}, {mrrm_mod_byte, BX, mrrm_rm_di, 0x5d}},
      {{0x5e, 0x5e, 0x4e}, {mrrm_mod_byte, BX, mrrm_rm_bp, 0x5e}},
      {{0x5f, 0x5f, 0x4f}, {mrrm_mod_byte, BX, mrrm_rm_bx, 0x5f}},

      {{0x60, 0x60, 0x50}, {mrrm_mod_byte, BP, mrrm_rm_bx_si, 0x60}},
      {{0x61, 0x61, 0x51}, {mrrm_mod_byte, BP, mrrm_rm_bx_di, 0x61}},
      {{0x62, 0x62, 0x52}, {mrrm_mod_byte, BP, mrrm_rm_bp_si, 0x62}},
      {{0x63, 0x63, 0x53}, {mrrm_mod_byte, BP, mrrm_rm_bp_di, 0x63}},
      {{0x64, 0x64, 0x54}, {mrrm_mod_byte, BP, mrrm_rm_si, 0x64}},
      {{0x65, 0x65, 0x55}, {mrrm_mod_byte, BP, mrrm_rm_di, 0x65}},
      {{0x66, 0x66, 0x56}, {mrrm_mod_byte, BP, mrrm_rm_bp, 0x66}},
      {{0x67, 0x67, 0x57}, {mrrm_mod_byte, BP, mrrm_rm_bx, 0x67}},
      {{0x68, 0x68, 0x58}, {mrrm_mod_byte, SP, mrrm_rm_bx_si, 0x68}},
      {{0x69, 0x69, 0x59}, {mrrm_mod_byte, SP, mrrm_rm_bx_di, 0x69}},
      {{0x6a, 0x6a, 0x5a}, {mrrm_mod_byte, SP, mrrm_rm_bp_si, 0x6a}},
      {{0x6b, 0x6b, 0x5b}, {mrrm_mod_byte, SP, mrrm_rm_bp_di, 0x6b}},
      {{0x6c, 0x6c, 0x5c}, {mrrm_mod_byte, SP, mrrm_rm_si, 0x6c}},
      {{0x6d, 0x6d, 0x5d}, {mrrm_mod_byte, SP, mrrm_rm_di, 0x6d}},
      {{0x6e, 0x6e, 0x5e}, {mrrm_mod_byte, SP, mrrm_rm_bp, 0x6e}},
      {{0x6f, 0x6f, 0x5f}, {mrrm_mod_byte, SP, mrrm_rm_bx, 0x6f}},

      {{0x70, 0x70, 0x60}, {mrrm_mod_byte, SI, mrrm_rm_bx_si, 0x70}},
      {{0x71, 0x71, 0x61}, {mrrm_mod_byte, SI, mrrm_rm_bx_di, 0x71}},
      {{0x72, 0x72, 0x62}, {mrrm_mod_byte, SI, mrrm_rm_bp_si, 0x72}},
      {{0x73, 0x73, 0x63}, {mrrm_mod_byte, SI, mrrm_rm_bp_di, 0x73}},
      {{0x74, 0x74, 0x64}, {mrrm_mod_byte, SI, mrrm_rm_si, 0x74}},
      {{0x75, 0x75, 0x65}, {mrrm_mod_byte, SI, mrrm_rm_di, 0x75}},
      {{0x76, 0x76, 0x66}, {mrrm_mod_byte, SI, mrrm_rm_bp, 0x76}},
      {{0x77, 0x77, 0x67}, {mrrm_mod_byte, SI, mrrm_rm_bx, 0x77}},
      {{0x78, 0x78, 0x68}, {mrrm_mod_byte, DI, mrrm_rm_bx_si, 0x78}},
      {{0x79, 0x79, 0x69}, {mrrm_mod_byte, DI, mrrm_rm_bx_di, 0x79}},
      {{0x7a, 0x7a, 0x6a}, {mrrm_mod_byte, DI, mrrm_rm_bp_si, 0x7a}},
      {{0x7b, 0x7b, 0x6b}, {mrrm_mod_byte, DI, mrrm_rm_bp_di, 0x7b}},
      {{0x7c, 0x7c, 0x6c}, {mrrm_mod_byte, DI, mrrm_rm_si, 0x7c}},
      {{0x7d, 0x7d, 0x6d}, {mrrm_mod_byte, DI, mrrm_rm_di, 0x7d}},
      {{0x7e, 0x7e, 0x6e}, {mrrm_mod_byte, DI, mrrm_rm_bp, 0x7e}},
      {{0x7f, 0x7f, 0x6f}, {mrrm_mod_byte, DI, mrrm_rm_bx, 0x7f}},

      {{0x80, 0x80, 0x70}, {mrrm_mod_word, AX, mrrm_rm_bx_si, 0x7080}},
      {{0x81, 0x81, 0x71}, {mrrm_mod_word, AX, mrrm_rm_bx_di, 0x7181}},
      {{0x82, 0x82, 0x72}, {mrrm_mod_word, AX, mrrm_rm_bp_si, 0x7282}},
      {{0x83, 0x83, 0x73}, {mrrm_mod_word, AX, mrrm_rm_bp_di, 0x7383}},
      {{0x84, 0x84, 0x74}, {mrrm_mod_word, AX, mrrm_rm_si, 0x7484}},
      {{0x85, 0x85, 0x75}, {mrrm_mod_word, AX, mrrm_rm_di, 0x7585}},
      {{0x86, 0x86, 0x76}, {mrrm_mod_word, AX, mrrm_rm_bp, 0x7686}},
      {{0x87, 0x87, 0x77}, {mrrm_mod_word, AX, mrrm_rm_bx, 0x7787}},
      {{0x88, 0x88, 0x78}, {mrrm_mod_word, CX, mrrm_rm_bx_si, 0x7888}},
      {{0x89, 0x89, 0x79}, {mrrm_mod_word, CX, mrrm_rm_bx_di, 0x7989}},
      {{0x8a, 0x8a, 0x7a}, {mrrm_mod_word, CX, mrrm_rm_bp_si, 0x7a8a}},
      {{0x8b, 0x8b, 0x7b}, {mrrm_mod_word, CX, mrrm_rm_bp_di, 0x7b8b}},
      {{0x8c, 0x8c, 0x7c}, {mrrm_mod_word, CX, mrrm_rm_si, 0x7c8c}},
      {{0x8d, 0x8d, 0x7d}, {mrrm_mod_word, CX, mrrm_rm_di, 0x7d8d}},
      {{0x8e, 0x8e, 0x7e}, {mrrm_mod_word, CX, mrrm_rm_bp, 0x7e8e}},
      {{0x8f, 0x8f, 0x7f}, {mrrm_mod_word, CX, mrrm_rm_bx, 0x7f8f}},

      {{0x90, 0x90, 0x80}, {mrrm_mod_word, DX, mrrm_rm_bx_si, 0x8090}},
      {{0x91, 0x91, 0x81}, {mrrm_mod_word, DX, mrrm_rm_bx_di, 0x8191}},
      {{0x92, 0x92, 0x82}, {mrrm_mod_word, DX, mrrm_rm_bp_si, 0x8292}},
      {{0x93, 0x93, 0x83}, {mrrm_mod_word, DX, mrrm_rm_bp_di, 0x8393}},
      {{0x94, 0x94, 0x84}, {mrrm_mod_word, DX, mrrm_rm_si, 0x8494}},
      {{0x95, 0x95, 0x85}, {mrrm_mod_word, DX, mrrm_rm_di, 0x8595}},
      {{0x96, 0x96, 0x86}, {mrrm_mod_word, DX, mrrm_rm_bp, 0x8696}},
      {{0x97, 0x97, 0x87}, {mrrm_mod_word, DX, mrrm_rm_bx, 0x8797}},
      {{0x98, 0x98, 0x88}, {mrrm_mod_word, BX, mrrm_rm_bx_si, 0x8898}},
      {{0x99, 0x99, 0x89}, {mrrm_mod_word, BX, mrrm_rm_bx_di, 0x8999}},
      {{0x9a, 0x9a, 0x8a}, {mrrm_mod_word, BX, mrrm_rm_bp_si, 0x8a9a}},
      {{0x9b, 0x9b, 0x8b}, {mrrm_mod_word, BX, mrrm_rm_bp_di, 0x8b9b}},
      {{0x9c, 0x9c, 0x8c}, {mrrm_mod_word, BX, mrrm_rm_si, 0x8c9c}},
      {{0x9d, 0x9d, 0x8d}, {mrrm_mod_word, BX, mrrm_rm_di, 0x8d9d}},
      {{0x9e, 0x9e, 0x8e}, {mrrm_mod_word, BX, mrrm_rm_bp, 0x8e9e}},
      {{0x9f, 0x9f, 0x8f}, {mrrm_mod_word, BX, mrrm_rm_bx, 0x8f9f}},

      {{0xa0, 0xa0, 0x90}, {mrrm_mod_word, BP, mrrm_rm_bx_si, 0x90a0}},
      {{0xa1, 0xa1, 0x91}, {mrrm_mod_word, BP, mrrm_rm_bx_di, 0x91a1}},
      {{0xa2, 0xa2, 0x92}, {mrrm_mod_word, BP, mrrm_rm_bp_si, 0x92a2}},
      {{0xa3, 0xa3, 0x93}, {mrrm_mod_word, BP, mrrm_rm_bp_di, 0x93a3}},
      {{0xa4, 0xa4, 0x94}, {mrrm_mod_word, BP, mrrm_rm_si, 0x94a4}},
      {{0xa5, 0xa5, 0x95}, {mrrm_mod_word, BP, mrrm_rm_di, 0x95a5}},
      {{0xa6, 0xa6, 0x96}, {mrrm_mod_word, BP, mrrm_rm_bp, 0x96a6}},
      {{0xa7, 0xa7, 0x97}, {mrrm_mod_word, BP, mrrm_rm_bx, 0x97a7}},
      {{0xa8, 0xa8, 0x98}, {mrrm_mod_word, SP, mrrm_rm_bx_si, 0x98a8}},
      {{0xa9, 0xa9, 0x99}, {mrrm_mod_word, SP, mrrm_rm_bx_di, 0x99a9}},
      {{0xaa, 0xaa, 0x9a}, {mrrm_mod_word, SP, mrrm_rm_bp_si, 0x9aaa}},
      {{0xab, 0xab, 0x9b}, {mrrm_mod_word, SP, mrrm_rm_bp_di, 0x9bab}},
      {{0xac, 0xac, 0x9c}, {mrrm_mod_word, SP, mrrm_rm_si, 0x9cac}},
      {{0xad, 0xad, 0x9d}, {mrrm_mod_word, SP, mrrm_rm_di, 0x9dad}},
      {{0xae, 0xae, 0x9e}, {mrrm_mod_word, SP, mrrm_rm_bp, 0x9eae}},
      {{0xaf, 0xaf, 0x9f}, {mrrm_mod_word, SP, mrrm_rm_bx, 0x9faf}},

      {{0xb0, 0xb0, 0xa0}, {mrrm_mod_word, SI, mrrm_rm_bx_si, 0xa0b0}},
      {{0xb1, 0xb1, 0xa1}, {mrrm_mod_word, SI, mrrm_rm_bx_di, 0xa1b1}},
      {{0xb2, 0xb2, 0xa2}, {mrrm_mod_word, SI, mrrm_rm_bp_si, 0xa2b2}},
      {{0xb3, 0xb3, 0xa3}, {mrrm_mod_word, SI, mrrm_rm_bp_di, 0xa3b3}},
      {{0xb4, 0xb4, 0xa4}, {mrrm_mod_word, SI, mrrm_rm_si, 0xa4b4}},
      {{0xb5, 0xb5, 0xa5}, {mrrm_mod_word, SI, mrrm_rm_di, 0xa5b5}},
      {{0xb6, 0xb6, 0xa6}, {mrrm_mod_word, SI, mrrm_rm_bp, 0xa6b6}},
      {{0xb7, 0xb7, 0xa7}, {mrrm_mod_word, SI, mrrm_rm_bx, 0xa7b7}},
      {{0xb8, 0xb8, 0xa8}, {mrrm_mod_word, DI, mrrm_rm_bx_si, 0xa8b8}},
      {{0xb9, 0xb9, 0xa9}, {mrrm_mod_word, DI, mrrm_rm_bx_di, 0xa9b9}},
      {{0xba, 0xba, 0xaa}, {mrrm_mod_word, DI, mrrm_rm_bp_si, 0xaaba}},
      {{0xbb, 0xbb, 0xab}, {mrrm_mod_word, DI, mrrm_rm_bp_di, 0xabbb}},
      {{0xbc, 0xbc, 0xac}, {mrrm_mod_word, DI, mrrm_rm_si, 0xacbc}},
      {{0xbd, 0xbd, 0xad}, {mrrm_mod_word, DI, mrrm_rm_di, 0xadbd}},
      {{0xbe, 0xbe, 0xae}, {mrrm_mod_word, DI, mrrm_rm_bp, 0xaebe}},
      {{0xbf, 0xbf, 0xaf}, {mrrm_mod_word, DI, mrrm_rm_bx, 0xafbf}},

      {{0xc0, 0xc0, 0xb0}, {mrrm_mod_register, AX, AX, 0}},
      {{0xc1, 0xc1, 0xb1}, {mrrm_mod_register, AX, CX, 0}},
      {{0xc2, 0xc2, 0xb2}, {mrrm_mod_register, AX, DX, 0}},
      {{0xc3, 0xc3, 0xb3}, {mrrm_mod_register, AX, BX, 0}},
      {{0xc4, 0xc4, 0xb4}, {mrrm_mod_register, AX, BP, 0}},
      {{0xc5, 0xc5, 0xb5}, {mrrm_mod_register, AX, SP, 0}},
      {{0xc6, 0xc6, 0xb6}, {mrrm_mod_register, AX, SI, 0}},
      {{0xc7, 0xc7, 0xb7}, {mrrm_mod_register, AX, DI, 0}},
      {{0xc8, 0xc8, 0xb8}, {mrrm_mod_register, CX, AX, 0}},
      {{0xc9, 0xc9, 0xb9}, {mrrm_mod_register, CX, CX, 0}},
      {{0xca, 0xca, 0xba}, {mrrm_mod_register, CX, DX, 0}},
      {{0xcb, 0xcb, 0xbb}, {mrrm_mod_register, CX, BX, 0}},
      {{0xcc, 0xcc, 0xbc}, {mrrm_mod_register, CX, BP, 0}},
      {{0xcd, 0xcd, 0xbd}, {mrrm_mod_register, CX, SP, 0}},
      {{0xce, 0xce, 0xbe}, {mrrm_mod_register, CX, SI, 0}},
      {{0xcf, 0xcf, 0xbf}, {mrrm_mod_register, CX, DI, 0}},

      {{0xd0, 0xd0, 0xc0}, {mrrm_mod_register, DX, AX, 0}},
      {{0xd1, 0xd1, 0xc1}, {mrrm_mod_register, DX, CX, 0}},
      {{0xd2, 0xd2, 0xc2}, {mrrm_mod_register, DX, DX, 0}},
      {{0xd3, 0xd3, 0xc3}, {mrrm_mod_register, DX, BX, 0}},
      {{0xd4, 0xd4, 0xc4}, {mrrm_mod_register, DX, BP, 0}},
      {{0xd5, 0xd5, 0xc5}, {mrrm_mod_register, DX, SP, 0}},
      {{0xd6, 0xd6, 0xc6}, {mrrm_mod_register, DX, SI, 0}},
      {{0xd7, 0xd7, 0xc7}, {mrrm_mod_register, DX, DI, 0}},
      {{0xd8, 0xd8, 0xc8}, {mrrm_mod_register, BX, AX, 0}},
      {{0xd9, 0xd9, 0xc9}, {mrrm_mod_register, BX, CX, 0}},
      {{0xda, 0xda, 0xca}, {mrrm_mod_register, BX, DX, 0}},
      {{0xdb, 0xdb, 0xcb}, {mrrm_mod_register, BX, BX, 0}},
      {{0xdc, 0xdc, 0xcc}, {mrrm_mod_register, BX, BP, 0}},
      {{0xdd, 0xdd, 0xcd}, {mrrm_mod_register, BX, SP, 0}},
      {{0xde, 0xde, 0xce}, {mrrm_mod_register, BX, SI, 0}},
      {{0xdf, 0xdf, 0xcf}, {mrrm_mod_register, BX, DI, 0}},

      {{0xe0, 0xe0, 0xd0}, {mrrm_mod_register, BP, AX, 0}},
      {{0xe1, 0xe1, 0xd1}, {mrrm_mod_register, BP, CX, 0}},
      {{0xe2, 0xe2, 0xd2}, {mrrm_mod_register, BP, DX, 0}},
      {{0xe3, 0xe3, 0xd3}, {mrrm_mod_register, BP, BX, 0}},
      {{0xe4, 0xe4, 0xd4}, {mrrm_mod_register, BP, BP, 0}},
      {{0xe5, 0xe5, 0xd5}, {mrrm_mod_register, BP, SP, 0}},
      {{0xe6, 0xe6, 0xd6}, {mrrm_mod_register, BP, SI, 0}},
      {{0xe7, 0xe7, 0xd7}, {mrrm_mod_register, BP, DI, 0}},
      {{0xe8, 0xe8, 0xd8}, {mrrm_mod_register, SP, AX, 0}},
      {{0xe9, 0xe9, 0xd9}, {mrrm_mod_register, SP, CX, 0}},
      {{0xea, 0xea, 0xda}, {mrrm_mod_register, SP, DX, 0}},
      {{0xeb, 0xeb, 0xdb}, {mrrm_mod_register, SP, BX, 0}},
      {{0xec, 0xec, 0xdc}, {mrrm_mod_register, SP, BP, 0}},
      {{0xed, 0xed, 0xdd}, {mrrm_mod_register, SP, SP, 0}},
      {{0xee, 0xee, 0xde}, {mrrm_mod_register, SP, SI, 0}},
      {{0xef, 0xef, 0xdf}, {mrrm_mod_register, SP, DI, 0}},

      {{0xf0, 0xf0, 0xe0}, {mrrm_mod_register, SI, AX, 0}},
      {{0xf1, 0xf1, 0xe1}, {mrrm_mod_register, SI, CX, 0}},
      {{0xf2, 0xf2, 0xe2}, {mrrm_mod_register, SI, DX, 0}},
      {{0xf3, 0xf3, 0xe3}, {mrrm_mod_register, SI, BX, 0}},
      {{0xf4, 0xf4, 0xe4}, {mrrm_mod_register, SI, BP, 0}},
      {{0xf5, 0xf5, 0xe5}, {mrrm_mod_register, SI, SP, 0}},
      {{0xf6, 0xf6, 0xe6}, {mrrm_mod_register, SI, SI, 0}},
      {{0xf7, 0xf7, 0xe7}, {mrrm_mod_register, SI, DI, 0}},
      {{0xf8, 0xf8, 0xe8}, {mrrm_mod_register, DI, AX, 0}},
      {{0xf9, 0xf9, 0xe9}, {mrrm_mod_register, DI, CX, 0}},
      {{0xfa, 0xfa, 0xea}, {mrrm_mod_register, DI, DX, 0}},
      {{0xfb, 0xfb, 0xeb}, {mrrm_mod_register, DI, BX, 0}},
      {{0xfc, 0xfc, 0xec}, {mrrm_mod_register, DI, BP, 0}},
      {{0xfd, 0xfd, 0xed}, {mrrm_mod_register, DI, SP, 0}},
      {{0xfe, 0xfe, 0xee}, {mrrm_mod_register, DI, SI, 0}},
      {{0xff, 0xff, 0xef}, {mrrm_mod_register, DI, DI, 0}},
  };

  for (unsigned i = 0; i < ARRAY_SIZE(tests); ++i) {
    struct bus bus;
    struct cpu cpu;

    testing_dummy_cpu_init(&cpu, &bus, tests[i].buffer, 6);

    struct mod_reg_rm mrr = fetch_mod_reg_rm(&cpu);
    assert(mrr.mod == tests[i].result.mod);
    assert(mrr.reg == tests[i].result.reg);
    assert(mrr.rm == tests[i].result.rm);
    assert(mrr.disp == tests[i].result.disp);

    testing_dummy_cpu_destroy(&bus);
  }
}
#endif

static u16 calc_mod_reg_rm_addr(struct cpu *cpu, struct mod_reg_rm mrr) {
  // Can not calculate the address for a register.
  assert(mrr.mod != mrrm_mod_register);

  // Indirect addressing on register BP is handled as a 16-bit displacement.
  if (mrr.mod == mrrm_mod_indirect && mrr.rm == 0x06) {
    return mrr.disp;
  }

#if !defined(NDEBUG)
  // Indirect mode should not have a displacement, except for the case above.
  if (mrr.mod == mrrm_mod_indirect) {
    assert(mrr.disp == 0);
  }

  // Check the bounds for the 8-bit displacement.
  if (mrr.mod == mrrm_mod_byte) {
    assert((mrr.disp & 0xff00) == 0);
  }
#endif

  switch (mrr.rm) {
    case mrrm_rm_bx_si:
      return cpu->regs.reg_16[BX] + cpu->regs.reg_16[SI] + mrr.disp;

    case mrrm_rm_bx_di:
      return cpu->regs.reg_16[BX] + cpu->regs.reg_16[DI] + mrr.disp;

    case mrrm_rm_bp_si:
      return cpu->regs.reg_16[BP] + cpu->regs.reg_16[SI] + mrr.disp;

    case mrrm_rm_bp_di:
      return cpu->regs.reg_16[BP] + cpu->regs.reg_16[DI] + mrr.disp;

    case mrrm_rm_si:
      return cpu->regs.reg_16[SI] + mrr.disp;

    case mrrm_rm_di:
      return cpu->regs.reg_16[DI] + mrr.disp;

    case mrrm_rm_bp:
      return cpu->regs.reg_16[BP] + mrr.disp;

    case mrrm_rm_bx:
      return cpu->regs.reg_16[BX] + mrr.disp;

    default:
      assert(0);
      return 0;
  }
}

#if defined(TESTING)
void test_calc_mod_reg_rm_addr(void) {
  static struct {
    struct mod_reg_rm input;
    u16 result;
  } tests[] = {
      {{mrrm_mod_indirect, 0, mrrm_rm_bx_si, 0x0000}, 0x1214},
      {{mrrm_mod_indirect, 0, mrrm_rm_bx_di, 0x0000}, 0x1416},
      {{mrrm_mod_indirect, 0, mrrm_rm_bp_si, 0x0000}, 0x1416},
      {{mrrm_mod_indirect, 0, mrrm_rm_bp_di, 0x0000}, 0x1618},
      {{mrrm_mod_indirect, 0, mrrm_rm_si, 0x0000}, 0x0c0d},
      {{mrrm_mod_indirect, 0, mrrm_rm_di, 0x0000}, 0x0e0f},
      {{mrrm_mod_indirect, 0, mrrm_rm_bp, 0x1234}, 0x1234},
      {{mrrm_mod_indirect, 0, mrrm_rm_bx, 0x0000}, 0x0607},

      {{mrrm_mod_byte, 0, mrrm_rm_bx_si, 0x10}, 0x1224},
      {{mrrm_mod_byte, 0, mrrm_rm_bx_di, 0x11}, 0x1427},
      {{mrrm_mod_byte, 0, mrrm_rm_bp_si, 0x12}, 0x1428},
      {{mrrm_mod_byte, 0, mrrm_rm_bp_di, 0x13}, 0x162b},
      {{mrrm_mod_byte, 0, mrrm_rm_si, 0x14}, 0x0c21},
      {{mrrm_mod_byte, 0, mrrm_rm_di, 0x15}, 0x0e24},
      {{mrrm_mod_byte, 0, mrrm_rm_bp, 0x16}, 0x081f},
      {{mrrm_mod_byte, 0, mrrm_rm_bx, 0x17}, 0x061e},

      {{mrrm_mod_word, 0, mrrm_rm_bx_si, 0x0001}, 0x1215},
      {{mrrm_mod_word, 0, mrrm_rm_bx_di, 0x0203}, 0x1619},
      {{mrrm_mod_word, 0, mrrm_rm_bp_si, 0x0405}, 0x181b},
      {{mrrm_mod_word, 0, mrrm_rm_bp_di, 0x0607}, 0x1c1f},
      {{mrrm_mod_word, 0, mrrm_rm_si, 0x0809}, 0x1416},
      {{mrrm_mod_word, 0, mrrm_rm_di, 0x0a0b}, 0x181a},
      {{mrrm_mod_word, 0, mrrm_rm_bp, 0x0c0d}, 0x1416},
      {{mrrm_mod_word, 0, mrrm_rm_bx, 0x0e0f}, 0x1416},
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
    assert(addr == tests[i].result);

    testing_dummy_cpu_destroy(&bus);
  }
}
#endif

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
      return cpu->regs.reg_8[mrr.rm];
    }

    default:
      assert(0);
      return 0;
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
      cpu->regs.reg_8[mrr.rm] = value;
      break;

    default:
      assert(0);
  }
}

static void update_flags_sign_zero_parity_8(struct flags_map *flags, u8 value) {
  flags->sign = value * 0x80;
  flags->zero = !value;
  flags->parity = parity[value];
}

#if defined(TESTING)
void test_cpu_flags_sign_zero_parity_8(void) {
}
#endif

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

#if defined(TESTING)
void test_instruction_rol_8(void) {
  static struct {
    u8 input;
    u8 result;
    u8 count;
    u8 carry;
    u8 overflow;
  } tests[] = {
      {0b00000000, 0b00000000, 1, 0, 0},
      {0b00000001, 0b00000010, 1, 0, 0},
      {0b10000000, 0b00000001, 1, 1, 0},
      {0b10111111, 0b01111111, 1, 1, 0},
      // TODO: Test for overflow flag.
  };
  for (unsigned i = 0; i < ARRAY_SIZE(tests); ++i) {
    struct flags_map flags = decode_flags(0x0);
    u8 result = instruction_rol_8(tests[i].input, tests[i].count, &flags);
    assert(result == tests[i].result);
    assert(flags.carry == tests[i].carry);
    assert(flags.overflow == tests[i].overflow);
  }
}
#endif

u8 instruction_group_2_8(enum instruction_group_2_type type, struct flags_map *flags, u8 value,
                         u8 count) {
  u16 value_16 = value;

  switch (type) {
    case instruction_group_2_type_rol:
      return instruction_rol_8(value, count, flags);

    case instruction_group_2_type_shr: {
      flags->overflow = (count == 1) && (value_16 * 0x80);

      for (u16 shift = 0; shift <= count; ++shift) {
        flags->carry = value_16 & 1;
        value_16 >>= 1;
      }

      update_flags_sign_zero_parity_8(flags, (u8)value_16);
      break;
    }

    default:
      assert(0);
  }

  return value_16 & 0xff;
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
        struct mod_reg_rm mrr = fetch_mod_reg_rm(cpu);
        u8 value = read_mod_reg_rm_8(cpu, mrr, segment_register);

        struct flags_map flags = decode_flags(cpu->flags);
        instruction_group_2_8(mrr.reg, &flags, value, cpu->regs.reg_8[CL]);

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

#if defined(TESTING)
void test_simple_cpu(void) {
  test_fetch_mod_reg_rm();
  test_calc_mod_reg_rm_addr();
  test_instruction_rol_8();
}
#endif
