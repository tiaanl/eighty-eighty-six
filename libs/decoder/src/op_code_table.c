#include "op_code_table.h"

#include "decoder_funcs.h"

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
 * O - The instruction has no ModR/M byte. The offset of the operand is encoded as a word  in the
 *     instruction.
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
struct op_code_mapping op_code_table_group_80[] = {
    /* 00 */ {it_add, am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 01 */ {it_or,  am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 02 */ {it_adc, am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 03 */ {it_sbb, am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 04 */ {it_and, am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 05 */ {it_sub, am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 06 */ {it_div, am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 07 */ {it_cmp, am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
};

struct op_code_mapping op_code_table_group_81[] = {
    /* 00 */ {it_add, am_Ew, am_Iw, am_Xx, decode_Ew_Iw_Xx},
    /* 01 */ {it_or,  am_Ew, am_Iw, am_Xx, decode_Ew_Iw_Xx},
    /* 02 */ {it_adc, am_Ew, am_Iw, am_Xx, decode_Ew_Iw_Xx},
    /* 03 */ {it_sbb, am_Ew, am_Iw, am_Xx, decode_Ew_Iw_Xx},
    /* 04 */ {it_and, am_Ew, am_Iw, am_Xx, decode_Ew_Iw_Xx},
    /* 05 */ {it_sub, am_Ew, am_Iw, am_Xx, decode_Ew_Iw_Xx},
    /* 06 */ {it_div, am_Ew, am_Iw, am_Xx, decode_Ew_Iw_Xx},
    /* 07 */ {it_cmp, am_Ew, am_Iw, am_Xx, decode_Ew_Iw_Xx},
};

struct op_code_mapping op_code_table_group_83[] = {
    /* 00 */ {it_add, am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
    /* 01 */ {it_or,  am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
    /* 02 */ {it_adc, am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
    /* 03 */ {it_sbb, am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
    /* 04 */ {it_and, am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
    /* 05 */ {it_sub, am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
    /* 06 */ {it_div, am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
    /* 07 */ {it_cmp, am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
};

struct op_code_mapping op_code_table_group_c0[] = {
    /* 00 */ {it_rol,     am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 01 */ {it_ror,     am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 02 */ {it_rcl,     am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 03 */ {it_rcr,     am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 04 */ {it_shl,     am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 05 */ {it_shr,     am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 06 */ {it_shl,     am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 07 */ {it_sar,     am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
};

struct op_code_mapping op_code_table_group_c1[] = {
    /* 00 */ {it_rol,     am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
    /* 01 */ {it_ror,     am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
    /* 02 */ {it_rcl,     am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
    /* 03 */ {it_rcr,     am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
    /* 04 */ {it_shl,     am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
    /* 05 */ {it_shr,     am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
    /* 06 */ {it_shl,     am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
    /* 07 */ {it_sar,     am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx},
};

struct op_code_mapping op_code_table_group_d0[] = {
    /* 00 */ {it_rol,     am_Eb, am_1, am_Xx, decode_Eb_1_Xx},
    /* 01 */ {it_ror,     am_Eb, am_1, am_Xx, decode_Eb_1_Xx},
    /* 02 */ {it_rcl,     am_Eb, am_1, am_Xx, decode_Eb_1_Xx},
    /* 03 */ {it_rcr,     am_Eb, am_1, am_Xx, decode_Eb_1_Xx},
    /* 04 */ {it_shl,     am_Eb, am_1, am_Xx, decode_Eb_1_Xx},
    /* 05 */ {it_shr,     am_Eb, am_1, am_Xx, decode_Eb_1_Xx},
    /* 06 */ {it_shl,     am_Eb, am_1, am_Xx, decode_Eb_1_Xx},
    /* 07 */ {it_sar,     am_Eb, am_1, am_Xx, decode_Eb_1_Xx},
};

struct op_code_mapping op_code_table_group_d1[] = {
    /* 00 */ {it_rol,     am_Ew, am_1, am_Xx, decode_Ew_1_Xx},
    /* 01 */ {it_ror,     am_Ew, am_1, am_Xx, decode_Ew_1_Xx},
    /* 02 */ {it_rcl,     am_Ew, am_1, am_Xx, decode_Ew_1_Xx},
    /* 03 */ {it_rcr,     am_Ew, am_1, am_Xx, decode_Ew_1_Xx},
    /* 04 */ {it_shl,     am_Ew, am_1, am_Xx, decode_Ew_1_Xx},
    /* 05 */ {it_shr,     am_Ew, am_1, am_Xx, decode_Ew_1_Xx},
    /* 06 */ {it_shl,     am_Ew, am_1, am_Xx, decode_Ew_1_Xx},
    /* 07 */ {it_sar,     am_Ew, am_1, am_Xx, decode_Ew_1_Xx},
};

struct op_code_mapping op_code_table_group_d2[] = {
    /* 00 */ {it_rol,     am_Eb, am_Xx, am_Xx, decode_Eb_Xx_Xx},
    /* 01 */ {it_ror,     am_Eb, am_Xx, am_Xx, decode_Eb_Xx_Xx},
    /* 02 */ {it_rcl,     am_Eb, am_Xx, am_Xx, decode_Eb_Xx_Xx},
    /* 03 */ {it_rcr,     am_Eb, am_Xx, am_Xx, decode_Eb_Xx_Xx},
    /* 04 */ {it_shl,     am_Eb, am_Xx, am_Xx, decode_Eb_Xx_Xx},
    /* 05 */ {it_shr,     am_Eb, am_Xx, am_Xx, decode_Eb_Xx_Xx},
    /* 06 */ {it_shl,     am_Eb, am_Xx, am_Xx, decode_Eb_Xx_Xx},
    /* 07 */ {it_sar,     am_Eb, am_Xx, am_Xx, decode_Eb_Xx_Xx},
};

struct op_code_mapping op_code_table_group_d3[] = {
    /* 00 */ {it_rol,     am_Ew, am_Xx, am_Xx, decode_Ew_Xx_Xx},
    /* 01 */ {it_ror,     am_Ew, am_Xx, am_Xx, decode_Ew_Xx_Xx},
    /* 02 */ {it_rcl,     am_Ew, am_Xx, am_Xx, decode_Ew_Xx_Xx},
    /* 03 */ {it_rcr,     am_Ew, am_Xx, am_Xx, decode_Ew_Xx_Xx},
    /* 04 */ {it_shl,     am_Ew, am_Xx, am_Xx, decode_Ew_Xx_Xx},
    /* 05 */ {it_shr,     am_Ew, am_Xx, am_Xx, decode_Ew_Xx_Xx},
    /* 06 */ {it_shl,     am_Ew, am_Xx, am_Xx, decode_Ew_Xx_Xx},
    /* 07 */ {it_sar,     am_Ew, am_Xx, am_Xx, decode_Ew_Xx_Xx},
};

struct op_code_mapping op_code_table_group_f6[] = {
    /* 00 */ {it_test,    am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 01 */ {it_test,    am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* 02 */ {it_not,     am_Eb, am_Xx, am_Xx, decode_Eb_Xx_Xx},
    /* 03 */ {it_neg,     am_Eb, am_Xx, am_Xx, decode_Eb_Xx_Xx},
    /* 04 */ {it_mul,     am_AL, am_Eb, am_Xx, decode_AL_Eb_Xx},
    /* 05 */ {it_imul,    am_AL, am_Eb, am_Xx, decode_AL_Eb_Xx},
    /* 06 */ {it_div,     am_AL, am_Eb, am_Xx, decode_AL_Eb_Xx},
    /* 07 */ {it_idiv,    am_AL, am_Eb, am_Xx, decode_AL_Eb_Xx},
};

struct op_code_mapping op_code_table_group_f7[] = {
    /* 00 */ {it_test,    am_Ew, am_Iw, am_Xx, decode_Ew_Iw_Xx},
    /* 01 */ {it_test,    am_Ew, am_Iw, am_Xx, decode_Ew_Iw_Xx},
    /* 02 */ {it_not,     am_Ew, am_Xx, am_Xx, decode_Ew_Xx_Xx},
    /* 03 */ {it_neg,     am_Ew, am_Xx, am_Xx, decode_Ew_Xx_Xx},
    /* 04 */ {it_mul,     am_AX, am_Ew, am_Xx, decode_AX_Ew_Xx},
    /* 05 */ {it_imul,    am_AX, am_Ew, am_Xx, decode_AX_Ew_Xx},
    /* 06 */ {it_div,     am_AX, am_Ew, am_Xx, decode_AX_Ew_Xx},
    /* 07 */ {it_idiv,    am_AX, am_Ew, am_Xx, decode_AX_Ew_Xx},
};

struct op_code_mapping op_code_table[] = {
    /* 00 */ {it_add,      am_Eb,               am_Gb, am_Xx, decode_Eb_Gb_Xx},
    /* 01 */ {it_add,      am_Ew,               am_Gw, am_Xx, decode_Ew_Gw_Xx},
    /* 02 */ {it_add,      am_Gb,               am_Eb, am_Xx, decode_Gb_Eb_Xx},
    /* 03 */ {it_add,      am_Gw,               am_Ew, am_Xx, decode_Gw_Ew_Xx},
    /* 04 */ {it_add,      am_AL,               am_Ib, am_Xx, decode_AL_Ib_Xx},
    /* 05 */ {it_add,      am_AX,               am_Iw, am_Xx, decode_AX_Iw_Xx},
    /* 06 */ {it_push,     am_ES,               am_Xx, am_Xx, decode_ES_Xx_Xx},
    /* 07 */ {it_pop,      am_ES,               am_Xx, am_Xx, decode_ES_Xx_Xx},
    /* 08 */ {it_or,       am_Eb,               am_Gb, am_Xx, decode_Eb_Gb_Xx},
    /* 09 */ {it_or,       am_Ew,               am_Gw, am_Xx, decode_Ew_Gw_Xx},
    /* 0a */ {it_or,       am_Gb,               am_Eb, am_Xx, decode_Gb_Eb_Xx},
    /* 0b */ {it_or,       am_Gw,               am_Ew, am_Xx, decode_Gw_Ew_Xx},
    /* 0c */ {it_or,       am_AL,               am_Ib, am_Xx, decode_AL_Ib_Xx},
    /* 0d */ {it_or,       am_AX,               am_Iw, am_Xx, decode_AX_Iw_Xx},
    /* 0e */ {it_push,     am_CS,               am_Xx, am_Xx, decode_CS_Xx_Xx},
    /* 0f */ {it_invalid}, // Extended op codes.

    /* 10 */ {it_adc,      am_Eb,               am_Gb, am_Xx, decode_Eb_Gb_Xx},
    /* 11 */ {it_adc,      am_Ew,               am_Gw, am_Xx, decode_Ew_Gw_Xx},
    /* 12 */ {it_adc,      am_Gb,               am_Eb, am_Xx, decode_Gb_Eb_Xx},
    /* 13 */ {it_adc,      am_Gw,               am_Ew, am_Xx, decode_Gw_Ew_Xx},
    /* 14 */ {it_adc,      am_AL,               am_Ib, am_Xx, decode_AL_Ib_Xx},
    /* 15 */ {it_adc,      am_AX,               am_Iw, am_Xx, decode_AX_Iw_Xx},
    /* 16 */ {it_push,     am_SS,               am_Xx, am_Xx, decode_SS_Xx_Xx},
    /* 17 */ {it_pop,      am_SS,               am_Xx, am_Xx, decode_SS_Xx_Xx},
    /* 18 */ {it_sbb,      am_Eb,               am_Gb, am_Xx, decode_Eb_Gb_Xx},
    /* 19 */ {it_sbb,      am_Ew,               am_Gw, am_Xx, decode_Ew_Gw_Xx},
    /* 1a */ {it_sbb,      am_Gb,               am_Eb, am_Xx, decode_Gb_Eb_Xx},
    /* 1b */ {it_sbb,      am_Gw,               am_Ew, am_Xx, decode_Gw_Ew_Xx},
    /* 1c */ {it_sbb,      am_AL,               am_Ib, am_Xx, decode_AL_Ib_Xx},
    /* 1d */ {it_sbb,      am_AX,               am_Iw, am_Xx, decode_AX_Iw_Xx},
    /* 1e */ {it_push,     am_DS,               am_Xx, am_Xx, decode_DS_Xx_Xx},
    /* 1f */ {it_pop,      am_DS,               am_Xx, am_Xx, decode_DS_Xx_Xx},

    /* 20 */ {it_and,      am_Eb,               am_Gb, am_Xx, decode_Eb_Gb_Xx},
    /* 21 */ {it_and,      am_Ew,               am_Gw, am_Xx, decode_Ew_Gw_Xx},
    /* 22 */ {it_and,      am_Gb,               am_Eb, am_Xx, decode_Gb_Eb_Xx},
    /* 23 */ {it_and,      am_Gw,               am_Ew, am_Xx, decode_Gw_Ew_Xx},
    /* 24 */ {it_and,      am_AL,               am_Ib, am_Xx, decode_AL_Ib_Xx},
    /* 25 */ {it_and,      am_AX,               am_Iw, am_Xx, decode_AX_Iw_Xx},
    /* 26 */ {it_prefix,   am_Xx,               am_Xx, am_Xx, decode_seg_reg_override_es}, // Segment register override (ES)
    /* 27 */ {it_daa,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* 28 */ {it_sub,      am_Eb,               am_Gb, am_Xx, decode_Eb_Gb_Xx},
    /* 29 */ {it_sub,      am_Ew,               am_Gw, am_Xx, decode_Ew_Gw_Xx},
    /* 2a */ {it_sub,      am_Gb,               am_Eb, am_Xx, decode_Gb_Eb_Xx},
    /* 2b */ {it_sub,      am_Gw,               am_Ew, am_Xx, decode_Gw_Ew_Xx},
    /* 2c */ {it_sub,      am_AL,               am_Ib, am_Xx, decode_AL_Ib_Xx},
    /* 2d */ {it_sub,      am_AX,               am_Iw, am_Xx, decode_AX_Iw_Xx},
    /* 2e */ {it_prefix,   am_Xx,               am_Xx, am_Xx, decode_seg_reg_override_cs}, // Segment register override (CS)
    /* 2f */ {it_das,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},

    /* 30 */ {it_xor,      am_Eb,               am_Gb, am_Xx, decode_Eb_Gb_Xx},
    /* 31 */ {it_xor,      am_Ew,               am_Gw, am_Xx, decode_Ew_Gw_Xx},
    /* 32 */ {it_xor,      am_Gb,               am_Eb, am_Xx, decode_Gb_Eb_Xx},
    /* 33 */ {it_xor,      am_Gw,               am_Ew, am_Xx, decode_Gw_Ew_Xx},
    /* 34 */ {it_xor,      am_AL,               am_Ib, am_Xx, decode_AL_Ib_Xx},
    /* 35 */ {it_xor,      am_AX,               am_Iw, am_Xx, decode_AX_Iw_Xx},
    /* 36 */ {it_prefix,   am_Xx,               am_Xx, am_Xx, decode_seg_reg_override_ss}, // Segment register override (SS)
    /* 37 */ {it_aaa,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* 38 */ {it_cmp,      am_Eb,               am_Gb, am_Xx, decode_Eb_Gb_Xx},
    /* 39 */ {it_cmp,      am_Ew,               am_Gw, am_Xx, decode_Ew_Gw_Xx},
    /* 3a */ {it_cmp,      am_Gb,               am_Eb, am_Xx, decode_Gb_Eb_Xx},
    /* 3b */ {it_cmp,      am_Gw,               am_Ew, am_Xx, decode_Gw_Ew_Xx},
    /* 3c */ {it_cmp,      am_AL,               am_Ib, am_Xx, decode_AL_Ib_Xx},
    /* 3d */ {it_cmp,      am_AX,               am_Iw, am_Xx, decode_AX_Iw_Xx},
    /* 3e */ {it_prefix,   am_Xx,               am_Xx, am_Xx, decode_seg_reg_override_ds}, // Segment register override (DS)
    /* 3f */ {it_aas,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},

    /* 40 */ {it_inc,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 41 */ {it_inc,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 42 */ {it_inc,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 43 */ {it_inc,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 44 */ {it_inc,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 45 */ {it_inc,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 46 */ {it_inc,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 47 */ {it_inc,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 48 */ {it_dec,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 49 */ {it_dec,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 4a */ {it_dec,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 4b */ {it_dec,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 4c */ {it_dec,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 4d */ {it_dec,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 4e */ {it_dec,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 4f */ {it_dec,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},

    /* 50 */ {it_push,     am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 51 */ {it_push,     am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 52 */ {it_push,     am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 53 */ {it_push,     am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 54 */ {it_push,     am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 55 */ {it_push,     am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 56 */ {it_push,     am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 57 */ {it_push,     am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 58 */ {it_pop,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 59 */ {it_pop,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 5a */ {it_pop,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 5b */ {it_pop,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 5c */ {it_pop,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 5d */ {it_pop,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 5e */ {it_pop,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},
    /* 5f */ {it_pop,      am_Rw,               am_Xx, am_Xx, decode_Rw_Xx_Xx},

    /* 60 */ {it_pusha,    am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* 61 */ {it_popa,     am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* 62 */ {it_bound,    am_Gw,               am_Ma, am_Xx, decode_Gw_Ma_Xx},
    /* 63 */ {it_arpl,     am_Ew,               am_Gw, am_Xx, decode_Ew_Gw_Xx},
    /* 64 */ {it_invalid},
    /* 65 */ {it_invalid},
    /* 66 */ {it_prefix,   am_Xx,               am_Xx, am_Xx, 0},                          // Operand size override
    /* 67 */ {it_prefix,   am_Xx,               am_Xx, am_Xx, 0},                          // Operand address size override
    /* 68 */ {it_push,     am_Iw,               am_Xx, am_Xx, decode_Iw_Xx_Xx},
    /* 69 */ {it_imul,     am_Gw,               am_Ew, am_Iw, decode_Gw_Ew_Iw},
    /* 6a */ {it_push,     am_Ib,               am_Xx, am_Xx, decode_Ib_Xx_Xx},
    /* 6b */ {it_imul,     am_Gw,               am_Ew, am_Ib, decode_Gw_Ew_Ib},
    /* 6c */ {it_insb,     am_Yb,               am_DX, am_Xx, decode_Yb_DX_Xx},
    /* 6d */ {it_insw,     am_Yw,               am_DX, am_Xx, decode_Yw_DX_Xx},
    /* 6e */ {it_outs,     am_DX,               am_Xb, am_Xx, decode_DX_Xb_Xx},
    /* 6f */ {it_outs,     am_DX,               am_Xw, am_Xx, decode_DX_Xw_Xx},

    /* 70 */ {it_jo,       am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* 71 */ {it_jno,      am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* 72 */ {it_jb,       am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* 73 */ {it_jnb,      am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* 74 */ {it_jz,       am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* 75 */ {it_jnz,      am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* 76 */ {it_jbe,      am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* 77 */ {it_jnbe,     am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* 78 */ {it_js,       am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* 79 */ {it_jns,      am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* 7a */ {it_jp,       am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* 7b */ {it_jnp,      am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* 7c */ {it_jl,       am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* 7d */ {it_jnl,      am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* 7e */ {it_jle,      am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* 7f */ {it_jnle,     am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},

    /* 80 */ {it_group,    am_Xx,               am_Xx, am_Xx, 0, op_code_table_group_80},
    /* 81 */ {it_group,    am_Xx,               am_Xx, am_Xx, 0, op_code_table_group_81},
    /* 82 */ {it_invalid}, // Should this be invalid or same as 80?
    /* 83 */ {it_group,    am_Xx,               am_Xx, am_Xx, 0, op_code_table_group_83},
    /* 84 */ {it_test,     am_Eb,               am_Gb, am_Xx, decode_Eb_Gb_Xx},
    /* 85 */ {it_test,     am_Ew,               am_Gw, am_Xx, decode_Ew_Gw_Xx},
    /* 86 */ {it_xchg,     am_Eb,               am_Gb, am_Xx, decode_Eb_Gb_Xx},
    /* 87 */ {it_xchg,     am_Ew,               am_Gw, am_Xx, decode_Ew_Gw_Xx},
    /* 88 */ {it_mov,      am_Eb,               am_Gb, am_Xx, decode_Eb_Gb_Xx},
    /* 89 */ {it_mov,      am_Ew,               am_Gw, am_Xx, decode_Ew_Gw_Xx},
    /* 8a */ {it_mov,      am_Gb,               am_Eb, am_Xx, decode_Gb_Eb_Xx},
    /* 8b */ {it_mov,      am_Gw,               am_Ew, am_Xx, decode_Gw_Ew_Xx},
    /* 8c */ {it_mov,      am_Ew,               am_Sw, am_Xx, decode_Ew_Sw_Xx},
    /* 8d */ {it_lea,      am_Gw,               am_M,  am_Xx, decode_Gw_M_Xx},
    /* 8e */ {it_mov,      am_Sw,               am_Ew, am_Xx, decode_Sw_Ew_Xx},
    /* 8f */ {it_pop,      am_Ew,               am_Xx, am_Xx, decode_Ew_Xx_Xx},

    /* 90 */ {it_invalid},
    /* 91 */ {it_xchg,     am_AX,               am_Rw, am_Xx, decode_AX_Rw_Xx},
    /* 92 */ {it_xchg,     am_AX,               am_Rw, am_Xx, decode_AX_Rw_Xx},
    /* 93 */ {it_xchg,     am_AX,               am_Rw, am_Xx, decode_AX_Rw_Xx},
    /* 94 */ {it_xchg,     am_AX,               am_Rw, am_Xx, decode_AX_Rw_Xx},
    /* 95 */ {it_xchg,     am_AX,               am_Rw, am_Xx, decode_AX_Rw_Xx},
    /* 96 */ {it_xchg,     am_AX,               am_Rw, am_Xx, decode_AX_Rw_Xx},
    /* 97 */ {it_xchg,     am_AX,               am_Rw, am_Xx, decode_AX_Rw_Xx},
    /* 98 */ {it_cbw,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* 99 */ {it_cwd,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* 9a */ {it_callf,    am_Ap,               am_Xx, am_Xx, decode_Ap_Xx_Xx},
    /* 9b */ {it_fwait,    am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* 9c */ {it_pushf,    am_Fw,               am_Xx, am_Xx, decode_Fw_Xx_Xx},
    /* 9d */ {it_popf,     am_Fw,               am_Xx, am_Xx, decode_Fw_Xx_Xx},
    /* 9e */ {it_sahf,     am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* 9f */ {it_lahf,     am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},

    /* a0 */ {it_mov,      am_AL,               am_Ob, am_Xx, decode_AL_Ob_Xx},
    /* a1 */ {it_mov,      am_AX,               am_Ow, am_Xx, decode_AX_Ow_Xx},
    /* a2 */ {it_mov,      am_Ob,               am_AL, am_Xx, decode_Ob_AL_Xx},
    /* a3 */ {it_mov,      am_Ow,               am_AX, am_Xx, decode_Ow_AX_Xx},
    /* a4 */ {it_movs,     am_Xb,               am_Yb, am_Xx, decode_Xb_Yb_Xx},
    /* a5 */ {it_movs,     am_Xw,               am_Yw, am_Xx, decode_Xw_Yw_Xx},
    /* a6 */ {it_cmps,     am_Xb,               am_Yb, am_Xx, decode_Xb_Yb_Xx},
    /* a7 */ {it_cmps,     am_Xw,               am_Yw, am_Xx, decode_Xw_Yw_Xx},
    /* a8 */ {it_test,     am_AL,               am_Ib, am_Xx, decode_AL_Ib_Xx},
    /* a9 */ {it_test,     am_AX,               am_Iw, am_Xx, decode_AX_Iw_Xx},
    /* aa */ {it_stos,     am_Yb,               am_AL, am_Xx, decode_Yb_AL_Xx},            // rep stosb [ES:DI+offset], al
    /* ab */ {it_stos,     am_Yw,               am_AX, am_Xx, decode_Yw_AX_Xx},
    /* ac */ {it_lods,     am_AL,               am_Xb, am_Xx, decode_AL_Xb_Xx},
    /* ad */ {it_lods,     am_AX,               am_Xw, am_Xx, decode_AX_Xw_Xx},
    /* ae */ {it_scas,     am_AL,               am_Yb, am_Xx, decode_AL_Yb_Xx},
    /* af */ {it_scas,     am_AX,               am_Yw, am_Xx, decode_AX_Yw_Xx},

    /* b0 */ {it_mov,      am_Rb,               am_Ib, am_Xx, decode_Rb_Ib_Xx},
    /* b1 */ {it_mov,      am_Rb,               am_Ib, am_Xx, decode_Rb_Ib_Xx},
    /* b2 */ {it_mov,      am_Rb,               am_Ib, am_Xx, decode_Rb_Ib_Xx},
    /* b3 */ {it_mov,      am_Rb,               am_Ib, am_Xx, decode_Rb_Ib_Xx},
    /* b4 */ {it_mov,      am_Rb,               am_Ib, am_Xx, decode_Rb_Ib_Xx},
    /* b5 */ {it_mov,      am_Rb,               am_Ib, am_Xx, decode_Rb_Ib_Xx},
    /* b6 */ {it_mov,      am_Rb,               am_Ib, am_Xx, decode_Rb_Ib_Xx},
    /* b7 */ {it_mov,      am_Rb,               am_Ib, am_Xx, decode_Rb_Ib_Xx},
    /* b8 */ {it_mov,      am_Rw,               am_Iw, am_Xx, decode_Rw_Iw_Xx},
    /* b9 */ {it_mov,      am_Rw,               am_Iw, am_Xx, decode_Rw_Iw_Xx},
    /* ba */ {it_mov,      am_Rw,               am_Iw, am_Xx, decode_Rw_Iw_Xx},
    /* bb */ {it_mov,      am_Rw,               am_Iw, am_Xx, decode_Rw_Iw_Xx},
    /* bc */ {it_mov,      am_Rw,               am_Iw, am_Xx, decode_Rw_Iw_Xx},
    /* bd */ {it_mov,      am_Rw,               am_Iw, am_Xx, decode_Rw_Iw_Xx},
    /* be */ {it_mov,      am_Rw,               am_Iw, am_Xx, decode_Rw_Iw_Xx},
    /* bf */ {it_mov,      am_Rw,               am_Iw, am_Xx, decode_Rw_Iw_Xx},

    /* c0 */ {it_group,    am_Xx,               am_Xx, am_Xx, 0, op_code_table_group_c0},
    /* c1 */ {it_group,    am_Xx,               am_Xx, am_Xx, 0, op_code_table_group_c1},
    /* c2 */ {it_ret,      am_Iw,               am_Xx, am_Xx, decode_Iw_Xx_Xx},
    /* c3 */ {it_ret,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* c4 */ {it_les,      am_Gw,               am_Ew, am_Xx, decode_Gw_Ew_Xx},
    /* c5 */ {it_lds,      am_Gw,               am_Ew, am_Xx, decode_Gw_Ew_Xx},
    /* c6 */ {it_mov,      am_Eb,               am_Ib, am_Xx, decode_Eb_Ib_Xx},
    /* c7 */ {it_mov,      am_Ew,               am_Iw, am_Xx, decode_Ew_Iw_Xx},
    /* c8 */ {it_enter,    am_Iw,               am_Ib, am_Xx, decode_Iw_Ib_Xx},
    /* c9 */ {it_leave,    am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* ca */ {it_retf,     am_Iw,               am_Xx, am_Xx, decode_Iw_Xx_Xx},
    /* cb */ {it_retf,     am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* cc */ {it_int3,     am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* cd */ {it_int,      am_Ib,               am_Xx, am_Xx, decode_Ib_Xx_Xx},
    /* ce */ {it_into,     am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* cf */ {it_iret,     am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},

    /* d0 */ {it_group,    am_Xx,               am_Xx, am_Xx, 0, op_code_table_group_d0},
    /* d1 */ {it_group,    am_Xx,               am_Xx, am_Xx, 0, op_code_table_group_d1},
    /* d2 */ {it_group,    am_Xx,               am_Xx, am_Xx, 0, op_code_table_group_d2},
    /* d3 */ {it_group,    am_Xx,               am_Xx, am_Xx, 0, op_code_table_group_d3},
    /* d4 */ {it_aam,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* d5 */ {it_aad,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* d6 */ {it_salc,     am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* d7 */ {it_xlat,     am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* d8 */ {it_invalid}, // FPU instructions
    /* d9 */ {it_invalid}, // FPU instructions
    /* da */ {it_invalid}, // FPU instructions
    /* db */ {it_invalid}, // FPU instructions
    /* dc */ {it_invalid}, // FPU instructions
    /* dd */ {it_invalid}, // FPU instructions
    /* de */ {it_invalid}, // FPU instructions
    /* df */ {it_invalid}, // FPU instructions

    /* e0 */ {it_loopne,   am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* e1 */ {it_loope,    am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* e2 */ {it_loop,     am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* e3 */ {it_jcxz,     am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* e4 */ {it_in,       am_AL,               am_Ib, am_Xx, decode_AL_Ib_Xx},
    /* e5 */ {it_in,       am_AX,               am_Ib, am_Xx, decode_AX_Ib_Xx},
    /* e6 */ {it_out,      am_Ib,               am_AL, am_Xx, decode_Ib_AL_Xx},
    /* e7 */ {it_out,      am_Ib,               am_AX, am_Xx, decode_Ib_AX_Xx},
    /* e8 */ {it_call,     am_Jw,               am_Xx, am_Xx, decode_Jw_Xx_Xx},
    /* e9 */ {it_jmp,      am_Jw,               am_Xx, am_Xx, decode_Jw_Xx_Xx},
    /* ea */ {it_jmp,      am_Ap,               am_Xx, am_Xx, decode_Ap_Xx_Xx},            // mov cs:ip
    /* eb */ {it_jmp,      am_Jb,               am_Xx, am_Xx, decode_Jb_Xx_Xx},
    /* ec */ {it_in,       am_AL,               am_DX, am_Xx, decode_AL_DX_Xx},
    /* ed */ {it_in,       am_AX,               am_DX, am_Xx, decode_AX_DX_Xx},
    /* ee */ {it_out,      am_DX,               am_AL, am_Xx, decode_DX_AL_Xx},
    /* ef */ {it_out,      am_DX,               am_AX, am_Xx, decode_DX_AX_Xx},

    /* f0 */ {it_invalid}, // Lock?
    /* f1 */ {it_int1,     am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* f2 */ {it_prefix,   am_Xx,               am_Xx, am_Xx, decode_rep_prefix},          // repne prefix
    /* f3 */ {it_prefix,   am_Xx,               am_Xx, am_Xx, decode_repne_prefix},        // rep prefix
    /* f4 */ {it_hlt,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* f5 */ {it_cmc,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* f6 */ {it_group,    am_Eb,               am_Ib, am_Xx, 0, op_code_table_group_f6},
    /* f7 */ {it_group,    am_Xx,               am_Xx, am_Xx, 0, op_code_table_group_f7},
    /* f8 */ {it_clc,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* f9 */ {it_stc,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* fa */ {it_cli,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* fb */ {it_sti,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* fc */ {it_cld,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* fd */ {it_std,      am_Xx,               am_Xx, am_Xx, decode_Xx_Xx_Xx},
    /* fe */ {it_dec,      am_Eb,               am_Xx, am_Xx, decode_Eb_Xx_Xx},
    /* ff */ {it_inc,      am_Eb,               am_Xx, am_Xx, decode_Eb_Xx_Xx},
};
// clang-format on
