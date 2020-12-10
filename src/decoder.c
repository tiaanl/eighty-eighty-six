#include "decoder.h"

#include "instructions.h"

int decode_instruction(const u8 *buffer, struct instruction *instruction) {
  u8 op_code = buffer[0];

  struct op_code_mapping *mapping = &op_code_table[op_code];

  instruction->type = mapping->instruction_type;

  return 0;
}
