#ifndef BUS_H_
#define BUS_H_

#include "address.h"
#include "platform.h"

typedef u8 (*bus_fetch_func)(void *, struct address);
typedef void (*bus_store_func)(void *, struct address, u8);

struct bus_mapping_node {
  u32 begin;
  u32 end;

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

void bus_add_mapping(struct bus *bus, struct address begin, u32 size, void *obj,
                     bus_fetch_func fetch_func, bus_store_func store_func);

u8 bus_fetch(struct bus *bus, struct address address);
void bus_store(struct bus *bus, struct address address, u8 value);

#endif // BUS_H_
