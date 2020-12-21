void decoder_tests(void);
void mod_rm_tests(void);

#include "bus.h"
#include "cpu.h"

static u8 testing_fetch_memory(const u8 *memory, struct address address) {
  return memory[flatten_address(address)];
}

static void testing_store_memory(u8 *memory, struct address address, u8 value) {
  memory[flatten_address(address)] = value;
}

void testing_dummy_cpu_init(struct cpu *cpu, struct bus *bus, u8 *buffer, unsigned buffer_size) {
  bus_init(bus);
  bus_add_mapping(bus, segment_offset(0, 0), buffer_size, buffer,
                  (bus_fetch_func)testing_fetch_memory, (bus_store_func)testing_store_memory);

  cpu_init(cpu, bus);
}

void testing_dummy_cpu_destroy(struct bus *bus) {
  bus_destroy(bus);
}

int main(void) {
  // decoder_tests();
  mod_rm_tests();

  // test_cpu();
  return 0;
}
