#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern void UpdateBattleTargetedRisingMissileTrailSprite(void *) asm("func_080D6D84");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern s32 DivideSigned32(s32, s32) asm("func_080ECD98");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleRisingMissileImpactFragmentFanEffect(struct BattleAnimationGroup *group) asm("func_080DF680");

void UpdateBattleRisingMissileImpactFragmentFanEffect(struct BattleAnimationGroup *group) {
    char *group_bytes = group;
    register s32 *effect_state_slot asm("r8") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register s32 *phase_advance_slot asm("r2");
    u32 phase = *effect_state_slot;

    switch (phase) {
    case BATTLE_MISSILE_IMPACT_LAUNCH:
        {
            register s32 *impact_x_view asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.rising_missile_fragment_fan.impact_x));
            register s32 zero asm("r1");
            void *effect_sprite;
            register s32 x asm("r3");

            x = *impact_x_view;
            x -= 0x100;
            x = (s16)x;
            effect_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0, x,
                (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.rising_missile_fragment_fan.impact_y)) + 0x80),
                0x20, (s32)UpdateBattleTargetedRisingMissileTrailSprite, 1);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = effect_sprite;
            *(void **)((char *)effect_sprite + 0x28) = group_bytes;
            zero = 0;
            *(s32 *)((char *)effect_sprite + 0x2C) = zero;
            *(s32 *)((char *)effect_sprite + 0x30) = *impact_x_view;
            PlayBattleAnimationSound(0);
            phase_advance_slot = effect_state_slot;
            goto increment_state;
        }
        return;
    case BATTLE_MISSILE_IMPACT_CREATE:
        if ((*(s32 *)*(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) & BATTLE_SPRITE_HIDDEN) != 0) {
            register s16 *impact_x_view asm("r5") = (s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.rising_missile_fragment_fan.impact_x));
            register s16 *impact_y_view asm("r4");
            register s32 fragment_or_trail_slot_index asm("r6");
            register s32 *sprite_slots asm("r8");
            register s32 *sprite_slots_base asm("r4");
            register u32 *random_callback_slot asm("r5");
            register s32 spawn_x asm("r3");
            register s32 coordinate_zero_offset asm("r4");
            s32 * volatile saved_effect_state_slot;
            volatile s32 fragment_angle;
            register s16 *saved_impact_x_view asm("r10");
            register s16 *saved_impact_y_view asm("r9");
            void *effect_sprite;

            coordinate_zero_offset = 0;
            spawn_x = impact_x_view[coordinate_zero_offset];
            impact_y_view = (s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.rising_missile_fragment_fan.impact_y));
            effect_sprite = CreateBattleAnimationSprite(group_bytes, 2, 0,
                spawn_x, (s32)*impact_y_view, 0, 0, 0);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) = effect_sprite;
            fragment_or_trail_slot_index = 0;
            phase_advance_slot = effect_state_slot;
            saved_effect_state_slot = phase_advance_slot;
            saved_impact_x_view = impact_x_view;
            saved_impact_y_view = impact_y_view;
            sprite_slots_base = (s32 *)BATTLE_ANIMATION_OFFSET(sprites[0]);
            sprite_slots_base = (s32 *)((s32)sprite_slots_base + (s32)group_bytes);
            sprite_slots = sprite_slots_base;
            random_callback_slot = &gRandomNumberCallback;
            do {
                register s32 angle_random asm("r0");
                register s32 fragment_fan_offset asm("r1");
                register s32 angle_or_coordinate_index asm("r4");
                register u32 fragment_speed_fixed8 asm("r0");
                void *particle_sprite;
                s32 sprite_slot_offset;

                angle_random = CallFunctionR0(*random_callback_slot);
                fragment_fan_offset = fragment_or_trail_slot_index - 2;
                fragment_fan_offset <<= 3;
                angle_or_coordinate_index = (u32)(angle_random * 9) >> 15;
                angle_or_coordinate_index += fragment_fan_offset;
                angle_or_coordinate_index += 233;
                fragment_angle = angle_or_coordinate_index;
                fragment_speed_fixed8 = CallFunctionR0(*random_callback_slot);
                fragment_speed_fixed8 >>= 7;
                fragment_speed_fixed8 += 0x100;
                __asm__ volatile ("" : : "r" (angle_or_coordinate_index));
                {
                    register s32 spawn_x2 asm("r3");
                    register s32 spawn_y asm("r1");
                    register s16 *impact_coordinate_view asm("r2");
                    impact_coordinate_view = saved_impact_x_view;
                    __asm__ volatile ("" : "+r" (impact_coordinate_view));
                    angle_or_coordinate_index = 0;
                    spawn_x2 = impact_coordinate_view[angle_or_coordinate_index];
                    impact_coordinate_view = saved_impact_y_view;
                    __asm__ volatile ("" : "+r" (impact_coordinate_view));
                    angle_or_coordinate_index = 0;
                    spawn_y = impact_coordinate_view[angle_or_coordinate_index];
                    particle_sprite = CreateBattleAngledProjectileSprite(group_bytes, 3, 0,
                        spawn_x2, spawn_y, 0x20,
                        fragment_angle, fragment_speed_fixed8, 0);
                }
                sprite_slot_offset = fragment_or_trail_slot_index + 2;
                sprite_slot_offset <<= 2;
                *(s32 *)((char *)sprite_slots + sprite_slot_offset) = (s32)particle_sprite;
                {
                    register s32 next_index_bits asm("r0") = fragment_or_trail_slot_index + 1;
                    next_index_bits <<= 24;
                    next_index_bits = (u32)next_index_bits >> 24;
                    fragment_or_trail_slot_index = next_index_bits;
                }
            } while ((u32)fragment_or_trail_slot_index < BATTLE_RISING_MISSILE_IMPACT_FRAGMENT_FAN_COUNT);
            SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
            PlayBattleAnimationSound(1);
            phase_advance_slot = saved_effect_state_slot;
            goto increment_state;
        }
        return;

increment_state:
        *phase_advance_slot = *phase_advance_slot + 1;
        return;

    case BATTLE_MISSILE_IMPACT_WAIT_FOR_SPRITES:
        {
            register s32 fragment_or_trail_slot_index asm("r6") = 0x18;
            register void *missile_sprite asm("r1") = *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            u32 trail_elapsed_updates;
            u32 trail_scan_bound;
            s32 sprite_slot_offset;

            trail_elapsed_updates = *(u32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames));
            trail_scan_bound = (trail_elapsed_updates >> 1) + 0x19;
            if ((u32)fragment_or_trail_slot_index < trail_scan_bound && *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[24])) == 0) {
                u32 saved_trail_scan_bound = trail_scan_bound;
                s32 *sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
                do {
                    register s32 next_index_bits asm("r0") = fragment_or_trail_slot_index + 1;
                    next_index_bits <<= 24;
                    next_index_bits = (u32)next_index_bits >> 24;
                    fragment_or_trail_slot_index = next_index_bits;
                } while ((u32)fragment_or_trail_slot_index < saved_trail_scan_bound &&
                    (sprite_slot_offset = fragment_or_trail_slot_index << 2,
                     *(s32 *)((char *)sprite_slots + sprite_slot_offset)) == 0);
            }
            if (fragment_or_trail_slot_index == ((*(volatile u32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames)) >> 1) + 0x19)) {
                fragment_or_trail_slot_index = 1;
                if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0) {
                    s32 *sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
                    do {
                        register s32 next_index_bits asm("r0") = fragment_or_trail_slot_index + 1;
                        next_index_bits <<= 24;
                        next_index_bits = (u32)next_index_bits >> 24;
                        fragment_or_trail_slot_index = next_index_bits;
                    } while ((u32)fragment_or_trail_slot_index <= 6 &&
                        (sprite_slot_offset = fragment_or_trail_slot_index << 2,
                         *(s32 *)((char *)sprite_slots + sprite_slot_offset)) == 0);
                }
                if (fragment_or_trail_slot_index == 7) {
                    DestroySpriteGroup(group_bytes);
                }
            }
        }
        return;
    }
    return;
}
