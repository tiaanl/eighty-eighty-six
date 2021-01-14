#ifndef DECODER_H_
#define DECODER_H_

#include <base/streams.h>
#include <instructions/instructions.h>

void decode_instruction(struct input_stream *stream, u32 position, struct instruction *instruction);

#endif // DECODER_H_
