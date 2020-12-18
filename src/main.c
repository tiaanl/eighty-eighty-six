#include "address.h"
#include "bus.h"
#include "cpu.h"

#include <malloc.h>

extern unsigned char small[];
extern unsigned int small_len;

#include "../tests/xwing.com.c"

// 1mb
#define DEFAULT_RAM_SIZE 0x100000

u8 mem_fetch(void *obj, struct address address) {
  u8 *memory = obj;

  return memory[flatten_address(address)];
}

void mem_store(void *obj, struct address address, u8 value) {
  u8 *memory = obj;

  memory[flatten_address(address)] = value;
}

int load_bios_into_memory(u8 *memory, unsigned memory_size, const char *file_name) {
  FILE *handle = fopen(file_name, "rb");
  if (!handle) {
    return -1;
  }

  fseek(handle, 0, SEEK_END);
  unsigned file_size = ftell(handle);
  fseek(handle, 0, SEEK_SET);
  fread(memory + memory_size - file_size, 1, file_size, handle);
  fclose(handle);

  return (int)file_size;
}

int main(void) {
  struct bus bus;
  bus_init(&bus);

  // HACK: Add 100 more bytes, because the cpu prefetches 16-byte buffer and the bios starts right
  //       at the end of memory, so as soon as the second instruction is decoded, we run over the
  //       buffer.
  u8 *memory = malloc(DEFAULT_RAM_SIZE + 100);

  // memcpy(memory, small, small_len);
  // memcpy(memory, XWING_COM, XWING_COM_len);

  if (load_bios_into_memory(memory, DEFAULT_RAM_SIZE,
                            "/home/tilo/Code/eighty-eighty-six/bins/"
                            "BIOS_5160_09MAY86_U18_59X7268_62X0890_27256_F800.BIN") < 0) {
    fprintf(stderr, "Could not load BIOS image.");
    return 1;
  }

  bus_add_mapping(&bus, segment_offset(0x0000, 0x0000), DEFAULT_RAM_SIZE + 100, memory, mem_fetch,
                  mem_store);

  struct cpu cpu;
  cpu_init(&cpu, &bus);
  cpu_run(&cpu);

  bus_destroy(&bus);

  free(memory);

  return 0;
}
