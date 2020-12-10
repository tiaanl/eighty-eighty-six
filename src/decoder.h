#ifndef DECODER_H_
#define DECODER_H_

struct cpu;
struct instruction;

int decode_instruction(struct cpu *cpu, struct instruction *instruction);
int decode_instruction_with_mod_rm(struct cpu *cpu, struct instruction *instruction);

#endif // DECODER_H_
