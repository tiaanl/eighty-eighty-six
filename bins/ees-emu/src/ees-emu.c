#include <base/address.h>
#include <cpu/bus.h>
#include <cpu/cpu.h>
#include <malloc.h>

// 1Mb of memory
#define DEFAULT_MEMORY_SIZE 0x100000

int main() {
  static struct address reset_vector = {
      .segment = 0xF000,
      .offset = 0xFFF0,
  };

  byte *memory = calloc(DEFAULT_MEMORY_SIZE, 1);

  struct bus bus;
  bus_init(&bus, memory, DEFAULT_MEMORY_SIZE);

  struct cpu cpu;
  cpu_init(&cpu, &bus, reset_vector);

  // Write into memory a jump to address 0000:0100.
  u32 flat = flatten_address(reset_vector);
  memory[flat] = 0xea;
  memory[flat + 2] = 0x01;

  // Load the test program into memory at 0000:0100.
  FILE *handle = fopen("/home/tilo/Code/life-16/life.com", "rb");
  fseek(handle, 0, SEEK_END);
  long file_size = ftell(handle);
  fseek(handle, 0, SEEK_SET);
  fread(memory + 0x0100, file_size, 1, handle);
  fclose(handle);

  while (1) {
    cpu_step(&cpu);
  }

  free(memory);

  return 0;
}
