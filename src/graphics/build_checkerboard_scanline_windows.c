#include "m2c_prelude.h"
#include "screen_effects.h"

extern u8 gScanlineWindowBuffers[] asm("D_02000000");

s32 DivideUnsigned32(u8, s32) asm("func_080ECF00");
u8 ModuloUnsigned32(u8, s32) asm("func_080ECF78");

void BuildCheckerboardScanlineWindows(void) asm("func_08095F9C");

void BuildCheckerboardScanlineWindows(void)
{
    u8 write_bank;
    s32 *window_bounds;
    u8 *progress;
    u8 progress_value;
    u8 scanline;
    register u8 *progress_source asm("r0");

    write_bank = 1 ^ *(u8 *)SCANLINE_WINDOW_ADDRESS(displayed_bank);
    window_bounds = (s32 *)(gScanlineWindowBuffers + write_bank * SCANLINE_WINDOW_BANK_BYTES);
    progress_source = (u8 *)SCANLINE_WINDOW_ADDRESS(checkerboard_progress);
    progress_value = *progress_source;
    progress = progress_source;
    if (progress_value <= 0x27) {
        scanline = 0;
        do {
            s32 horizontal_bounds;

            if (ModuloUnsigned32(scanline, 0x28) <= *progress) {
                register s32 result asm("r0");

                result = DivideUnsigned32(scanline, 0x28);
                asm volatile("" : "+r"(result));
                result &= 1;
                if (result == 0) {
                    register s32 selected asm("r0") = 0x78B4003C;
                    asm volatile("" : "+r"(selected));
                    horizontal_bounds = selected;
                } else {
                    register s32 selected asm("r0") = 0xB4F03C78;
                    asm volatile("" : "+r"(selected));
                    horizontal_bounds = selected;
                }
            } else {
                horizontal_bounds = 0;
            }
            *window_bounds = horizontal_bounds;
            window_bounds += 2;
            scanline = (u8)(scanline + 1);
        } while (scanline <= 0x9F);
        return;
    }
    if (progress_value <= 0x4F) {
        scanline = 0;
        do {
            s32 horizontal_bounds;

            if ((s32)ModuloUnsigned32(scanline, 0x28) > (s32)(0x50 - *progress)) {
                register s32 selected asm("r0") = 0x00F000F0;
                asm volatile("" : "+r"(selected));
                horizontal_bounds = selected;
            } else {
                register s32 result asm("r0");

                result = DivideUnsigned32(scanline, 0x28);
                asm volatile("" : "+r"(result));
                result &= 1;
                if (result == 0) {
                    register s32 selected asm("r0") = 0x78B4003C;
                    asm volatile("" : "+r"(selected));
                    horizontal_bounds = selected;
                } else {
                    register s32 selected asm("r0") = 0xB4F03C78;
                    asm volatile("" : "+r"(selected));
                    horizontal_bounds = selected;
                }
            }
            *window_bounds = horizontal_bounds;
            window_bounds += 2;
            scanline = (u8)(scanline + 1);
        } while (scanline <= 0x9F);
        return;
    }
    {
        register s32 horizontal_bounds asm("r1");

        scanline = 0;
        horizontal_bounds = 0x00F000F0;
        asm volatile("" : "+r"(horizontal_bounds));
        do {
            *window_bounds = horizontal_bounds;
            window_bounds += 2;
            scanline = (u8)(scanline + 1);
        } while (scanline <= 0x9F);
    }
}
