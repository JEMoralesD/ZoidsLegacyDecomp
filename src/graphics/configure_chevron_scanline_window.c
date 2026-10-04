#include "m2c_prelude.h"
#include "screen_effects.h"
extern u8 D_0300004C;
extern u8 D_03000074;
extern u8 gScanlineWindowFlags asm("D_03005EE8");
extern u8 gChevronWindowProgress asm("D_03005EF4");
extern u8 gDisplayWindowInsideLayers asm("D_03005EF8");
extern u8 gDisplayWindowOutsideLayers asm("D_03005EFA");
extern u8 gChevronWindowDirection asm("D_03005F10");
extern u8 gChevronWindowInvertBounds asm("D_03005F11");
M2C_UNK ResetScanlineWindowBuffers() asm("func_08095494");                                /* extern */

void ConfigureChevronScanlineWindow(s8 direction, s8 invert_bounds, u8 effect_flags) asm("func_0809564C");

void ConfigureChevronScanlineWindow(s8 direction, s8 invert_bounds, u8 effect_flags) {
    *(s8 *)((u32)&gScanlineWindowFlags) = effect_flags | SCANLINE_WINDOW_CHEVRON;
    *(u16 *)((u32)&D_0300004C) |= 0x6000;
    *(s8 *)((u32)&gChevronWindowDirection) = direction;
    *(s8 *)((u32)&gChevronWindowInvertBounds) = invert_bounds;
    *(s16 *)((u32)&gChevronWindowProgress) = 0;
    *(s16 *)((u32)&gDisplayWindowInsideLayers) = 0;
    *(s16 *)((u32)&gDisplayWindowOutsideLayers) = 0x3F;
    ResetScanlineWindowBuffers();
    *(u8 *)((u32)&D_03000074) |= 2;
}
