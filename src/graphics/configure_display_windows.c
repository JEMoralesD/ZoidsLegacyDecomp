#include "m2c_prelude.h"
#include "screen_effects.h"
extern s8 gScanlineWindowFlags asm("D_03005EE8");
extern u16 D_0300004C;
extern u16 D_03005EFC;
extern u16 D_03005EFE;
extern u16 D_03005F00;
extern u16 D_03005F02;
extern u16 gDisplayWindowInsideLayers asm("D_03005EF8");
extern u16 gDisplayWindowOutsideLayers asm("D_03005EFA");
extern u8 D_03000074;

void ConfigureDisplayWindows(u8 enable_window0, u16 window0_horizontal, u16 window0_vertical, u8 enable_window1, u16 window1_horizontal, u16 window1_vertical, u16 inside_layers, u16 outside_layers) asm("func_0809538C");

void ConfigureDisplayWindows(u8 enable_window0, u16 window0_horizontal, u16 window0_vertical, u8 enable_window1, u16 window1_horizontal, u16 window1_vertical, u16 inside_layers, u16 outside_layers) {
    gScanlineWindowFlags = SCANLINE_WINDOW_FIXED;
    if (enable_window0 != 0) {
        D_0300004C |= 0x2000;
        D_03005EFC = window0_horizontal;
        D_03005EFE = window0_vertical;
    }
    if (enable_window1 != 0) {
        D_0300004C = (u16)(D_0300004C | 0x4000);
        D_03005F00 = window1_horizontal;
        D_03005F02 = window1_vertical;
    }
    gDisplayWindowInsideLayers = inside_layers;
    gDisplayWindowOutsideLayers = outside_layers;
    D_03000074 |= 2;
}
