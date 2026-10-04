#include "m2c_prelude.h"
#include "battle_animation.h"
extern u8 gBattleScanlineWindowDisplayRequest asm("D_02034862");
extern u8 gBattleScanlineWindowMode asm("D_02034863");
extern u8 gBattleScanlineWindowBufferIndex asm("D_02034868");
M2C_UNK BiosCpuSet(M2C_UNK, M2C_UNK, M2C_UNK) asm("func_80ECD2C");       /* extern */

void ResetBattleScanlineWindow(void) asm("func_080D1C38");

void ResetBattleScanlineWindow(void) {
    *(s8 *)((u32)&gBattleScanlineWindowDisplayRequest) = 0;
    *(s8 *)((u32)&gBattleScanlineWindowMode) = BATTLE_SCANLINE_WINDOW_DISABLED;
    *(s8 *)((u32)&gBattleScanlineWindowBufferIndex) = 0;
    BiosCpuSet(0x080007A4, 0x0300605C, 0x04000028);
}
