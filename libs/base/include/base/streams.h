#ifndef STREAMS_H_
#define STREAMS_H_

#include "platform.h"

typedef u8 (*fetch_func)(void *context, u32 position);

struct input_stream {
  void *context;
  fetch_func fetch_func;
};

void input_stream_init(struct input_stream *stream, void *context, fetch_func fetch_func);

u8 input_stream_fetch_u8(struct input_stream *stream, u32 position);
u16 input_stream_fetch_u16(struct input_stream *stream, u32 position);

i8 input_stream_fetch_i8(struct input_stream *stream, u32 position);
i16 input_stream_fetch_i16(struct input_stream *stream, u32 position);

/* ---------------------------------------------------------------------------------------------- */

typedef void (*store_func)(void *context, u32 position, u8 value);

struct output_stream {
  void *context;
  store_func store_func;
  u32 position;
};

void output_stream_init(struct output_stream *stream, void *context, store_func store_func);

void output_stream_store_u8(struct output_stream *stream, u8 value);
void output_stream_store_u16(struct output_stream *stream, u16 value);

/**
 * Generic store function that writes a single byte into `context[pos]`.
 */
void output_stream_flat_memory_store(void *context, u32 position, u8 value);

#endif
