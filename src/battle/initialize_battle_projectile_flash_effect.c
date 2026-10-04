#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleProjectileFlashEffect(struct BattleAnimationGroup *group) asm("func_080D2A44");

void InitializeBattleProjectileFlashEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
