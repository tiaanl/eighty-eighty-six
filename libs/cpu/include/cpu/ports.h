#ifndef CPU_PORTS_H_
#define CPU_PORTS_H_

#include <base/platform.h>

#define PORT_COUNT 0x10000

typedef void (*port_out_func)(void *context, word address, byte value);
typedef byte (*port_in_func)(void *context, word address);

struct port {
  void *context;
  port_out_func out_func;
  port_in_func in_func;
};

struct ports {
  struct port map[PORT_COUNT];
  byte mem[PORT_COUNT];
};

void ports_init(struct ports *ports);
void ports_map_address(struct ports *ports, word start, word end, void *context,
                       port_out_func out_func, port_in_func in_func);

void ports_out(struct ports *ports, word address, byte value);
byte ports_in(struct ports *ports, word address);

#endif // CPU_PORTS_H_
