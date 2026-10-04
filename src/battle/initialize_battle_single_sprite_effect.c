#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
s32 CreateBattleAnimationSprite(void *, s32, s32, s16, s32, s32, s32, s32) asm("func_080D2450");

void InitializeBattleSingleSpriteEffect(void *group) asm("func_080D27A8");

void InitializeBattleSingleSpriteEffect(void *group) {
    BATTLE_ANIMATION_FIELD(group, s32, sprites[0]) = CreateBattleAnimationSprite(group, 0, 0, BATTLE_ANIMATION_FIELD(group, s16, x), (s32) BATTLE_ANIMATION_FIELD(group, s16, y), 0x400, 0, 0);
}
