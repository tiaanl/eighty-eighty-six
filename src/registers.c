#include "registers.h"

#include <assert.h>

const char *register_8_string_map[] = {
    "al", "cl", "dl", "bl", // low bytes
    "ah", "ch", "dh", "bh", // hi bytes
};

const char *register_16_string_map[] = {
    "ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "ip", "flags",
};

const char *segment_register_string_map[] = {
    "es",
    "cs",
    "ss",
    "ds",
};

const char *register_8_to_string(enum register_8 reg) {
  assert(reg < REGISTER_8_COUNT);
  return register_8_string_map[reg];
}

const char *register_16_to_string(enum register_16 reg) {
  assert(reg < REGISTER_16_COUNT);
  return register_16_string_map[reg];
}

const char *segment_register_to_string(enum segment_register reg) {
  assert(reg < SEGMENT_REGISTER_COUNT);
  return segment_register_string_map[reg];
}
