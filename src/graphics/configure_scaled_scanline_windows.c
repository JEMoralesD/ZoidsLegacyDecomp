#include "m2c_prelude.h"
#include "screen_effects.h"
extern void ResetScanlineWindowBuffers(void) asm("func_08095494");
extern s32 gScaledScanlineWindowSources[] asm("D_03005F04");
extern u16 gScanlineWindowHorizontalScales[] asm("D_03005EEA");
extern u16 gScanlineWindowVerticalScales[] asm("D_03005EEE");
extern u16 gDisplayWindowInsideLayers asm("D_03005EF8");
extern u16 gDisplayWindowOutsideLayers asm("D_03005EFA");

void ConfigureScaledScanlineWindows(s32 window0_bounds, s16 window0_scale_x, s16 window0_scale_y, s32 window1_bounds, u16 window1_scale_x, u16 window1_scale_y, u16 inside_layers, u16 outside_layers, u8 effect_flags) asm("func_080954D8");

void ConfigureScaledScanlineWindows(s32 window0_bounds, s16 window0_scale_x, s16 window0_scale_y, s32 window1_bounds, u16 window1_scale_x, u16 window1_scale_y, u16 inside_layers, u16 outside_layers, u8 effect_flags) {
    *(u8 *)SCANLINE_WINDOW_ADDRESS(flags) = effect_flags | SCANLINE_WINDOW_SCALED;
    gScaledScanlineWindowSources[0] = window0_bounds;
    gScanlineWindowHorizontalScales[0] = window0_scale_x;
    gScanlineWindowVerticalScales[0] = window0_scale_y;
    if (window0_bounds != 0) {
        *(u16 *)0x0300004C |= 0x2000;
    }
    gScaledScanlineWindowSources[1] = window1_bounds;
    gScanlineWindowHorizontalScales[1] = window1_scale_x;
    gScanlineWindowVerticalScales[1] = window1_scale_y;
    if (window1_bounds != 0) {
        *(u16 *)0x0300004C |= 0x4000;
    }
    gDisplayWindowInsideLayers = inside_layers;
    gDisplayWindowOutsideLayers = outside_layers;
    ResetScanlineWindowBuffers();
    *(u8 *)0x03000074 |= 2;
}
