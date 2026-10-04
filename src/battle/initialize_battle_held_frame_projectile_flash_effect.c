#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleHeldFrameProjectileFlashEffect(struct BattleAnimationGroup *group) asm("func_080D3358");

void InitializeBattleHeldFrameProjectileFlashEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
