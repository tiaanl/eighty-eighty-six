#include "cpu/ports.h"

#include <string.h>

void ports_init(struct ports *ports) {
  memset(ports, 0, sizeof(*ports));
}

void ports_map_address(struct ports *ports, word start, word end, void *context,
                       port_out_func out_func, port_in_func in_func) {
  for (word i = start; i <= end; ++i) {
    ports->map[i].context = context;
    ports->map[i].in_func = in_func;
    ports->map[i].out_func = out_func;
  }
}

void ports_out(struct ports *ports, word address, byte value) {
  void *context = ports->map[address].context;
  if (context) {
    ports->map[address].out_func(context, address, value);
  }

  ports->mem[address] = value;
}

byte ports_in(struct ports *ports, word address) {
  void *context = ports->map[address].context;
  if (context) {
    return ports->map[address].in_func(context, address);
  }

  return ports->mem[address];
}
