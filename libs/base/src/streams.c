#include "base/streams.h"

void input_stream_init(struct input_stream *stream, void *context, fetch_func fetch_func) {
  stream->context = context;
  stream->fetch_func = fetch_func;
}

u8 input_stream_fetch_u8(struct input_stream *stream, u32 position) {
  return stream->fetch_func(stream->context, position);
}

u16 input_stream_fetch_u16(struct input_stream *stream, u32 position) {
  return input_stream_fetch_u8(stream, position) +
         (input_stream_fetch_u8(stream, position + 1) << 8);
}

i8 input_stream_fetch_i8(struct input_stream *stream, u32 position) {
  return (i8)input_stream_fetch_u8(stream, position);
}

i16 input_stream_fetch_i16(struct input_stream *stream, u32 position) {
  return (i16)input_stream_fetch_u16(stream, position);
}

void output_stream_init(struct output_stream *stream, void *context, store_func store_func) {
  stream->context = context;
  stream->store_func = store_func;
  stream->position = 0;
}

void output_stream_store_u8(struct output_stream *stream, u8 value) {
  stream->store_func(stream->context, stream->position++, value);
}

void output_stream_store_u16(struct output_stream *stream, u16 value) {
  output_stream_store_u8(stream, value);
  output_stream_store_u8(stream, value >> 8);
}

void output_stream_flat_memory_store(void *context, u32 position, u8 value) {
  u8 *storage = context;
  storage[position] = value;
}
