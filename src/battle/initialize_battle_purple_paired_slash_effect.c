#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattlePurplePairedSlashEffect(struct BattleAnimationGroup *group) asm("func_080DB464");

void InitializeBattlePurplePairedSlashEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
