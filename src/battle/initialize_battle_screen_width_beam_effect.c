#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleScreenWidthBeamEffect(void *group) asm("func_080D61C4");

void InitializeBattleScreenWidthBeamEffect(void *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
