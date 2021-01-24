#include "base/reader.h"

void reader_init(struct reader *reader, void *context, reader_func reader_func) {
  reader->context = context;
  reader->reader_func = reader_func;
}

u8 reader_fetch_u8(struct reader *reader, u32 position) {
  return reader->reader_func(reader->context, position);
}

u16 reader_fetch_u16(struct reader *reader, u32 position) {
  return reader_fetch_u8(reader, position) + (reader_fetch_u8(reader, position + 1) << 8);
}

i8 reader_fetch_i8(struct reader *reader, u32 position) {
  return (i8)reader_fetch_u8(reader, position);
}

i16 reader_fetch_i16(struct reader *reader, u32 position) {
  return (i16)reader_fetch_u16(reader, position);
}
