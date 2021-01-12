#include <base/print_format.h>
#include <encoder/encoder.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  UNUSED(argc);
  UNUSED(argv);

  struct instruction instruction;
  memset(&instruction, 0, sizeof(instruction));

  instruction.type = it_mov;
  instruction.destination.type = ot_register;
  instruction.destination.size = os_16;
  instruction.destination.data.as_register.reg_8 = DL;
  instruction.source.type = ot_immediate;
  instruction.source.size = os_16;
  instruction.source.data.as_immediate.immediate_16 = 3;

  u8 buffer[1024] = {};

  u8 *current = buffer;
  current += encode(&instruction, buffer, sizeof(buffer));

  for (unsigned i = 0; i < 8; ++i) {
    printf(HEX_8 " ", buffer[i]);
  }
  printf("\n");

  return 0;
}
