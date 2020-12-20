#ifndef TESTING_H_
#define TESTING_H_

#include "cpu.h"

void testing_dummy_cpu_init(struct cpu *cpu, struct bus *bus, u8 *buffer, unsigned buffer_size);
void testing_dummy_cpu_destroy(struct bus *bus);

#endif // TESTING_H_
