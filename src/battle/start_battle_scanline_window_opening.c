#include "m2c_prelude.h"
#include "battle_animation.h"
extern void UpdateBattleScanlineWindow(void) asm("func_080D1E58");
extern s32 D_03000054[];
extern u16 D_0300004C;

void StartBattleScanlineWindowOpening(void) asm("func_080D1C70");

void StartBattleScanlineWindowOpening(void)
{
    *(s8 *)0x02034863 = BATTLE_SCANLINE_WINDOW_OPENING;
    *(s16 *)0x02034864 = 0;
    *(u8 *)0x03000074 |= 4;
    D_03000054[3] = 0;
    D_0300004C |= 0x200;
    *(u16 *)0x0400000A = *(u16 *)0x0400000C;
    *(s8 *)0x02034862 = 1;
    UpdateBattleScanlineWindow();
}
