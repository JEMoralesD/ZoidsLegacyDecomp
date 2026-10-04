#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleScanlineWindowOpeningEffect(struct BattleAnimationGroup *group) asm("func_080DBE58");

void InitializeBattleScanlineWindowOpeningEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
