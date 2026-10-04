#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void InitializeBattleFlameArcParticleAndRingProjectileEffect(struct BattleAnimationGroup *group) asm("func_080E0124");

void InitializeBattleFlameArcParticleAndRingProjectileEffect(struct BattleAnimationGroup *group) {
    s32 *effect_state_slot = &group->state;
    s32 *unused_slot = (s32 *)&group->effect.flame_arc_particle_ring.unused;
    *unused_slot = 0;
    *effect_state_slot = 0;
}
