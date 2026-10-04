#include "m2c_prelude.h"
#include "screen_effects.h"

extern u8 gScanlineWindowBuffers[] asm("D_02000000");

void BuildChevronScanlineWindow(void) asm("func_08095EE4");

void BuildChevronScanlineWindow(void)
{
    u8 write_bank = 1 ^ *(u8 *)SCANLINE_WINDOW_ADDRESS(displayed_bank);
    u16 *window_bounds = (u16 *)(gScanlineWindowBuffers + write_bank * SCANLINE_WINDOW_BANK_BYTES);
    u8 scanline;
    register u8 *direction;
    register u16 *progress;
    register u8 *invert_bounds asm("r5");
    register u32 screen_width asm("r4");

    scanline = 0;
    asm volatile("" : "+r"(scanline));
    direction = (u8 *)SCANLINE_WINDOW_ADDRESS(chevron_direction);
    progress = (u16 *)SCANLINE_WINDOW_ADDRESS(chevron_progress);
    screen_width = 240;
    invert_bounds = (u8 *)SCANLINE_WINDOW_ADDRESS(chevron_invert_bounds);

    do {
        u32 boundary_x;

        if (*direction == 0) {
            s32 boundary_position = screen_width - *progress;

            if (scanline <= 79) {
                boundary_position += 80;
                boundary_position -= scanline;
            } else {
                boundary_position -= 80;
                boundary_position += scanline;
            }
            {
                register u32 narrowed asm("r0") = (u16)boundary_position;
                asm volatile("" : "+r"(narrowed));
                boundary_x = narrowed;
            }
            asm volatile("" : "+r"(boundary_x));
            {
                register s32 signed_value asm("r0") = (s16)boundary_x;
                asm volatile("" : "+r"(signed_value));
                if (signed_value < 0) {
                    boundary_x = 0;
                } else if (signed_value > 240) {
                    boundary_x = 240;
                }
            }
            if (*invert_bounds == 0) {
                goto store_right_bound;
            }
        } else {
            s32 boundary_position = *progress;

            if (scanline <= 79) {
                boundary_position -= 80;
                boundary_position += scanline;
            } else {
                boundary_position += 80;
                boundary_position -= scanline;
            }
            {
                register u32 narrowed asm("r0") = (u16)boundary_position;
                asm volatile("" : "+r"(narrowed));
                boundary_x = narrowed;
            }
            asm volatile("" : "+r"(boundary_x));
            {
                register s32 signed_value asm("r0") = (s16)boundary_x;
                asm volatile("" : "+r"(signed_value));
                if (signed_value < 0) {
                    boundary_x = 0;
                } else if (signed_value > 240) {
                    boundary_x = 240;
                }
            }
            if (*invert_bounds != 0) {
                goto store_right_bound;
            }
        }
        asm volatile("" : "+r"(boundary_x));
        {
            register s32 packed_value asm("r0");
            asm volatile("lsl %0, %1, #16\n\t"
                         "asr %0, %0, #8"
                         : "=r"(packed_value)
                         : "r"(boundary_x));
            packed_value |= screen_width;
            *window_bounds = packed_value;
        }
        goto next_scanline;
store_right_bound:
        *window_bounds = boundary_x;
next_scanline:
        window_bounds += 4;
        scanline++;
    } while (scanline <= 159);
}
