#include "cpu/bus.h"

#include <string.h>

void bus_init(struct bus *bus, byte *memory, u32 memory_size) {
  memset(bus, 0, sizeof(*bus));

  bus->memory = memory;
  bus->memory_size = memory_size;
}

void bus_add_listener(struct bus *bus, void *context, bus_listener_func fetch_func,
                      bus_listener_func store_func) {
  struct bus_listener *listener = &bus->listeners[bus->bus_listener_count++];

  listener->context = context;
  listener->fetch_func = fetch_func;
  listener->store_func = store_func;
}

byte bus_fetch_byte(struct bus *bus, u32 addr) {
  byte value = bus->memory[addr];

  for (unsigned i = 0; i < bus->bus_listener_count; ++i) {
    bus->listeners[i].fetch_func(addr, value, bus->listeners[i].context);
  }

  return value;
}

void bus_store_byte(struct bus *bus, u32 addr, byte value) {
  bus->memory[addr] = value;

  for (unsigned i = 0; i < bus->bus_listener_count; ++i) {
    bus->listeners[i].store_func(addr, value, bus->listeners[i].context);
  }
}

word bus_fetch_word(struct bus *bus, u32 addr) {
  return bus_fetch_byte(bus, addr) | (bus_fetch_byte(bus, addr) << 8);
}

void bus_store_word(struct bus *bus, u32 addr, word value) {
  bus_store_byte(bus, addr, value);
  bus_store_byte(bus, addr, value >> 8);
}
