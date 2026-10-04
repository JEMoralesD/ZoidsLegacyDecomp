#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");

void InitializeBattleAlternatingStarAndSparkEffect(struct BattleAnimationGroup *group) asm("func_080DC47C");

void InitializeBattleAlternatingStarAndSparkEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
    PlayBattleAnimationSound(0);
}
