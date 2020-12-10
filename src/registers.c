#include "registers.h"

#include <assert.h>

const char *register_8_string_map[] = {
    "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh",
};

const char *register_16_string_map[] = {
    "ax", "bx", "cx", "dx", "si", "di", "bp", "sp", "flags",
};

const char *register_8_to_string(enum register_8 reg) {
  assert(reg <= REGISTER_8_COUNT);
  return register_8_string_map[reg];
}

const char *register_16_to_string(enum register_16 reg) {
  assert(reg <= REGISTER_16_COUNT);
  return register_16_string_map[reg];
}
