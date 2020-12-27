#include "streams.h"

void input_stream_init(struct input_stream *stream, void *context, fetch_func fetch_func) {
  stream->context = context;
  stream->fetch_func = fetch_func;
  stream->position = 0;
}

u8 input_stream_peek_u8(struct input_stream *stream, i32 offset) {
  return stream->fetch_func(stream->context, stream->position + offset);
}

u16 input_stream_peek_u16(struct input_stream *stream, i32 offset) {
  return stream->fetch_func(stream->context, stream->position + offset) +
         (stream->fetch_func(stream->context, stream->position + offset + 1) << 8);
}

i8 input_stream_peek_i8(struct input_stream *stream, i32 offset) {
  return (i8)input_stream_peek_u8(stream, offset);
}

i16 input_stream_peek_i16(struct input_stream *stream, i32 offset) {
  return (i16)input_stream_peek_u16(stream, offset);
}

u8 input_stream_fetch_u8(struct input_stream *stream) {
  u8 value = stream->fetch_func(stream->context, stream->position);
  stream->position++;
  return value;
}

u16 input_stream_fetch_u16(struct input_stream *stream) {
  return input_stream_fetch_u8(stream) + (input_stream_fetch_u8(stream) << 8);
}

i8 input_stream_fetch_i8(struct input_stream *stream) {
  return (i8)input_stream_fetch_u8(stream);
}

i16 input_stream_fetch_i16(struct input_stream *stream) {
  return (i16)input_stream_fetch_u16(stream);
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
