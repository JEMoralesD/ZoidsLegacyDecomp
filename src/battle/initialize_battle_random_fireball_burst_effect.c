#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleRandomFireballBurstEffect(struct BattleAnimationGroup *group) asm("func_080E0050");

void InitializeBattleRandomFireballBurstEffect(struct BattleAnimationGroup *group) {
    register s32 *effect_state_slot asm("r2");
    register s32 *emission_count_slot asm("r0");
    register s32 zero asm("r1");

    effect_state_slot = (s32 *)group;
    effect_state_slot += BATTLE_ANIMATION_OFFSET(state) / sizeof(s32);
    emission_count_slot = (s32 *)group;
    emission_count_slot += BATTLE_ANIMATION_OFFSET(effect.random_fireball_burst.emission_count) / sizeof(s32);
    zero = 0;
    *emission_count_slot = zero;
    *effect_state_slot = zero;
}
