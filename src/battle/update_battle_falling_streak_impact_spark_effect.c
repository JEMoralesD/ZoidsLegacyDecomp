#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern s16 Sin256(s32) asm("func_08092A90");
extern s16 Cos256(s32) asm("func_08092ADC");
extern void DestroySprite(void *) asm("func_08094554");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleFallingStreakImpactSparkEffect(struct BattleAnimationGroup *group) asm("func_080DC040");

void UpdateBattleFallingStreakImpactSparkEffect(struct BattleAnimationGroup *group) {
    char *group_bytes = group;
    register s32 *saved_effect_state asm("r8") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register u32 phase asm("r0") = *saved_effect_state;
    u8 sprite_index;

    switch (phase) {
    case BATTLE_FALLING_STREAK_IMPACT_LAUNCH: {
        register s32 launch_or_impact_x asm("r4");
        register s32 trig_component_or_launch_y asm("r0");
        register s32 impact_coordinate asm("r2");
        void *effect_sprite;

        trig_component_or_launch_y = Cos256(0x36);
        {
            register char *impact_coordinate_view asm("r1") = group_bytes;
            impact_coordinate_view += BATTLE_ANIMATION_OFFSET(effect.impact.x);
            impact_coordinate = *(s32 *)impact_coordinate_view;
        }
        trig_component_or_launch_y = (s16)trig_component_or_launch_y;
        {
            register s32 scaled_trig_component asm("r1") = trig_component_or_launch_y << 1;
            scaled_trig_component += trig_component_or_launch_y;
            trig_component_or_launch_y = scaled_trig_component << 6;
        }
        if (trig_component_or_launch_y < 0) {
            trig_component_or_launch_y += 0xFF;
        }
        trig_component_or_launch_y >>= 8;
        trig_component_or_launch_y = impact_coordinate - trig_component_or_launch_y;
        launch_or_impact_x = (s16)trig_component_or_launch_y;

        trig_component_or_launch_y = Sin256(0x36);
        {
            register char *impact_coordinate_view asm("r1") = group_bytes;
            impact_coordinate_view += BATTLE_ANIMATION_OFFSET(effect.impact.y);
            impact_coordinate = *(s32 *)impact_coordinate_view;
        }
        trig_component_or_launch_y = (s16)trig_component_or_launch_y;
        {
            register s32 scaled_trig_component asm("r1") = trig_component_or_launch_y << 1;
            scaled_trig_component += trig_component_or_launch_y;
            trig_component_or_launch_y = scaled_trig_component << 6;
        }
        if (trig_component_or_launch_y < 0) {
            trig_component_or_launch_y += 0xFF;
        }
        trig_component_or_launch_y >>= 8;
        trig_component_or_launch_y = impact_coordinate - trig_component_or_launch_y;
        trig_component_or_launch_y = (s16)trig_component_or_launch_y;
        effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, BATTLE_FALLING_STREAK_RESOURCE_STREAK, 0, launch_or_impact_x, trig_component_or_launch_y,
            (BATTLE_SPRITE_LOOP_ANIMATION | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), 0x4A, 0xC00, 1);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = effect_sprite;
        PlayBattleAnimationSound(0);
        {
            register s32 *effect_state_slot asm("r1") = saved_effect_state;
            *effect_state_slot = *effect_state_slot + 1;
        }
        break;
    }
    case BATTLE_FALLING_STREAK_IMPACT_CREATE_EXPLOSION_AND_SPARKS: {
        s32 *elapsed_updates_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.elapsed_frames));
        register char *impact_x_view asm("r5");
        register char *impact_y_view asm("r4");
        register char *saved_impact_x_view asm("sl");
        register char *saved_impact_y_view asm("r9");
        register s32 *sprite_slots asm("r8");
        s32 * volatile saved_effect_state_slot;
        volatile s32 spark_angle_slot;

        *elapsed_updates_slot = *elapsed_updates_slot + 1;
        if (*elapsed_updates_slot != BATTLE_FALLING_STREAK_FLIGHT_UPDATES) {
            break;
        }
        DestroySprite(*(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])));
        impact_x_view = group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.x);
        {
            register s32 impact_x_load_offset asm("r2");
            register s32 launch_or_impact_x asm("r3");
            register s32 impact_y_load_offset asm("r1");
            register s32 impact_y asm("r0");

            asm volatile(
                "mov %0, #0\n\t"
                "ldrsh %1, [%2, %0]"
                : "=r"(impact_x_load_offset), "=r"(launch_or_impact_x)
                : "r"(impact_x_view));
            impact_y_view = group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.y);
            asm volatile(
                "mov %0, #0\n\t"
                "ldrsh %1, [%2, %0]"
                : "=r"(impact_y_load_offset), "=r"(impact_y)
                : "r"(impact_y_view));
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAnimationSprite(group_bytes, BATTLE_FALLING_STREAK_RESOURCE_EXPLOSION, 0,
                launch_or_impact_x, impact_y, BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1, 0, 0);
        }

        sprite_index = 0;
        {
            register s32 *effect_state_view asm("r2") = saved_effect_state;
            saved_effect_state_slot = effect_state_view;
        }
        saved_impact_x_view = impact_x_view;
        saved_impact_y_view = impact_y_view;
        {
            register s32 *sprite_slots_init asm("r4") = (s32 *)BATTLE_ANIMATION_OFFSET(sprites[0]);
            asm volatile("" : "+r"(sprite_slots_init));
            sprite_slots_init = (s32 *)((char *)sprite_slots_init + (s32)group_bytes);
            sprite_slots = sprite_slots_init;
        }
        {
            register u32 *random_callback_slot asm("r5") = &gRandomNumberCallback;
            do {
                register s32 spark_angle asm("r4");
                register s32 random asm("r0");
                register s32 speed_fixed8 asm("r1");
                register s32 minimum_speed_fixed8 asm("r0");
                register char *impact_x_load_view asm("r2");
                register s32 impact_x_load_offset asm("r4");
                register s32 launch_or_impact_x asm("r3");
                register char *impact_y_load_view asm("r2");
                register s32 impact_y_load_offset asm("r4");
                register s32 impact_y asm("r0");
                register s32 next_sprite_index_bits asm("r2");
                register s32 sprite_slot_address_or_offset asm("r1");
                void *particle_sprite;

                random = CallFunctionR0(*random_callback_slot);
                spark_angle = sprite_index << 3;
                random = (u32)random >> 13;
                random += 0x2C;
                spark_angle += random;
                spark_angle_slot = spark_angle;
                speed_fixed8 = (u32)(CallFunctionR0(*random_callback_slot) * 0x201) >> 15;
                minimum_speed_fixed8 = 0x200;
                asm volatile("" : "+r"(minimum_speed_fixed8));
                speed_fixed8 += minimum_speed_fixed8;
                asm volatile(
                    "mov %0, %3\n\t"
                    "mov %1, #0\n\t"
                    "ldrsh %2, [%0, %1]"
                    : "=r"(impact_x_load_view), "=r"(impact_x_load_offset), "=r"(launch_or_impact_x)
                    : "r"(saved_impact_x_view));
                asm volatile(
                    "mov %0, %3\n\t"
                    "mov %1, #0\n\t"
                    "ldrsh %2, [%0, %1]"
                    : "=r"(impact_y_load_view), "=r"(impact_y_load_offset), "=r"(impact_y)
                    : "r"(saved_impact_y_view));
                particle_sprite = CreateBattleAngledProjectileSprite(group_bytes, BATTLE_FALLING_STREAK_RESOURCE_SPARK, 0, launch_or_impact_x, impact_y,
                    (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), spark_angle_slot, speed_fixed8, 1);
                next_sprite_index_bits = sprite_index + 1;
                sprite_slot_address_or_offset = next_sprite_index_bits << 2;
                asm volatile("add %0, %1" : "+r"(sprite_slot_address_or_offset)
                             : "r"(sprite_slots));
                *(void **)sprite_slot_address_or_offset = particle_sprite;
                next_sprite_index_bits <<= 24;
                sprite_index = (u32)next_sprite_index_bits >> 24;
            } while (sprite_index <= 7);
        }
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
        PlayBattleAnimationSound(1);
        {
            register s32 *effect_state_slot asm("r1") = saved_effect_state_slot;
            *effect_state_slot = *effect_state_slot + 1;
        }
        break;
    }
    case BATTLE_FALLING_STREAK_IMPACT_WAIT_FOR_SPRITES:
        sprite_index = 0;
        if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            register char *sprite_slots asm("r1") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
            do {
                sprite_index = (u8)(sprite_index + 1);
            } while (sprite_index <= 8 &&
                *(s32 *)(sprite_slots + (sprite_index << 2)) == 0);
        }
        if (sprite_index == 9) {
            DestroySpriteGroup(group_bytes);
        }
        break;
    }
}
