#include "op_code_table.h"

int decode_instruction_with_mod_rm(struct op_code_mapping *mapping, const u8 *buffer,
                                   unsigned buffer_size, struct instruction *instruction);
int decode_instruction_no_mod_rm(struct op_code_mapping *mapping, const u8 *buffer,
                                 unsigned buffer_size, struct instruction *instruction);
int decode_instruction_no_operands(struct op_code_mapping *mapping, const u8 *buffer,
                                   unsigned buffer_size, struct instruction *instruction);
int decode_instruction_skip(struct op_code_mapping *mapping, const u8 *buffer, unsigned buffer_size,
                            struct instruction *instruction);

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
    /* 00 */ {ADD,       Eb, Gb, Xx, decode_instruction_with_mod_rm},
    /* 01 */ {ADD,       Ew, Gw, Xx, decode_instruction_with_mod_rm},
    /* 02 */ {ADD,       Gb, Eb, Xx, decode_instruction_with_mod_rm},
    /* 03 */ {ADD,       Gw, Ew, Xx, decode_instruction_with_mod_rm},
    /* 04 */ {ADD,       AL, Ib, Xx, decode_instruction_no_mod_rm},
    /* 05 */ {ADD,       AX, Iw, Xx, decode_instruction_no_mod_rm},
    /* 06 */ {PUSH,      Sw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 07 */ {POP,       Sw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 08 */ {OR,        Eb, Gb, Xx, decode_instruction_with_mod_rm},
    /* 09 */ {OR,        Ew, Gw, Xx, decode_instruction_with_mod_rm},
    /* 0a */ {OR,        Gb, Eb, Xx, decode_instruction_with_mod_rm},
    /* 0b */ {OR,        Gw, Ew, Xx, decode_instruction_with_mod_rm},
    /* 0c */ {OR,        AL, Ib, Xx, decode_instruction_no_mod_rm},
    /* 0d */ {OR,        AX, Iw, Xx, decode_instruction_no_mod_rm},
    /* 0e */ {PUSH,      Sw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 0f */ {NOP,       Xx, Xx, Xx, 0},

    /* 10 */ {ADC,       Eb, Gb, Xx, decode_instruction_with_mod_rm},
    /* 11 */ {ADC,       Ew, Gw, Xx, decode_instruction_with_mod_rm},
    /* 12 */ {ADC,       Gb, Eb, Xx, decode_instruction_with_mod_rm},
    /* 13 */ {ADC,       Gw, Ew, Xx, decode_instruction_with_mod_rm},
    /* 14 */ {ADC,       AL, Ib, Xx, decode_instruction_no_mod_rm},
    /* 15 */ {ADC,       AX, Iw, Xx, decode_instruction_no_mod_rm},
    /* 16 */ {PUSH,      Sw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 17 */ {POP,       Sw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 18 */ {SBB,       Eb, Gb, Xx, decode_instruction_with_mod_rm},
    /* 19 */ {SBB,       Ew, Gw, Xx, decode_instruction_with_mod_rm},
    /* 1a */ {SBB,       Gb, Eb, Xx, decode_instruction_with_mod_rm},
    /* 1b */ {SBB,       Gw, Ew, Xx, decode_instruction_with_mod_rm},
    /* 1c */ {SBB,       AL, Ib, Xx, decode_instruction_no_mod_rm},
    /* 1d */ {SBB,       AX, Iw, Xx, decode_instruction_no_mod_rm},
    /* 1e */ {PUSH,      Sw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 1f */ {POP,       Sw, Xx, Xx, decode_instruction_no_mod_rm},

    /* 20 */ {AND,       Eb, Gb, Xx, decode_instruction_with_mod_rm},
    /* 21 */ {AND,       Ew, Gw, Xx, decode_instruction_with_mod_rm},
    /* 22 */ {AND,       Gb, Eb, Xx, decode_instruction_with_mod_rm},
    /* 23 */ {AND,       Gw, Ew, Xx, decode_instruction_with_mod_rm},
    /* 24 */ {AND,       AL, Ib, Xx, decode_instruction_no_mod_rm},
    /* 25 */ {AND,       AX, Iw, Xx, decode_instruction_no_mod_rm},
    /* 26 */ {NOP,       Xx, Xx, Xx, ud32},  // Segment register override: ES
    /* 27 */ {DAA,       Xx, Xx, Xx, decode_instruction_no_operands},
    /* 28 */ {SUB,       Eb, Gb, Xx, decode_instruction_with_mod_rm},
    /* 29 */ {SUB,       Ew, Gw, Xx, decode_instruction_with_mod_rm},
    /* 2a */ {SUB,       Gb, Eb, Xx, decode_instruction_with_mod_rm},
    /* 2b */ {SUB,       Gw, Ew, Xx, decode_instruction_with_mod_rm},
    /* 2c */ {SUB,       AL, Ib, Xx, decode_instruction_no_mod_rm},
    /* 2d */ {SUB,       AX, Iw, Xx, decode_instruction_no_mod_rm},
    /* 2e */ {NOP,       Xx, Xx, Xx, ud32},  // Segment register override: CS
    /* 2f */ {DAS,       Xx, Xx, Xx, decode_instruction_no_operands},

    /* 30 */ {XOR,       Eb, Gb, Xx, decode_instruction_with_mod_rm},
    /* 31 */ {XOR,       Ew, Gw, Xx, decode_instruction_with_mod_rm},
    /* 32 */ {XOR,       Gb, Eb, Xx, decode_instruction_with_mod_rm},
    /* 33 */ {XOR,       Gw, Ew, Xx, decode_instruction_with_mod_rm},
    /* 34 */ {XOR,       AL, Ib, Xx, decode_instruction_no_mod_rm},
    /* 35 */ {XOR,       AX, Iw, Xx, decode_instruction_no_mod_rm},
    /* 36 */ {NOP,       Xx, Xx, Xx, ud32},  // Segment register override: SS
    /* 37 */ {AAA,       Xx, Xx, Xx, decode_instruction_no_operands},
    /* 38 */ {CMP,       Eb, Gb, Xx, decode_instruction_with_mod_rm},
    /* 39 */ {CMP,       Ew, Gw, Xx, decode_instruction_with_mod_rm},
    /* 3a */ {CMP,       Gb, Eb, Xx, decode_instruction_with_mod_rm},
    /* 3b */ {CMP,       Gw, Ew, Xx, decode_instruction_with_mod_rm},
    /* 3c */ {CMP,       AL, Ib, Xx, decode_instruction_no_mod_rm},
    /* 3d */ {CMP,       AX, Iw, Xx, decode_instruction_no_mod_rm},
    /* 3e */ {NOP,       Xx, Xx, Xx, ud32},  // Segment register override: DS
    /* 3f */ {AAS,       Xx, Xx, Xx, decode_instruction_no_operands},

    /* 40 */ {INC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},  // TODO: This was Gw
    /* 41 */ {INC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 42 */ {INC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 43 */ {INC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 44 */ {INC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 45 */ {INC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 46 */ {INC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 47 */ {INC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 48 */ {DEC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 49 */ {DEC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 4a */ {DEC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 4b */ {DEC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 4c */ {DEC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 4d */ {DEC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 4e */ {DEC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 4f */ {DEC,       Ew, Xx, Xx, decode_instruction_no_mod_rm},

    /* 50 */ {PUSH,      Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 51 */ {PUSH,      Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 52 */ {PUSH,      Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 53 */ {PUSH,      Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 54 */ {PUSH,      Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 55 */ {PUSH,      Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 56 */ {PUSH,      Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 57 */ {PUSH,      Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 58 */ {POP,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 59 */ {POP,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 5a */ {POP,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 5b */ {POP,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 5c */ {POP,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 5d */ {POP,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 5e */ {POP,       Ew, Xx, Xx, decode_instruction_no_mod_rm},
    /* 5f */ {POP,       Ew, Xx, Xx, decode_instruction_no_mod_rm},

    /* 60 */ {PUSHA,     Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 61 */ {POPA,      Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 62 */ {BOUND,     Gw, Ma, Xx, evex32},
    /* 63 */ {ARPL,      Ew, Gw, Xx, decode_instruction_with_mod_rm},
    /* 64 */ {NOP,       Xx, Xx, Xx, ud32},  // Segment override: FS??
    /* 65 */ {NOP,       Xx, Xx, Xx, ud32},  // Segment override: GS??
    /* 66 */ {NOP,       Xx, Xx, Xx, ud32},  // Operand size override
    /* 67 */ {NOP,       Xx, Xx, Xx, ud32},  // Operand address size override
    /* 68 */ {PUSH,      Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 69 */ {IMUL,      Gw, Ew, Iw, decode_instruction_with_mod_rm},
    /* 6a */ {PUSH,      Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* 6b */ {IMUL,      Gw, Ew, Ib, decode_instruction_with_mod_rm},
    /* 6c */ {REP_INSB,  Yb, DX, Xx, decode_instruction_no_mod_rm},
    /* 6d */ {REP_INSW,  Yw, DX, Xx, decode_instruction_no_mod_rm},
    /* 6e */ {REP_OUTSB, DX, Xb, Xx, decode_instruction_no_mod_rm},
    /* 6f */ {REP_OUTSW, DX, Xw, Xx, decode_instruction_no_mod_rm},

    /* 70 */ {JO,        Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 71 */ {JNO,       Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 72 */ {JB,        Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 73 */ {JNB,       Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 74 */ {JZ,        Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 75 */ {JNZ,       Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 76 */ {JBE,       Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 77 */ {JNBE,      Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 78 */ {JS,        Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 79 */ {JNS,       Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 7a */ {JP,        Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 7b */ {JNP,       Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 7c */ {JL,        Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 7d */ {JNL,       Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 7e */ {JLE,       Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* 7f */ {JNLE,      Jb, Xx, Xx, decode_instruction_no_mod_rm},

    /* 80 */ {CMP,       Eb, Ib, Xx, decode_instruction_with_mod_rm},  // ADD, OR, ADC, SBB, AND, SUB, XOR
    /* 81 */ {CMP,       Ew, Iw, Xx, decode_instruction_with_mod_rm},  // ADD, OR, ADC, SBB, AND, SUB, XOR
    /* 82 */ {NOP,       Xx, Xx, Xx, decode_instruction_with_mod_rm},  // Copy of 80??!
    /* 83 */ {CMP,       Ew, Ib, Xx, decode_instruction_with_mod_rm},  // ADD, OR, ADC, SBB, AND, SUB, XOR
    /* 84 */ {TEST,      Eb, Gb, Xx, decode_instruction_with_mod_rm},
    /* 85 */ {TEST,      Ew, Gw, Xx, decode_instruction_with_mod_rm},
    /* 86 */ {XCHG,      Eb, Gb, Xx, decode_instruction_with_mod_rm},
    /* 87 */ {XCHG,      Ew, Gw, Xx, decode_instruction_with_mod_rm},
    /* 88 */ {MOV,       Eb, Gb, Xx, decode_instruction_with_mod_rm},
    /* 89 */ {MOV,       Ew, Gw, Xx, decode_instruction_with_mod_rm},
    /* 8a */ {MOV,       Gb, Eb, Xx, decode_instruction_with_mod_rm},
    /* 8b */ {MOV,       Gw, Ew, Xx, decode_instruction_with_mod_rm},
    /* 8c */ {MOV,       Ew, Sw, Xx, decode_instruction_with_mod_rm},
    /* 8d */ {LEA,       Gw, Mp, Xx, decode_instruction_with_mod_rm},
    /* 8e */ {MOV,       Sw, Ew, Xx, decode_instruction_with_mod_rm},
    /* 8f */ {POP,       Ew, Xx, Xx, decode_instruction_with_mod_rm},  // XOP prefix? ?!?!?

    /* 90 */ {XCHG,      RX, AX, Xx, nop},
    /* 91 */ {XCHG,      RX, AX, Xx, decode_instruction_no_mod_rm},
    /* 92 */ {XCHG,      RX, AX, Xx, decode_instruction_no_mod_rm},
    /* 93 */ {XCHG,      RX, AX, Xx, decode_instruction_no_mod_rm},
    /* 94 */ {XCHG,      RX, AX, Xx, decode_instruction_no_mod_rm},
    /* 95 */ {XCHG,      RX, AX, Xx, decode_instruction_no_mod_rm},
    /* 96 */ {XCHG,      RX, AX, Xx, decode_instruction_no_mod_rm},
    /* 97 */ {XCHG,      RX, AX, Xx, decode_instruction_no_mod_rm},
    /* 98 */ {CBW,       Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 99 */ {CWD,       Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* 9a */ {CALLF,     Ap, Xx, Xx, decode_instruction_no_mod_rm},
    /* 9b */ {FWAIT,     Xx, Xx, Xx, decode_instruction_no_operands},
    /* 9c */ {PUSHF,     Fw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 9d */ {POPF,      Fw, Xx, Xx, decode_instruction_no_mod_rm},
    /* 9e */ {SAHF,      Xx, Xx, Xx, decode_instruction_no_operands},
    /* 9f */ {LAHF,      Xx, Xx, Xx, decode_instruction_no_operands},

    /* a0 */ {MOV,       AL, Iw, Xx, decode_instruction_no_mod_rm},
    /* a1 */ {MOV,       AX, Iw, Xx, decode_instruction_no_mod_rm},
    /* a2 */ {MOV,       Od, AL, Xx, decode_instruction_no_mod_rm},
    /* a3 */ {MOV,       Od, AX, Xx, decode_instruction_no_mod_rm},
    /* a4 */ {REP_MOVSB, Yb, Xb, Xx, decode_instruction_no_mod_rm},
    /* a5 */ {REP_MOVSW, Yw, Xw, Xx, decode_instruction_no_mod_rm},
    /* a6 */ {REP_CMPSB, Xb, Yb, Xx, decode_instruction_no_mod_rm},
    /* a7 */ {REP_CMPSW, Xw, Yw, Xx, decode_instruction_no_mod_rm},
    /* a8 */ {TEST,      AL, Ib, Xx, decode_instruction_no_mod_rm},
    /* a9 */ {TEST,      AX, Iw, Xx, decode_instruction_no_mod_rm},
    /* aa */ {REP_STOSB, Yb, AL, Xx, decode_instruction_no_mod_rm},
    /* ab */ {REP_STOSW, Yw, AX, Xx, decode_instruction_no_mod_rm},
    /* ac */ {REP_LODSB, AL, Xb, Xx, decode_instruction_no_mod_rm},
    /* ad */ {REP_LODSW, AX, Xw, Xx, decode_instruction_no_mod_rm},
    /* ae */ {REP_SCASB, AL, Yb, Xx, decode_instruction_no_mod_rm},
    /* af */ {REP_SCASW, AX, Yw, Xx, decode_instruction_no_mod_rm},

    /* b0 */ {MOV,       Eb, Ib, Xx, decode_instruction_no_mod_rm},
    /* b1 */ {MOV,       Eb, Ib, Xx, decode_instruction_no_mod_rm},
    /* b2 */ {MOV,       Eb, Ib, Xx, decode_instruction_no_mod_rm},
    /* b3 */ {MOV,       Eb, Ib, Xx, decode_instruction_no_mod_rm},
    /* b4 */ {MOV,       Eb, Ib, Xx, decode_instruction_no_mod_rm},
    /* b5 */ {MOV,       Eb, Ib, Xx, decode_instruction_no_mod_rm},
    /* b6 */ {MOV,       Eb, Ib, Xx, decode_instruction_no_mod_rm},
    /* b7 */ {MOV,       Eb, Ib, Xx, decode_instruction_no_mod_rm},
    /* b8 */ {MOV,       Ew, Iw, Xx, decode_instruction_no_mod_rm},
    /* b9 */ {MOV,       Ew, Iw, Xx, decode_instruction_no_mod_rm},
    /* ba */ {MOV,       Ew, Iw, Xx, decode_instruction_no_mod_rm},
    /* bb */ {MOV,       Ew, Iw, Xx, decode_instruction_no_mod_rm},
    /* bc */ {MOV,       Ew, Iw, Xx, decode_instruction_no_mod_rm},
    /* bd */ {MOV,       Ew, Iw, Xx, decode_instruction_no_mod_rm},
    /* be */ {MOV,       Ew, Iw, Xx, decode_instruction_no_mod_rm},
    /* bf */ {MOV,       Ew, Iw, Xx, decode_instruction_no_mod_rm},

    /* c0 */ {SAR,       Eb, Ib, Xx, decode_instruction_with_mod_rm},  // ROL, ROR, RCL, RCR, SHL, SHR, SAL, SAR
    /* c1 */ {SAR,       Ew, Ib, Xx, decode_instruction_with_mod_rm},  // ROL, ROR, RCL, RCR, SHL, SHR, SAL, SAR
    /* c2 */ {RET,       Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* c3 */ {RET,       Xx, Xx, Xx, decode_instruction_no_mod_rm}, // no operands?
    /* c4 */ {LES,       Gw, Ew, Xx, decode_instruction_with_mod_rm},
    /* c5 */ {LDS,       Gw, Ew, Xx, decode_instruction_with_mod_rm},
    /* c6 */ {MOV,       Eb, Ib, Xx, decode_instruction_with_mod_rm},
    /* c7 */ {MOV,       Ew, Iw, Xx, decode_instruction_with_mod_rm},
    /* c8 */ {ENTER,     Iw, Ib, Xx, decode_instruction_no_mod_rm},
    /* c9 */ {LEAVE,     Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* ca */ {RETF,      Iw, Xx, Xx, decode_instruction_no_mod_rm},
    /* cb */ {RETF,      Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* cc */ {INT3,      Xx, Xx, Xx, decode_instruction_no_operands},
    /* cd */ {INT,       Ib, Xx, Xx, decode_instruction_no_mod_rm},
    /* ce */ {INTO,      Xx, Xx, Xx, decode_instruction_no_operands},
    /* cf */ {IRET,      Xx, Xx, Xx, decode_instruction_no_mod_rm},

    /* d0 */ {SAR,       Eb, I1, Xx, decode_instruction_with_mod_rm},  // ROL, ROR, RCL, RCR, SHL, RHR, SAL, SAR
    /* d1 */ {SAR,       Ew, I1, Xx, decode_instruction_with_mod_rm},  // ROL, ROR, RCL, RCR, SHL, RHR, SAL, SAR
    /* d2 */ {SAR,       Eb, Xx, Xx, decode_instruction_with_mod_rm},  // ROL, ROR, RCL, RCR, SHL, RHR, SAL, SAR
    /* d3 */ {SAR,       Ew, Xx, Xx, decode_instruction_with_mod_rm},  // ROL, ROR, RCL, RCR, SHL, RHR, SAL, SAR
    /* d4 */ {AAM,       Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* d5 */ {AAD,       Xx, Xx, Xx, decode_instruction_no_mod_rm},
    /* d6 */ {SALC,      Xx, Xx, Xx, decode_instruction_no_operands},
    /* d7 */ {XLAT,      Xx, Xx, Xx, decode_instruction_no_operands},
    /* d8 */ {NOP,       Xx, Xx, Xx, fp_escape},  // FPU instructions
    /* d9 */ {NOP,       Xx, Xx, Xx, fp_escape},  // FPU instructions
    /* da */ {NOP,       Xx, Xx, Xx, fp_escape},  // FPU instructions
    /* db */ {NOP,       Xx, Xx, Xx, fp_escape},  // FPU instructions
    /* dc */ {NOP,       Xx, Xx, Xx, fp_escape},  // FPU instructions
    /* dd */ {NOP,       Xx, Xx, Xx, fp_escape},  // FPU instructions
    /* de */ {NOP,       Xx, Xx, Xx, fp_escape},  // FPU instructions
    /* df */ {NOP,       Xx, Xx, Xx, fp_escape},  // FPU instructions

    /* e0 */ {LOOPNE,    Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* e1 */ {LOOPE,     Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* e2 */ {LOOP,      Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* e3 */ {JCXZ,      Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* e4 */ {IN,        AL, Ib, Xx, decode_instruction_no_mod_rm},
    /* e5 */ {IN,        AX, Ib, Xx, decode_instruction_no_mod_rm},
    /* e6 */ {OUT,       Ib, AL, Xx, decode_instruction_no_mod_rm},
    /* e7 */ {OUT,       Ib, AX, Xx, decode_instruction_no_mod_rm},
    /* e8 */ {CALL,      Jw, Xx, Xx, decode_instruction_no_mod_rm},
    /* e9 */ {JMP,       Jw, Xx, Xx, decode_instruction_no_mod_rm},
    /* ea */ {JMPF,      Ap, Xx, Xx, decode_instruction_no_mod_rm},
    /* eb */ {JMP,       Jb, Xx, Xx, decode_instruction_no_mod_rm},
    /* ec */ {IN,        AL, DX, Xx, decode_instruction_no_mod_rm},
    /* ed */ {IN,        AX, DX, Xx, decode_instruction_no_mod_rm},
    /* ee */ {OUT,       DX, AL, Xx, decode_instruction_no_mod_rm},
    /* ef */ {OUT,       DX, AX, Xx, decode_instruction_no_mod_rm},

    /* f0 */ {NOP,       Xx, Xx, Xx, ud32},  // Lock?
    /* f1 */ {INT1,      Xx, Xx, Xx, decode_instruction_no_operands},
    /* f2 */ {NOP,       Xx, Xx, Xx, decode_instruction_skip},  // REPNE
    /* f3 */ {NOP,       Xx, Xx, Xx, decode_instruction_skip},  // REPE
    /* f4 */ {HLT,       Xx, Xx, Xx, decode_instruction_no_operands},
    /* f5 */ {CMC,       Xx, Xx, Xx, decode_instruction_no_operands},
    /* f6 */ {TEST,      Eb, Ib, Xx, decode_instruction_with_mod_rm},  // TEST, NOT, NEG, MUL, IMUL, DIV, IDIV
    /* f7 */ {TEST,      Ew, Iw, Xx, decode_instruction_with_mod_rm},  // TEST, NOT, NEG, MUL, IMUL, DIV, IDIV
    /* f8 */ {CLC,       Xx, Xx, Xx, decode_instruction_no_operands},
    /* f9 */ {STC,       Xx, Xx, Xx, decode_instruction_no_operands},
    /* fa */ {CLI,       Xx, Xx, Xx, decode_instruction_no_operands},
    /* fb */ {STI,       Xx, Xx, Xx, decode_instruction_no_operands},
    /* fc */ {CLD,       Xx, Xx, Xx, decode_instruction_no_operands},
    /* fd */ {STD,       Xx, Xx, Xx, decode_instruction_no_operands},
    /* fe */ {DEC,       Eb, Xx, Xx, decode_instruction_with_mod_rm},
    /* ff */ {INC,       Eb, Xx, Xx, decode_instruction_with_mod_rm},
};
// clang-format on
