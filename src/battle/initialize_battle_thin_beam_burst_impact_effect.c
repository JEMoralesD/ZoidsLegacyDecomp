#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleThinBeamBurstImpactEffect(struct BattleAnimationGroup *group) asm("func_080DD7CC");

void InitializeBattleThinBeamBurstImpactEffect(struct BattleAnimationGroup *group) {
    register s32 *effect_state_slot asm("r2");
    register s32 *particle_updates_slot asm("r0");
    register s32 initial_value asm("r1");

    effect_state_slot = group;
    effect_state_slot += BATTLE_ANIMATION_OFFSET(state) / sizeof(s32);
    particle_updates_slot = group;
    particle_updates_slot += BATTLE_ANIMATION_OFFSET(effect.thin_beam_impact.particle_updates) / sizeof(s32);
    initial_value = 0;
    *particle_updates_slot = initial_value;
    *effect_state_slot = initial_value;
}
