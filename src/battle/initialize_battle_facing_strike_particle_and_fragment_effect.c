#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleFacingStrikeParticleAndFragmentEffect(struct BattleAnimationGroup *group) asm("func_080DA3EC");

void InitializeBattleFacingStrikeParticleAndFragmentEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
