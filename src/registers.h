#ifndef REGISTERS_H_
#define REGISTERS_H_

enum register_8 {
  AL,
  AH,
  BL,
  BH,
  CL,
  CH,
  DL,
  DH,

  REGISTER_8_COUNT,
};

enum register_16 {
  AX,
  BX,
  CX,
  DX,
  SI,
  DI,
  BP,
  SP,
  FLAGS,

  REGISTER_16_COUNT,
};

enum register_32 {
  EAX,
  EBX,
  ECX,
  EDX,
  ESI,
  EDI,
  EBP,
  ESP,
  EFLAGS,

  REGISTER_32_COUNT,
};

const char *register_8_to_string(enum register_8 reg);
const char *register_16_to_string(enum register_16 reg);
const char *register_32_to_string(enum register_32 reg);

#endif // REGISTERS_H_
