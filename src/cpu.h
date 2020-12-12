#ifndef CPU_H_
#define CPU_H_

#include "instruction.h"
#include "platform.h"
#include "registers.h"

struct bus;

enum flags {
  FLAG_CF = 0x0001, // Carry
  FLAG_PF = 0x0004, // Parity
  FLAG_AF = 0x0010, // Adjust
  FLAG_ZF = 0x0040, // Zero
  FLAG_SF = 0x0080, // Sign
  FLAG_TF = 0x0100, // Trap
  FLAG_IF = 0x0200, // Interrupt enable
  FLAG_DF = 0x0400, // Direction
  FLAG_OF = 0x0800, // Overflow
};

struct cpu {
  // AX, BX, CX, DX, SI, DI, BP, SP, IP, FLAGS, CS, DS, SS, ES
  u16 registers[14];
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

u8 cpu_flag_is_set(struct cpu *cpu, enum flags flag);
void cpu_set_flag(struct cpu *cpu, enum flags flag, u8 value);

#endif // CPU_H_
