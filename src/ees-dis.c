#include "decoder/decoder.h"
#include "disassembler.h"
#include "memory_reader.h"
#include "platform.h"

#include <malloc.h>
#include <stdio.h>

struct binary_data {
  u8 *data;
  unsigned data_size;
};

struct binary_data read_file(const char *filename) {
  FILE *file = fopen(filename, "rb");
  fseek(file, 0, SEEK_END);
  unsigned file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  u8 *buffer = malloc(file_size);

  size_t bytes_read = fread(buffer, 1, file_size, file);
  fclose(file);

  struct binary_data result = {
      buffer,
      bytes_read,
  };

  return result;
}

u8 binary_data_fetch(void *ctx, u32 pos) {
  struct binary_data *data = ctx;
  return data->data[pos];
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "USAGE: %s <binary file>", argv[0]);
    return 1;
  }

  const char *filename = argv[1];

  struct binary_data data = read_file(filename);

  struct memory_reader reader;
  memory_reader_init(&reader, &data, binary_data_fetch);
  // memory_reader_set_position(&reader, data.data_size - 0x10);

  while (reader.position < data.data_size) {
    struct instruction instruction;
    instruction_init(&instruction);
    unsigned pos = reader.position;
    decode_instruction(&reader, &instruction);
    disassemble(&instruction, segment_offset(0, pos));
  }

  return 0;
}