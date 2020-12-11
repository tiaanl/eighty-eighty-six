#include "bus.h"

#include <assert.h>
#include <stdlib.h>

void bus_init(struct bus *bus) {
  bus->first_mapping = 0;
}

void bus_destroy(struct bus *bus) {
  // TODO
}

void bus_add_mapping(struct bus *bus, u16 begin, u16 end, void *obj, bus_fetch_func fetch_func,
                     bus_store_func store_func) {
  // Allocate a new node.
  struct bus_mapping_node *node = malloc(sizeof(struct bus_mapping_node));

  node->begin = begin;
  node->end = end;
  node->obj = obj;
  node->fetch = fetch_func;
  node->store = store_func;
  node->next = bus->first_mapping;

  bus->first_mapping = node;
}

static struct bus_mapping_node *get_node(struct bus *bus, u16 addr) {
  for (struct bus_mapping_node *current = bus->first_mapping; current; current = current->next) {
    if (addr >= current->begin && addr < current->end) {
      return current;
    }
  }

  return 0;
}

u8 bus_fetch(struct bus *bus, u16 addr) {
  struct bus_mapping_node *node = get_node(bus, addr);
  if (!node) {
    assert(0);
    return 0;
  }

  return node->fetch(node->obj, addr);
}

void bus_store(struct bus *bus, u16 addr, u8 value) {
  struct bus_mapping_node *node = get_node(bus, addr);
  if (!node) {
    assert(0);
    return;
  }

  node->store(node->obj, addr, value);
}
