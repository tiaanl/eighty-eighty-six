#include "memory_reader.h"

void memory_reader_init(struct memory_reader *memory_reader, void *context, fetch_func fetch_func) {
  memory_reader->context = context;
  memory_reader->fetch_func = fetch_func;
  memory_reader->position = 0;
}

void memory_reader_set_position(struct memory_reader *memory_reader, u32 position) {
  memory_reader->position = position;
}

u8 memory_reader_fetch_u8(struct memory_reader *memory_reader) {
  return memory_reader->fetch_func(memory_reader->context, memory_reader->position++);
}

u16 memory_reader_fetch_u16(struct memory_reader *memory_reader) {
  return memory_reader_fetch_u8(memory_reader) + (memory_reader_fetch_u8(memory_reader) << 8);
}

i8 memory_reader_fetch_i8(struct memory_reader *memory_reader) {
  return (i8)memory_reader_fetch_u8(memory_reader);
}

i16 memory_reader_fetch_i16(struct memory_reader *memory_reader) {
  return (i16)memory_reader_fetch_u8(memory_reader);
}
