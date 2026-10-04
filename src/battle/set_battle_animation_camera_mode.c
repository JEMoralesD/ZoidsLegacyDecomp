#include "m2c_prelude.h"
#include "battle_animation.h"
extern s32 gBattleAnimationCameraParameter asm("D_0203404C"); extern s8 gBattleAnimationCameraFrame asm("D_02034054"); extern u8 gBattleAnimationState[] asm("D_02033FD0");
void SetBattleAnimationCameraMode(u8 camera_mode, s32 parameter) asm("func_080D12A0");

void SetBattleAnimationCameraMode(u8 camera_mode, s32 parameter) {
    u8 effective_mode = camera_mode;
    if (((effective_mode == 6) || (effective_mode == 8)) && (gBattleAnimationState[0xD] == BATTLE_ANIMATION_SHIELD_IMPACT)) {
        effective_mode = 9;
    }
    *(u8 *)0x02034030 = effective_mode;
    gBattleAnimationCameraParameter = parameter;
    gBattleAnimationCameraFrame = 0;
}
