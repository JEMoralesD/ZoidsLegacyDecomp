#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleTiltedTurquoiseRingParticleEffect(struct BattleAnimationGroup *group) asm("func_080DBE98");

void InitializeBattleTiltedTurquoiseRingParticleEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
