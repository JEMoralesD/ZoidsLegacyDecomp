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
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleFallingMissileImpactSparkEffect(void *group) asm("func_080D76B8");

void UpdateBattleFallingMissileImpactSparkEffect(void *group) {
    char *group_bytes = group;
    register s32 *effect_state asm("r8") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
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
            {
                register s32 *slot asm("r2") = effect_state;
                *slot = *slot + 1;
            }
            return;
        }
        return;
    case BATTLE_MISSILE_IMPACT_CREATE:
        if ((*(s32 *)*(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) & BATTLE_SPRITE_HIDDEN) != 0) {
            register s32 spawn_x asm("r3");
            register s16 *impact_x_slot asm("r5") = (s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.x));
            s16 *impact_y_slot;
            register s32 argument_register_0 asm("r0");
            register s32 argument_register_1 asm("r1");
            register s32 argument_register_2 asm("r2");
            register s32 index asm("r6");
            register s32 *sprite_slots asm("r9");
            register u32 *rng asm("r8");
            register s32 *sprite_slots_base asm("r4");
            s32 * volatile state_slot;
            s16 * volatile saved_impact_x_slot;
            register s16 *saved_impact_y_slot asm("r10");
            void *created;

            __asm__ volatile ("" : "=r" (argument_register_0));
            __asm__ volatile ("" : "=r" (argument_register_1));
            __asm__ volatile ("" : "=r" (argument_register_2));
            spawn_x = *impact_x_slot;
            __asm__ volatile ("" :: "r" (argument_register_0));
            __asm__ volatile ("" :: "r" (argument_register_1));
            __asm__ volatile ("" :: "r" (argument_register_2));
            impact_y_slot = (s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.y));
            created = CreateBattleAnimationSprite(group_bytes, 2, 0,
                spawn_x, (s32)*impact_y_slot, 0, 0, 0);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) = created;
            index = 0;
            state_slot = effect_state;
            saved_impact_x_slot = impact_x_slot;
            saved_impact_y_slot = impact_y_slot;
            sprite_slots_base = (s32 *)0xC;
            sprite_slots_base = (s32 *)((s32)sprite_slots_base + (s32)group_bytes);
            sprite_slots = sprite_slots_base;
            argument_register_0 = (s32)&gRandomNumberCallback;
            __asm__ volatile ("" : "+r" (argument_register_0));
            rng = (u32 *)argument_register_0;
            do {
                register s32 angle_random asm("r4");
                register s32 spark_angle asm("r5");
                register s32 angle_jitter asm("r0");
                register s32 minimum_speed_fixed8 asm("r4");
                s32 random_speed_fixed8;
                s32 speed_fixed8;
                void *spark_sprite;
                s32 sprite_offset;

                {
                    register u32 *rng1 asm("r1") = rng;
                    angle_random = CallFunctionR0(*rng1);
                }
                spark_angle = DivideSigned32(index << 6, 7);
                angle_jitter = (u32)(angle_random * 9) >> 15;
                angle_jitter -= 0x17;
                spark_angle += angle_jitter;
                {
                    register u32 *rng2 asm("r2") = rng;
                    random_speed_fixed8 = (CallFunctionR0(*rng2) * 0x101) >> 15;
                }
                minimum_speed_fixed8 = 0x100;
                speed_fixed8 = random_speed_fixed8 + minimum_speed_fixed8;
                {
                    register s32 spawn_x asm("r3");
                    register s32 *impact_y_view asm("r4");
                    spawn_x = *(s16 *)saved_impact_x_slot;
                    __asm__ volatile ("" : : "r" (minimum_speed_fixed8));
                    impact_y_view = saved_impact_y_slot;
                    spark_sprite = CreateBattleAngledProjectileSprite(group_bytes, 3, 0,
                        spawn_x, (s32)*(s16 *)impact_y_view,
                        (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), spark_angle, speed_fixed8, 0);
                }
                sprite_offset = index + 2;
                sprite_offset <<= 2;
                *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)spark_sprite;
                {
                    register s32 next asm("r0") = index + 1;
                    next <<= 24;
                    next = (u32)next >> 24;
                    index = next;
                }
            } while ((u32)index <= 7);
            SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
            PlayBattleAnimationSound(1);
            {
                register s32 *slot asm("r4") = state_slot;
                *slot = *slot + 1;
            }
            return;
        }
        return;

    case BATTLE_MISSILE_IMPACT_WAIT_FOR_SPRITES:
        {
            register s32 index asm("r6") = 0x18;
            register void *child asm("r1") = *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            u32 elapsed_trail_frames;
            u32 trail_slot_end;
            s32 off;

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
                    (off = index << 2,
                     *(s32 *)((char *)sprite_slots + off)) == 0);
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
                    } while ((u32)index <= 9 &&
                        (off = index << 2,
                         *(s32 *)((char *)sprite_slots + off)) == 0);
                }
                if (index == 10) {
                    DestroySpriteGroup(group_bytes);
                }
            }
        }
        return;
    }
    return;
}
