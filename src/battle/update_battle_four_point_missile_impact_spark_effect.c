#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleTargetedMissileTrailSprite(void *) asm("func_080D63D0");
extern s16 Sin256(s16) asm("func_08092A90");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern s32 DivideSigned32(s32, s32) asm("func_080ECD98");
extern s32 ModuloUnsigned32(s32, s32) asm("func_080ECF78");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleFourPointMissileImpactSparkEffect(void *group) asm("func_080D6894");

void UpdateBattleFourPointMissileImpactSparkEffect(void *group) {
    char *group_bytes = group;
    register u32 *state_slot asm("r5") = (u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    s32 outgoing_reserve;
    u32 saved_y;
    s32 zero_value;
    char *saved_children;
    s32 *impact_elapsed_slot;
    s32 impact_sprite_offset;

    asm volatile("" : "=m"(outgoing_reserve), "=m"(saved_y), "=m"(zero_value),
                  "=m"(saved_children), "=m"(impact_elapsed_slot), "=m"(impact_sprite_offset));

    switch (*state_slot) {
    case BATTLE_MISSILE_IMPACT_LAUNCH: {
        register s32 *x_slot asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.x));
        register s32 x asm("r3") = *x_slot - 0x100;
        register s32 zero asm("r2");
        void *created;

        x = (s16)x;
        created = CreateBattleAnimationSprite(group_bytes, 0, 0, x,
            (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.y)), BATTLE_SPRITE_LOOP_ANIMATION, (s32)UpdateBattleTargetedMissileTrailSprite, 1);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = created;
        BATTLE_SPRITE_FIELD(created, void *, user_data.missile_trail.group) = group_bytes;
        zero = 0;
        BATTLE_SPRITE_FIELD(created, s32, user_data.missile_trail.elapsed_frames) = zero;
        BATTLE_SPRITE_FIELD(created, s32, user_data.missile_trail.impact_x) = *x_slot;
        BATTLE_SPRITE_FIELD(created, s32, user_data.missile_trail.trail_resource_slot_base) = zero;
        PlayBattleAnimationSound(0);
        *state_slot = *state_slot + 1;
        break;
    }
    case BATTLE_MISSILE_IMPACT_CREATE:
        if ((**(u32 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) & BATTLE_SPRITE_HIDDEN) == 0) {
            break;
        }
        {
            register s32 *impact_elapsed_init asm("r0") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.elapsed_frames));
            register u32 elapsed_impact_frames asm("r1") = *impact_elapsed_init;
            register s32 remainder asm("r2");

            impact_elapsed_slot = impact_elapsed_init;
            if (elapsed_impact_frames <= 0xC) {
                remainder = 3;
                remainder &= elapsed_impact_frames;
                zero_value = remainder;

                if (remainder == 0) {
                register s32 impact_index_init asm("r0") = (u8)(elapsed_impact_frames >> 2);
                register s32 impact_index asm("sl") = impact_index_init;
                register s32 spread_bits asm("r6");
                register s32 spread asm("r9");
                register s32 x_value asm("r5");
                register s32 random asm("r8");
                register s32 y_value asm("r4");
                register s32 spread_phase asm("r0");
                register u32 index asm("r6");
                s32 spread_sine;
                void *created;

                spread_sine = Sin256((s16)DivideSigned32(
                    impact_index_init << 7, 3));
                if (spread_sine < 0) {
                    spread_sine += 0xF;
                }
                spread_bits = spread_sine >> 4;
                spread_bits <<= 24;
                {
                    register s32 spread_init asm("r4") =
                        (u32)spread_bits >> 24;
                    asm volatile("" : "+r"(spread_init));
                    spread = spread_init;
                }
                x_value = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.x));
                {
                    register s32 impact_index_view asm("r1") = impact_index;
                    register s32 offset asm("r0") = impact_index_view << 1;
                    asm volatile("" : "+r"(impact_index_view));
                    offset += impact_index;
                    offset <<= 2;
                    x_value += offset;
                }
                x_value = (u16)x_value;
                {
                    register u32 *rng asm("r2") = &gRandomNumberCallback;
                    random = CallFunctionR0(*rng);
                }
                y_value = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.y));
                spread_phase = (u8)ModuloUnsigned32(impact_index, 3);
                {
                    register s32 product asm("r1") = spread;
                    asm volatile("mul %0, %1" : "+r"(product) : "r"(spread_phase));
                    spread_phase = product;
                }
                asm volatile("" : "+r"(spread_phase));
                spread_phase >>= 1;
                y_value += spread_phase;
                y_value -= (u32)spread_bits >> 25;
                {
                    register s32 perturb asm("r0");
                    register s32 random_view asm("r2") = random;
                    asm volatile("" : "+r"(random_view));
                    perturb = random_view << 3;
                    asm volatile("add %0, %1" : "+r"(perturb) : "r"(random));
                    perturb = (u32)perturb >> 15;
                    perturb += 0xFFFC;
                    y_value += perturb;
                }
                y_value <<= 16;
                x_value <<= 16;
                {
                    register s32 x_arg asm("r3") = x_value >> 16;
                    register u32 y_bits asm("r2") = (u32)y_value >> 16;
                    register volatile s32 *outgoing asm("sp");
                    saved_y = y_bits;
                    y_value >>= 16;
                    outgoing[0] = y_value;
                    {
                        register s32 zero_arg asm("r4") = zero_value;
                        asm volatile("" : "+r"(zero_arg));
                        outgoing[1] = zero_arg;
                        outgoing[2] = zero_arg;
                        outgoing[3] = zero_arg;
                    }
                    created = CreateBattleAnimationSprite(group_bytes, 2, 0, x_arg);
                }
                {
                    register s32 slot_index asm("r1") = impact_index;
                    register s32 impact_slot_offset asm("r3") = slot_index << 2;
                    register char *children asm("r2");
                    slot_index = impact_slot_offset + slot_index;
                    slot_index += 1;
                    slot_index <<= 2;
                    children = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
                    *(void **)(children + slot_index) = created;
                    index = 0;
                    impact_sprite_offset = impact_slot_offset;
                    saved_children = children;
                }
                {
                    register u32 *rng_init asm("r2") = &gRandomNumberCallback;
                    register u32 *rng asm("r8") = rng_init;
                    register s32 x_bits asm("r9") = x_value;
                    asm volatile("" : "+r"(rng_init));

                    do {
                        register s32 angle asm("r5");
                        register s32 angle_random asm("r4");
                        register s32 speed_fixed8 asm("r1");
                        void *spark_sprite;

                        {
                            register u32 *rng_view asm("r4") = rng;
                            asm volatile("" : "+r"(rng_view));
                            angle_random = CallFunctionR0(*rng_view);
                        }
                        angle = DivideSigned32(index << 6, 3);
                        {
                            register s32 perturb asm("r0") =
                                (u32)(angle_random * 9) >> 15;
                            perturb -= 0x24;
                            angle += perturb;
                        }
                        {
                            register u32 *rng_view asm("r1") = rng;
                            asm volatile("" : "+r"(rng_view));
                            speed_fixed8 = ((u32)(CallFunctionR0(*rng_view) * 0x101)
                                >> 15);
                            {
                                register s32 minimum_speed_fixed8 asm("r2") = 0x100;
                                asm volatile("" : "+r"(minimum_speed_fixed8));
                                speed_fixed8 += minimum_speed_fixed8;
                            }
                        }
                        {
                            register volatile s32 *outgoing asm("sp");
                            register s32 saved_y_view asm("r4") = saved_y;
                            register s32 y_arg asm("r0") = (s16)saved_y_view;

                            asm volatile("" : "+r"(saved_y_view));
                            outgoing[0] = y_arg;
                            outgoing[1] = (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1);
                            outgoing[2] = angle;
                            outgoing[3] = speed_fixed8;
                            outgoing[4] = 0;
                            {
                                register void *owner_arg asm("r0") = group_bytes;
                                register s32 resource_slot_arg asm("r1") = 3;
                                register s32 zero_arg asm("r2") = 0;
                                register s32 x_view asm("r4") = x_bits;
                                register s32 x_arg asm("r3") = x_view >> 16;
                                asm volatile("" : "+r"(owner_arg),
                                    "+r"(resource_slot_arg), "+r"(zero_arg));
                                asm volatile("" : "+r"(x_view));
                                spark_sprite = CreateBattleAngledProjectileSprite(
                                    owner_arg, resource_slot_arg, zero_arg, x_arg);
                            }
                        }
                        {
                            register s32 slot_index asm("r1") = impact_sprite_offset;
                            register char *children asm("r2");
                            slot_index += impact_index;
                            slot_index = index + slot_index;
                            slot_index += 2;
                            slot_index <<= 2;
                            children = saved_children;
                            *(void **)(children + slot_index) = spark_sprite;
                        }
                        {
                            register u32 next asm("r0") = index + 1;
                            asm volatile("" : "+r"(next));
                            index = (u8)next;
                        }
                    } while (index <= 3);
                }
                }

                {
                    register s32 *impact_elapsed_view asm("r4") = impact_elapsed_slot;
                    asm volatile("" : "+r"(impact_elapsed_view));
                    if (*impact_elapsed_view == 0) {
                        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
                        PlayBattleAnimationSound(1);
                    }
                }
                *impact_elapsed_slot = *impact_elapsed_slot + 1;
            } else {
                *state_slot = 2;
            }
        }
        break;

    case BATTLE_MISSILE_IMPACT_WAIT_FOR_SPRITES: {
        register u32 index asm("r6") = 0x18;
        register char *missile_bytes asm("r1") = *(char **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        register u32 elapsed_trail_frames asm("r0") = *(u32 *)(missile_bytes + 0x2C);
        register u32 bound asm("r2");
        elapsed_trail_frames >>= 1;
        asm volatile("" : "+r"(elapsed_trail_frames));
        bound = elapsed_trail_frames;
        bound += 0x19;

        if (index < bound && *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[24])) == 0) {
            register u32 scan_bound asm("r3") = bound;
            register char *children asm("r2") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
            do {
                register u32 next asm("r0") = index + 1;
                asm volatile("" : "+r"(next));
                index = (u8)next;
            } while (index < scan_bound &&
                *(s32 *)(children + (index << 2)) == 0);
        }
        if (index == ((*(u32 *)(missile_bytes + 0x2C) >> 1) + 0x19)) {
            index = 1;
            if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0) {
                register char *children asm("r1") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
                do {
                    register u32 next asm("r0") = index + 1;
                    asm volatile("" : "+r"(next));
                    index = (u8)next;
                } while (index <= 0x14 &&
                    *(s32 *)(children + (index << 2)) == 0);
            }
            if (index == 0x15) {
                DestroySpriteGroup(group_bytes);
            }
        }
        break;
    }
    }
}
