#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern void DestroySprite(void *) asm("func_08094554");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void QueueCopy(void *, void *, s32) asm("func_08095208");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite(void *, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern void SpawnBattleInwardChargeParticle(void *) asm("func_080DCDC4");

extern s32 gBattleZoidScrollX asm("D_02034034");
extern s32 gBattleCyanChargePaletteBankWord asm("D_020348B4");
extern struct SpriteBackgroundScrollOffsets gSpriteBackgroundScroll[] asm("D_03000054");
extern u8 gBattleCyanChargePaletteSequence[] asm("D_087A2AF0");
extern u8 gBattleCyanChargePaletteBlocks[] asm("D_087A2A50");

void UpdateBattleChargedCyanBeamEffect(struct BattleAnimationGroup *group) asm("func_080DCEC0");

void UpdateBattleChargedCyanBeamEffect(struct BattleAnimationGroup *group) {
    char *group_bytes = group;
    register s32 *effect_state_slot asm("r7") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    u32 phase = *effect_state_slot;
    u32 charge_animation_step;
    register s32 *saved_effect_state_slot asm("r6");

    if (phase <= 1) {
        goto emit_charge_particle;
    }
    saved_effect_state_slot = effect_state_slot;
    if (phase != 2) {
        goto dispatch;
    }
    charge_animation_step = *(u16 *)(*(char **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) + BATTLE_SPRITE_OFFSET(animation_step));
    if (charge_animation_step > 7) {
        goto check_charge_particle_cleanup;
    }
emit_charge_particle:
    {
        s32 *charge_updates_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.charged_cyan_beam.charge_updates));
        if ((*charge_updates_slot & 1) == 0) {
            /* Native R1 supplies speed_scale = 1 through the partial call signature. */
            SpawnBattleInwardChargeParticle(group_bytes);
        }
        *charge_updates_slot += 1;
        saved_effect_state_slot = effect_state_slot;
        if (*charge_updates_slot == BATTLE_CYAN_BEAM_CHARGE_UPDATES) {
            *saved_effect_state_slot += 1;
        }
        goto dispatch;
    }
check_charge_particle_cleanup:
    if (charge_animation_step == 9) {
        u8 charge_particle_slot = 1;
        char *sprite_slots = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
        do {
            void *charge_particle = *(void **)(sprite_slots + (charge_particle_slot << 2));
            if (charge_particle != 0) {
                DestroySprite(charge_particle);
            }
            charge_particle_slot = (u8)(charge_particle_slot + 1);
        } while (charge_particle_slot < BATTLE_ANIMATION_GROUP_SPRITE_COUNT);
    }

dispatch:
    {
    u32 current_phase = *saved_effect_state_slot;
    switch (current_phase) {
    case BATTLE_CHARGED_CYAN_BEAM_EMIT_CHARGE_PARTICLES:
        if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.charged_cyan_beam.charge_updates)) == 1) {
            PlayBattleAnimationSound(0);
        }
        break;
    case BATTLE_CHARGED_CYAN_BEAM_CREATE_CHARGE_ORB:
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAnimationSprite(group_bytes, 0, 0, *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)), (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), 0, 0);
        goto advance;
    case BATTLE_CHARGED_CYAN_BEAM_WAIT_FOR_CHARGE_ORB: {
        char *charge_orb_sprite = *(char **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        if (charge_orb_sprite == 0) {
            goto advance;
        }
        if (*(u16 *)(charge_orb_sprite + BATTLE_SPRITE_OFFSET(frame_timer)) == 0) {
            QueueCopy(&gBattleCyanChargePaletteBlocks[gBattleCyanChargePaletteSequence[*(u16 *)(charge_orb_sprite + BATTLE_SPRITE_OFFSET(animation_step))] << 4],
                (void *)((gBattleCyanChargePaletteBankWord << 5) + 0x05000200), 0x10);
        }
        break;
    }
    case BATTLE_CHARGED_CYAN_BEAM_WAIT_FOR_BEAM_DELAY: {
        s32 *beam_delay_updates_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.charged_cyan_beam.beam_delay_updates));
        *beam_delay_updates_slot += 1;
        if (*beam_delay_updates_slot != BATTLE_CYAN_BEAM_DELAY_UPDATES) {
            break;
        }
        goto advance;
    }
    case BATTLE_CHARGED_CYAN_BEAM_CREATE_BEAM: {
        s32 group_x = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x));
        s32 scroll_x_offset = gBattleZoidScrollX / 0x100 - 0x80;
        s32 x = (s16)(group_x + scroll_x_offset);
        s32 y = (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)) + gSpriteBackgroundScroll[0].y_fixed8 / 0x100);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAnimationSprite(group_bytes, 2, 0, x, y, (BATTLE_SPRITE_BACKGROUND_RELATIVE | BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), 0, 0);
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_RECOIL_SCROLL, 0);
        PlayBattleAnimationSound(1);
    advance:
        *saved_effect_state_slot += 1;
        break;
    }
    case BATTLE_CHARGED_CYAN_BEAM_WAIT_FOR_BEAM:
        if (*(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            DestroySpriteGroup(group_bytes);
        }
        break;
    }
    }
}
