#include <base/platform.h>
#include <base/streams.h>
#include <decoder/decoder.h>
#include <disassembler/disassembler.h>
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

struct executable_header_mz {
  u16 id;
  u16 extra_bytes;
  u16 pages;
  u16 relocation_items;
  u16 header_size;
  u16 minimum_allocation;
  u16 maximum_allocation;
  u16 initial_ss;
  u16 initial_sp;
  u16 checksum;
  u16 initial_ip;
  u16 initial_cs;
  u16 relocation_table;
  u16 overlay;
  u16 overlay_information;
};

static void parse_exe_mz(const u8 *data, size_t data_size) {
  struct executable_header_mz *header = (struct executable_header_mz *)data;

  u32 header_size = header->header_size * 16;
  u32 extra_start = header->pages * 512 - (512 - header->extra_bytes);
  u32 code_start = header_size;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "USAGE: %s <binary file>", argv[0]);
    return 1;
  }

  const char *filename = argv[1];

  struct binary_data data = read_file(filename);

  struct input_stream in_stream;
  input_stream_init(&in_stream, &data, binary_data_fetch);
  // input_stream_set_position(&reader, data.data_size - 0x10);

  if (*(u16 *)data.data == 0x5a4d) {
    struct executable_header_mz *header = (struct executable_header_mz *)data.data;

    u32 header_size = header->header_size * 16;
    u32 code_start = header_size;
    // u32 extra_start = header->pages * 512 - (512 - header->extra_bytes);

    in_stream.position = code_start;
  }

  static char buffer[128];
  static size_t buffer_size = sizeof(buffer);

  while (in_stream.position < data.data_size) {
    struct instruction instruction;
    instruction_init(&instruction);
    unsigned pos = in_stream.position;
    decode_instruction(&in_stream, &instruction);
    disassemble(buffer, buffer_size, &instruction, segment_offset(0, pos));
    printf("%s\n", buffer);
  }

  return 0;
}
