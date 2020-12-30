#ifndef CPU_CPU_H_
#define CPU_CPU_H_

#include "cpu/bus.h"

#include <base/address.h>
#include <base/platform.h>
#include <cpu/flags.h>
#include <decoder/registers.h>

union regs {
  word word[register_16_count];
  byte byte[register_8_count];
};

struct cpu {
  struct bus *bus;

  union regs regs;
  word segment_16[segment_register_count];
  word ip;
  union flags flags;
};

void cpu_init(struct cpu *cpu, struct bus *bus, struct address reset_vector);
void cpu_step(struct cpu *cpu);

#endif // CPU_CPU_H_
