#include "instructions.h"

#include <assert.h>

const char *instruction_type_to_string(enum instruction_type instruction_type) {
  switch (instruction_type) {
  case ADD:
    return "add";
  case HLT:
    return "hlt";
  case INC:
    return "inc";
  case JE:
    return "je";
  case LOOP:
    return "loop";
  case MOV:
    return "mov";
  case NOP:
    return "nop";
  case TEST:
    return "test";

  default:
    assert(0);
    return "??";
  }
}
