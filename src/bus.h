#ifndef BUS_H_
#define BUS_H_

#include "platform.h"

typedef u8 (*bus_fetch_func)(void *, u16 addr);
typedef void (*bus_store_func)(void *, u16 addr, u8);

struct bus_mapping_node {
  u16 begin;
  u16 end;

  void *obj;
  bus_fetch_func fetch;
  bus_store_func store;

  struct bus_mapping_node *next;
};

struct bus {
  struct bus_mapping_node *first_mapping;
};

void bus_init(struct bus *bus);
void bus_destroy(struct bus *bus);

void bus_add_mapping(struct bus *bus, u16 begin, u16 end, void *obj, bus_fetch_func fetch_func,
                     bus_store_func store_func);

u8 bus_fetch(struct bus *bus, u16 addr);
void bus_store(struct bus *bus, u16 addr, u8 value);

#endif // BUS_H_
