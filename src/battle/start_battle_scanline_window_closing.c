#include "m2c_prelude.h"
#include "battle_animation.h"

extern volatile u8 gBattleScanlineWindowMode asm("D_02034863");
extern volatile u16 gBattleScanlineWindowBandPhase asm("D_02034864");

void StartBattleScanlineWindowClosing(void) asm("func_080D1D44");

void StartBattleScanlineWindowClosing(void)
{
    gBattleScanlineWindowMode = BATTLE_SCANLINE_WINDOW_CLOSING;
    gBattleScanlineWindowBandPhase = 0x40;
}
