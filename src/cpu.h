#ifndef CPU_H_
#define CPU_H_

#include "instructions.h"
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

struct mod_rm {
  u8 mod;
  u8 reg;
  u8 rm;
};

struct mod_rm mod_rm(u8 b);

void cpu_init(struct cpu *cpu, const u8 *code, unsigned int code_len);
void cpu_run(struct cpu *cpu);

void cpu_instruction_add_mem8_reg8(struct cpu *cpu);
void cpu_instruction_add_mem16_reg16(struct cpu *cpu);
void cpu_instruction_add_reg8_mem8(struct cpu *cpu);
void cpu_instruction_add_reg16_mem16(struct cpu *cpu);
void cpu_instruction_add_al_imm8(struct cpu *cpu);
void cpu_instruction_add_ax_imm16(struct cpu *cpu);

void cpu_instruction_mov_memx_regx(struct cpu *cpu);

void cpu_instruction_mov_regx_immx(struct cpu *cpu);

#endif // CPU_H_
