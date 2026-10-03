#include "m2c_prelude.h"

void func_080D2450(void *, s32, s32, s16, s32, s32, s32, s32);
void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
void PlayBattleAnimationSound(s32) asm("func_080D2790");
void DestroySpriteGroup(void *) asm("func_08095114");
extern s32 D_03000054[];

void sub_080DC410(s32 *arg0) {
    func_080D2450(arg0, 0, 0,
                  (s16)(arg0[1] + *(s32 *)0x02034034 / 0x100),
                  (s32)(s16)(arg0[2] + D_03000054[1] / 0x100),
                  0x1420, 0, 0);
    SetBattleAnimationCameraMode(7, 0);
    PlayBattleAnimationSound(0);
    DestroySpriteGroup(arg0);
}
