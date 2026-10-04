#include "m2c_prelude.h"
#include "battle_display.h"
void DrawBattleGaugeFill(s32 fill_level, u8 *gauge_pixels) asm("func_080BAA40");

void DrawBattleGaugeFill(s32 fill_level, u8 *gauge_pixels) {
    register s32 level asm("r6");
    s32 clear_remainder;
    register s32 pixel_index asm("r5");
    register volatile u8 *cursor asm("r2");
    register s32 pixel_mask asm("r4");

    cursor = gauge_pixels;
    level = (u8)fill_level;
    {
        register s32 started_input asm("r7");

        started_input = 0;
        clear_remainder = started_input;
    }
    pixel_index = 10;
    do {
        if (level != BATTLE_GAUGE_GRAY) {
            if (level + 10 <= pixel_index) {
                clear_remainder = 1;
            }
            if (!({
                register s32 parity asm("r0");

                parity = 1;
                parity &= pixel_index;
                parity;
                })) {
                if (clear_remainder) {
                    register s32 value asm("r0");
                    register s32 byte asm("r1");

                    cursor[0] = (0xF0 & cursor[0]) | 3;
                    byte = cursor[4];
                    asm volatile("add r0, r3, #0" : "=r"(value));
                    value &= byte;
                    pixel_mask = 10;
                    value |= pixel_mask;
                    cursor[4] = value;
                    if ((u32)pixel_index > 0x10) {
                        cursor[8] = (0xF0 & cursor[8]) | pixel_mask;
                    }
                }
                goto next;
            }
            if (clear_remainder) {
                register s32 mask asm("r3");
                register s32 value asm("r0");
                register s32 byte asm("r1");

                byte = cursor[0];
                mask = 0xF;
                value = mask;
                asm volatile("" : "+r"(value));
                value &= byte;
                byte = 0x30;
                value |= byte;
                cursor[0] = value;
                byte = cursor[4];
                value = mask;
                asm volatile("" : "+r"(value));
                value &= byte;
                pixel_mask = 0xA0;
                value |= pixel_mask;
                cursor[4] = value;
                if ((u32)pixel_index > 0x10) {
                    byte = cursor[8];
                    value = mask;
                    asm volatile("" : "+r"(value));
                    value &= byte;
                    value |= pixel_mask;
                    cursor[8] = value;
                }
            }
            if ((u32)(pixel_index & 7) <= 6) {
                goto selected_advance_byte;
            }
            goto selected_advance_row;
selected_advance_byte:
            cursor += 1;
            goto next;
selected_advance_row:
            cursor += 0x1D;
            goto next;
        } else if (!({
            register s32 parity asm("r0");

            parity = 1;
            parity &= pixel_index;
            parity;
        })) {
            register s32 value asm("r0");
            register s32 byte asm("r1");
            register s32 nibble asm("r3");

            byte = cursor[0];
            pixel_mask = 0xF0;
            value = pixel_mask;
            asm volatile("" : "+r"(value));
            value &= byte;
            nibble = 0xE;
            value |= nibble;
            cursor[0] = value;
            byte = cursor[4];
            value = pixel_mask;
            asm volatile("" : "+r"(value));
            value &= byte;
            value |= nibble;
            cursor[4] = value;
            if ((u32)pixel_index > 0x10) {
                byte = cursor[8];
                value = pixel_mask;
                asm volatile("" : "+r"(value));
                value &= byte;
                value |= nibble;
                cursor[8] = value;
            }
            goto next;
        } else {
            register s32 value asm("r0");
            register s32 byte asm("r1");
            register s32 nibble asm("r3");

            byte = cursor[0];
            pixel_mask = 0xF;
            value = pixel_mask;
            asm volatile("" : "+r"(value));
            value &= byte;
            nibble = 0xE0;
            value |= nibble;
            cursor[0] = value;
            byte = cursor[4];
            value = pixel_mask;
            asm volatile("" : "+r"(value));
            value &= byte;
            value |= nibble;
            cursor[4] = value;
            if ((u32)pixel_index > 0x10) {
                byte = cursor[8];
                value = pixel_mask;
                asm volatile("" : "+r"(value));
                value &= byte;
                value |= nibble;
                cursor[8] = value;
            }
            if ((u32)(pixel_index & 7) <= 6) {
                cursor += 1;
            } else {
                cursor += 0x1D;
            }
            goto next;
        }
next:
        {
            register u8 next_counter asm("r0");

            next_counter = pixel_index + 1;
            asm volatile("" : "+r"(next_counter));
            pixel_index = next_counter;
        }
    } while ((u32)pixel_index <= 0x1D);
}
