#include "m2c_prelude.h"
#include "screen_effects.h"

extern u8 gScanlineWindowBuffers[] asm("D_02000000");
extern s16 *gScaledScanlineWindowSources[] asm("D_03005F04");
extern u16 gScanlineWindowHorizontalScales[] asm("D_03005EEA");
extern u16 gScanlineWindowVerticalScales[] asm("D_03005EEE");
s16 BiosDiv(s32, s32) asm("func_080ECD30");

void BuildScaledScanlineWindows(void) asm("func_0809570C");

void BuildScaledScanlineWindows(void)
{
    u8 write_bank = 1 ^ *(u8 *)SCANLINE_WINDOW_ADDRESS(displayed_bank);
    u16 *window_bounds = (u16 *)(gScanlineWindowBuffers + write_bank * SCANLINE_WINDOW_BANK_BYTES);
    u8 window_id;

    window_id = 0;
    do {
        s16 *source_bounds = gScaledScanlineWindowSources[window_id];
        u32 next_window_id = window_id + 1;

        /* A NULL source leaves the cursor in place, so window 1 can write window 0. */
        if (source_bounds != 0) {
            u32 scale_x = gScanlineWindowHorizontalScales[window_id];
            u32 source_row_step = gScanlineWindowVerticalScales[window_id];
            s16 scanline;

            if (scale_x == 0 || source_row_step == 0) {
                scanline = 0;
                do {
                    *window_bounds = 0;
                    window_bounds += 4;
                    scanline++;
                } while (scanline <= 159);
            } else {
                s32 source_y_fixed8;
                s16 *source_row;

                source_row_step = (u16)BiosDiv(0x10000, source_row_step);
                source_y_fixed8 = -(source_row_step * 80) + 0x5000;
                scanline = 0;
                do {
                    source_y_fixed8 += source_row_step;
                    if ((u32)source_y_fixed8 > 0x9FFF) {
                        *window_bounds = 0;
                        asm volatile("" : "=r"(source_row));
                    } else {
                        s32 coordinate = source_y_fixed8;
                        u16 left_raw;
                        s32 source_left, source_right;

                        asm volatile("" : "+r"(coordinate));
                        if (source_y_fixed8 < 0) {
                            coordinate += 255;
                        }
                        source_row = (s16 *)((coordinate >> 8) * 4 + ({
                            register s32 source_base asm("r1") = (s32)source_bounds;
                            asm volatile("" : "+r"(source_base));
                            source_base;
                        }));
                        left_raw = source_row[0];
                        asm volatile("" : "+r"(left_raw));
                        source_left = (s16)left_raw;
                        asm volatile("" :: "r"(left_raw));
                        source_right = source_row[1];

                        if (source_left < source_right) {
                            u16 left, right;

                            coordinate = source_left - 120;
                            coordinate *= scale_x;
                            if (coordinate < 0) {
                                coordinate += 255;
                            }
                            left = (coordinate >> 8) + 120;
                            coordinate = source_right - 120;
                            coordinate *= scale_x;
                            if (coordinate < 0) {
                                coordinate += 255;
                            }
                            right = (coordinate >> 8) + 120;
                            if ((s16)left < 0) {
                                left = 0;
                            } else if ((s16)left > 240) {
                                left = 240;
                            }
                            if ((s16)right < 0) {
                                right = 0;
                            } else if ((s16)right > 240) {
                                right = 240;
                            }
                            asm volatile("" :: "r"(left));
                            asm volatile("" :: "r"(left));
                            asm volatile("" :: "r"(left));
                            {
                                s32 packed = (s16)left << 8;
                                s32 packed2 = (s16)right | packed;

                                asm volatile("" : "+r"(packed2));
                                *window_bounds = packed2;
                            }
                        } else {
                            *window_bounds = 0;
                        }
                    }
                    window_bounds += 4;
                    scanline++;
                } while (scanline <= 159);
            }
            window_bounds -= 0x27F;
        }
        window_id = next_window_id;
    } while (window_id <= 1);
}
