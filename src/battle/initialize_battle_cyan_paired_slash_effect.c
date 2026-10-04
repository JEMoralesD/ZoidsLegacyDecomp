#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleCyanPairedSlashEffect(struct BattleAnimationGroup *group) asm("func_080DB1D0");

void InitializeBattleCyanPairedSlashEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, int, state) = 0;
}
