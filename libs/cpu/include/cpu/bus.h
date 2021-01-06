#ifndef CPU_BUS_H_
#define CPU_BUS_H_

#include <base/platform.h>

typedef void (*bus_listener_func)(u32 addr, u8 value, void *context);

struct bus_listener {
  void *context;
  bus_listener_func fetch_func;
  bus_listener_func store_func;
};

struct bus {
  byte *memory;
  u32 memory_size;

  struct bus_listener listeners[8];
  unsigned bus_listener_count;
};

void bus_init(struct bus *bus, byte *memory, u32 memory_size);

void bus_add_listener(struct bus *bus, void *context, bus_listener_func fetch_func,
                      bus_listener_func store_func);

byte bus_fetch_byte(struct bus *bus, u32 addr);
void bus_store_byte(struct bus *bus, u32 addr, byte value);
word bus_fetch_word(struct bus *bus, u32 addr);
void bus_store_word(struct bus *bus, u32 addr, word b);

#endif // CPU_BUS_H_
