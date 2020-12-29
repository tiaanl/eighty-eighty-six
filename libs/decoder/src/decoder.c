#include "decoder/decoder.h"

#include <base/print_format.h>
#include "decoder/mod_rm.h"
#include "op_code_table.h"

#include <assert.h>
#include <stdio.h>

static void dump_buffer(const u8 *buffer, unsigned buffer_size) {
  for (unsigned i = 0; i < buffer_size; ++i) {
    printf(HEX_8 " ", buffer[i]);
  }
  printf("\n");
}

static struct op_code_mapping *instruction_group_mapping(u8 op_code, struct input_stream *stream) {
  switch (op_code) {
    case 0xf7: {
      u8 mod_rm_byte = input_stream_peek_u8(stream, 0);
      struct mod_rm mrm = decode_mod_rm(mod_rm_byte);
      return &op_code_table_group_f7[mrm.reg];
    }

    default:
      assert(0);
      break;
  }

  return 0;
}

void decode_instruction(struct input_stream *stream, struct instruction *instruction) {
  u32 start_position = stream->position;

  u8 op_code = input_stream_fetch_u8(stream);

  instruction->segment_register = DS;
  if (op_code == 0x2e || op_code == 0x26 || op_code == 0x3e || op_code == 0x36) {
    switch (op_code) {
      case 0x2e:
        instruction->segment_register = CS;
        break;

      case 0x26:
        instruction->segment_register = ES;
        break;

      case 0x3e:
        instruction->segment_register = DS;
        break;

      case 0x36:
        instruction->segment_register = SS;
        break;

      default:
        assert(0);
        break;
    }

    op_code = input_stream_fetch_u8(stream);
  }

  // FIXME: We skip the rep & repne prefixes for now
  if (op_code == 0xf2 || op_code == 0xf3) {
    op_code = input_stream_fetch_u8(stream);
  }

  struct op_code_mapping *mapping = &op_code_table[op_code];
  if (mapping->instruction_type == it_group) {
    mapping = instruction_group_mapping(op_code, stream);
  }

  assert(mapping);

  instruction->type = mapping->instruction_type;

  assert(mapping->decode_func);

  mapping->decode_func(stream, instruction);

  u8 instruction_size = stream->position - start_position;
  stream->position = start_position;
  for (int i = 0; i < instruction_size; ++i) {
    instruction->buffer[i] = input_stream_fetch_u8(stream);
  }
  instruction->instruction_size = instruction_size;
}
