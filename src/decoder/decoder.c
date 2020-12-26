#include "decoder.h"

#include "../address.h"
#include "../mod_rm.h"
#include "../print_format.h"
#include "op_code_table.h"

#include <assert.h>
#include <stdio.h>

static void dump_buffer(const u8 *buffer, unsigned buffer_size) {
  for (unsigned i = 0; i < buffer_size; ++i) {
    printf(HEX_8 " ", buffer[i]);
  }
  printf("\n");
}

struct instruction_reader_writer {
  struct input_stream *original;
  struct output_stream instruction_stream;
};

static struct op_code_mapping *instruction_group_mapping(u8 op_code, struct input_stream *stream) {
  switch (op_code) {
    case 0xf7: {
      u8 mod_rm_byte = input_stream_peek_u8(stream);
      printf(HEX_8 "\n", input_stream_peek_u8(stream));
      printf(HEX_8 "\n", input_stream_peek_u8(stream));
      printf(HEX_8 "\n", input_stream_peek_u8(stream));
      struct mod_rm mrm = decode_mod_rm(mod_rm_byte);
      return &op_code_table_group_f7[mrm.reg];
    }

    default:
      assert(0);
      break;
  }

  return 0;
}

u8 instruction_reader_writer_fetch_u8(void *context, u32 position) {
  struct instruction_reader_writer *irw = context;

  u8 value = irw->original->fetch_func(irw->original->context, position);
  output_stream_store_u8(&irw->instruction_stream, value);
  irw->original->position++;

  return value;
}

struct decode_context {
  struct input_stream *in;
  struct output_stream out;
};

void decode_instruction(struct input_stream *reader, struct instruction *instruction) {
#if 0
  struct input_stream backup = *reader;
  for (int i = 0; i < 6; ++i) {
    printf(HEX_8 " ", input_stream_fetch_u8(&backup));
  }
  printf("\n");
#endif // 0

#if !defined(NDEBUG)
  struct input_stream wrapper;
  struct instruction_reader_writer irw;
  irw.original = reader;
  output_stream_init(&irw.instruction_stream, &instruction->buffer,
                     output_stream_flat_memory_store);

  input_stream_init(&wrapper, &irw, instruction_reader_writer_fetch_u8);

  reader = &wrapper;
#endif

  u8 op_code = input_stream_fetch_u8(reader);

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

    op_code = input_stream_fetch_u8(reader);
  }

  struct op_code_mapping *mapping = &op_code_table[op_code];
  if (mapping->instruction_type == it_group) {
    mapping = instruction_group_mapping(op_code, reader);
  }

  assert(mapping);

  instruction->type = mapping->instruction_type;

  assert(mapping->decode_func);

  mapping->decode_func(reader, instruction);

#if !defined(NDEBUG)
  instruction->instruction_size = irw.instruction_stream.position;
#endif
}
