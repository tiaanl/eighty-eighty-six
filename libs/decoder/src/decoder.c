#include "decoder/decoder.h"

#include "decoder_context.h"
#include "op_code_table.h"

#include <assert.h>

static struct op_code_mapping *handle_group_mapping(struct op_code_mapping *mapping,
                                                    struct reader *reader, u32 position) {
  assert(!mapping->tmp1);
  assert(!mapping->tmp2);
  assert(!mapping->tmp3);
  assert(mapping->group_table);
  u8 mod_rm_byte = reader_fetch_u8(reader, position + 1);
  byte group_index = MOD_RM_REG(mod_rm_byte);
  mapping = &mapping->group_table[group_index];
  return mapping;
}

struct op_code_mapping *handle_prefix_mapping(struct op_code_mapping *mapping, u8 op_code,
                                              struct decoder_context *decoder_context,
                                              struct instruction *instruction) {
  mapping->decode_func(decoder_context, instruction);
  op_code = decoder_fetch_u8(decoder_context);
  return &op_code_table[op_code];
}

int decode_instruction(struct reader *reader, u32 position, struct instruction *instruction) {
  struct decoder_context decoder_context;
  decoder_context_init(&decoder_context, reader, position, DS);

  instruction_init(instruction);

  u8 op_code = decoder_fetch_u8(&decoder_context);

  struct op_code_mapping *mapping = &op_code_table[op_code];

  // If the mapping is a prefix, then we run the decoder function for it and fetch the next byte
  // as the op_code and get a new mapping.
  if (mapping->op_code_type == oct_prefix && mapping->decode_func) {
    mapping = handle_prefix_mapping(mapping, op_code, &decoder_context, instruction);
  }

  // If the mapping is a group, then we replace the mapping with one in the group table.
  if (mapping->op_code_type == oct_group) {
    mapping = handle_group_mapping(mapping, reader, position);
  }

  // At this point we MUST have a valid mapping.
  assert(mapping);

  instruction->type = mapping->instruction_type;

  if (mapping->decode_func) {
    mapping->decode_func(&decoder_context, instruction);
  }

  u8 instruction_size = decoder_context.position - position;
  for (int i = 0; i < instruction_size; ++i) {
    instruction->buffer[i] = reader_fetch_u8(reader, position + i);
  }
  instruction->instruction_size = instruction_size;

  return instruction_size;
}
