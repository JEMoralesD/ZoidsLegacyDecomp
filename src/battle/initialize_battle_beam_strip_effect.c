#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleBeamStripEffect(void *group) asm("func_080D32CC");

void InitializeBattleBeamStripEffect(void *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
