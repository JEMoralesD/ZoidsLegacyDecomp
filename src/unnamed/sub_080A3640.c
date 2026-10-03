#include "m2c_prelude.h"
extern u8 gBattleState[];
extern u8 D_000027BE[];
extern u8 D_02033F36;

extern void DestroySpriteGroup(s32) asm("func_08095114");
extern void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
extern void func_080CD110(void);
extern void StartBattleEquipmentAnimation(u8, u8, s32, u8, s32) asm("func_080D0CA0");
extern s32 GetBattleAnimationResult(void) asm("func_080D1C18");
extern void StartBattleScanlineWindowClosing(void) asm("func_080D1D44");
extern u8 GetBattleScanlineWindowPhase(void) asm("func_080D1E38");
extern void RequestBattleBackgroundShakeStop(void) asm("func_080D2200");
extern s32 IsBattleBackgroundShakeFinished(void) asm("func_080D222C");
extern void func_080ED17C(s32);

s32 sub_080A3640(u8 arg0, void **arg1) {
    u8 *temp_r4;
    u8 *p;
    u8 *pidx;
    int t;

    if (GetBattleScanlineWindowPhase() == 1) {
        StartBattleScanlineWindowClosing();
        while ((u32)GetBattleScanlineWindowPhase() <= 1U) {
            func_080ED17C(1);
        }
    }
    if ((IsBattleBackgroundShakeFinished() << 0x18) == 0) {
        RequestBattleBackgroundShakeStop();
        while ((IsBattleBackgroundShakeFinished() << 0x18) == 0) {
            func_080ED17C(1);
        }
    }
    p = gBattleState;
    pidx = &D_02033F36;
    p += (int)D_000027BE;
    if (p[*pidx] != 0) {
        DestroySpriteGroup(*(s32 *)0x020316F8);
        func_080CD110();
    }
    StartBattleEquipmentAnimation(*pidx, *(u8 *)0x020317D6, (temp_r4 = (u8 *)*arg1, t = temp_r4[3] << 8, temp_r4[2] | t), temp_r4[1], 0);
    while ((GetBattleAnimationResult() << 0x18) == 0) {
        func_080ED17C(1);
    }
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
