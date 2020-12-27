#ifndef DECODER_H_
#define DECODER_H_

#include "../instruction.h"
#include "../streams.h"

void decode_instruction(struct input_stream *stream, struct instruction *instruction);

#endif // DECODER_H_
