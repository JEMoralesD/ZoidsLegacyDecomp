#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern s16 Sin256(s16) asm("func_08092A90");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite(void *, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite(void *, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern s32 DivideUnsigned32(s32, s32) asm("func_080ECF00");

extern u32 gRandomNumberCallback asm("D_03000010");

struct BattleCyanBeamSineImpactView {
    u32 flags;
    s32 x;
    s32 y;
    void *sprite_slots[BATTLE_ANIMATION_GROUP_SPRITE_COUNT];
    s32 phase;
    s32 impact_started;
    u32 emission_count;
};

void UpdateBattleCyanBeamSineImpactTrailEffect(struct BattleCyanBeamSineImpactView *group) asm("func_080DD06C");

void UpdateBattleCyanBeamSineImpactTrailEffect(struct BattleCyanBeamSineImpactView *group) {
    struct BattleCyanBeamSineImpactView *effect_view = group;
    register s32 *phase_or_impact_started_slot asm("r4") = &effect_view->phase;
    s32 phase_or_envelope_step = *phase_or_impact_started_slot;

    if (phase_or_envelope_step == BATTLE_CONTACT_EFFECT_CREATE) {
        effect_view->sprite_slots[0] = CreateBattleAnimationSprite(
            effect_view, 0, 1, 0x80, (s16)effect_view->y, (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_2), phase_or_envelope_step, 1);
        PlayBattleAnimationSound(0);
        *phase_or_impact_started_slot = *phase_or_impact_started_slot + 1;
        asm volatile("" : "+r"(phase_or_impact_started_slot));
        phase_or_impact_started_slot++;
    } else {
        register s32 *impact_started_slot asm("r0") = &effect_view->impact_started;
        register s32 impact_started_value asm("r1") = *impact_started_slot;

        phase_or_impact_started_slot = impact_started_slot;
        if (impact_started_value != 0) {
            goto emit_impact_pair;
        }
        if (*(u16 *)((u8 *)effect_view->sprite_slots[0] + BATTLE_SPRITE_OFFSET(animation_step)) == BATTLE_CYAN_BEAM_IMPACT_START_ANIMATION_STEP) {
            *phase_or_impact_started_slot = 1;
            SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
            PlayBattleAnimationSound(1);
        }
    }

    {
        register s32 impact_started asm("r0") = *phase_or_impact_started_slot;
        register s32 completed_emission_count asm("r0");
        register s32 *emission_count_slot asm("sl");
        register s32 *emission_count_init asm("r1") = (s32 *)BATTLE_ANIMATION_OFFSET(effect.cyan_beam_sine_impact.emission_count);

        asm volatile("add %0, %0, %1"
                     : "+r"(emission_count_init) : "r"(effect_view));
        emission_count_slot = emission_count_init;

        if (impact_started == 0) {
            goto check_completion;
        }
emit_impact_pair:
        {
            register s32 *active_emission_count_slot asm("r0") = &effect_view->emission_count;
            register s32 active_emission_count asm("r1") = *active_emission_count_slot;
            asm volatile("" : "+r"(active_emission_count));
            emission_count_slot = active_emission_count_slot;

        if ((u32)active_emission_count < BATTLE_CYAN_BEAM_SINE_IMPACT_EMISSION_COUNT) {
            register s32 packed_envelope_height asm("r6");
            register s32 envelope_height_pixels asm("r9");
            register s32 y_random asm("r8");
            register s32 impact_x asm("r5");
            register s32 impact_y asm("r4");
            s32 envelope_sine_fixed8;

            envelope_sine_fixed8 = Sin256(
                (s16)DivideUnsigned32(active_emission_count << 7, BATTLE_CYAN_BEAM_SINE_IMPACT_EMISSION_COUNT - 1));
            if (envelope_sine_fixed8 < 0) {
                envelope_sine_fixed8 += 7;
            }
            packed_envelope_height = envelope_sine_fixed8 >> 3;
            packed_envelope_height <<= 24;
            {
                register s32 envelope_height_init asm("r2") = (u32)packed_envelope_height >> 24;
                envelope_height_pixels = envelope_height_init;
                asm volatile("" : : "r"(envelope_height_init));
            }

            impact_x = effect_view->x;
            {
                register s32 *emission_count_view asm("r1") = emission_count_slot;
                impact_x += *emission_count_view << 3;
            }
            impact_x = (u16)(impact_x - 0x20);
            {
                register u32 *random_callback_slot asm("r2") = &gRandomNumberCallback;
                y_random = CallFunctionR0(*random_callback_slot);
            }
            impact_y = effect_view->y;
            {
                register s32 *emission_count_view asm("r1") = emission_count_slot;
                register s32 phase_or_envelope_step asm("r0") = *emission_count_view;
                register s32 mask asm("r1") = 3;
                register s32 envelope_row_product asm("r2") = envelope_height_pixels;

                phase_or_envelope_step &= mask;
                envelope_row_product *= phase_or_envelope_step;
                impact_y += DivideUnsigned32(envelope_row_product, 3);
            }
            impact_y -= (u32)packed_envelope_height >> 25;
            {
                register s32 y_jitter asm("r0") =
                    (u32)(y_random * 9) >> 15;
                y_jitter += 0xFFFC;
                impact_y += y_jitter;
            }
            impact_x = (s16)impact_x;
            impact_y = (s16)impact_y;

            {
                register char *sprite_slots asm("r6");
                register void *explosion_sprite asm("r0") = CreateBattleAnimationSprite(
                    effect_view, 1, 0, impact_x, impact_y, 0x100, 0, 0);
                register s32 *emission_count_view asm("r2") = emission_count_slot;
                register s32 explosion_slot_address asm("r1") = *emission_count_view;

                explosion_slot_address += 1;
                explosion_slot_address <<= 2;
                sprite_slots = (char *)effect_view + BATTLE_ANIMATION_OFFSET(sprites[0]);
                asm volatile("add %0, %1, %0"
                             : "+r"(explosion_slot_address) : "r"(sprite_slots));
                *(void **)explosion_slot_address = explosion_sprite;

                {
                register u32 *random_callback_slot asm("r1") = &gRandomNumberCallback;
                register s32 fragment_speed_fixed8 asm("r1") =
                    ((u32)(CallFunctionR0(*random_callback_slot) * 0x101) >> 15) + 0x200;
                register void *fragment_sprite asm("r0") = CreateBattleAngledProjectileSprite(
                    effect_view, 2, 0, impact_x, impact_y, 0x20, 0, fragment_speed_fixed8, 0);
                register s32 *fragment_emission_count_view asm("r1") = emission_count_slot;
                register s32 emission_count asm("r2") = *fragment_emission_count_view;
                register s32 fragment_slot_offset asm("r1") = emission_count;

                fragment_slot_offset += 16;
                fragment_slot_offset <<= 2;
                asm volatile("add %0, %1" : "+r"(sprite_slots) : "r"(fragment_slot_offset));
                *(void **)sprite_slots = fragment_sprite;
                emission_count += 1;
                {
                    register s32 *emission_count_store asm("r0") = emission_count_slot;
                    *emission_count_store = emission_count;
                }
                }
            }
        }
        }
check_completion:

        {
            register s32 *emission_count_check asm("r1") = emission_count_slot;
            completed_emission_count = *emission_count_check;
        }
        if (completed_emission_count == BATTLE_CYAN_BEAM_SINE_IMPACT_EMISSION_COUNT) {
            u8 sprite_index = 0;
            if (effect_view->sprite_slots[0] == 0) {
                do {
                    sprite_index = (u8)(sprite_index + 1);
                } while (sprite_index < BATTLE_CYAN_BEAM_SINE_IMPACT_TRACKED_SPRITE_COUNT && effect_view->sprite_slots[sprite_index] == 0);
            }
            if (sprite_index == BATTLE_CYAN_BEAM_SINE_IMPACT_TRACKED_SPRITE_COUNT) {
                DestroySpriteGroup(effect_view);
            }
        }
    }
}
