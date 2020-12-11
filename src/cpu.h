#ifndef CPU_H_
#define CPU_H_

#include "platform.h"
#include "instruction.h"

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

struct cpu {
  struct registers registers;
  const u8 *code;
  unsigned int code_len;
};

void cpu_init(struct cpu *cpu, const u8 *code, unsigned int code_len);
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
unsigned cpu_prefetch(struct cpu* cpu, u8* buffer, unsigned size);

#endif // CPU_H_
