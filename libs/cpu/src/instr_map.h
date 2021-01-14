#ifndef CPU_INSTR_MAP_H_
#define CPU_INSTR_MAP_H_

#include "cpu/cpu.h"

#include <instructions/instructions.h>

typedef void (*exec_func)(struct cpu *cpu, struct instruction *instruction);

struct instr_mapping {
  enum instruction_type instruction_type;
  exec_func exec_func;
};

extern struct instr_mapping instr_map[];

#endif // CPU_INSTR_MAP_H_
