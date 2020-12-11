#include "bus.h"
#include "cpu.h"

#include <malloc.h>
#include <string.h>

extern unsigned char small[];
extern unsigned int small_len;

u8 mem_fetch(void *obj, u16 addr) {
  u8 *memory = obj;

  return memory[addr];
}

void mem_store(void *obj, u16 addr, u8 value) {
  u8 *memory = obj;

  memory[addr] = value;
}

int main(void) {
  struct bus bus;
  bus_init(&bus);

  u8 *memory = malloc(0xfff);

  memcpy(memory, small, small_len);

  bus_add_mapping(&bus, 0x0000, 0xffff, memory, mem_fetch, mem_store);

  struct cpu cpu;
  cpu_init(&cpu, &bus);
  cpu_run(&cpu);

  bus_destroy(&bus);

  free(memory);

  return 0;
}
