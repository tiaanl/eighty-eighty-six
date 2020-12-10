#include "cpu.h"
#include "platform.h"

static u8 program[] = {
    0x66, 0xbe, 0xf4, 0x01,       // 0x0000  mov si, 0x01f4
    0x67, 0x8a, 0x0c,             // 0x0004  mov cl, BYTE PTR [si]
    0x66, 0x46,                   // 0x0007  inc si
    0xb5, 0x00,                   // 0x0009  mov ch, 0x00
    0xb0, 0x00,                   // 0x000b  mov al, 0x00
    0x67, 0x8a, 0x1c,             // 0x000d  mov bl, BYTE PTR [si]  <loop>
    0xf6, 0xc3, 0x01,             // 0x0010  test bl, 0x01
    0x74, 0x02,                   // 0x0013  je 0x02
    0x00, 0xd8,                   // 0x0015  add al, bl
    0x66, 0x46,                   // 0x0017  inc si
    0xe2, 0xf2,                   // 0x0019  loop <loop>
    0xa2, 0x58, 0x02, 0x00, 0x00, // 0x001b  mov ds:0x0258, al
    0xf4,                         // 0x0020  hlt
};

static u8 short_program[] = {
    // 0x00, 0xd8, // add al, bl
    0x02, 0x05, // add al, BYTE [di]
};

// void print_instruction(struct instruction *instruction) {
//  printf("%s\n", instruction_type_to_string(instruction->instruction_type));
//}

extern unsigned char small[];
extern unsigned int small_len;

int main(void) {
  struct cpu cpu;
  cpu_init(&cpu, small, small_len);
  cpu_run(&cpu);

  return 0;
}
