#include "cpu/bus.h"

#include <string.h>

void bus_init(struct bus *bus, byte *memory, u32 memory_size) {
  memset(bus, 0, sizeof(*bus));

  bus->memory = memory;
  bus->memory_size = memory_size;
}

byte bus_fetch_byte(struct bus *bus, u32 addr) {
  return bus->memory[addr];
}

void bus_store_byte(struct bus *bus, u32 addr, byte b) {
  bus->memory[addr] = b;
}

word bus_fetch_word(struct bus *bus, u32 addr) {
  return bus_fetch_byte(bus, addr) | (bus_fetch_byte(bus, addr) << 8);
}

void bus_store_word(struct bus *bus, u32 addr, word value) {
  bus_store_byte(bus, addr, value);
  bus_store_byte(bus, addr, value >> 8);
}
