#include "decoder_context.h"

void decoder_context_init(struct decoder_context *context, struct reader *reader, u32 position,
                          enum segment_register segment_register_override) {
  assert(reader);

  context->reader = reader;
  context->position = position;
  context->segment_register_override = segment_register_override;
}
