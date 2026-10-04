#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattlePurpleRingWaveEffect(struct BattleAnimationGroup *group) asm("func_080D9F64");

void InitializeBattlePurpleRingWaveEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
