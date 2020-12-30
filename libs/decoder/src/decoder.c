#include "decoder/decoder.h"

#include "op_code_table.h"

#include <assert.h>
#include <base/print_format.h>
#include <stdio.h>

void decode_instruction(struct input_stream *stream, struct instruction *instruction) {
  u32 start_position = stream->position;

  // Instruction defaults.
  memset(instruction, 0, sizeof(*instruction));
  instruction->segment_register = DS;
  instruction->rep_prefix = rp_none;

  u8 op_code = input_stream_fetch_u8(stream);
  struct op_code_mapping *mapping = &op_code_table[op_code];

  // If the mapping is a prefix, then we run the decoder function for it and fetch the next byte
  // as the op_code and get a new mapping.
  if (mapping->instruction_type == it_prefix && mapping->decode_func) {
    mapping->decode_func(stream, instruction);
    op_code = input_stream_fetch_u8(stream);
    mapping = &op_code_table[op_code];
  }

  // If the mapping is a group, then we replace the mapping with one in the group table.
  if (mapping->instruction_type == it_group) {
    u8 index = op_code >> 3 & 0x07;
    mapping = &mapping->group_table[index];
  }

  if (!mapping) {
    fprintf(stderr, "Mapping not found for op_code " HEX_8 "\n", op_code);
    assert(0);
    return;
  }

  assert(mapping);

  instruction->type = mapping->instruction_type;

  if (mapping->decode_func) {
    mapping->decode_func(stream, instruction);
  }

  u8 instruction_size = stream->position - start_position;
  stream->position = start_position;
  for (int i = 0; i < instruction_size; ++i) {
    instruction->buffer[i] = input_stream_fetch_u8(stream);
  }
  instruction->instruction_size = instruction_size;
}
