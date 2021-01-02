#ifndef DECODER_DECODER_CONTEXT_H_
#define DECODER_DECODER_CONTEXT_H_

#include "decoder/registers.h"

#include <base/streams.h>

struct decoder_context {
  struct input_stream *stream;
  u32 position;

  enum segment_register segment_register_override;
};

#define DECODER_FETCH(TYPE)                                                                        \
  static inline TYPE decoder_fetch_##TYPE(struct decoder_context *decoder_context) {               \
    return input_stream_fetch_##TYPE(decoder_context->stream, decoder_context->position++);        \
  }

DECODER_FETCH(u8)
DECODER_FETCH(i8)
DECODER_FETCH(u16)
DECODER_FETCH(i16)

#undef DECODER_FETCH

#endif // DECODER_DECODER_CONTEXT_H_
