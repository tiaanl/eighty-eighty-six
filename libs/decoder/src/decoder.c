#include "decoder/decoder.h"

#include "decoder_context.h"
#include "op_code_table.h"

#include <assert.h>

void decode_instruction(struct input_stream *stream, u32 position,
                        struct instruction *instruction) {
  struct decoder_context decoder_context = {
      .stream = stream,
      .position = position,
      .segment_register_override = DS,
  };

  // Instruction defaults.
  memset(instruction, 0, sizeof(*instruction));

  u8 op_code = decoder_fetch_u8(&decoder_context);

  struct op_code_mapping *mapping = &op_code_table[op_code];

  // If the mapping is a prefix, then we run the decoder function for it and fetch the next byte
  // as the op_code and get a new mapping.
  if (mapping->op_code_type == oct_prefix && mapping->decode_func) {
    mapping->decode_func(&decoder_context, instruction);
    op_code = decoder_fetch_u8(&decoder_context);
    mapping = &op_code_table[op_code];
  }

  // If the mapping is a group, then we replace the mapping with one in the group table.
  if (mapping->op_code_type == oct_group) {
    u8 index = op_code >> 3 & 0x07;
    mapping = &mapping->group_table[index];
  }

  assert(mapping);

  instruction->type = mapping->instruction_type;

  if (mapping->decode_func) {
    mapping->decode_func(&decoder_context, instruction);
  }

  u8 instruction_size = decoder_context.position - position;
  for (int i = 0; i < instruction_size; ++i) {
    instruction->buffer[i] = input_stream_fetch_u8(stream, position + i);
  }
  instruction->instruction_size = instruction_size;
}
