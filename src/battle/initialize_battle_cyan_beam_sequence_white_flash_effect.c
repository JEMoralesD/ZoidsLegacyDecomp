#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleCyanBeamSequenceWhiteFlashEffect(struct BattleAnimationGroup *group) asm("func_080DE5C8");

void InitializeBattleCyanBeamSequenceWhiteFlashEffect(struct BattleAnimationGroup *group) {
    int *effect_state_slot = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(state));
    int *sequence_updates_slot = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.cyan_beam_sequence.sequence_updates));
    *sequence_updates_slot = 0;
    *effect_state_slot = 0;
}
