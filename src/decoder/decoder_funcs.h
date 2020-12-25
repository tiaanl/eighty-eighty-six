#ifndef DECODE_FUNCS_H_
#define DECODE_FUNCS_H_

#include "../instruction.h"
#include "../streams.h"

void decode_Eb_Gb(struct input_stream *stream, struct instruction *instruction);
void decode_Ew_Gw(struct input_stream *stream, struct instruction *instruction);
void decode_Gb_Eb(struct input_stream *stream, struct instruction *instruction);
void decode_Gw_Ew(struct input_stream *stream, struct instruction *instruction);

#endif // DECODE_FUNCS_H_
