#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleFacingHorizontalSlashEffect(struct BattleAnimationGroup *group) asm("func_080DA798");

void InitializeBattleFacingHorizontalSlashEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
