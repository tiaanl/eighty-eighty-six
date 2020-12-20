#ifndef TESTING_H_
#define TESTING_H_

#include "cpu.h"

#define EXPECT_OP_BASE(Expected, Actual, Op, Format)                                               \
  do {                                                                                             \
    if (!((Expected)Op(Actual))) {                                                                 \
      printf("%s:%d\n - ", __FILE__, __LINE__);                                                    \
      printf("Expected: " Format ", Actual: " Format "\n", Expected, Actual);                      \
    }                                                                                              \
  } while (0)

#define EXPECT_U8_EQ(Expected, Actual) EXPECT_OP_BASE(Expected, Actual, ==, HEX_8)

#define EXPECT_U16_EQ(Expected, Actual) EXPECT_OP_BASE(Expected, Actual, ==, HEX_16)
#define EXPECT_I16_EQ(Expected, Actual) EXPECT_OP_BASE(Expected, Actual, ==, "%d")

void testing_dummy_cpu_init(struct cpu *cpu, struct bus *bus, u8 *buffer, unsigned buffer_size);
void testing_dummy_cpu_destroy(struct bus *bus);

#endif // TESTING_H_
