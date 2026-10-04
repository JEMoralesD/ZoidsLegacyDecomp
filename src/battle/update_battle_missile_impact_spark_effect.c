#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleTargetedMissileTrailSprite(void *) asm("func_080D63D0");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern s32 DivideSigned32(s32, s32) asm("func_080ECD98");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleMissileImpactSparkEffect(void *group) asm("func_080D66DC");

void UpdateBattleMissileImpactSparkEffect(void *group) {
    char *group_bytes = group;
    register s32 *effect_state asm("r8") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register s32 *increment_ptr asm("r4");
    u32 state = *effect_state;

    switch (state) {
    case BATTLE_MISSILE_IMPACT_LAUNCH:
        {
            register s32 *impact_x_slot asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.x));
            register s32 zero asm("r2");
            void *created;
            register s32 x asm("r3");

            x = *impact_x_slot;
            x -= 0x100;
            x = (s16)x;
            created = CreateBattleAnimationSprite(group_bytes, 0, 0, x,
                (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.y)), BATTLE_SPRITE_LOOP_ANIMATION, (s32)UpdateBattleTargetedMissileTrailSprite, 1);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = created;
            BATTLE_SPRITE_FIELD(created, void *, user_data.missile_trail.group) = group_bytes;
            zero = 0;
            BATTLE_SPRITE_FIELD(created, s32, user_data.missile_trail.elapsed_frames) = zero;
            BATTLE_SPRITE_FIELD(created, s32, user_data.missile_trail.impact_x) = *impact_x_slot;
            BATTLE_SPRITE_FIELD(created, s32, user_data.missile_trail.trail_resource_slot_base) = zero;
            PlayBattleAnimationSound(0);
            increment_ptr = effect_state;
            goto increment_state;
        }
        return;
    case BATTLE_MISSILE_IMPACT_CREATE:
        if ((*(s32 *)*(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) & BATTLE_SPRITE_HIDDEN) != 0) {
            register s32 *impact_x_slot asm("r5") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.x));
            s32 *impact_y_slot;
            register s32 spark_index asm("r6");
            register s32 *children asm("r9");
            register u32 *rng asm("r8");
            register s32 *child_base asm("r4");
            register s32 spawn_x asm("r3");
            s32 * volatile state_slot;
            s32 * volatile saved_impact_x_slot;
            register s32 *saved_impact_y_slot asm("r10");
            void *created;

            spawn_x = *(s16 *)impact_x_slot;
            impact_y_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.y));
            created = CreateBattleAnimationSprite(group_bytes, 2, 0,
                spawn_x, (s32)*(s16 *)impact_y_slot, 0, 0, 0);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) = created;
            spark_index = 0;
            state_slot = effect_state;
            saved_impact_x_slot = impact_x_slot;
            saved_impact_y_slot = impact_y_slot;
            child_base = (s32 *)BATTLE_ANIMATION_OFFSET(sprites[0]);
            __asm__ volatile ("" : "+r" (child_base));
            child_base = (s32 *)((s32)child_base + (s32)group_bytes);
            children = child_base;
            rng = &gRandomNumberCallback;
            do {
                register s32 random_angle asm("r4");
                register s32 spark_angle asm("r5");
                register s32 angle_jitter asm("r0");
                register s32 minimum_speed_fixed8 asm("r4");
                s32 random_speed_fixed8;
                s32 speed_fixed8;
                void *spark_sprite;
                s32 child_off;

                {
                    register u32 *rng1 asm("r1") = rng;
                    random_angle = CallFunctionR0(*rng1);
                }
                spark_angle = DivideSigned32(spark_index << 6, 7);
                angle_jitter = (u32)(random_angle * 9) >> 15;
                angle_jitter -= 0x24;
                spark_angle += angle_jitter;
                {
                    register u32 *rng2 asm("r2") = rng;
                    random_speed_fixed8 = (CallFunctionR0(*rng2) * 0x101) >> 15;
                }
                minimum_speed_fixed8 = 0x100;
                __asm__ volatile ("" : "+r" (minimum_speed_fixed8));
                speed_fixed8 = random_speed_fixed8 + minimum_speed_fixed8;
                {
                    register s32 spawn_x asm("r3");
                    register s32 *impact_y_view asm("r4");
                    spawn_x = *(s16 *)saved_impact_x_slot;
                    impact_y_view = saved_impact_y_slot;
                    spark_sprite = CreateBattleAngledProjectileSprite(group_bytes, 3, 0,
                        spawn_x, (s32)*(s16 *)impact_y_view,
                        (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), spark_angle, speed_fixed8, 0);
                }
                child_off = spark_index + 2;
                child_off <<= 2;
                *(s32 *)((char *)children + child_off) = (s32)spark_sprite;
                {
                    register s32 next asm("r0") = spark_index + 1;
                    next <<= 24;
                    next = (u32)next >> 24;
                    spark_index = next;
                }
            } while ((u32)spark_index <= 7);
            SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
            PlayBattleAnimationSound(1);
            increment_ptr = state_slot;
            goto increment_state;
        }
        return;

increment_state:
        *increment_ptr = *increment_ptr + 1;
        return;

    case BATTLE_MISSILE_IMPACT_WAIT_FOR_SPRITES:
        {
            register s32 spark_index asm("r6") = 0x18;
            register void *child asm("r1") = *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            u32 elapsed_frames;
            u32 bound;
            s32 off;

            elapsed_frames = *(u32 *)((char *)child + 0x2C);
            bound = (elapsed_frames >> 1) + 0x19;
            if ((u32)spark_index < bound && *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[24])) == 0) {
                u32 scan_bound = bound;
                s32 *children = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
                do {
                    register s32 next asm("r0") = spark_index + 1;
                    next <<= 24;
                    next = (u32)next >> 24;
                    spark_index = next;
                } while ((u32)spark_index < scan_bound &&
                    (off = spark_index << 2,
                     *(s32 *)((char *)children + off)) == 0);
            }
            if (spark_index == ((*(volatile u32 *)((char *)child + 0x2C) >> 1) + 0x19)) {
                spark_index = 1;
                if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0) {
                    s32 *children = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
                    do {
                        register s32 next asm("r0") = spark_index + 1;
                        next <<= 24;
                        next = (u32)next >> 24;
                        spark_index = next;
                    } while ((u32)spark_index <= 9 &&
                        (off = spark_index << 2,
                         *(s32 *)((char *)children + off)) == 0);
                }
                if (spark_index == 10) {
                    DestroySpriteGroup(group_bytes);
                }
            }
        }
        return;
    }
    return;
}
