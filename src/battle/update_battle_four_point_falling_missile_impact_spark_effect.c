#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleTargetedFallingMissileTrailSprite(void *) asm("func_080D6E6C");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern s32 DivideSigned32(s32, s32) asm("func_080ECD98");
extern u8 ModuloUnsigned32(u8, s32) asm("func_080ECF78");
extern s16 Sin256(s16) asm("func_08092A90");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleFourPointFallingMissileImpactSparkEffect(void *group) asm("func_080D7878");

void UpdateBattleFourPointFallingMissileImpactSparkEffect(void *group) {
    char *group_bytes = group;
    register s32 *effect_state asm("r5") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    u32 state = *effect_state;

    switch (state) {
    case BATTLE_MISSILE_IMPACT_LAUNCH:
        {
            register s32 *impact_x_slot asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.x));
            void *created;
            register s32 x asm("r3");
            register s32 *y_ptr asm("r0");
            register s32 y asm("r0");

            x = *impact_x_slot;
            x -= 0x100;
            x = (s16)x;
            y_ptr = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.y));
            y = *y_ptr;
            y -= 0x80;
            y = (s16)y;
            created = CreateBattleAnimationSprite(group_bytes, 0, 0, x,
                y, BATTLE_SPRITE_LOOP_ANIMATION, (s32)UpdateBattleTargetedFallingMissileTrailSprite, 1);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = created;
            BATTLE_SPRITE_FIELD(created, void *, user_data.missile_trail.group) = group_bytes;
            BATTLE_SPRITE_FIELD(created, s32, user_data.missile_trail.elapsed_frames) = 0;
            BATTLE_SPRITE_FIELD(created, s32, user_data.missile_trail.impact_x) = *impact_x_slot;
            PlayBattleAnimationSound(0);
            *effect_state += 1;
            return;
        }
        return;
    case BATTLE_MISSILE_IMPACT_CREATE:
        if ((*(s32 *)*(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) & BATTLE_SPRITE_HIDDEN) != 0) {
            u32 impact_y_bits;
            s32 *impact_elapsed_slot;
            s32 *impact_elapsed_address = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.elapsed_frames));
            u32 elapsed_impact_frames = *impact_elapsed_address;

            asm volatile("" : "+r"(impact_elapsed_address));
            impact_elapsed_slot = impact_elapsed_address;

            if (elapsed_impact_frames <= 12) {
                s32 emission_phase = 3;
                emission_phase &= elapsed_impact_frames;
                if (emission_phase == 0) {
                    u8 impact_index = elapsed_impact_frames >> 2;
                    s32 spread_sine;
                    s32 spread_fixed4;
                    u8 spread;
                    s32 impact_offset_12;
                    u32 random;
                    s32 spread_offset;
                    u16 spread_offset_bits;
                    s32 signed_spread_offset;
                    s32 x;
                    u16 impact_x_bits;
                    s32 y;
                    s32 *sprite_slots;
                    s32 impact_slot_base;
                    void *created;
                    void *spark_sprite;
                    s32 sprite_offset;
                    u8 spark_index;

                    spread_sine = Sin256((s16)DivideSigned32(impact_index << 7, 3));
                    if (spread_sine < 0) {
                        spread_sine += 15;
                    }
                    spread_fixed4 = spread_sine >> 4;
                    spread = spread_fixed4;
                    impact_offset_12 = impact_index << 1;
                    impact_offset_12 += impact_index;
                    impact_offset_12 <<= 2;
                    random = CallFunctionR0(gRandomNumberCallback);
                    spread_offset = spread * ModuloUnsigned32(impact_index, 3);
                    spread_offset >>= 1;
                    spread_offset -= (u8)spread_fixed4 >> 1;
                    spread_offset -= 4;
                    spread_offset += (random * 9) >> 15;
                    spread_offset_bits = spread_offset;
                    x = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.x));
                    x += DivideSigned32(impact_offset_12 * 2, 3);
                    signed_spread_offset = (s16)spread_offset_bits;
                    x += DivideSigned32(signed_spread_offset, 3);
                    impact_x_bits = x;
                    y = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.y));
                    y += DivideSigned32(impact_offset_12, 3);
                    signed_spread_offset *= 2;
                    y += DivideSigned32(signed_spread_offset, 3);
                    {
                        s32 impact_y_shifted = y << 16;
                        s32 impact_x_value = (s16)impact_x_bits;

                        impact_y_bits = (u32)impact_y_shifted >> 16;
                        created = CreateBattleAnimationSprite(group_bytes, 2, 0, impact_x_value,
                            impact_y_shifted >> 16, emission_phase, emission_phase, emission_phase);
                    }
                    {
                        s32 impact_slot_base_init;
                        s32 *sprite_slots_init;
                        s32 sprite_offset;

                        impact_slot_base_init = impact_index * 4;
                        sprite_offset = impact_slot_base_init + impact_index + 1;
                        sprite_offset <<= 2;
                        sprite_slots_init = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
                        *(s32 *)((char *)sprite_slots_init + sprite_offset) = (s32)created;
                        spark_index = 0;
                        impact_slot_base = impact_slot_base_init;
                        sprite_slots = sprite_slots_init;
                    }
                    {
                        u32 *rng = &gRandomNumberCallback;
                        asm volatile("" : "+r"(rng));
                        asm volatile("" :: "r"(spark_index));
                        do {
                            u32 angle_random = CallFunctionR0(*rng);
                            s32 spark_angle = DivideSigned32(spark_index << 6, 3);
                            s32 angle_jitter = (angle_random * 9) >> 15;
                            s32 sprite_offset;

                            angle_jitter -= 0x17;
                            spark_angle += angle_jitter;
                            spark_sprite = CreateBattleAngledProjectileSprite(
                                group_bytes, 3, 0, (s16)impact_x_bits,
                                ({ register s32 impact_y_view asm("r3") = impact_y_bits;
                                   register s32 impact_y_value asm("r0") = impact_y_view << 16;
                                   impact_y_value >>= 16; impact_y_value; }),
                                (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), spark_angle,
                                ((CallFunctionR0(*rng) * 0x101) >> 15) + 0x100, 0);
                            sprite_offset = spark_index + (impact_slot_base + impact_index) + 2;
                            sprite_offset <<= 2;
                            *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)spark_sprite;
                            spark_index += 1;
                        } while (spark_index <= 3);
                    }
                }
                if (*impact_elapsed_slot == 0) {
                    SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
                    PlayBattleAnimationSound(1);
                }
                *impact_elapsed_slot += 1;
            } else {
                *effect_state = 2;
            }
            return;
        }
        return;
    case BATTLE_MISSILE_IMPACT_WAIT_FOR_SPRITES:
        {
            register s32 index asm("r7") = 0x18;
            register char *group_view asm("r2") = (char *)group_bytes;
            register void *child asm("r1") = ({ asm volatile("" : "+r"(group_view)); *(void **)(group_view + BATTLE_ANIMATION_OFFSET(sprites[0])); });
            u32 elapsed_trail_frames;
            u32 trail_slot_end;
            s32 sprite_offset;

            elapsed_trail_frames = BATTLE_SPRITE_FIELD(child, u32, user_data.missile_trail.elapsed_frames);
            trail_slot_end = (elapsed_trail_frames >> 1) + 0x19;
            if ((u32)index < trail_slot_end && *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[24])) == 0) {
                u32 trail_scan_end = trail_slot_end;
                s32 *sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
                do {
                    register s32 next asm("r0") = index + 1;
                    next <<= 24;
                    next = (u32)next >> 24;
                    index = next;
                } while ((u32)index < trail_scan_end &&
                    (sprite_offset = index << 2,
                     *(s32 *)((char *)sprite_slots + sprite_offset)) == 0);
            }
            if (index == ((*(volatile u32 *)((char *)child + 0x2C) >> 1) + 0x19)) {
                index = 1;
                if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0) {
                    s32 *sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
                    do {
                        register s32 next asm("r0") = index + 1;
                        next <<= 24;
                        next = (u32)next >> 24;
                        index = next;
                    } while ((u32)index <= 0x14 &&
                        (sprite_offset = index << 2,
                         *(s32 *)((char *)sprite_slots + sprite_offset)) == 0);
                }
                if (index == 0x15) {
                    DestroySpriteGroup(group_bytes);
                }
            }
        }
        return;
    }
    return;
}
