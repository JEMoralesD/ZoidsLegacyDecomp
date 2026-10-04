#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void DestroySpriteGroup(void *) asm("func_08095114");
void *CreateBattleAnimationSprite(void *, s32, s32, s16, s32, s32, s32, s32) asm("func_080D2450");
void PlayBattleAnimationSound(s32) asm("func_080D2790");

void UpdateBattleYellowParticleBeamChargeEffect(struct BattleAnimationGroup *group) asm("func_080E04F0");

void UpdateBattleYellowParticleBeamChargeEffect(struct BattleAnimationGroup *group) {
    s32 *effect_state_slot;
    s32 phase;
    s32 sound_slot;

    effect_state_slot = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(state));
    phase = *effect_state_slot;
    if (phase == BATTLE_YELLOW_PARTICLE_BEAM_CHARGE_CREATE) {
        BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) = CreateBattleAnimationSprite(group, 0, 0,
            BATTLE_ANIMATION_FIELD(group, s16, x),
            (s32)BATTLE_ANIMATION_FIELD(group, s16, y),
            BATTLE_SPRITE_SEMITRANSPARENT, phase, phase);
        sound_slot = 0;
        goto play_phase_sound;
    }
    if (phase == BATTLE_YELLOW_PARTICLE_BEAM_CHARGE_WAIT_FOR_FIRE_STEP) {
        if (BATTLE_SPRITE_FIELD(BATTLE_ANIMATION_FIELD(group, void *, sprites[0]), u16, animation_step) == BATTLE_YELLOW_PARTICLE_BEAM_CHARGE_FIRE_ANIMATION_STEP) {
            sound_slot = 1;
play_phase_sound:
            PlayBattleAnimationSound(sound_slot);
            *effect_state_slot = *effect_state_slot + 1;
        }
    } else if (BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) == 0) {
        DestroySpriteGroup(group);
    }
}
