#include "m2c_prelude.h"
#include "screen_effects.h"
extern u8 D_0300004C;
extern u8 D_03000074;
extern u8 gScanlineWindowFlags asm("D_03005EE8");
extern u8 gDisplayWindowInsideLayers asm("D_03005EF8");
extern u8 gDisplayWindowOutsideLayers asm("D_03005EFA");
extern u8 gScanlineWindowPolygonStream asm("D_03005F0C");
M2C_UNK ResetScanlineWindowBuffers() asm("func_08095494");                                /* extern */

void ConfigurePolygonScanlineWindows(s32 polygon_stream, s16 inside_layers, s16 outside_layers, u8 effect_flags) asm("func_080955A0");

void ConfigurePolygonScanlineWindows(s32 polygon_stream, s16 inside_layers, s16 outside_layers, u8 effect_flags) {
    *(s8 *)((u32)&gScanlineWindowFlags) = effect_flags | SCANLINE_WINDOW_POLYGONS;
    *(u16 *)((u32)&D_0300004C) |= 0x6000;
    *(s32 *)((u32)&gScanlineWindowPolygonStream) = polygon_stream;
    *(s16 *)((u32)&gDisplayWindowInsideLayers) = inside_layers;
    *(s16 *)((u32)&gDisplayWindowOutsideLayers) = outside_layers;
    ResetScanlineWindowBuffers();
    *(u8 *)((u32)&D_03000074) |= 2;
}
