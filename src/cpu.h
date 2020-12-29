#ifndef CPU_H_
#define CPU_H_

#include "instruction.h"
#include "platform.h"

#define SIMPLE_CPU

struct bus;

enum flags {
  f_carry = 0x0001,     // Carry
  f_parity = 0x0004,    // Parity
  f_adjust = 0x0010,    // Adjust
  f_zero = 0x0040,      // Zero
  f_sign = 0x0080,      // Sign
  f_trap = 0x0100,      // Trap
  f_interrupt = 0x0200, // Interrupt enable
  f_direction = 0x0400, // Direction
  f_overflow = 0x0800,  // Overflow
};

union cpu_regs {
  u16 reg_16[REGISTER_16_COUNT];
  u8 reg_8[REGISTER_8_COUNT];
};

struct cpu {
  union cpu_regs regs;
  u16 ip;
  u16 flags;
  u16 segments[SEGMENT_REGISTER_COUNT];

  struct bus *bus;
};

void cpu_init(struct cpu *cpu, struct bus *bus);
void cpu_run(struct cpu *cpu);

// Try to fetch the requested amount of bytes from the CPU and return the actual amount
// fetched.
unsigned cpu_prefetch(struct cpu *cpu, u8 *buffer, unsigned size);

u8 cpu_get_register_8(struct cpu *cpu, enum register_8 reg);
void cpu_set_register_8(struct cpu *cpu, enum register_8 reg, u8 value);
u16 cpu_get_register_16(struct cpu *cpu, enum register_16 reg);
void cpu_set_register_16(struct cpu *cpu, enum register_16 reg, u16 value);
u16 cpu_get_segment(struct cpu *cpu, enum segment_register reg);

static inline u8 cpu_flag_is_set(struct cpu *cpu, enum flags flag) {
  return (cpu->flags & flag) == flag;
}

static inline void cpu_clear_flag(struct cpu *cpu, enum flags flag) {
  cpu->flags &= ~flag;
}

static inline void cpu_set_flag(struct cpu *cpu, enum flags flag) {
  cpu->flags |= flag;
}

static inline void cpu_set_flag_value(struct cpu *cpu, enum flags flag, int value) {
  if (value) {
    cpu_set_flag(cpu, flag);
  } else {
    cpu_clear_flag(cpu, flag);
  }
}

void print_registers(struct cpu *cpu);

#if defined(TESTING)
void test_cpu(void);
#endif

#endif // CPU_H_
