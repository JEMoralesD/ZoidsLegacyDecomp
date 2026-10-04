#include "m2c_prelude.h"
#include "screen_effects.h"
M2C_UNK ResetScanlineWindowBuffers() asm("func_08095494");                                /* extern */

void ConfigureCheckerboardScanlineWindows(s16 inside_layers, s16 outside_layers, u8 effect_flags) asm("func_080956B4");

void ConfigureCheckerboardScanlineWindows(s16 inside_layers, s16 outside_layers, u8 effect_flags) {
    *(s8 *)SCANLINE_WINDOW_ADDRESS(flags) = effect_flags | SCANLINE_WINDOW_CHECKERBOARD;
    *(u16 *)0x0300004C |= 0x6000;
    *(s8 *)SCANLINE_WINDOW_ADDRESS(checkerboard_progress) = 0;
    *(s16 *)SCANLINE_WINDOW_ADDRESS(inside_layers) = inside_layers;
    *(s16 *)SCANLINE_WINDOW_ADDRESS(outside_layers) = outside_layers;
    ResetScanlineWindowBuffers();
    *(u8 *)0x03000074 |= 2;
}
