#include "cpu.h"

extern unsigned char small[];
extern unsigned int small_len;

int main(void) {
  struct cpu cpu;
  cpu_init(&cpu, small, small_len);
  cpu_run(&cpu);

  return 0;
}
