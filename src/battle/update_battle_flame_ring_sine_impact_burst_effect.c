#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");
extern s16 Sin256(s16) asm("func_08092A90");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern s32 IsBattleAnimationSpriteAtFacingPosition(void *, s32, s32) asm("func_080D2754");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern s32 DivideSigned32(s32, s32) asm("func_080ECD98");
extern s32 ModuloUnsigned32(s32, s32) asm("func_080ECF78");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleFlameRingSineImpactBurstEffect(struct BattleAnimationGroup *group) asm("func_080E02E0");

void UpdateBattleFlameRingSineImpactBurstEffect(struct BattleAnimationGroup *group) {
    char *group_bytes = group;
    register s32 *effect_state_slot asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    s32 outgoing_stack_reserve;
    u32 saved_impact_y_bits;
    char *saved_sprite_slots;
    s32 *saved_effect_state_slot;

    asm volatile("" : "=m"(outgoing_stack_reserve), "=m"(saved_impact_y_bits),
                           "=m"(saved_sprite_slots),
                           "=m"(saved_effect_state_slot));

    if (*effect_state_slot == BATTLE_SINE_IMPACT_BURST_LAUNCH) {
        void *effect_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0,
            (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.x)) - 0x100),
            *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.y)), (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_HOLD_LAST_FRAME | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), (s32)UpdateBattleHorizontalProjectileSprite, 1);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = effect_sprite;
        *(s32 *)((char *)effect_sprite + BATTLE_SPRITE_OFFSET(user_data.horizontal_projectile.speed)) = BATTLE_SINE_IMPACT_BURST_SPEED_PIXELS;
        PlayBattleAnimationSound(0);
        *effect_state_slot = *effect_state_slot + 1;
    }

    {
        register u32 phase asm("r0") = *effect_state_slot;
    if (phase <= BATTLE_SINE_IMPACT_BURST_LAST_EMISSION) {
        if (phase == BATTLE_SINE_IMPACT_BURST_WAIT_FOR_POSITION &&
                (IsBattleAnimationSpriteAtFacingPosition(*(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])),
                    *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.x)),
                    *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.y))) << 24) != 0) {
            SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
            PlayBattleAnimationSound(1);
            *effect_state_slot = *effect_state_slot + 1;
        }
        {
            register s32 *effect_state_slot_view asm("r0") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
            register u32 burst_progress asm("r1");
            burst_progress = *effect_state_slot_view;
            saved_effect_state_slot = effect_state_slot_view;

            if (burst_progress > BATTLE_SINE_IMPACT_BURST_WAIT_FOR_POSITION) {
                register s32 burst_index_init asm("r0") = (u8)(burst_progress - BATTLE_SINE_IMPACT_BURST_FIRST_EMISSION);
                register s32 burst_index asm("sl") = burst_index_init;
                register s32 packed_envelope_height asm("r6");
                register s32 envelope_height_pixels asm("r9");
                register s32 impact_x asm("r5");
                register s32 random_bits asm("r8");
                register s32 impact_y asm("r4");
                register s32 envelope_row_work asm("r0");
                s32 envelope_sine_fixed8;
                void *effect_sprite;

                envelope_sine_fixed8 = Sin256((s16)DivideSigned32(
                    burst_index_init << 7, BATTLE_SINE_IMPACT_BURST_EMISSION_COUNT - 1));
                if (envelope_sine_fixed8 < 0) {
                    envelope_sine_fixed8 += 0xF;
                }
                packed_envelope_height = envelope_sine_fixed8 >> 4;
                packed_envelope_height <<= 24;
                {
                    register s32 envelope_height_init asm("r2") =
                        (u32)packed_envelope_height >> 24;
                    asm volatile("" : "+r"(envelope_height_init));
                    envelope_height_pixels = envelope_height_init;
                }
                impact_x = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.x));
                {
                    register s32 burst_index_view asm("r1") = burst_index;
                    register s32 burst_x_offset asm("r0") = burst_index_view << 3;
                    asm volatile("" : "+r"(burst_index_view));
                    impact_x += burst_x_offset;
                }
                impact_x = (u16)impact_x;
                {
                    register u32 *random_callback_slot asm("r2") = &gRandomNumberCallback;
                    random_bits = CallFunctionR0(*random_callback_slot);
                }
                impact_y = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.y));
                envelope_row_work = (u8)ModuloUnsigned32(burst_index, 3);
                {
                    register s32 envelope_row_product asm("r1") = envelope_height_pixels;
                    asm volatile("mul %0, %1"
                        : "+r"(envelope_row_product) : "r"(envelope_row_work));
                    envelope_row_work = envelope_row_product;
                }
                asm volatile("" : "+r"(envelope_row_work));
                envelope_row_work >>= 1;
                impact_y += envelope_row_work;
                impact_y -= (u32)packed_envelope_height >> 25;
                {
                    register s32 impact_y_jitter asm("r0");
                    register s32 random_bits_view asm("r2") = random_bits;
                    asm volatile("" : "+r"(random_bits_view));
                    impact_y_jitter = random_bits_view << 3;
                    asm volatile("add %0, %1"
                                 : "+r"(impact_y_jitter) : "r"(random_bits));
                    impact_y_jitter = (u32)impact_y_jitter >> 15;
                    impact_y_jitter += 0xFFFC;
                    asm volatile("" : "+r"(random_bits_view));
                    impact_y += impact_y_jitter;
                }
                impact_y <<= 16;
                impact_x <<= 16;
                {
                    register s32 impact_x_argument asm("r3") = impact_x >> 16;
                    register u32 impact_y_bits asm("r2") = (u32)impact_y >> 16;
                    saved_impact_y_bits = impact_y_bits;
                    impact_y >>= 16;
                    effect_sprite = CreateBattleAnimationSprite(group_bytes, 1, 0,
                        impact_x_argument, impact_y, 0, 0, 0);
                }
                {
                    register s32 burst_slot_group_index asm("r2") = burst_index + 1;
                    register s32 explosion_slot_offset asm("r1") = burst_slot_group_index << 2;
                    register char *sprite_slots asm("r3");
                    explosion_slot_offset += burst_slot_group_index;
                    explosion_slot_offset <<= 2;
                    sprite_slots = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
                    /* The sixth burst writes slots 30–34, overlapping state and impact coordinates. */
                    *(void **)(sprite_slots + explosion_slot_offset) = effect_sprite;
                    {
                        register u32 sprite_slot_index asm("r6") = 0;
                        register u32 *random_callback_slot asm("r9");
                        register s32 burst_slot_group asm("r8");

                        saved_sprite_slots = sprite_slots;
                        asm volatile("" : "+m"(saved_sprite_slots));
                        random_callback_slot = &gRandomNumberCallback;
                        burst_slot_group = burst_slot_group_index;

                        do {
                            register s32 angle asm("r4");
                            register s32 particle_speed_fixed8 asm("r1");
                            register s32 minimum_speed_fixed8 asm("r0");
                            register s32 slot_index asm("r1");
                            void *particle_sprite;

                            {
                                register u32 *random_callback_view asm("r1") = random_callback_slot;
                                angle = (u32)(CallFunctionR0(*random_callback_view) * 0x41)
                                    >> 15;
                            }
                            angle -= 0x20;
                            {
                                register u32 *random_callback_view asm("r2") = random_callback_slot;
                                particle_speed_fixed8 = (u32)(CallFunctionR0(*random_callback_view) *
                                    0x101) >> 15;
                            }
                            minimum_speed_fixed8 = 0x100;
                            particle_speed_fixed8 += minimum_speed_fixed8;
                            asm volatile("" : "+r"(minimum_speed_fixed8));
                            {
                                register volatile s32 *outgoing_arguments asm("sp");
                                register u32 impact_y_bits asm("r2") = saved_impact_y_bits;
                                register s32 impact_y_argument asm("r0");
                                register s32 particle_flags asm("r0");
                                register s32 zero asm("r0");
                                register char *group_argument asm("r0");
                                register s32 particle_resource_slot asm("r1");
                                register s32 animation_zero asm("r2");
                                register s32 impact_x_argument asm("r3");

                                impact_y_argument = impact_y_bits << 16;
                                impact_y_argument >>= 16;
                                outgoing_arguments[0] = impact_y_argument;
                                particle_flags = (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1);
                                outgoing_arguments[1] = particle_flags;
                                outgoing_arguments[2] = angle;
                                outgoing_arguments[3] = particle_speed_fixed8;
                                zero = 0;
                                outgoing_arguments[4] = zero;
                                group_argument = group_bytes;
                                particle_resource_slot = 2;
                                animation_zero = 0;
                                asm volatile("asr %0, %1, #16"
                                    : "=r"(impact_x_argument) : "r"(impact_x));
                                particle_sprite = CreateBattleAngledProjectileSprite(
                                    group_argument, particle_resource_slot, animation_zero, impact_x_argument);
                            }
                            slot_index = burst_slot_group << 2;
                            slot_index += burst_slot_group;
                            slot_index += sprite_slot_index;
                            slot_index += 1;
                            slot_index <<= 2;
                            {
                                register char *children_view asm("r2") =
                                    saved_sprite_slots;
                                asm volatile("add %0, %1, %0"
                                    : "+r"(slot_index)
                                    : "r"(children_view));
                                *(void **)slot_index = particle_sprite;
                            }
                            {
                                register u32 next_index_bits asm("r0") = sprite_slot_index + 1;
                                next_index_bits <<= 24;
                                sprite_slot_index = next_index_bits >> 24;
                            }
                        } while (sprite_slot_index < BATTLE_SINE_IMPACT_BURST_PARTICLES_PER_EMISSION);
                    }
                }
                {
                    register s32 *saved_phase_slot asm("r1") = saved_effect_state_slot;
                    register s32 saved_phase_value asm("r0");
                    asm volatile(".short 0x6808, 0x3001, 0x6008"
                                 : "=r"(saved_phase_value)
                                 : "r"(saved_phase_slot) : "memory");
                }
            }
        }
    } else {
        register u32 sprite_slot_index asm("r6") = 0;
        if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            register char *sprite_slots asm("r1") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
            do {
                register u32 next_index_bits asm("r0") = sprite_slot_index + 1;
                next_index_bits <<= 24;
                sprite_slot_index = next_index_bits >> 24;
            } while (sprite_slot_index < BATTLE_SINE_IMPACT_BURST_CLEANUP_SLOT_COUNT &&
                *(s32 *)(sprite_slots + (sprite_slot_index << 2)) == 0);
        }
        if (sprite_slot_index == BATTLE_SINE_IMPACT_BURST_CLEANUP_SLOT_COUNT) {
            DestroySpriteGroup(group_bytes);
        }
    }
    }
}
