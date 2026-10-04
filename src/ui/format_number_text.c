#include "m2c_prelude.h"
#include "window.h"

u8 BiosDiv(s32, s32) asm("func_080ECD30");                         /* extern */
s32 DivideSigned32(s32, s32) asm("func_080ECD98");                        /* extern */

void FormatNumberText(s32 value, s32 digit_width, s32 format_flags, s32 destination_address) asm("func_08098284");

void FormatNumberText(s32 value, s32 digit_width, s32 format_flags, s32 destination_address) {
    s32 has_printed_digit;
    u8 *destination;
    s32 prefix_store;
    s32 case_store;
    s32 case_mode;
    register u8 *var_r0_2 asm("r0");
    register s32 var_r2 asm("r2");
    register s32 var_r2_2 asm("r2");
    s32 temp_r0;
    s32 remaining_value;
    s32 decimal_place;
    s32 place_value;
    u8 *var_r1;
    s8 var_r0;
    register s32 var_r0_3 asm("r0");
    u32 output_glyph_count;
    u8 width;
    u8 temp_r1_2;
    u8 temp_r2;
    u8 ascii_flag;
    u8 flags;

    remaining_value = value;
    destination = (u8 *)destination_address;
    width = (u8)digit_width;
    flags = (u8)format_flags;
    place_value = 0x3B9ACA00;
    has_printed_digit = 0;
    output_glyph_count = 0;
    if (NUMBER_TEXT_SIGN_PREFIX & flags) {
        if (remaining_value > 0) {
            if (!(NUMBER_TEXT_ASCII & flags)) {
                var_r2 = 0x7B81;
                goto block_11;
            }
            var_r0 = 0x2B;
            goto block_13;
        }
        if (remaining_value < 0) {
            if (!(NUMBER_TEXT_ASCII & flags)) {
                *(u16 *)destination = 0x7C81;
            } else {
                var_r0 = 0x2D;
                goto block_13;
            }
        } else if (!(NUMBER_TEXT_ASCII & flags)) {
            var_r2 = 0x7D81;
block_11:
            prefix_store = var_r2;
            asm volatile("" : "+&r"(prefix_store) : "r"(var_r2));
            *(u16 *)destination = prefix_store;
        } else {
            var_r0 = 0x20;
block_13:
            *destination = var_r0;
        }
        output_glyph_count = (u8)(output_glyph_count + 1);
    }
    if (remaining_value < 0) {
        remaining_value = 0 - remaining_value;
    }
    decimal_place = 0;
    ascii_flag = NUMBER_TEXT_ASCII & flags;
    do {
        if (decimal_place >= (s32) (0xA - width)) {
            temp_r2 = BiosDiv(remaining_value, place_value);
            if ((temp_r2 != 0) || (has_printed_digit != 0) || (decimal_place == 9)) {
                if (ascii_flag == 0) {
                    *(u16 *)(destination + output_glyph_count * 2) = (temp_r2 << 8) + 0x4F82;
                } else {
                    *(destination + output_glyph_count) = temp_r2 + 0x30;
                }
                output_glyph_count = (u32) (u8) (output_glyph_count + 1);
                remaining_value -= place_value * temp_r2;
                temp_r2 = 1;
                asm volatile("" : "+r"(temp_r2));
                has_printed_digit = temp_r2;
            } else {
                temp_r0 = NUMBER_TEXT_PADDING_MASK & flags;
                if (temp_r0 == NUMBER_TEXT_LEADING_SPACES)
                    goto switch_case_2;
                if (temp_r0 <= NUMBER_TEXT_LEADING_SPACES)
                    goto switch_end;
                if (temp_r0 == NUMBER_TEXT_LEADING_ZEROES)
                    goto switch_case_3;
                goto switch_end;
switch_case_2:
                    case_mode = ascii_flag;
                    asm volatile("" : "+r"(case_mode));
                    if (case_mode == 0) {
                        var_r0_2 = (u8 *)((s32)(output_glyph_count * 2) + (s32)destination);
                        var_r2_2 = 0x4081;
                        goto block_36;
                    } else {
                        var_r1 = destination + output_glyph_count;
                        var_r0_3 = 0x20;
                        goto block_38;
                    }
switch_case_3:
                    if (ascii_flag != 0)
                        goto switch_case_3_byte;
                    var_r0_2 = (u8 *)((s32)(output_glyph_count * 2) + (s32)destination);
                    var_r2_2 = 0x4F82;
block_36:
                    case_store = var_r2_2;
                    asm volatile("" : "+&r"(case_store) : "r"(var_r2_2));
                    *(u16 *)var_r0_2 = case_store;
                    goto block_39;
switch_case_3_byte:
                    var_r1 = destination + output_glyph_count;
                    var_r0_3 = 0x30;
block_38:
                    *var_r1 = var_r0_3;
block_39:
                    output_glyph_count = (u32) (u8) (output_glyph_count + 1);
switch_end:
                ;
            }
        }
        place_value = DivideSigned32(place_value, 0xA);
        decimal_place = (s32) (u8) (decimal_place + 1);
    } while ((u32) decimal_place <= 9U);
    if ((NUMBER_TEXT_PADDING_MASK & flags) == NUMBER_TEXT_TRAILING_SPACES) {
        if (!(NUMBER_TEXT_ASCII & flags)) {
            if (output_glyph_count < (u32) width) {
                do {
                    *(u16 *)(destination + output_glyph_count * 2) = 0x4081;
                    output_glyph_count = (u32) (u8) (output_glyph_count + 1);
                } while (output_glyph_count < (u32) width);
            }
        } else if (output_glyph_count < (u32) width) {
            do {
                *(destination + output_glyph_count) = 0x20;
                output_glyph_count = (u32) (u8) (output_glyph_count + 1);
            } while (output_glyph_count < (u32) width);
        }
    }
    temp_r1_2 = NUMBER_TEXT_ASCII & flags;
    if (temp_r1_2 == 0) {
        *((output_glyph_count * 2) + destination) = temp_r1_2;
        return;
    }
    *(destination + output_glyph_count) = 0;
}
