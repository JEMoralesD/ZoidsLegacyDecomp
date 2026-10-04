#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleDustBurstAndRisingSmokeEffect(struct BattleAnimationGroup *group) asm("func_080DB7BC");

void InitializeBattleDustBurstAndRisingSmokeEffect(struct BattleAnimationGroup *group) {
    int *effect_state = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(state));
    int *smoke_count = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.dust_burst.smoke_count));
    *smoke_count = 0;
    *effect_state = 0;
}
