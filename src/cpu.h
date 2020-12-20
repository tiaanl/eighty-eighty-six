#ifndef CPU_H_
#define CPU_H_

#include "instruction.h"
#include "platform.h"

#define SIMPLE_CPU

struct bus;

const char *register_8_to_string(enum register_8 reg);
const char *register_16_to_string(enum register_16 reg);
const char *segment_register_to_string(enum segment_register reg);

enum flags {
  fl_cf = 0x0001, // Carry
  fl_pf = 0x0004, // Parity
  fl_af = 0x0010, // Adjust
  fl_zf = 0x0040, // Zero
  fl_sf = 0x0080, // Sign
  fl_tf = 0x0100, // Trap
  fl_if = 0x0200, // Interrupt enable
  fl_df = 0x0400, // Direction
  fl_of = 0x0800, // Overflow
};

struct cpu {
  union {
    u16 reg_16[REGISTER_16_COUNT];
    u8 reg_8[REGISTER_8_COUNT];
  };
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

#endif // CPU_H_
