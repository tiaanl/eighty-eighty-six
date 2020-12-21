#ifndef TESTING_H_
#define TESTING_H_

#include "cpu.h"
#include "print_format.h"

#include <stdio.h>

#define EXPECT_OP_BASE(Actual, Expected, Op, Format)                                               \
  do {                                                                                             \
    if (!((Expected)Op(Actual))) {                                                                 \
      printf("%s:%d\n", __FILE__, __LINE__);                                                       \
      printf("FAIL: %s (" Format ") %s %s (" Format ")\n", #Actual, Actual, #Op, #Expected,        \
             Expected);                                                                            \
    }                                                                                              \
  } while (0)

#define EXPECT_U8_EQ(Actual, Expected) EXPECT_OP_BASE(Actual, Expected, ==, HEX_8)

#define EXPECT_U16_EQ(Actual, Expected) EXPECT_OP_BASE(Actual, Expected, ==, HEX_16)
#define EXPECT_I16_EQ(Actual, Expected) EXPECT_OP_BASE(Actual, Expected, ==, "%d")

void testing_dummy_cpu_init(struct cpu *cpu, struct bus *bus, u8 *buffer, unsigned buffer_size);
void testing_dummy_cpu_destroy(struct bus *bus);

#endif // TESTING_H_
