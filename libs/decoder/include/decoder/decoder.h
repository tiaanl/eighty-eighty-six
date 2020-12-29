#ifndef DECODER_H_
#define DECODER_H_

#include <base/streams.h>
#include <decoder/instruction.h>

void decode_instruction(struct input_stream *stream, struct instruction *instruction);

#endif // DECODER_H_
