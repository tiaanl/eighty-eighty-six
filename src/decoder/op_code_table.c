#include "op_code_table.h"

int decode_instruction_with_mod_rm(struct op_code_mapping *mapping, const u8 *buffer,
                                   unsigned buffer_size, struct instruction *instruction);
int decode_instruction_no_mod_rm(struct op_code_mapping *mapping, const u8 *buffer,
                                 unsigned buffer_size, struct instruction *instruction);
int decode_instruction_no_operands(struct op_code_mapping *mapping, const u8 *buffer,
                                   unsigned buffer_size, struct instruction *instruction);

#define Eb MEM_8
#define Ew MEM_16
#define Gb REG_8
#define Gw REG_16
#define AL REG_AL
#define AX REG_AX
#define Ib IMM_8
#define Iw IMM_16
#define Sw REG_SEG
#define Jb JMP_8
#define Jw JMP_16

#define Xb 0 // Memory addressed by the DS:rSI register pair.
#define Xw 0 // Memory addressed by the DS:rSI register pair.
#define Yb 0 // Memory addressed by the ES:rDI register pair.
#define Yw 0 // Memory addressed by the ES:rDI register pair.

#define Mp 0 // The ModR/M byte may refer only to memory.
#define Ma 0 // The ModR/M byte may refer only to memory.

#define RX 0 // The mod field of the ModR/M byte may refer only to a general register.

#define Ap 0 // Direct address (p = 32 or 48 bit pointer size)

#define Od 0
#define Ow 0

#define I1 0

#define DX 0

#define Fw 0 // Flag register (16-bit)

#define Xx OPERAND_NONE

#define ud32 0
#define simple32 0
#define evex32 0
#define xop32 0
#define nop 0
#define vex32 0
#define fp_escape 0

// clang-format off
struct op_code_mapping op_code_table[] = {
    /* 00 */ {ADD,       Eb, Gb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 01 */ {ADD,       Ew, Gw, Xx, Xx, decode_instruction_with_mod_rm},
    /* 02 */ {ADD,       Gb, Eb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 03 */ {ADD,       Gw, Ew, Xx, Xx, decode_instruction_with_mod_rm},
    /* 04 */ {ADD,       AL, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* 05 */ {ADD,       AX, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 06 */ {PUSH,      Sw, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 07 */ {POP,       Sw, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 08 */ {OR,        Eb, Gb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 09 */ {OR,        Ew, Gw, Xx, Xx, decode_instruction_with_mod_rm},
    /* 0a */ {OR,        Gb, Eb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 0b */ {OR,        Gw, Ew, Xx, Xx, decode_instruction_with_mod_rm},
    /* 0c */ {OR,        AL, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* 0d */ {OR,        AX, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 0e */ {PUSH,      Sw, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 0f */ {NOP,       Xx, Xx, Xx, Xx, 0},  // Prefix for `op_code_table_0f`

    /* 10 */ {ADC,       Eb, Gb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 11 */ {ADC,       Ew, Gw, Xx, Xx, decode_instruction_with_mod_rm},
    /* 12 */ {ADC,       Gb, Eb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 13 */ {ADC,       Gw, Ew, Xx, Xx, decode_instruction_with_mod_rm},
    /* 14 */ {ADC,       AL, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* 15 */ {ADC,       AX, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 16 */ {PUSH,      Sw, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 17 */ {POP,       Sw, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 18 */ {SBB,       Eb, Gb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 19 */ {SBB,       Ew, Gw, Xx, Xx, decode_instruction_with_mod_rm},
    /* 1a */ {SBB,       Gb, Eb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 1b */ {SBB,       Gw, Ew, Xx, Xx, decode_instruction_with_mod_rm},
    /* 1c */ {SBB,       AL, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* 1d */ {SBB,       AX, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 1e */ {PUSH,      Sw, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 1f */ {POP,       Sw, Xx, Xx, Xx, decode_instruction_no_mod_rm},

    /* 20 */ {AND,       Eb, Gb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 21 */ {AND,       Ew, Gw, Xx, Xx, decode_instruction_with_mod_rm},
    /* 22 */ {AND,       Gb, Eb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 23 */ {AND,       Gw, Ew, Xx, Xx, decode_instruction_with_mod_rm},
    /* 24 */ {AND,       AL, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* 25 */ {AND,       AX, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 26 */ {NOP,       Xx, Xx, Xx, Xx, ud32},  // Segment register override: ES
    /* 27 */ {DAA,       Xx, Xx, Xx, Xx, simple32},
    /* 28 */ {SUB,       Eb, Gb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 29 */ {SUB,       Ew, Gw, Xx, Xx, decode_instruction_with_mod_rm},
    /* 2a */ {SUB,       Gb, Eb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 2b */ {SUB,       Gw, Ew, Xx, Xx, decode_instruction_with_mod_rm},
    /* 2c */ {SUB,       AL, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* 2d */ {SUB,       AX, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 2e */ {NOP,       Xx, Xx, Xx, Xx, ud32},  // Segment register override: CS
    /* 2f */ {DAS,       Xx, Xx, Xx, Xx, simple32},

    /* 30 */ {XOR,       Eb, Gb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 31 */ {XOR,       Ew, Gw, Xx, Xx, decode_instruction_with_mod_rm},
    /* 32 */ {XOR,       Gb, Eb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 33 */ {XOR,       Gw, Ew, Xx, Xx, decode_instruction_with_mod_rm},
    /* 34 */ {XOR,       AL, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* 35 */ {XOR,       AX, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 36 */ {NOP,       Xx, Xx, Xx, Xx, ud32},  // Segment register override: SS
    /* 37 */ {AAA,       Xx, Xx, Xx, Xx, simple32},
    /* 38 */ {CMP,       Eb, Gb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 39 */ {CMP,       Ew, Gw, Xx, Xx, decode_instruction_with_mod_rm},
    /* 3a */ {CMP,       Gb, Eb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 3b */ {CMP,       Gw, Ew, Xx, Xx, decode_instruction_with_mod_rm},
    /* 3c */ {CMP,       AL, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* 3d */ {CMP,       AX, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 3e */ {NOP,       Xx, Xx, Xx, Xx, ud32},  // Segment register override: DS
    /* 3f */ {AAS,       Xx, Xx, Xx, Xx, simple32},

    /* 40 */ {INC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},  // TODO: This was Gw
    /* 41 */ {INC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 42 */ {INC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 43 */ {INC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 44 */ {INC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 45 */ {INC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 46 */ {INC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 47 */ {INC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 48 */ {DEC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 49 */ {DEC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 4a */ {DEC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 4b */ {DEC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 4c */ {DEC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 4d */ {DEC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 4e */ {DEC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 4f */ {DEC,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},

    /* 50 */ {PUSH,      Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 51 */ {PUSH,      Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 52 */ {PUSH,      Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 53 */ {PUSH,      Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 54 */ {PUSH,      Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 55 */ {PUSH,      Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 56 */ {PUSH,      Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 57 */ {PUSH,      Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 58 */ {POP,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 59 */ {POP,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 5a */ {POP,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 5b */ {POP,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 5c */ {POP,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 5d */ {POP,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 5e */ {POP,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 5f */ {POP,       Ew, Xx, Xx, Xx, decode_instruction_no_mod_rm},

    /* 60 */ {PUSHA,     Xx, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 61 */ {POPA,      Xx, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 62 */ {BOUND,     Gw, Ma, Xx, Xx, evex32},
    /* 63 */ {ARPL,      Ew, Gw, Xx, Xx, decode_instruction_with_mod_rm},
    /* 64 */ {NOP,       Xx, Xx, Xx, Xx, ud32},  // Segment override: FS??
    /* 65 */ {NOP,       Xx, Xx, Xx, Xx, ud32},  // Segment override: GS??
    /* 66 */ {NOP,       Xx, Xx, Xx, Xx, ud32},  // Operand size override
    /* 67 */ {NOP,       Xx, Xx, Xx, Xx, ud32},  // Operand address size override
    /* 68 */ {PUSH,      Iw, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 69 */ {IMUL,      Gw, Ew, Iw, Xx, decode_instruction_with_mod_rm},
    /* 6a */ {PUSH,      Ib, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 6b */ {IMUL,      Gw, Ew, Ib, Xx, decode_instruction_with_mod_rm},
    /* 6c */ {REP_INSB,  Yb, DX, Xx, Xx, decode_instruction_no_mod_rm},
    /* 6d */ {REP_INSW,  Yw, DX, Xx, Xx, decode_instruction_no_mod_rm},
    /* 6e */ {REP_OUTSB, DX, Xb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 6f */ {REP_OUTSW, DX, Xw, Xx, Xx, decode_instruction_no_mod_rm},

    /* 70 */ {JO,        Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 71 */ {JNO,       Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 72 */ {JB,        Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 73 */ {JNB,       Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 74 */ {JZ,        Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 75 */ {JNZ,       Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 76 */ {JBE,       Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 77 */ {JNBE,      Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 78 */ {JS,        Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 79 */ {JNS,       Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 7a */ {JP,        Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 7b */ {JNP,       Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 7c */ {JL,        Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 7d */ {JNL,       Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 7e */ {JLE,       Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 7f */ {JNLE,      Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},

    /* 80 */ {CMP,       Eb, Ib, Xx, Xx, decode_instruction_with_mod_rm},  // ADD, OR, ADC, SBB, AND, SUB, XOR
    /* 81 */ {CMP,       Ew, Iw, Xx, Xx, decode_instruction_with_mod_rm},  // ADD, OR, ADC, SBB, AND, SUB, XOR
    /* 82 */ {NOP,       Xx, Xx, Xx, Xx, decode_instruction_with_mod_rm},  // Copy of 80??!
    /* 83 */ {CMP,       Ew, Ib, Xx, Xx, decode_instruction_with_mod_rm},  // ADD, OR, ADC, SBB, AND, SUB, XOR
    /* 84 */ {TEST,      Eb, Gb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 85 */ {TEST,      Ew, Gw, Xx, Xx, decode_instruction_with_mod_rm},
    /* 86 */ {XCHG,      Eb, Gb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 87 */ {XCHG,      Ew, Gw, Xx, Xx, decode_instruction_with_mod_rm},
    /* 88 */ {MOV,       Eb, Gb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 89 */ {MOV,       Ew, Gw, Xx, Xx, decode_instruction_with_mod_rm},
    /* 8a */ {MOV,       Gb, Eb, Xx, Xx, decode_instruction_with_mod_rm},
    /* 8b */ {MOV,       Gw, Ew, Xx, Xx, decode_instruction_with_mod_rm},
    /* 8c */ {MOV,       Ew, Sw, Xx, Xx, decode_instruction_with_mod_rm},
    /* 8d */ {LEA,       Gw, Mp,  Xx, Xx, decode_instruction_with_mod_rm},
    /* 8e */ {MOV,       Sw, Ew, Xx, Xx, decode_instruction_with_mod_rm},
    /* 8f */ {POP,       Ew, Xx, Xx, Xx, xop32},  // XOP prefix? ?!?!?

    /* 90 */ {XCHG,      RX, AX, Xx, Xx, nop},
    /* 91 */ {XCHG,      RX, AX, Xx, Xx, decode_instruction_no_mod_rm},
    /* 92 */ {XCHG,      RX, AX, Xx, Xx, decode_instruction_no_mod_rm},
    /* 93 */ {XCHG,      RX, AX, Xx, Xx, decode_instruction_no_mod_rm},
    /* 94 */ {XCHG,      RX, AX, Xx, Xx, decode_instruction_no_mod_rm},
    /* 95 */ {XCHG,      RX, AX, Xx, Xx, decode_instruction_no_mod_rm},
    /* 96 */ {XCHG,      RX, AX, Xx, Xx, decode_instruction_no_mod_rm},
    /* 97 */ {XCHG,      RX, AX, Xx, Xx, decode_instruction_no_mod_rm},
    /* 98 */ {CBW,       Xx, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 99 */ {CWD,       Xx, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 9a */ {CALLF,     Ap, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 9b */ {FWAIT,     Xx, Xx, Xx, Xx, simple32},
    /* 9c */ {PUSHF,     Fw, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 9d */ {POPF,      Fw, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 9e */ {SAHF,      Xx, Xx, Xx, Xx, simple32},
    /* 9f */ {LAHF,      Xx, Xx, Xx, Xx, simple32},

    /* a0 */ {MOV,       AL, Od, Xx, Xx, decode_instruction_no_mod_rm},
    /* a1 */ {MOV,       AX, Ow, Xx, Xx, decode_instruction_no_mod_rm},
    /* a2 */ {MOV,       Od, AL, Xx, Xx, decode_instruction_no_mod_rm},
    /* a3 */ {MOV,       Od, AX, Xx, Xx, decode_instruction_no_mod_rm},
    /* a4 */ {REP_MOVSB, Yb, Xb, Xx, Xx, decode_instruction_no_mod_rm},
    /* a5 */ {REP_MOVSW, Yw, Xw, Xx, Xx, decode_instruction_no_mod_rm},
    /* a6 */ {REP_CMPSB, Xb, Yb, Xx, Xx, decode_instruction_no_mod_rm},
    /* a7 */ {REP_CMPSW, Xw, Yw, Xx, Xx, decode_instruction_no_mod_rm},
    /* a8 */ {TEST,      AL, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* a9 */ {TEST,      AX, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* aa */ {REP_STOSB, Yb, AL, Xx, Xx, decode_instruction_no_mod_rm},
    /* ab */ {REP_STOSW, Yw, AX, Xx, Xx, decode_instruction_no_mod_rm},
    /* ac */ {REP_LODSB, AL, Xb, Xx, Xx, decode_instruction_no_mod_rm},
    /* ad */ {REP_LODSW, AX, Xw, Xx, Xx, decode_instruction_no_mod_rm},
    /* ae */ {REP_SCASB, AL, Yb, Xx, Xx, decode_instruction_no_mod_rm},
    /* af */ {REP_SCASW, AX, Yw, Xx, Xx, decode_instruction_no_mod_rm},

    /* b0 */ {MOV,       Eb, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* b1 */ {MOV,       Eb, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* b2 */ {MOV,       Eb, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* b3 */ {MOV,       Eb, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* b4 */ {MOV,       Eb, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* b5 */ {MOV,       Eb, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* b6 */ {MOV,       Eb, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* b7 */ {MOV,       Eb, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* b8 */ {MOV,       Ew, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* b9 */ {MOV,       Ew, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* ba */ {MOV,       Ew, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* bb */ {MOV,       Ew, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* bc */ {MOV,       Ew, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* bd */ {MOV,       Ew, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* be */ {MOV,       Ew, Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* bf */ {MOV,       Ew, Iw, Xx, Xx, decode_instruction_no_mod_rm},

    /* c0 */ {SAR,       Eb, Ib, Xx, Xx, decode_instruction_with_mod_rm},  // ROL, ROR, RCL, RCR, SHL, SHR, SAL, SAR
    /* c1 */ {SAR,       Ew, Ib, Xx, Xx, decode_instruction_with_mod_rm},  // ROL, ROR, RCL, RCR, SHL, SHR, SAL, SAR
    /* c2 */ {RET,       Iw, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* c3 */ {RET,       Xx, Xx, Xx, Xx, decode_instruction_no_mod_rm}, // no operands?
    /* c4 */ {LES,       Gw, Mp, Xx, Xx, vex32},
    /* c5 */ {LDS,       Gw, Mp, Xx, Xx, vex32},
    /* c6 */ {MOV,       Eb, Ib, Xx, Xx, decode_instruction_with_mod_rm},
    /* c7 */ {MOV,       Ew, Iw, Xx, Xx, decode_instruction_with_mod_rm},
    /* c8 */ {ENTER,     Iw, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* c9 */ {LEAVE,     Xx, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* ca */ {RETF,      Iw, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* cb */ {RETF,      Xx, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* cc */ {INT3,      Xx, Xx, Xx, Xx, simple32},
    /* cd */ {INT,       Ib, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* ce */ {INTO,      Xx, Xx, Xx, Xx, simple32},
    /* cf */ {IRET,      Xx, Xx, Xx, Xx, decode_instruction_no_mod_rm},

    /* d0 */ {SAR,       Eb, I1, Xx, Xx, decode_instruction_with_mod_rm},  // ROL, ROR, RCL, RCR, SHL, RHR, SAL, SAR
    /* d1 */ {SAR,       Ew, I1, Xx, Xx, decode_instruction_with_mod_rm},  // ROL, ROR, RCL, RCR, SHL, RHR, SAL, SAR
    /* d2 */ {SAR,       Eb, Xx, Xx, Xx, decode_instruction_with_mod_rm},  // ROL, ROR, RCL, RCR, SHL, RHR, SAL, SAR
    /* d3 */ {SAR,       Ew, Xx, Xx, Xx, decode_instruction_with_mod_rm},  // ROL, ROR, RCL, RCR, SHL, RHR, SAL, SAR
    /* d4 */ {AAM,       Xx, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* d5 */ {AAD,       Xx, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* d6 */ {SALC,      Xx, Xx, Xx, Xx, simple32},
    /* d7 */ {XLAT,      Xx, Xx, Xx, Xx, simple32},
    /* d8 */ {NOP,       Xx, Xx, Xx, Xx, fp_escape},  // FPU instructions
    /* d9 */ {NOP,       Xx, Xx, Xx, Xx, fp_escape},  // FPU instructions
    /* da */ {NOP,       Xx, Xx, Xx, Xx, fp_escape},  // FPU instructions
    /* db */ {NOP,       Xx, Xx, Xx, Xx, fp_escape},  // FPU instructions
    /* dc */ {NOP,       Xx, Xx, Xx, Xx, fp_escape},  // FPU instructions
    /* dd */ {NOP,       Xx, Xx, Xx, Xx, fp_escape},  // FPU instructions
    /* de */ {NOP,       Xx, Xx, Xx, Xx, fp_escape},  // FPU instructions
    /* df */ {NOP,       Xx, Xx, Xx, Xx, fp_escape},  // FPU instructions

    /* e0 */ {LOOPNE,    Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* e1 */ {LOOPE,     Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* e2 */ {LOOP,      Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* e3 */ {JCXZ,      Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* e4 */ {IN,        AL, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* e5 */ {IN,        AX, Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* e6 */ {OUT,       Ib, AL, Xx, Xx, decode_instruction_no_mod_rm},
    /* e7 */ {OUT,       Ib, AX, Xx, Xx, decode_instruction_no_mod_rm},
    /* e8 */ {CALL,      Jw, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* e9 */ {JMP,       Jw, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* ea */ {JMPF,      Ap, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* eb */ {JMP,       Jb, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* ec */ {IN,        AL, DX, Xx, Xx, decode_instruction_no_mod_rm},
    /* ed */ {IN,        AX, DX, Xx, Xx, decode_instruction_no_mod_rm},
    /* ee */ {OUT,       DX, AL, Xx, Xx, decode_instruction_no_mod_rm},
    /* ef */ {OUT,       DX, AX, Xx, Xx, decode_instruction_no_mod_rm},

    /* f0 */ {NOP,       Xx, Xx, Xx, Xx, ud32},  // Lock?
    /* f1 */ {INT1,      Xx, Xx, Xx, Xx, simple32},
    /* f2 */ {NOP,       Xx, Xx, Xx, Xx, ud32},  // REPNE
    /* f3 */ {NOP,       Xx, Xx, Xx, Xx, ud32},  // REPE
    /* f4 */ {HLT,       Xx, Xx, Xx, Xx, simple32},
    /* f5 */ {CMC,       Xx, Xx, Xx, Xx, simple32},
    /* f6 */ {TEST,      Eb, Ib, Xx, Xx, decode_instruction_with_mod_rm},  // TEST, NOT, NEG, MUL, IMUL, DIV, IDIV
    /* f7 */ {TEST,      Ew, Iw, Xx, Xx, decode_instruction_with_mod_rm},  // TEST, NOT, NEG, MUL, IMUL, DIV, IDIV
    /* f8 */ {CLC,       Xx, Xx, Xx, Xx, simple32},
    /* f9 */ {STC,       Xx, Xx, Xx, Xx, simple32},
    /* fa */ {CLI,       Xx, Xx, Xx, Xx, simple32},
    /* fb */ {STI,       Xx, Xx, Xx, Xx, simple32},
    /* fc */ {CLD,       Xx, Xx, Xx, Xx, simple32},
    /* fd */ {STD,       Xx, Xx, Xx, Xx, simple32},
    /* fe */ {DEC,       Eb, Xx, Xx, Xx, decode_instruction_with_mod_rm},
    /* ff */ {INC,       Eb, Xx, Xx, Xx, decode_instruction_with_mod_rm},
};
// clang-format on

// clang-format off
struct op_code_mapping op_code_table_0f[] = {
    /* 00 */ {NOP},
    /* 01 */ {NOP},
    /* 02 */ {NOP},
    /* 03 */ {NOP},
    /* 04 */ {NOP},
    /* 05 */ {NOP},
    /* 06 */ {NOP},
    /* 07 */ {NOP},
    /* 08 */ {NOP},
    /* 09 */ {NOP},
    /* 0a */ {NOP},
    /* 0b */ {NOP},
    /* 0c */ {NOP},
    /* 0d */ {NOP},
    /* 0e */ {NOP},
    /* 0f */ {NOP},

    /* 10 */ {NOP},
    /* 11 */ {NOP},
    /* 12 */ {NOP},
    /* 13 */ {NOP},
    /* 14 */ {NOP},
    /* 15 */ {NOP},
    /* 16 */ {NOP},
    /* 17 */ {NOP},
    /* 18 */ {NOP},
    /* 19 */ {NOP},
    /* 1a */ {NOP},
    /* 1b */ {NOP},
    /* 1c */ {NOP},
    /* 1d */ {NOP},
    /* 1e */ {NOP},
    /* 1f */ {NOP},

    /* 20 */ {NOP},
    /* 21 */ {NOP},
    /* 22 */ {NOP},
    /* 23 */ {NOP},
    /* 24 */ {NOP},
    /* 25 */ {NOP},
    /* 26 */ {NOP},
    /* 27 */ {NOP},
    /* 28 */ {NOP},
    /* 29 */ {NOP},
    /* 2a */ {NOP},
    /* 2b */ {NOP},
    /* 2c */ {NOP},
    /* 2d */ {NOP},
    /* 2e */ {NOP},
    /* 2f */ {NOP},

    /* 30 */ {NOP},
    /* 31 */ {NOP},
    /* 32 */ {NOP},
    /* 33 */ {NOP},
    /* 34 */ {NOP},
    /* 35 */ {NOP},
    /* 36 */ {NOP},
    /* 37 */ {NOP},
    /* 38 */ {NOP},
    /* 39 */ {NOP},
    /* 3a */ {NOP},
    /* 3b */ {NOP},
    /* 3c */ {NOP},
    /* 3d */ {NOP},
    /* 3e */ {NOP},
    /* 3f */ {NOP},

    /* 40 */ {NOP},
    /* 41 */ {NOP},
    /* 42 */ {NOP},
    /* 43 */ {NOP},
    /* 44 */ {NOP},
    /* 45 */ {NOP},
    /* 46 */ {NOP},
    /* 47 */ {NOP},
    /* 48 */ {NOP},
    /* 49 */ {NOP},
    /* 4a */ {NOP},
    /* 4b */ {NOP},
    /* 4c */ {NOP},
    /* 4d */ {NOP},
    /* 4e */ {NOP},
    /* 4f */ {NOP},

    /* 50 */ {NOP},
    /* 51 */ {NOP},
    /* 52 */ {NOP},
    /* 53 */ {NOP},
    /* 54 */ {NOP},
    /* 55 */ {NOP},
    /* 56 */ {NOP},
    /* 57 */ {NOP},
    /* 58 */ {NOP},
    /* 59 */ {NOP},
    /* 5a */ {NOP},
    /* 5b */ {NOP},
    /* 5c */ {NOP},
    /* 5d */ {NOP},
    /* 5e */ {NOP},
    /* 5f */ {NOP},

    /* 60 */ {NOP},
    /* 61 */ {NOP},
    /* 62 */ {NOP},
    /* 63 */ {NOP},
    /* 64 */ {NOP},
    /* 65 */ {NOP},
    /* 66 */ {NOP},
    /* 67 */ {NOP},
    /* 68 */ {NOP},
    /* 69 */ {NOP},
    /* 6a */ {NOP},
    /* 6b */ {NOP},
    /* 6c */ {NOP},
    /* 6d */ {NOP},
    /* 6e */ {NOP},
    /* 6f */ {NOP},

    /* 70 */ {NOP},
    /* 71 */ {NOP},
    /* 72 */ {NOP},
    /* 73 */ {NOP},
    /* 74 */ {NOP},
    /* 75 */ {NOP},
    /* 76 */ {NOP},
    /* 77 */ {NOP},
    /* 78 */ {NOP},
    /* 79 */ {NOP},
    /* 7a */ {NOP},
    /* 7b */ {NOP},
    /* 7c */ {NOP},
    /* 7d */ {NOP},
    /* 7e */ {NOP},
    /* 7f */ {NOP},

    /* 80 */ {NOP},
    /* 81 */ {NOP},
    /* 82 */ {NOP},
    /* 83 */ {NOP},
    /* 84 */ {JZ, Jw, Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 85 */ {NOP},
    /* 86 */ {NOP},
    /* 87 */ {NOP},
    /* 88 */ {NOP},
    /* 89 */ {NOP},
    /* 8a */ {NOP},
    /* 8b */ {NOP},
    /* 8c */ {NOP},
    /* 8d */ {NOP},
    /* 8e */ {NOP},
    /* 8f */ {NOP},

    /* 90 */ {NOP},
    /* 91 */ {NOP},
    /* 92 */ {NOP},
    /* 93 */ {NOP},
    /* 94 */ {NOP},
    /* 95 */ {NOP},
    /* 96 */ {NOP},
    /* 97 */ {NOP},
    /* 98 */ {NOP},
    /* 99 */ {NOP},
    /* 9a */ {NOP},
    /* 9b */ {NOP},
    /* 9c */ {NOP},
    /* 9d */ {NOP},
    /* 9e */ {NOP},
    /* 9f */ {NOP},

    /* a0 */ {NOP},
    /* a1 */ {NOP},
    /* a2 */ {NOP},
    /* a3 */ {NOP},
    /* a4 */ {NOP},
    /* a5 */ {NOP},
    /* a6 */ {NOP},
    /* a7 */ {NOP},
    /* a8 */ {NOP},
    /* a9 */ {NOP},
    /* aa */ {NOP},
    /* ab */ {NOP},
    /* ac */ {NOP},
    /* ad */ {NOP},
    /* ae */ {NOP},
    /* af */ {NOP},

    /* b0 */ {NOP},
    /* b1 */ {NOP},
    /* b2 */ {NOP},
    /* b3 */ {NOP},
    /* b4 */ {NOP},
    /* b5 */ {NOP},
    /* b6 */ {NOP},
    /* b7 */ {NOP},
    /* b8 */ {NOP},
    /* b9 */ {NOP},
    /* ba */ {NOP},
    /* bb */ {NOP},
    /* bc */ {NOP},
    /* bd */ {NOP},
    /* be */ {NOP},
    /* bf */ {NOP},

    /* c0 */ {NOP},
    /* c1 */ {NOP},
    /* c2 */ {NOP},
    /* c3 */ {NOP},
    /* c4 */ {NOP},
    /* c5 */ {NOP},
    /* c6 */ {NOP},
    /* c7 */ {NOP},
    /* c8 */ {NOP},
    /* c9 */ {NOP},
    /* ca */ {NOP},
    /* cb */ {NOP},
    /* cc */ {NOP},
    /* cd */ {NOP},
    /* ce */ {NOP},
    /* cf */ {NOP},

    /* d0 */ {NOP},
    /* d1 */ {NOP},
    /* d2 */ {NOP},
    /* d3 */ {NOP},
    /* d4 */ {NOP},
    /* d5 */ {NOP},
    /* d6 */ {NOP},
    /* d7 */ {NOP},
    /* d8 */ {NOP},
    /* d9 */ {NOP},
    /* da */ {NOP},
    /* db */ {NOP},
    /* dc */ {NOP},
    /* dd */ {NOP},
    /* de */ {NOP},
    /* df */ {NOP},

    /* e0 */ {NOP},
    /* e1 */ {NOP},
    /* e2 */ {NOP},
    /* e3 */ {NOP},
    /* e4 */ {NOP},
    /* e5 */ {NOP},
    /* e6 */ {NOP},
    /* e7 */ {NOP},
    /* e8 */ {NOP},
    /* e9 */ {NOP},
    /* ea */ {NOP},
    /* eb */ {NOP},
    /* ec */ {NOP},
    /* ed */ {NOP},
    /* ee */ {NOP},
    /* ef */ {NOP},

    /* f0 */ {NOP},
    /* f1 */ {NOP},
    /* f2 */ {NOP},
    /* f3 */ {NOP},
    /* f4 */ {NOP},
    /* f5 */ {NOP},
    /* f6 */ {NOP},
    /* f7 */ {NOP},
    /* f8 */ {NOP},
    /* f9 */ {NOP},
    /* fa */ {NOP},
    /* fb */ {NOP},
    /* fc */ {NOP},
    /* fd */ {NOP},
    /* fe */ {NOP},
    /* ff */ {NOP},
};
// clang-format on
