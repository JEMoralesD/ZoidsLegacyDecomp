#include "m2c_prelude.h"
#include "battle_animation.h"
void ResetBattleAnimationResourceAllocation() asm("func_080D2328");
void StartBattleAnimation() asm("func_080D0B60");

void StartBattleSceneAnimation(u8 side, u8 zoid_id, u16 animation_id) asm("func_080D0F94");

void StartBattleSceneAnimation(u8 side, u8 zoid_id, u16 animation_id) {
    u8 *animation_state = (u8 *)0x02033FD0;
    s32 zero = 0;
    *(s8 *)(animation_state + 13) = BATTLE_ANIMATION_SCENE;
    ResetBattleAnimationResourceAllocation();
    *(s16 *)(animation_state + 10) = 0x40;
    StartBattleAnimation((animation_id * 0x60) + 0x087D818C, side, zoid_id, animation_id, zero, zero);
}
