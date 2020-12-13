#include "address.h"
#include "bus.h"
#include "cpu.h"

#include <malloc.h>
#include <string.h>

extern unsigned char small[];
extern unsigned int small_len;

#include "../tests/xwing.com.c"

u8 mem_fetch(void *obj, struct address address) {
  u8 *memory = obj;

  return memory[flat_address(address)];
}

void mem_store(void *obj, struct address address, u8 value) {
  u8 *memory = obj;

  memory[flat_address(address)] = value;
}

int main(void) {
  struct bus bus;
  bus_init(&bus);

  u8 *memory = malloc(0xffff);

  // memcpy(memory, small, small_len);
  memcpy(memory, XWING_COM, XWING_COM_len);

  bus_add_mapping(&bus, 0x0000, 0xffff, memory, mem_fetch, mem_store);

  struct cpu cpu;
  cpu_init(&cpu, &bus);
  cpu_run(&cpu);

  bus_destroy(&bus);

  free(memory);

  return 0;
}
