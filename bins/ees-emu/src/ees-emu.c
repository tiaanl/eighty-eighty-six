#include <base/address.h>
#include <cpu/bus.h>
#include <cpu/cpu.h>
#include <cpu/ports.h>
#include <debugger/debugger.h>
#include <malloc.h>
#include <pthread.h>

// 1Mb of memory
#define DEFAULT_MEMORY_SIZE 0x100000

void *cpu_loop(void *cpu) {
  while (1) {
    cpu_step(cpu);
  }

  return 0;
}

int main() {
  static struct address reset_vector = {
      .segment = 0xf000,
      .offset = 0xfff0,
  };

  byte *memory = calloc(DEFAULT_MEMORY_SIZE, 1);

  struct bus bus;
  bus_init(&bus, memory, DEFAULT_MEMORY_SIZE);

  struct ports *ports = malloc(sizeof(struct ports));
  ports_init(ports);

  struct cpu cpu;
  cpu_init(&cpu, ports, &bus, reset_vector);

  // const char *bios_file = "/home/tilo/Code/life-16/life.com";
  const char *bios_file = "/home/tilo/Code/Faux86/data/pcxtbios.bin";

  // Load the BIOS at the very end of memory.
  FILE *handle = fopen(bios_file, "rb");
  fseek(handle, 0, SEEK_END);
  long file_size = ftell(handle);
  fseek(handle, 0, SEEK_SET);
  fread(memory + DEFAULT_MEMORY_SIZE - file_size, file_size, 1, handle);
  fclose(handle);

  struct debugger *debugger = malloc(sizeof(*debugger));
  if (debugger_init(debugger) < 0) {
    return 1;
  }

  debugger_map_bus_listener(debugger, &bus);

  pthread_t cpu_thread;
  if (pthread_create(&cpu_thread, 0, cpu_loop, &cpu)) {
    return 1;
  }

  while (1) {
    if (!debugger_pre_step(debugger, &cpu)) {
      break;
    }
    // cpu_step(&cpu);
    if (!debugger_post_step(debugger, &cpu)) {
      break;
    }
  }

  debugger_destroy(debugger);
  free(debugger);

  free(memory);
  free(ports);

  return 0;
}
