#include "decoder/registers.h"

#include <assert.h>

const char *register_8_to_string(enum register_8 reg) {
  static const char *mapping[] = {
      "ah", "al", "ch", "cl", "dh", "dl", "bh", "bl",
  };

  assert(reg < register_8_count);
  return mapping[reg];
}

const char *register_16_to_string(enum register_16 reg) {
  static const char *mapping[] = {
      "ax", "cx", "dx", "bx", "sp", "bp", "si", "di",
  };

  assert(reg < register_16_count);
  return mapping[reg];
}

const char *segment_register_to_string(enum segment_register reg) {
  static const char *mapping[] = {
      "es",
      "cs",
      "ss",
      "ds",
  };

  assert(reg < segment_register_count);
  return mapping[reg];
}
