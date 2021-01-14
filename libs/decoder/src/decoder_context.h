#ifndef DECODER_DECODER_CONTEXT_H_
#define DECODER_DECODER_CONTEXT_H_

#include <base/streams.h>
#include <instructions/registers.h>

struct decoder_context {
  struct input_stream *stream;
  u32 position;

  enum segment_register segment_register_override;
};

#define DECODER_FETCH(TYPE)                                                                        \
  static inline TYPE decoder_fetch_##TYPE(struct decoder_context *decoder_context) {               \
    TYPE result = input_stream_fetch_##TYPE(decoder_context->stream, decoder_context->position);   \
    decoder_context->position += sizeof(TYPE);                                                     \
    return result;                                                                                 \
  }

DECODER_FETCH(u8)
DECODER_FETCH(i8)
DECODER_FETCH(u16)
DECODER_FETCH(i16)

#undef DECODER_FETCH

#endif // DECODER_DECODER_CONTEXT_H_
