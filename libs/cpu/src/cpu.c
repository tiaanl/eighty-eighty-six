#include "cpu/cpu.h"

#include "instr_map.h"

#include <assert.h>
#include <base/print_format.h>
#include <decoder/decoder.h>
#include <disassembler/disassembler.h>
#include <stdio.h>
#include <string.h>

static void cpu_exec(struct cpu *cpu, struct instruction *instruction) {
  struct instr_mapping *mapping = &instr_map[instruction->type];

  assert(mapping);

  assert(instruction->type == mapping->instruction_type);

  assert(mapping->exec_func);
  mapping->exec_func(cpu, instruction);
}

void cpu_init(struct cpu *cpu, struct ports *ports, struct bus *bus, struct address reset_vector) {
  memset(cpu, 0, sizeof(*cpu));

  cpu->ports = ports;
  cpu->bus = bus;

  cpu->segment_16[CS] = reset_vector.segment;
  cpu->ip = reset_vector.offset;
}

void print_registers(struct cpu *cpu) {
  printf("ax: " HEX_16 ", bx: " HEX_16 ", cx: " HEX_16 ", dx: " HEX_16 ", ip: " HEX_16 "\n",
         cpu->regs.word[AX], cpu->regs.word[BX], cpu->regs.word[CX], cpu->regs.word[DX], cpu->ip);
  printf("si: " HEX_16 ", di: " HEX_16 ", bp: " HEX_16 ", sp: " HEX_16 ", fl: " HEX_16 "\n",
         cpu->regs.word[SI], cpu->regs.word[DI], cpu->regs.word[BP], cpu->regs.word[SP], 0);
}

u8 input_stream_fetch_from_bus(void *context, u32 position) {
  struct bus *bus = context;
  return bus_fetch_byte(bus, position);
}

void cpu_step(struct cpu *cpu) {
  struct address cs_ip = segment_offset(cpu->segment_16[CS], cpu->ip);
  u32 flat = flatten_address(cs_ip);

  struct input_stream stream;
  input_stream_init(&stream, cpu->bus, input_stream_fetch_from_bus);

  struct instruction instruction;
  decode_instruction(&stream, flat, &instruction);

  print_registers(cpu);

  assert(instruction.instruction_size);
  cpu->ip += instruction.instruction_size;

  char buf[128];
  disassemble(buf, sizeof(buf), &instruction, flat);
  puts(buf);
  puts("");

  cpu_exec(cpu, &instruction);
}
