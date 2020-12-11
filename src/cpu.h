#ifndef CPU_H_
#define CPU_H_

#include "instruction.h"
#include "platform.h"

struct bus;

struct registers {
  union {
    u16 all[14];
    struct {
      u16 ax;
      u16 bx;
      u16 cx;
      u16 dx;

      u16 ip;
      u16 si;
      u16 di;
      u16 bp;
      u16 sp;
      u16 flags;

      u16 cs;
      u16 ds;
      u16 ss;
      u16 es;
    };
  };
};

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
  struct registers registers;
  struct bus *bus;
};

void cpu_init(struct cpu *cpu, struct bus *bus);
void cpu_run(struct cpu *cpu);

u8 cpu_peek_u8(struct cpu *cpu, int offset);
i8 cpu_peek_i8(struct cpu *cpu, int offset);
u16 cpu_peek_u16(struct cpu *cpu, int offset);
i16 cpu_peek_i16(struct cpu *cpu, int offset);
u8 cpu_fetch_u8(struct cpu *cpu);
i8 cpu_fetch_i8(struct cpu *cpu);
u16 cpu_advance_ip(struct cpu *cpu, u16 count);

// Try to fetch the requested amount of bytes from the CPU and return the actual amount
// fetched.
unsigned cpu_prefetch(struct cpu *cpu, u8 *buffer, unsigned size);

u8 cpu_flag_is_set(struct cpu *cpu, enum flags flag);
void cpu_set_flag(struct cpu *cpu, enum flags flag, u8 value);

#endif // CPU_H_
