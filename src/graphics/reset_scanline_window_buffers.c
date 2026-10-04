#include "m2c_prelude.h"
#include "screen_effects.h"
extern u8 gScanlineWindowBuffers[] asm("D_02000000");
void ResetScanlineWindowBuffers(void) asm("func_08095494");

void ResetScanlineWindowBuffers(void) {
    u8 bank;
    u8 scanline;
    void *record;
    s32 base;
    s32 base4;
    s32 empty_horizontal_bounds;
    s32 full_vertical_bounds;
    bank = 0;
    base = (s32) gScanlineWindowBuffers;
    base4 = base + 4;
    empty_horizontal_bounds = 0;
    full_vertical_bounds = 0xA000A0;
    do {
        scanline = 0;
        do {
            record = (void *) ((scanline * 8) + (bank * SCANLINE_WINDOW_BANK_BYTES));
            *(s32 *)((s32) record + base) = empty_horizontal_bounds;
            *(s32 *)((s32) record + base4) = full_vertical_bounds;
            scanline += 1;
        } while ((u32) scanline <= 0x9FU);
        bank += 1;
    } while ((u32) bank <= 1U);
}
