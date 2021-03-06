#include <base/platform.h>
#include <base/reader.h>
#include <decoder/decoder.h>
#include <disassembler/disassembler.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

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

void print_usage(const char *app_name) {
  fprintf(stderr, "USAGE: %s <binary file>", app_name);
}

struct options {
  char *filename;
  u32 offset;
};

int parse_options(struct options *options, int argc, char **argv) {
  int opt;
  while ((opt = getopt(argc, argv, "o:")) != -1) {
    if (opt == 'o') {
      char *end;
      options->offset = strtol(optarg, &end, 10);
      if (end == optarg) {
        print_usage(argv[0]);
        return 1;
      }
    }
  }

  if (optind >= argc) {
    print_usage(argv[0]);
    return 1;
  }

  options->filename = argv[optind];

  return 0;
}

int main(int argc, char *argv[]) {
  struct options options = {
      .filename = 0,
      .offset = 0,
  };
  int result = parse_options(&options, argc, argv);
  if (result != 0) {
    return result;
  }

  struct binary_data data = read_file(options.filename);

  struct reader reader;
  reader_init(&reader, &data, binary_data_fetch);

  /* DOS MZ executable format. */
  if (*(u16 *)data.data == 0x5a4d) {
    struct executable_header_mz *header = (struct executable_header_mz *)data.data;

    u32 header_size = header->header_size * 16;
    u32 code_start = header_size;
    // u32 extra_start = header->pages * 512 - (512 - header->extra_bytes);

    options.offset += code_start;
  }

  /* BIOS file */
  if (data.data[data.data_size - 0x10] == 0xea) {
    word offset = data.data[data.data_size - 0x0f] + (data.data[data.data_size - 0x0e] << 8);
    word segment = data.data[data.data_size - 0x0d] + (data.data[data.data_size - 0x0c] << 8);
    u32 flat = segment << 4 | offset;
    u32 offset_in_memory = flat - (0x100000 - data.data_size);
    options.offset = offset_in_memory;
    printf("Detected BIOS file, starting at: 0x%05x\n", offset_in_memory);
  }

  static char buffer[128];
  static size_t buffer_size = sizeof(buffer);

  while (options.offset < data.data_size) {
    struct instruction instruction;
    instruction_init(&instruction);
    decode_instruction(&reader, options.offset, &instruction);
    disassemble(buffer, buffer_size, &instruction, options.offset);
    options.offset += instruction.instruction_size;
    printf("%s\n", buffer);
  }

  return 0;
}
