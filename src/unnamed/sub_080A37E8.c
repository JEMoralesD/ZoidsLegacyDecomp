#include "m2c_prelude.h"

extern u8 D_020317D7;
extern u8 D_02033F36;
extern u8 D_020317D6;

void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
s32 GetBattleAnimationResult(void) asm("func_080D1C18");
void StartBattleScanlineWindowClosing(void) asm("func_080D1D44");
u8 GetBattleScanlineWindowPhase(void) asm("func_080D1E38");
void RequestBattleBackgroundShakeStop(void) asm("func_080D2200");
s32 IsBattleBackgroundShakeFinished(void) asm("func_080D222C");
void StartBattleImpactAnimation(u8, u8, u16, s32) asm("func_080D0D50");
void StartBattleShieldImpactAnimation(u8, u8, u16, s32) asm("func_080D0F08");
void func_080ED17C(s32);

s32 sub_080A37E8(u8 arg0, u8 **arg1)
{
    if (GetBattleScanlineWindowPhase() == 1) {
        StartBattleScanlineWindowClosing();
        while ((u32)GetBattleScanlineWindowPhase() <= 1U) {
            func_080ED17C(1);
        }
    }
    if ((IsBattleBackgroundShakeFinished() << 24) == 0) {
        RequestBattleBackgroundShakeStop();
        while ((IsBattleBackgroundShakeFinished() << 24) == 0) {
            func_080ED17C(1);
        }
    }
    if ((D_020317D7 & 1) == 0) {
        u8 *input;
        s32 high;

        StartBattleImpactAnimation(D_02033F36, D_020317D6,
            (input = *arg1, high = input[2] << 8, input[1] | high), 0);
    } else {
        u8 *input;
        s32 high;

        StartBattleShieldImpactAnimation(D_02033F36, D_020317D6,
            (input = *arg1, high = input[2] << 8, input[1] | high), 0);
    }
    while ((GetBattleAnimationResult() << 24) == 0) {
        func_080ED17C(1);
    }
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
