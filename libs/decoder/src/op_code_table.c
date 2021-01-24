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

#define _ 0

// clang-format off
struct op_code_mapping op_code_table_group_80[] = {
    /* 00 */ {oct_instruction, it_add, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 01 */ {oct_instruction, it_or,  ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 02 */ {oct_instruction, it_adc, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 03 */ {oct_instruction, it_sbb, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 04 */ {oct_instruction, it_and, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 05 */ {oct_instruction, it_sub, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 06 */ {oct_instruction, it_div, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 07 */ {oct_instruction, it_cmp, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
};

struct op_code_mapping op_code_table_group_81[] = {
    /* 00 */ {oct_instruction, it_add, ids_word, am_Ew, am_Iw, _    , decode_Ew_Iw_Xx},
    /* 01 */ {oct_instruction, it_or,  ids_word, am_Ew, am_Iw, _    , decode_Ew_Iw_Xx},
    /* 02 */ {oct_instruction, it_adc, ids_word, am_Ew, am_Iw, _    , decode_Ew_Iw_Xx},
    /* 03 */ {oct_instruction, it_sbb, ids_word, am_Ew, am_Iw, _    , decode_Ew_Iw_Xx},
    /* 04 */ {oct_instruction, it_and, ids_word, am_Ew, am_Iw, _    , decode_Ew_Iw_Xx},
    /* 05 */ {oct_instruction, it_sub, ids_word, am_Ew, am_Iw, _    , decode_Ew_Iw_Xx},
    /* 06 */ {oct_instruction, it_div, ids_word, am_Ew, am_Iw, _    , decode_Ew_Iw_Xx},
    /* 07 */ {oct_instruction, it_cmp, ids_word, am_Ew, am_Iw, _    , decode_Ew_Iw_Xx},
};

struct op_code_mapping op_code_table_group_83[] = {
    /* 00 */ {oct_instruction, it_add, ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
    /* 01 */ {oct_instruction, it_or,  ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
    /* 02 */ {oct_instruction, it_adc, ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
    /* 03 */ {oct_instruction, it_sbb, ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
    /* 04 */ {oct_instruction, it_and, ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
    /* 05 */ {oct_instruction, it_sub, ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
    /* 06 */ {oct_instruction, it_div, ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
    /* 07 */ {oct_instruction, it_cmp, ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
};

struct op_code_mapping op_code_table_group_c0[] = {
    /* 00 */ {oct_instruction, it_rol, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 01 */ {oct_instruction, it_ror, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 02 */ {oct_instruction, it_rcl, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 03 */ {oct_instruction, it_rcr, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 04 */ {oct_instruction, it_shl, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 05 */ {oct_instruction, it_shr, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 06 */ {oct_instruction, it_shl, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 07 */ {oct_instruction, it_sar, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
};

struct op_code_mapping op_code_table_group_c1[] = {
    /* 00 */ {oct_instruction, it_rol, ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
    /* 01 */ {oct_instruction, it_ror, ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
    /* 02 */ {oct_instruction, it_rcl, ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
    /* 03 */ {oct_instruction, it_rcr, ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
    /* 04 */ {oct_instruction, it_shl, ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
    /* 05 */ {oct_instruction, it_shr, ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
    /* 06 */ {oct_instruction, it_shl, ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
    /* 07 */ {oct_instruction, it_sar, ids_word, am_Ew, am_Ib, _    , decode_Ew_Ib_Xx},
};

struct op_code_mapping op_code_table_group_d0[] = {
    /* 00 */ {oct_instruction, it_rol, ids_byte, am_Eb, am_1, _    , decode_Eb_1_Xx},
    /* 01 */ {oct_instruction, it_ror, ids_byte, am_Eb, am_1, _    , decode_Eb_1_Xx},
    /* 02 */ {oct_instruction, it_rcl, ids_byte, am_Eb, am_1, _    , decode_Eb_1_Xx},
    /* 03 */ {oct_instruction, it_rcr, ids_byte, am_Eb, am_1, _    , decode_Eb_1_Xx},
    /* 04 */ {oct_instruction, it_shl, ids_byte, am_Eb, am_1, _    , decode_Eb_1_Xx},
    /* 05 */ {oct_instruction, it_shr, ids_byte, am_Eb, am_1, _    , decode_Eb_1_Xx},
    /* 06 */ {oct_instruction, it_shl, ids_byte, am_Eb, am_1, _    , decode_Eb_1_Xx},
    /* 07 */ {oct_instruction, it_sar, ids_byte, am_Eb, am_1, _    , decode_Eb_1_Xx},
};

struct op_code_mapping op_code_table_group_d1[] = {
    /* 00 */ {oct_instruction, it_rol, ids_word, am_Ew, am_1, _    , decode_Ew_1_Xx},
    /* 01 */ {oct_instruction, it_ror, ids_word, am_Ew, am_1, _    , decode_Ew_1_Xx},
    /* 02 */ {oct_instruction, it_rcl, ids_word, am_Ew, am_1, _    , decode_Ew_1_Xx},
    /* 03 */ {oct_instruction, it_rcr, ids_word, am_Ew, am_1, _    , decode_Ew_1_Xx},
    /* 04 */ {oct_instruction, it_shl, ids_word, am_Ew, am_1, _    , decode_Ew_1_Xx},
    /* 05 */ {oct_instruction, it_shr, ids_word, am_Ew, am_1, _    , decode_Ew_1_Xx},
    /* 06 */ {oct_instruction, it_shl, ids_word, am_Ew, am_1, _    , decode_Ew_1_Xx},
    /* 07 */ {oct_instruction, it_sar, ids_word, am_Ew, am_1, _    , decode_Ew_1_Xx},
};

struct op_code_mapping op_code_table_group_d2[] = {
    /* 00 */ {oct_instruction, it_rol, ids_byte, am_Eb, _    , _    , decode_Eb_Xx_Xx},
    /* 01 */ {oct_instruction, it_ror, ids_byte, am_Eb, _    , _    , decode_Eb_Xx_Xx},
    /* 02 */ {oct_instruction, it_rcl, ids_byte, am_Eb, _    , _    , decode_Eb_Xx_Xx},
    /* 03 */ {oct_instruction, it_rcr, ids_byte, am_Eb, _    , _    , decode_Eb_Xx_Xx},
    /* 04 */ {oct_instruction, it_shl, ids_byte, am_Eb, _    , _    , decode_Eb_Xx_Xx},
    /* 05 */ {oct_instruction, it_shr, ids_byte, am_Eb, _    , _    , decode_Eb_Xx_Xx},
    /* 06 */ {oct_instruction, it_shl, ids_byte, am_Eb, _    , _    , decode_Eb_Xx_Xx},
    /* 07 */ {oct_instruction, it_sar, ids_byte, am_Eb, _    , _    , decode_Eb_Xx_Xx},
};

struct op_code_mapping op_code_table_group_d3[] = {
    /* 00 */ {oct_instruction, it_rol, ids_word, am_Ew, _    , _    , decode_Ew_Xx_Xx},
    /* 01 */ {oct_instruction, it_ror, ids_word, am_Ew, _    , _    , decode_Ew_Xx_Xx},
    /* 02 */ {oct_instruction, it_rcl, ids_word, am_Ew, _    , _    , decode_Ew_Xx_Xx},
    /* 03 */ {oct_instruction, it_rcr, ids_word, am_Ew, _    , _    , decode_Ew_Xx_Xx},
    /* 04 */ {oct_instruction, it_shl, ids_word, am_Ew, _    , _    , decode_Ew_Xx_Xx},
    /* 05 */ {oct_instruction, it_shr, ids_word, am_Ew, _    , _    , decode_Ew_Xx_Xx},
    /* 06 */ {oct_instruction, it_shl, ids_word, am_Ew, _    , _    , decode_Ew_Xx_Xx},
    /* 07 */ {oct_instruction, it_sar, ids_word, am_Ew, _    , _    , decode_Ew_Xx_Xx},
};

struct op_code_mapping op_code_table_group_f6[] = {
    /* 00 */ {oct_instruction, it_test, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 01 */ {oct_instruction, it_test, ids_byte, am_Eb, am_Ib, _    , decode_Eb_Ib_Xx},
    /* 02 */ {oct_instruction, it_not,  ids_byte, am_Eb, _    , _    , decode_Eb_Xx_Xx},
    /* 03 */ {oct_instruction, it_neg,  ids_byte, am_Eb, _    , _    , decode_Eb_Xx_Xx},
    /* 04 */ {oct_instruction, it_mul,  ids_byte, am_AL, am_Eb, _    , decode_AL_Eb_Xx},
    /* 05 */ {oct_instruction, it_imul, ids_byte, am_AL, am_Eb, _    , decode_AL_Eb_Xx},
    /* 06 */ {oct_instruction, it_div,  ids_byte, am_AL, am_Eb, _    , decode_AL_Eb_Xx},
    /* 07 */ {oct_instruction, it_idiv, ids_byte, am_AL, am_Eb, _    , decode_AL_Eb_Xx},
};

struct op_code_mapping op_code_table_group_f7[] = {
    /* 00 */ {oct_instruction, it_test, ids_word, am_Ew, am_Iw, _    , decode_Ew_Iw_Xx},
    /* 01 */ {oct_instruction, it_test, ids_word, am_Ew, am_Iw, _    , decode_Ew_Iw_Xx},
    /* 02 */ {oct_instruction, it_not,  ids_word, am_Ew, _    , _    , decode_Ew_Xx_Xx},
    /* 03 */ {oct_instruction, it_neg,  ids_word, am_Ew, _    , _    , decode_Ew_Xx_Xx},
    /* 04 */ {oct_instruction, it_mul,  ids_word, am_AX, am_Ew, _    , decode_AX_Ew_Xx},
    /* 05 */ {oct_instruction, it_imul, ids_word, am_AX, am_Ew, _    , decode_AX_Ew_Xx},
    /* 06 */ {oct_instruction, it_div,  ids_word, am_AX, am_Ew, _    , decode_AX_Ew_Xx},
    /* 07 */ {oct_instruction, it_idiv, ids_word, am_AX, am_Ew, _    , decode_AX_Ew_Xx},
};

struct op_code_mapping op_code_table[] = {
    /* 00 */ {oct_instruction, it_add,      ids_byte, am_Eb,               am_Gb, _    , decode_Eb_Gb_Xx},
    /* 01 */ {oct_instruction, it_add,      ids_word, am_Ew,               am_Gw, _    , decode_Ew_Gw_Xx},
    /* 02 */ {oct_instruction, it_add,      ids_byte, am_Gb,               am_Eb, _    , decode_Gb_Eb_Xx},
    /* 03 */ {oct_instruction, it_add,      ids_word, am_Gw,               am_Ew, _    , decode_Gw_Ew_Xx},
    /* 04 */ {oct_instruction, it_add,      ids_byte, am_AL,               am_Ib, _    , decode_AL_Ib_Xx},
    /* 05 */ {oct_instruction, it_add,      ids_word, am_AX,               am_Iw, _    , decode_AX_Iw_Xx},
    /* 06 */ {oct_instruction, it_push,     ids_none, am_ES,               _    , _    , decode_ES_Xx_Xx},
    /* 07 */ {oct_instruction, it_pop,      ids_none, am_ES,               _    , _    , decode_ES_Xx_Xx},
    /* 08 */ {oct_instruction, it_or,       ids_byte, am_Eb,               am_Gb, _    , decode_Eb_Gb_Xx},
    /* 09 */ {oct_instruction, it_or,       ids_word, am_Ew,               am_Gw, _    , decode_Ew_Gw_Xx},
    /* 0a */ {oct_instruction, it_or,       ids_byte, am_Gb,               am_Eb, _    , decode_Gb_Eb_Xx},
    /* 0b */ {oct_instruction, it_or,       ids_word, am_Gw,               am_Ew, _    , decode_Gw_Ew_Xx},
    /* 0c */ {oct_instruction, it_or,       ids_byte, am_AL,               am_Ib, _    , decode_AL_Ib_Xx},
    /* 0d */ {oct_instruction, it_or,       ids_word, am_AX,               am_Iw, _    , decode_AX_Iw_Xx},
    /* 0e */ {oct_instruction, it_push,     ids_none, am_CS,               _    , _    , decode_CS_Xx_Xx},
    /* 0f */ {oct_instruction}, // Extended op codes.

    /* 10 */ {oct_instruction, it_adc,      ids_byte, am_Eb,               am_Gb, _    , decode_Eb_Gb_Xx},
    /* 11 */ {oct_instruction, it_adc,      ids_word, am_Ew,               am_Gw, _    , decode_Ew_Gw_Xx},
    /* 12 */ {oct_instruction, it_adc,      ids_byte, am_Gb,               am_Eb, _    , decode_Gb_Eb_Xx},
    /* 13 */ {oct_instruction, it_adc,      ids_word, am_Gw,               am_Ew, _    , decode_Gw_Ew_Xx},
    /* 14 */ {oct_instruction, it_adc,      ids_byte, am_AL,               am_Ib, _    , decode_AL_Ib_Xx},
    /* 15 */ {oct_instruction, it_adc,      ids_word, am_AX,               am_Iw, _    , decode_AX_Iw_Xx},
    /* 16 */ {oct_instruction, it_push,     ids_none, am_SS,               _    , _    , decode_SS_Xx_Xx},
    /* 17 */ {oct_instruction, it_pop,      ids_none, am_SS,               _    , _    , decode_SS_Xx_Xx},
    /* 18 */ {oct_instruction, it_sbb,      ids_byte, am_Eb,               am_Gb, _    , decode_Eb_Gb_Xx},
    /* 19 */ {oct_instruction, it_sbb,      ids_word, am_Ew,               am_Gw, _    , decode_Ew_Gw_Xx},
    /* 1a */ {oct_instruction, it_sbb,      ids_byte, am_Gb,               am_Eb, _    , decode_Gb_Eb_Xx},
    /* 1b */ {oct_instruction, it_sbb,      ids_word, am_Gw,               am_Ew, _    , decode_Gw_Ew_Xx},
    /* 1c */ {oct_instruction, it_sbb,      ids_byte, am_AL,               am_Ib, _    , decode_AL_Ib_Xx},
    /* 1d */ {oct_instruction, it_sbb,      ids_word, am_AX,               am_Iw, _    , decode_AX_Iw_Xx},
    /* 1e */ {oct_instruction, it_push,     ids_none, am_DS,               _    , _    , decode_DS_Xx_Xx},
    /* 1f */ {oct_instruction, it_pop,      ids_none, am_DS,               _    , _    , decode_DS_Xx_Xx},

    /* 20 */ {oct_instruction, it_and,      ids_byte, am_Eb,               am_Gb, _    , decode_Eb_Gb_Xx},
    /* 21 */ {oct_instruction, it_and,      ids_word, am_Ew,               am_Gw, _    , decode_Ew_Gw_Xx},
    /* 22 */ {oct_instruction, it_and,      ids_byte, am_Gb,               am_Eb, _    , decode_Gb_Eb_Xx},
    /* 23 */ {oct_instruction, it_and,      ids_word, am_Gw,               am_Ew, _    , decode_Gw_Ew_Xx},
    /* 24 */ {oct_instruction, it_and,      ids_byte, am_AL,               am_Ib, _    , decode_AL_Ib_Xx},
    /* 25 */ {oct_instruction, it_and,      ids_word, am_AX,               am_Iw, _    , decode_AX_Iw_Xx},
    /* 26 */ {oct_prefix,      _,           ids_none, _    ,               _    , _    , decode_seg_reg_override_es}, // Segment register override (ES)
    /* 27 */ {oct_instruction, it_daa,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* 28 */ {oct_instruction, it_sub,      ids_byte, am_Eb,               am_Gb, _    , decode_Eb_Gb_Xx},
    /* 29 */ {oct_instruction, it_sub,      ids_word, am_Ew,               am_Gw, _    , decode_Ew_Gw_Xx},
    /* 2a */ {oct_instruction, it_sub,      ids_byte, am_Gb,               am_Eb, _    , decode_Gb_Eb_Xx},
    /* 2b */ {oct_instruction, it_sub,      ids_word, am_Gw,               am_Ew, _    , decode_Gw_Ew_Xx},
    /* 2c */ {oct_instruction, it_sub,      ids_byte, am_AL,               am_Ib, _    , decode_AL_Ib_Xx},
    /* 2d */ {oct_instruction, it_sub,      ids_word, am_AX,               am_Iw, _    , decode_AX_Iw_Xx},
    /* 2e */ {oct_prefix,      _,           ids_none, _    ,               _    , _    , decode_seg_reg_override_cs}, // Segment register override (CS)
    /* 2f */ {oct_instruction, it_das,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},

    /* 30 */ {oct_instruction, it_xor,      ids_byte, am_Eb,               am_Gb, _    , decode_Eb_Gb_Xx},
    /* 31 */ {oct_instruction, it_xor,      ids_word, am_Ew,               am_Gw, _    , decode_Ew_Gw_Xx},
    /* 32 */ {oct_instruction, it_xor,      ids_byte, am_Gb,               am_Eb, _    , decode_Gb_Eb_Xx},
    /* 33 */ {oct_instruction, it_xor,      ids_word, am_Gw,               am_Ew, _    , decode_Gw_Ew_Xx},
    /* 34 */ {oct_instruction, it_xor,      ids_byte, am_AL,               am_Ib, _    , decode_AL_Ib_Xx},
    /* 35 */ {oct_instruction, it_xor,      ids_word, am_AX,               am_Iw, _    , decode_AX_Iw_Xx},
    /* 36 */ {oct_prefix,      0,           ids_none, _    ,               _    , _    , decode_seg_reg_override_ss}, // Segment register override (SS)
    /* 37 */ {oct_instruction, it_aaa,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* 38 */ {oct_instruction, it_cmp,      ids_byte, am_Eb,               am_Gb, _    , decode_Eb_Gb_Xx},
    /* 39 */ {oct_instruction, it_cmp,      ids_word, am_Ew,               am_Gw, _    , decode_Ew_Gw_Xx},
    /* 3a */ {oct_instruction, it_cmp,      ids_byte, am_Gb,               am_Eb, _    , decode_Gb_Eb_Xx},
    /* 3b */ {oct_instruction, it_cmp,      ids_word, am_Gw,               am_Ew, _    , decode_Gw_Ew_Xx},
    /* 3c */ {oct_instruction, it_cmp,      ids_byte, am_AL,               am_Ib, _    , decode_AL_Ib_Xx},
    /* 3d */ {oct_instruction, it_cmp,      ids_word, am_AX,               am_Iw, _    , decode_AX_Iw_Xx},
    /* 3e */ {oct_prefix,      0,           ids_none, _    ,               _    , _    , decode_seg_reg_override_ds}, // Segment register override (DS)
    /* 3f */ {oct_instruction, it_aas,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},

    /* 40 */ {oct_instruction, it_inc,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 41 */ {oct_instruction, it_inc,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 42 */ {oct_instruction, it_inc,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 43 */ {oct_instruction, it_inc,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 44 */ {oct_instruction, it_inc,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 45 */ {oct_instruction, it_inc,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 46 */ {oct_instruction, it_inc,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 47 */ {oct_instruction, it_inc,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 48 */ {oct_instruction, it_dec,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 49 */ {oct_instruction, it_dec,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 4a */ {oct_instruction, it_dec,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 4b */ {oct_instruction, it_dec,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 4c */ {oct_instruction, it_dec,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 4d */ {oct_instruction, it_dec,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 4e */ {oct_instruction, it_dec,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 4f */ {oct_instruction, it_dec,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},

    /* 50 */ {oct_instruction, it_push,     ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 51 */ {oct_instruction, it_push,     ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 52 */ {oct_instruction, it_push,     ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 53 */ {oct_instruction, it_push,     ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 54 */ {oct_instruction, it_push,     ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 55 */ {oct_instruction, it_push,     ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 56 */ {oct_instruction, it_push,     ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 57 */ {oct_instruction, it_push,     ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 58 */ {oct_instruction, it_pop,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 59 */ {oct_instruction, it_pop,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 5a */ {oct_instruction, it_pop,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 5b */ {oct_instruction, it_pop,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 5c */ {oct_instruction, it_pop,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 5d */ {oct_instruction, it_pop,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 5e */ {oct_instruction, it_pop,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},
    /* 5f */ {oct_instruction, it_pop,      ids_word, am_Rw,               _    , _    , decode_Rw_Xx_Xx},

    /* 60 */ {oct_instruction, it_pusha,    ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* 61 */ {oct_instruction, it_popa,     ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* 62 */ {oct_instruction, it_bound,    ids_none, am_Gw,               am_Ma, _    , decode_Gw_Ma_Xx},
    /* 63 */ {oct_instruction, it_arpl,     ids_none, am_Ew,               am_Gw, _    , decode_Ew_Gw_Xx},
    /* 64 */ {oct_invalid},
    /* 65 */ {oct_invalid},
    /* 66 */ {oct_prefix}, // Operand size override
    /* 67 */ {oct_prefix}, // Operand address size override
    /* 68 */ {oct_instruction, it_push,     ids_none, am_Iw,               _    , _    , decode_Iw_Xx_Xx},
    /* 69 */ {oct_instruction, it_imul,     ids_none, am_Gw,               am_Ew, am_Iw, decode_Gw_Ew_Iw},
    /* 6a */ {oct_instruction, it_push,     ids_none, am_Ib,               _    , _    , decode_Ib_Xx_Xx},
    /* 6b */ {oct_instruction, it_imul,     ids_none, am_Gw,               am_Ew, am_Ib, decode_Gw_Ew_Ib},
    /* 6c */ {oct_instruction, it_ins,      ids_none, am_Yb,               am_DX, _    , decode_Yb_DX_Xx},
    /* 6d */ {oct_instruction, it_ins,      ids_none, am_Yw,               am_DX, _    , decode_Yw_DX_Xx},
    /* 6e */ {oct_instruction, it_outs,     ids_none, am_DX,               am_Xb, _    , decode_DX_Xb_Xx},
    /* 6f */ {oct_instruction, it_outs,     ids_none, am_DX,               am_Xw, _    , decode_DX_Xw_Xx},

    /* 70 */ {oct_instruction, it_jo,       ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* 71 */ {oct_instruction, it_jno,      ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* 72 */ {oct_instruction, it_jb,       ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* 73 */ {oct_instruction, it_jnb,      ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* 74 */ {oct_instruction, it_jz,       ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* 75 */ {oct_instruction, it_jnz,      ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* 76 */ {oct_instruction, it_jbe,      ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* 77 */ {oct_instruction, it_jnbe,     ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* 78 */ {oct_instruction, it_js,       ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* 79 */ {oct_instruction, it_jns,      ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* 7a */ {oct_instruction, it_jp,       ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* 7b */ {oct_instruction, it_jnp,      ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* 7c */ {oct_instruction, it_jl,       ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* 7d */ {oct_instruction, it_jnl,      ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* 7e */ {oct_instruction, it_jle,      ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* 7f */ {oct_instruction, it_jnle,     ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},

    /* 80 */ {oct_group,       _,           ids_none, _    ,               _    , _    , _, op_code_table_group_80},
    /* 81 */ {oct_group,       _,           ids_none, _    ,               _    , _    , _, op_code_table_group_81},
    /* 82 */ {oct_invalid}, // Should this be invalid or same as 80?
    /* 83 */ {oct_group,       _,           ids_none, _    ,               _    , _    , _, op_code_table_group_83},
    /* 84 */ {oct_instruction, it_test,     ids_none, am_Eb,               am_Gb, _    , decode_Eb_Gb_Xx},
    /* 85 */ {oct_instruction, it_test,     ids_none, am_Ew,               am_Gw, _    , decode_Ew_Gw_Xx},
    /* 86 */ {oct_instruction, it_xchg,     ids_none, am_Eb,               am_Gb, _    , decode_Eb_Gb_Xx},
    /* 87 */ {oct_instruction, it_xchg,     ids_none, am_Ew,               am_Gw, _    , decode_Ew_Gw_Xx},
    /* 88 */ {oct_instruction, it_mov,      ids_none, am_Eb,               am_Gb, _    , decode_Eb_Gb_Xx},
    /* 89 */ {oct_instruction, it_mov,      ids_none, am_Ew,               am_Gw, _    , decode_Ew_Gw_Xx},
    /* 8a */ {oct_instruction, it_mov,      ids_none, am_Gb,               am_Eb, _    , decode_Gb_Eb_Xx},
    /* 8b */ {oct_instruction, it_mov,      ids_none, am_Gw,               am_Ew, _    , decode_Gw_Ew_Xx},
    /* 8c */ {oct_instruction, it_mov,      ids_none, am_Ew,               am_Sw, _    , decode_Ew_Sw_Xx},
    /* 8d */ {oct_instruction, it_lea,      ids_none, am_Gw,               am_M,  _    , decode_Gw_M_Xx},
    /* 8e */ {oct_instruction, it_mov,      ids_none, am_Sw,               am_Ew, _    , decode_Sw_Ew_Xx},
    /* 8f */ {oct_instruction, it_pop,      ids_none, am_Ew,               _    , _    , decode_Ew_Xx_Xx},

    /* 90 */ {oct_invalid},
    /* 91 */ {oct_instruction, it_xchg,     ids_none, am_AX,               am_Rw, _    , decode_AX_Rw_Xx},
    /* 92 */ {oct_instruction, it_xchg,     ids_none, am_AX,               am_Rw, _    , decode_AX_Rw_Xx},
    /* 93 */ {oct_instruction, it_xchg,     ids_none, am_AX,               am_Rw, _    , decode_AX_Rw_Xx},
    /* 94 */ {oct_instruction, it_xchg,     ids_none, am_AX,               am_Rw, _    , decode_AX_Rw_Xx},
    /* 95 */ {oct_instruction, it_xchg,     ids_none, am_AX,               am_Rw, _    , decode_AX_Rw_Xx},
    /* 96 */ {oct_instruction, it_xchg,     ids_none, am_AX,               am_Rw, _    , decode_AX_Rw_Xx},
    /* 97 */ {oct_instruction, it_xchg,     ids_none, am_AX,               am_Rw, _    , decode_AX_Rw_Xx},
    /* 98 */ {oct_instruction, it_cbw,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* 99 */ {oct_instruction, it_cwd,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* 9a */ {oct_instruction, it_callf,    ids_none, am_Ap,               _    , _    , decode_Ap_Xx_Xx},
    /* 9b */ {oct_instruction, it_fwait,    ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* 9c */ {oct_instruction, it_pushf,    ids_none, am_Fw,               _    , _    , decode_Fw_Xx_Xx},
    /* 9d */ {oct_instruction, it_popf,     ids_none, am_Fw,               _    , _    , decode_Fw_Xx_Xx},
    /* 9e */ {oct_instruction, it_sahf,     ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* 9f */ {oct_instruction, it_lahf,     ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},

    /* a0 */ {oct_instruction, it_mov,      ids_none, am_AL,               am_Ob, _    , decode_AL_Ob_Xx},
    /* a1 */ {oct_instruction, it_mov,      ids_none, am_AX,               am_Ow, _    , decode_AX_Ow_Xx},
    /* a2 */ {oct_instruction, it_mov,      ids_none, am_Ob,               am_AL, _    , decode_Ob_AL_Xx},
    /* a3 */ {oct_instruction, it_mov,      ids_none, am_Ow,               am_AX, _    , decode_Ow_AX_Xx},
    /* a4 */ {oct_instruction, it_movs,     ids_none, am_Xb,               am_Yb, _    , decode_Xb_Yb_Xx},
    /* a5 */ {oct_instruction, it_movs,     ids_none, am_Xw,               am_Yw, _    , decode_Xw_Yw_Xx},
    /* a6 */ {oct_instruction, it_cmps,     ids_none, am_Xb,               am_Yb, _    , decode_Xb_Yb_Xx},
    /* a7 */ {oct_instruction, it_cmps,     ids_none, am_Xw,               am_Yw, _    , decode_Xw_Yw_Xx},
    /* a8 */ {oct_instruction, it_test,     ids_none, am_AL,               am_Ib, _    , decode_AL_Ib_Xx},
    /* a9 */ {oct_instruction, it_test,     ids_none, am_AX,               am_Iw, _    , decode_AX_Iw_Xx},
    /* aa */ {oct_instruction, it_stos,     ids_none, am_Yb,               am_AL, _    , decode_Yb_AL_Xx},  // rep stos BYTE PTR es:[di], al
    /* ab */ {oct_instruction, it_stos,     ids_none, am_Yw,               am_AX, _    , decode_Yw_AX_Xx},  // rep stos WORD PTR es:[di], ax
    /* ac */ {oct_instruction, it_lods,     ids_none, am_AL,               am_Xb, _    , decode_AL_Xb_Xx},
    /* ad */ {oct_instruction, it_lods,     ids_none, am_AX,               am_Xw, _    , decode_AX_Xw_Xx},
    /* ae */ {oct_instruction, it_scas,     ids_none, am_AL,               am_Yb, _    , decode_AL_Yb_Xx},
    /* af */ {oct_instruction, it_scas,     ids_none, am_AX,               am_Yw, _    , decode_AX_Yw_Xx},

    /* b0 */ {oct_instruction, it_mov,      ids_none, am_Rb,               am_Ib, _    , decode_Rb_Ib_Xx},
    /* b1 */ {oct_instruction, it_mov,      ids_none, am_Rb,               am_Ib, _    , decode_Rb_Ib_Xx},
    /* b2 */ {oct_instruction, it_mov,      ids_none, am_Rb,               am_Ib, _    , decode_Rb_Ib_Xx},
    /* b3 */ {oct_instruction, it_mov,      ids_none, am_Rb,               am_Ib, _    , decode_Rb_Ib_Xx},
    /* b4 */ {oct_instruction, it_mov,      ids_none, am_Rb,               am_Ib, _    , decode_Rb_Ib_Xx},
    /* b5 */ {oct_instruction, it_mov,      ids_none, am_Rb,               am_Ib, _    , decode_Rb_Ib_Xx},
    /* b6 */ {oct_instruction, it_mov,      ids_none, am_Rb,               am_Ib, _    , decode_Rb_Ib_Xx},
    /* b7 */ {oct_instruction, it_mov,      ids_none, am_Rb,               am_Ib, _    , decode_Rb_Ib_Xx},
    /* b8 */ {oct_instruction, it_mov,      ids_none, am_Rw,               am_Iw, _    , decode_Rw_Iw_Xx},
    /* b9 */ {oct_instruction, it_mov,      ids_none, am_Rw,               am_Iw, _    , decode_Rw_Iw_Xx},
    /* ba */ {oct_instruction, it_mov,      ids_none, am_Rw,               am_Iw, _    , decode_Rw_Iw_Xx},
    /* bb */ {oct_instruction, it_mov,      ids_none, am_Rw,               am_Iw, _    , decode_Rw_Iw_Xx},
    /* bc */ {oct_instruction, it_mov,      ids_none, am_Rw,               am_Iw, _    , decode_Rw_Iw_Xx},
    /* bd */ {oct_instruction, it_mov,      ids_none, am_Rw,               am_Iw, _    , decode_Rw_Iw_Xx},
    /* be */ {oct_instruction, it_mov,      ids_none, am_Rw,               am_Iw, _    , decode_Rw_Iw_Xx},
    /* bf */ {oct_instruction, it_mov,      ids_none, am_Rw,               am_Iw, _    , decode_Rw_Iw_Xx},

    /* c0 */ {oct_group,       _,           ids_none, _    ,               _    , _    , _, op_code_table_group_c0},
    /* c1 */ {oct_group,       _,           ids_none, _    ,               _    , _    , _, op_code_table_group_c1},
    /* c2 */ {oct_instruction, it_ret,      ids_none, am_Iw,               _    , _    , decode_Iw_Xx_Xx},
    /* c3 */ {oct_instruction, it_ret,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* c4 */ {oct_instruction, it_les,      ids_none, am_Gw,               am_Ew, _    , decode_Gw_Ew_Xx},
    /* c5 */ {oct_instruction, it_lds,      ids_none, am_Gw,               am_Ew, _    , decode_Gw_Ew_Xx},
    /* c6 */ {oct_instruction, it_mov,      ids_none, am_Eb,               am_Ib, _    , decode_Eb_Ib_Xx},
    /* c7 */ {oct_instruction, it_mov,      ids_none, am_Ew,               am_Iw, _    , decode_Ew_Iw_Xx},
    /* c8 */ {oct_instruction, it_enter,    ids_none, am_Iw,               am_Ib, _    , decode_Iw_Ib_Xx},
    /* c9 */ {oct_instruction, it_leave,    ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* ca */ {oct_instruction, it_retf,     ids_none, am_Iw,               _    , _    , decode_Iw_Xx_Xx},
    /* cb */ {oct_instruction, it_retf,     ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* cc */ {oct_instruction, it_int3,     ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* cd */ {oct_instruction, it_int,      ids_none, am_Ib,               _    , _    , decode_Ib_Xx_Xx},
    /* ce */ {oct_instruction, it_into,     ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* cf */ {oct_instruction, it_iret,     ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},

    /* d0 */ {oct_group,       _,           ids_none, _    ,               _    , _    , _, op_code_table_group_d0},
    /* d1 */ {oct_group,       _,           ids_none, _    ,               _    , _    , _, op_code_table_group_d1},
    /* d2 */ {oct_group,       _,           ids_none, _    ,               _    , _    , _, op_code_table_group_d2},
    /* d3 */ {oct_group,       _,           ids_none, _    ,               _    , _    , _, op_code_table_group_d3},
    /* d4 */ {oct_instruction, it_aam,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* d5 */ {oct_instruction, it_aad,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* d6 */ {oct_instruction, it_salc,     ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* d7 */ {oct_instruction, it_xlat,     ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* d8 */ {oct_invalid}, // FPU instructions
    /* d9 */ {oct_invalid}, // FPU instructions
    /* da */ {oct_invalid}, // FPU instructions
    /* db */ {oct_invalid}, // FPU instructions
    /* dc */ {oct_invalid}, // FPU instructions
    /* dd */ {oct_invalid}, // FPU instructions
    /* de */ {oct_invalid}, // FPU instructions
    /* df */ {oct_invalid}, // FPU instructions

    /* e0 */ {oct_instruction, it_loopne,   ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* e1 */ {oct_instruction, it_loope,    ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* e2 */ {oct_instruction, it_loop,     ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* e3 */ {oct_instruction, it_jcxz,     ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* e4 */ {oct_instruction, it_in,       ids_none, am_AL,               am_Ib, _    , decode_AL_Ib_Xx},
    /* e5 */ {oct_instruction, it_in,       ids_none, am_AX,               am_Ib, _    , decode_AX_Ib_Xx},
    /* e6 */ {oct_instruction, it_out,      ids_none, am_Ib,               am_AL, _    , decode_Ib_AL_Xx},
    /* e7 */ {oct_instruction, it_out,      ids_none, am_Ib,               am_AX, _    , decode_Ib_AX_Xx},
    /* e8 */ {oct_instruction, it_call,     ids_none, am_Jw,               _    , _    , decode_Jw_Xx_Xx},
    /* e9 */ {oct_instruction, it_jmp,      ids_none, am_Jw,               _    , _    , decode_Jw_Xx_Xx},
    /* ea */ {oct_instruction, it_jmp,      ids_none, am_Ap,               _    , _    , decode_Ap_Xx_Xx}, // mov cs:ip
    /* eb */ {oct_instruction, it_jmp,      ids_none, am_Jb,               _    , _    , decode_Jb_Xx_Xx},
    /* ec */ {oct_instruction, it_in,       ids_none, am_AL,               am_DX, _    , decode_AL_DX_Xx},
    /* ed */ {oct_instruction, it_in,       ids_none, am_AX,               am_DX, _    , decode_AX_DX_Xx},
    /* ee */ {oct_instruction, it_out,      ids_none, am_DX,               am_AL, _    , decode_DX_AL_Xx},
    /* ef */ {oct_instruction, it_out,      ids_none, am_DX,               am_AX, _    , decode_DX_AX_Xx},

    /* f0 */ {oct_invalid}, // Lock?
    /* f1 */ {oct_instruction, it_int1,     ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* f2 */ {oct_prefix,      _,           ids_none, _    ,               _    , _    , decode_repne_prefix}, // repne prefix
    /* f3 */ {oct_prefix,      _,           ids_none, _    ,               _    , _    , decode_rep_prefix}, // rep prefix
    /* f4 */ {oct_instruction, it_hlt,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* f5 */ {oct_instruction, it_cmc,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* f6 */ {oct_group,       _,           ids_none, _    ,               _    , _    , _, op_code_table_group_f6},
    /* f7 */ {oct_group,       _,           ids_none, _    ,               _    , _    , _, op_code_table_group_f7},
    /* f8 */ {oct_instruction, it_clc,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* f9 */ {oct_instruction, it_stc,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* fa */ {oct_instruction, it_cli,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* fb */ {oct_instruction, it_sti,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* fc */ {oct_instruction, it_cld,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* fd */ {oct_instruction, it_std,      ids_none, _    ,               _    , _    , decode_Xx_Xx_Xx},
    /* fe */ {oct_instruction, it_dec,      ids_none, am_Eb,               _    , _    , decode_Eb_Xx_Xx},
    /* ff */ {oct_instruction, it_inc,      ids_none, am_Eb,               _    , _    , decode_Eb_Xx_Xx},
};
// clang-format on
