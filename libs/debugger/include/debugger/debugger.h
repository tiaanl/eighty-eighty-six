#ifndef DEBUGGER_DEBUGGER_H_
#define DEBUGGER_DEBUGGER_H_

#include <base/platform.h>

#include <time.h>

struct bus;
struct cpu;

struct debugger {
  void *window;
  u32 texture;
  u32 vao;
  u32 program;

  struct {
    byte red;
    byte green;
    byte blue;
  } image[0x100000];
  byte fetch_time[0x100000];
  byte store_time[0x100000];

  struct timespec start_time;
};

int debugger_init(struct debugger *debugger);
void debugger_destroy(struct debugger *debugger);

void debugger_map_bus_listener(struct debugger *debugger, struct bus *bus);

int debugger_pre_step(struct debugger *debugger, struct cpu *cpu);
int debugger_post_step(struct debugger *debugger, struct cpu *cpu);

#endif // DEBUGGER_DEBUGGER_H_
