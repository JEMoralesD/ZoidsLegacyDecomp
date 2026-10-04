#include "m2c_prelude.h"
#include "battle_animation.h"
M2C_UNK UpdateBattleScanlineWindow() asm("func_080D1E58");
extern s8 gBattleScanlineWindowMode asm("D_02034863");
extern s16 gBattleScanlineWindowBandPhase asm("D_02034864");
extern s16 gBattleScanlineWindowLinePhase asm("D_02034866");
extern u8 D_03000074;
extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");
extern u16 D_0300004C;
extern u16 D_0400000A;
extern u16 D_0400000C;
extern s8 gBattleScanlineWindowDisplayRequest asm("D_02034862");

void StartBattleScanlineWindow(void) asm("func_080D1CD4");

void StartBattleScanlineWindow(void) {
    gBattleScanlineWindowMode = BATTLE_SCANLINE_WINDOW_ACTIVE;
    gBattleScanlineWindowBandPhase = -0x10;
    gBattleScanlineWindowLinePhase = 0;
    D_03000074 |= 4;
    gFieldCameraScrollOffsets[3] = 0;
    D_0300004C |= 0x200;
    D_0400000A = D_0400000C;
    gBattleScanlineWindowDisplayRequest = 1;
    UpdateBattleScanlineWindow();
}
