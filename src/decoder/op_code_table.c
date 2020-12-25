#include "op_code_table.h"

#define _ DT_NONE
#define __ 0

/**
 * Addressing Methods
 * ==================
 * A - Direct address: the instruction has no ModR/M byte; the address of the operand is encoded in
 *     the instruction.
 * E - A ModR/M byte follows the opcode and specifies the operand. The operand is either a
 *     general-purpose register or a memory address. If it is a memory address, the address is
 *     computed from a segment register and any of the following values: a base register, an index
 *     register, a scaling factor, a displacement.
 * F - FLAGS register.
 * G - The reg field of the ModR/M byte selects a general register.
 * I - Immediate data: the operand value is encoded in subsequent bytes of the instruction.
 * J - The instruction contains a relative offset to be added to the instruction pointer register.
 * M - The ModR/M byte may refer only to memory.
 * O - The instruction has no ModR/M byte. The offset of the operand is coded as a word or double
 *     word (depending on address size attribute) in the instruction.
 * S - The reg field of the ModR/M byte selects a segment register.
 * X - Memory addressed by the DS:SI register pair.
 * Y - Memory addressed by the ES:DI register pair.
 *
 * Operand Type
 * ============
 * a - Two one-word operands in memory (used only by the BOUND instruction).
 * b - Byte.
 * p - 32-bit pointer.
 * w - Word.
 */

// clang-format off
struct op_code_mapping op_code_table[] = {
    /* 00 */ {ADD,     am_Eb, am_Gb, am_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 01 */ {ADD,     am_Ew, am_Gw, am_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 02 */ {ADD,     am_Gb, am_Eb, am_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 03 */ {ADD,     am_Gw, am_Ew, am_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 04 */ {ADD,     am_AL, am_Ib, am_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 05 */ {ADD,     am_AX, am_Iw, am_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 06 */ {PUSH,    am_ES, am_Xx, am_Xx, DT_SEGMENT_REG,    _,                 _,         __},
    /* 07 */ {POP,     am_ES, am_Xx, am_Xx, DT_SEGMENT_REG,    _,                 _,         __},
    /* 08 */ {OR,      am_Eb, am_Gb, am_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 09 */ {OR,      am_Ew, am_Gw, am_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 0a */ {OR,      am_Gb, am_Eb, am_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 0b */ {OR,      am_Gw, am_Ew, am_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 0c */ {OR,      am_AL, am_Ib, am_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 0d */ {OR,      am_AX, am_Iw, am_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 0e */ {PUSH,    am_CS, am_Xx, am_Xx, DT_SEGMENT_REG,    _,                 _,         __},
    /* 0f */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // Extended op codes.

    /* 10 */ {ADC,     am_Eb, am_Gb, am_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 11 */ {ADC,     am_Ew, am_Gw, am_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 12 */ {ADC,     am_Gb, am_Eb, am_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 13 */ {ADC,     am_Gw, am_Ew, am_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 14 */ {ADC,     am_AL, am_Ib, am_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 15 */ {ADC,     am_AX, am_Iw, am_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 16 */ {PUSH,    am_SS, am_Xx, am_Xx, DT_SEGMENT_REG,    _,                 _,         __},
    /* 17 */ {POP,     am_SS, am_Xx, am_Xx, DT_SEGMENT_REG,    _,                 _,         __},
    /* 18 */ {SBB,     am_Eb, am_Gb, am_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 19 */ {SBB,     am_Ew, am_Gw, am_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 1a */ {SBB,     am_Gb, am_Eb, am_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 1b */ {SBB,     am_Gw, am_Ew, am_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 1c */ {SBB,     am_AL, am_Ib, am_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 1d */ {SBB,     am_AX, am_Iw, am_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 1e */ {PUSH,    am_DS, am_Xx, am_Xx, DT_SEGMENT_REG,    _,                 _,         __},
    /* 1f */ {POP,     am_DS, am_Xx, am_Xx, DT_SEGMENT_REG,    _,                 _,         __},

    /* 20 */ {AND,     am_Eb, am_Gb, am_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 21 */ {AND,     am_Ew, am_Gw, am_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 22 */ {AND,     am_Gb, am_Eb, am_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 23 */ {AND,     am_Gw, am_Ew, am_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 24 */ {AND,     am_AL, am_Ib, am_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 25 */ {AND,     am_AX, am_Iw, am_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 26 */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // Segment register override (ES)
    /* 27 */ {DAA,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* 28 */ {SUB,     am_Eb, am_Gb, am_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 29 */ {SUB,     am_Ew, am_Gw, am_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 2a */ {SUB,     am_Gb, am_Eb, am_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 2b */ {SUB,     am_Gw, am_Ew, am_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 2c */ {SUB,     am_AL, am_Ib, am_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 2d */ {SUB,     am_AX, am_Iw, am_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 2e */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // Segment register override (CS)
    /* 2f */ {DAS,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},

    /* 30 */ {XOR,     am_Eb, am_Gb, am_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 31 */ {XOR,     am_Ew, am_Gw, am_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 32 */ {XOR,     am_Gb, am_Eb, am_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 33 */ {XOR,     am_Gw, am_Ew, am_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 34 */ {XOR,     am_AL, am_Ib, am_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 35 */ {XOR,     am_AX, am_Iw, am_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 36 */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // Segment register override (SS)
    /* 37 */ {AAA,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* 38 */ {CMP,     am_Eb, am_Gb, am_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 39 */ {CMP,     am_Ew, am_Gw, am_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 3a */ {CMP,     am_Gb, am_Eb, am_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 3b */ {CMP,     am_Gw, am_Ew, am_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 3c */ {CMP,     am_AL, am_Ib, am_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 3d */ {CMP,     am_AX, am_Iw, am_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 3e */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // Segment register override (DS)
    /* 3f */ {AAS,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},

    /* 40 */ {INC,     am_AX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 41 */ {INC,     am_CX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 42 */ {INC,     am_DX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 43 */ {INC,     am_BX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 44 */ {INC,     am_SP, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 45 */ {INC,     am_BP, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 46 */ {INC,     am_SI, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 47 */ {INC,     am_DI, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 48 */ {DEC,     am_AX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 49 */ {DEC,     am_CX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4a */ {DEC,     am_DX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4b */ {DEC,     am_BX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4c */ {DEC,     am_SP, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4d */ {DEC,     am_BP, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4e */ {DEC,     am_SI, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4f */ {DEC,     am_DI, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},

    /* 50 */ {PUSH,    am_AX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 51 */ {PUSH,    am_CX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 52 */ {PUSH,    am_DX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 53 */ {PUSH,    am_BX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 54 */ {PUSH,    am_SP, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 55 */ {PUSH,    am_BP, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 56 */ {PUSH,    am_SI, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 57 */ {PUSH,    am_DI, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 58 */ {POP,     am_AX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 59 */ {POP,     am_CX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5a */ {POP,     am_DX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5b */ {POP,     am_BX, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5c */ {POP,     am_SP, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5d */ {POP,     am_BP, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5e */ {POP,     am_SI, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5f */ {POP,     am_DI, am_Xx, am_Xx, DT_OP_CODE_REG_16, _,                 _,         __},

    /* 60 */ {PUSHA,   am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* 61 */ {POPA,    am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* 62 */ {BOUND,   am_Gw, am_Ma, am_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 63 */ {ARPL,    am_Ew, am_Gw, am_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 64 */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // Segment override:              FS??
    /* 65 */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // Segment override:              GS??
    /* 66 */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // Operand size override
    /* 67 */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // Operand address size override
    /* 68 */ {PUSH,    am_Iw, am_Xx, am_Xx, DT_IMM_16,         _,                 _,         __},
    /* 69 */ {IMUL,    am_Gw, am_Ew, am_Iw, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   DT_IMM_16, DF_HAS_MOD_RM},
    /* 6a */ {PUSH,    am_Ib, am_Xx, am_Xx, DT_IMM_8,          _,                 _,         __},
    /* 6b */ {IMUL,    am_Gw, am_Ew, am_Ib, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   DT_IMM_8,  DF_HAS_MOD_RM},
    /* 6c */ {INSB,    am_Yb, am_DX, am_Xx, _,                 DT_DX,             _,         __},
    /* 6d */ {INSW,    am_Yw, am_DX, am_Xx, _,                 DT_DX,             _,         __},
    /* 6e */ {OUTSB,   am_DX, am_Xb, am_Xx, DT_DX,             _,                 _,         __},
    /* 6f */ {OUTSW,   am_DX, am_Xw, am_Xx, DT_DX,             _,                 _,         __},

    /* 70 */ {JO,      am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* 71 */ {JNO,     am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* 72 */ {JB,      am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* 73 */ {JNB,     am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* 74 */ {JZ,      am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* 75 */ {JNZ,     am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* 76 */ {JBE,     am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* 77 */ {JNBE,    am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* 78 */ {JS,      am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* 79 */ {JNS,     am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* 7a */ {JP,      am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* 7b */ {JNP,     am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* 7c */ {JL,      am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* 7d */ {JNL,     am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* 7e */ {JLE,     am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* 7f */ {JNLE,    am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},

    /* 80 */ {CMP,     am_Eb, am_Ib, am_Xx, DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM}, // ADD|OR|ADC|SBB|AND|SUB|XOR
    /* 81 */ {CMP,     am_Ew, am_Iw, am_Xx, DT_MOD_RM_RM_16,   DT_IMM_16,         _,         DF_HAS_MOD_RM}, // ADD|OR|ADC|SBB|AND|SUB|XOR
    /* 82 */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         DF_HAS_MOD_RM}, // Copy of 80??!
    /* 83 */ {CMP,     am_Ew, am_Ib, am_Xx, DT_MOD_RM_RM_16,   DT_IMM_8,          _,         DF_HAS_MOD_RM}, // ADD|OR|ADC|SBB|AND|SUB|XOR
    /* 84 */ {TEST,    am_Eb, am_Gb, am_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 85 */ {TEST,    am_Ew, am_Gw, am_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 86 */ {XCHG,    am_Eb, am_Gb, am_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 87 */ {XCHG,    am_Ew, am_Gw, am_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 88 */ {MOV,     am_Eb, am_Gb, am_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 89 */ {MOV,     am_Ew, am_Gw, am_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 8a */ {MOV,     am_Gb, am_Eb, am_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 8b */ {MOV,     am_Gw, am_Ew, am_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 8c */ {MOV,     am_Ew, am_Sw, am_Xx, DT_MOD_RM_RM_16,   DT_SEGMENT_REG,    _,         DF_HAS_MOD_RM},
    /* 8d */ {LEA,     am_Gw, am_M,  am_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 8e */ {MOV,     am_Sw, am_Ew, am_Xx, DT_SEGMENT_REG,    DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 8f */ {POP,     am_Ew, am_Xx, am_Xx, DT_MOD_RM_RM_16,   _,                 _,         DF_HAS_MOD_RM}, // XOP prefix? ?!?!?

    /* 90 */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* 91 */ {XCHG,    am_AX, am_CX, am_Xx, DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 92 */ {XCHG,    am_AX, am_DX, am_Xx, DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 93 */ {XCHG,    am_AX, am_BX, am_Xx, DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 94 */ {XCHG,    am_AX, am_SP, am_Xx, DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 95 */ {XCHG,    am_AX, am_BP, am_Xx, DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 96 */ {XCHG,    am_AX, am_SI, am_Xx, DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 97 */ {XCHG,    am_AX, am_DI, am_Xx, DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 98 */ {CBW,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* 99 */ {CWD,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* 9a */ {CALLF,   am_Ap, am_Xx, am_Xx, DT_Ap,             _,                 _,         __},
    /* 9b */ {FWAIT,   am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* 9c */ {PUSHF,   am_Fw, am_Xx, am_Xx, DT_Fw,             _,                 _,         __},
    /* 9d */ {POPF,    am_Fw, am_Xx, am_Xx, DT_Fw,             _,                 _,         __},
    /* 9e */ {SAHF,    am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* 9f */ {LAHF,    am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},

    /* a0 */ {MOV,     am_AL, am_Ob, am_Xx, DT_AL,             DT_JMP_16,         _,         __},
    /* a1 */ {MOV,     am_AX, am_Ow, am_Xx, DT_AX,             DT_JMP_16,         _,         __},
    /* a2 */ {MOV,     am_Ob, am_AL, am_Xx, DT_JMP_16,         DT_AL,             _,         __},
    /* a3 */ {MOV,     am_Ow, am_AX, am_Xx, DT_JMP_16,         DT_AX,             _,         __},
    /* a4 */ {MOVSB,   am_Xb, am_Yb, am_Xx, _,                 _,                 _,         __},
    /* a5 */ {MOVSW,   am_Xw, am_Yw, am_Xx, _,                 _,                 _,         __},
    /* a6 */ {CMPSB,   am_Xb, am_Yb, am_Xx, _,                 _,                 _,         __},
    /* a7 */ {CMPSW,   am_Xw, am_Yw, am_Xx, _,                 _,                 _,         __},
    /* a8 */ {TEST,    am_AL, am_Ib, am_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* a9 */ {TEST,    am_AX, am_Iw, am_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* aa */ {STOSB,   am_Yb, am_AL, am_Xx, _,                 _,                 _,         __},            // rep stosb [ES:DI+offset],          al
    /* ab */ {STOSW,   am_Yw, am_AX, am_Xx, _,                 _,                 _,         __},
    /* ac */ {LODSB,   am_AL, am_Xb, am_Xx, _,                 _,                 _,         __},
    /* ad */ {LODSW,   am_AX, am_Xw, am_Xx, _,                 _,                 _,         __},
    /* ae */ {SCASB,   am_AL, am_Yb, am_Xx, _,                 _,                 _,         __},
    /* af */ {SCASW,   am_AX, am_Yw, am_Xx, _,                 _,                 _,         __},

    /* b0 */ {MOV,     am_AL, am_Ib, am_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b1 */ {MOV,     am_CL, am_Ib, am_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b2 */ {MOV,     am_DL, am_Ib, am_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b3 */ {MOV,     am_BL, am_Ib, am_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b4 */ {MOV,     am_AH, am_Ib, am_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b5 */ {MOV,     am_CH, am_Ib, am_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b6 */ {MOV,     am_DH, am_Ib, am_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b7 */ {MOV,     am_BH, am_Ib, am_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b8 */ {MOV,     am_AX, am_Iw, am_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* b9 */ {MOV,     am_CX, am_Iw, am_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* ba */ {MOV,     am_DX, am_Iw, am_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* bb */ {MOV,     am_BX, am_Iw, am_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* bc */ {MOV,     am_BP, am_Iw, am_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* bd */ {MOV,     am_SP, am_Iw, am_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* be */ {MOV,     am_DI, am_Iw, am_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* bf */ {MOV,     am_SI, am_Iw, am_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},

    /* c0 */ {SAR,     am_Eb, am_Ib, am_Xx, DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|SHR|SAL|SAR
    /* c1 */ {SAR,     am_Ew, am_Ib, am_Xx, DT_MOD_RM_RM_16,   DT_IMM_8,          _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|SHR|SAL|SAR
    /* c2 */ {RET,     am_Iw, am_Xx, am_Xx, DT_IMM_16,         _,                 _,         __},
    /* c3 */ {RET,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // no operands?
    /* c4 */ {LES,     am_Gw, am_Ew, am_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* c5 */ {LDS,     am_Gw, am_Ew, am_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* c6 */ {MOV,     am_Eb, am_Ib, am_Xx, DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM},
    /* c7 */ {MOV,     am_Ew, am_Iw, am_Xx, DT_MOD_RM_RM_16,   DT_IMM_16,         _,         DF_HAS_MOD_RM},
    /* c8 */ {ENTER,   am_Iw, am_Ib, am_Xx, DT_IMM_16,         DT_IMM_8,          _,         __},
    /* c9 */ {LEAVE,   am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* ca */ {RETF,    am_Iw, am_Xx, am_Xx, DT_IMM_16,         _,                 _,         __},
    /* cb */ {RETF,    am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* cc */ {INT3,    am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* cd */ {INT,     am_Ib, am_Xx, am_Xx, DT_IMM_8,          _,                 _,         __},
    /* ce */ {INTO,    am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* cf */ {IRET,    am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},

    /* d0 */ {SAR,     am_Eb, am_1,  am_Xx, DT_MOD_RM_RM_8,    1,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d1 */ {SAR,     am_Ew, am_1,  am_Xx, DT_MOD_RM_RM_16,   1,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d2 */ {SAR,     am_Eb, am_Xx, am_Xx, DT_MOD_RM_RM_8,    _,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d3 */ {SAR,     am_Ew, am_Xx, am_Xx, DT_MOD_RM_RM_16,   _,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d4 */ {AAM,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* d5 */ {AAD,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* d6 */ {SALC,    am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* d7 */ {XLAT,    am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* d8 */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // FPU instructions
    /* d9 */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // FPU instructions
    /* da */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // FPU instructions
    /* db */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // FPU instructions
    /* dc */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // FPU instructions
    /* dd */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // FPU instructions
    /* de */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // FPU instructions
    /* df */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // FPU instructions

    /* e0 */ {LOOPNE,  am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* e1 */ {LOOPE,   am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* e2 */ {LOOP,    am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* e3 */ {JCXZ,    am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* e4 */ {IN,      am_AL, am_Ib, am_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* e5 */ {IN,      am_AX, am_Ib, am_Xx, DT_AX,             DT_IMM_8,          _,         __},
    /* e6 */ {OUT,     am_Ib, am_AL, am_Xx, DT_IMM_8,          DT_AL,             _,         __},
    /* e7 */ {OUT,     am_Ib, am_AX, am_Xx, DT_IMM_8,          DT_AX,             _,         __},
    /* e8 */ {CALL,    am_Jw, am_Xx, am_Xx, DT_JMP_16,         _,                 _,         __},
    /* e9 */ {JMP,     am_Jw, am_Xx, am_Xx, DT_JMP_16,         _,                 _,         __},
    /* ea */ {JMP,     am_Ap, am_Xx, am_Xx, DT_SEG_DIRECT,     _,                 _,         __},            // mov cs:ip
    /* eb */ {JMP,     am_Jb, am_Xx, am_Xx, DT_JMP_8,          _,                 _,         __},
    /* ec */ {IN,      am_AL, am_DX, am_Xx, DT_AL,             DT_DX,             _,         __},
    /* ed */ {IN,      am_AX, am_DX, am_Xx, DT_AX,             DT_DX,             _,         __},
    /* ee */ {OUT,     am_DX, am_AL, am_Xx, DT_DX,             DT_AL,             _,         __},
    /* ef */ {OUT,     am_DX, am_AX, am_Xx, DT_DX,             DT_AX,             _,         __},

    /* f0 */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // Lock?
    /* f1 */ {INT1,    am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* f2 */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // repne prefix
    /* f3 */ {INVALID, am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},            // rep prefix
    /* f4 */ {HLT,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* f5 */ {CMC,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* f6 */ {TEST,    am_Eb, am_Ib, am_Xx, DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM}, // TEST|NOT|NEG|MUL|IMUL|DIV|IDIV
    /* f7 */ {TEST,    am_Ew, am_Iw, am_Xx, DT_MOD_RM_RM_16,   DT_IMM_16,         _,         DF_HAS_MOD_RM}, // TEST|NOT|NEG|MUL|IMUL|DIV|IDIV
    /* f8 */ {CLC,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* f9 */ {STC,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* fa */ {CLI,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* fb */ {STI,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* fc */ {CLD,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* fd */ {STD,     am_Xx, am_Xx, am_Xx, _,                 _,                 _,         __},
    /* fe */ {DEC,     am_Eb, am_Xx, am_Xx, DT_MOD_RM_RM_8,    _,                 _,         DF_HAS_MOD_RM},
    /* ff */ {INC,     am_Eb, am_Xx, am_Xx, DT_MOD_RM_RM_8,    _,                 _,         DF_HAS_MOD_RM},
};
// clang-format on
