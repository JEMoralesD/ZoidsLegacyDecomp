#include "m2c_prelude.h"
#include "battle_animation.h"
void DestroySpriteGroup(void) asm("func_8095114");
u8 GetBattleScanlineWindowPhase(void) asm("func_080D1E38");
void StopBattleScanlineWindow(void) asm("func_080D1D5C");
s32 IsBattleBackgroundShakeFinished(void) asm("func_080D222C");
void StopBattleBackgroundShake(void) asm("func_080D220C");

void StopBattleAnimation(void) asm("func_080D120C");

void StopBattleAnimation(void) {
    u8 var_r4;
    s8 *p = (s8 *)0x02033FD0;
    s32 *arr;
    *p = 0;
    var_r4 = 0;
    arr = (s32 *)(p + 0x20);
    do {
        if (arr[var_r4] != 0) {
            DestroySpriteGroup();
        }
        var_r4 += 1;
    } while ((u32) var_r4 <= 0xF);
    if ((u32) GetBattleScanlineWindowPhase() <= 1) {
        StopBattleScanlineWindow();
    }
    if ((IsBattleBackgroundShakeFinished() << 0x18) == 0) {
        StopBattleBackgroundShake();
    }
    if (*(u8 *)0x02034860 != 0) {
        *(u16 *)0x0300004C = (0xFDFF & *(u16 *)0x0300004C) | 0x400;
        *(u8 *)0x02034860 = 0;
    }
    if (*(u8 *)0x02034861 != 0) {
        *(u16 *)0x0300004C = 0xFDFF & *(u16 *)0x0300004C;
        *(u8 *)0x02034861 = 0;
    }
}
