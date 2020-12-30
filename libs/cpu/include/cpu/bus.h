#ifndef CPU_BUS_H_
#define CPU_BUS_H_

#include <base/platform.h>

struct bus {
  byte *memory;
  u32 memory_size;
};

void bus_init(struct bus *bus, byte *memory, u32 memory_size);
byte bus_fetch_byte(struct bus *bus, u32 addr);
void bus_store_byte(struct bus *bus, u32 addr, byte b);

#endif // CPU_BUS_H_
