#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern s32 Sin256(u8) asm("func_08092A90");
extern s32 Cos256(u8) asm("func_08092ADC");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern s32 BiosArcTan2(s32, s32) asm("func_080ECD24");
extern u16 BiosSqrt(s32) asm("func_080ECD3C");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern s32 DivideSigned32(s32, s32) asm("func_080ECD98");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattlePurpleRingImpactEffect(struct BattleAnimationGroup *group) asm("func_080DA110");

void UpdateBattlePurpleRingImpactEffect(struct BattleAnimationGroup *group) {
    register char *group_bytes asm("r8") = group;
    register s32 *effect_state asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    u32 state = *effect_state;
    u8 sprite_index;

    if (state < BATTLE_PURPLE_RING_COUNT) {
        register s32 x asm("r3");
        s32 ring_index;
        s32 sprite_offset;
        s32 *sprite_slots_base;
        void *ring_sprite;

        x = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.x));
        x += 0xFF20;
        x += state * BATTLE_PURPLE_RING_SPACING_PIXELS;
        x = (s16)x;
        ring_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0, x,
            (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.y)), BATTLE_SPRITE_SEMITRANSPARENT, 0, 1);
        ring_index = *effect_state;
        sprite_offset = ring_index << 2;
        sprite_slots_base = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        *(s32 *)((char *)sprite_slots_base + sprite_offset) = (s32)ring_sprite;
        *effect_state = ring_index + 1;
    } else if (state == BATTLE_PURPLE_RING_IMPACT_CREATE_EXPLOSION) {
        register s32 *sprite_slots asm("r10");
        register u32 *random_callback_slot asm("r9");
        register s32 zero_flags asm("r6");
        s32 * volatile saved_effect_state;
        s16 * volatile saved_impact_x_view;
        s16 * volatile saved_impact_y_view;

        sprite_index = 0;
        saved_effect_state = effect_state;
        {
            register s16 *impact_x_view asm("r2") = (s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.x));
            saved_impact_x_view = impact_x_view;
        }
        {
            register s16 *impact_y_view asm("r4") = (s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.y));
            saved_impact_y_view = impact_y_view;
        }
        {
            register s32 *sprite_slots_init asm("r0") = (s32 *)BATTLE_ANIMATION_OFFSET(sprites[0]);
            __asm__ volatile ("" : "+r" (sprite_slots_init));
            sprite_slots_init = (s32 *)((s32)sprite_slots_init + (s32)group_bytes);
            sprite_slots = sprite_slots_init;
        }
        {
            register u32 *random_callback_init asm("r1") = &gRandomNumberCallback;
            __asm__ volatile ("" : "+r" (random_callback_init));
            random_callback_slot = random_callback_init;
        }
        zero_flags = 0;
        do {
            register s32 angle_random asm("r4");
            register s32 sprite_angle asm("r5");
            register s32 angle_jitter asm("r0");
            register s32 minimum_speed_fixed8 asm("r0");
            s32 speed_fixed8;
            void *impact_sprite;
            s32 sprite_offset;

            {
                register u32 *random_callback_view asm("r2") = random_callback_slot;
                angle_random = CallFunctionR0(*random_callback_view);
            }
            sprite_angle = DivideSigned32(sprite_index << 4, 7);
            angle_jitter = (u32)(angle_random * 5) >> 15;
            angle_jitter -= 10;
            sprite_angle += angle_jitter;
            {
                register u32 *random_callback_view asm("r4") = random_callback_slot;
                speed_fixed8 = (CallFunctionR0(*random_callback_view) * 0x401) >> 15;
            }
            minimum_speed_fixed8 = 0x200;
            __asm__ volatile ("" : "+r" (minimum_speed_fixed8));
            speed_fixed8 += minimum_speed_fixed8;
            {
                register s16 *impact_coordinate_view asm("r2");
                register s32 coordinate_index asm("r4");
                register s32 spawn_x asm("r3");
                register s32 spawn_y asm("r0");
                impact_coordinate_view = saved_impact_x_view;
                coordinate_index = 0;
                spawn_x = impact_coordinate_view[coordinate_index];
                impact_coordinate_view = saved_impact_y_view;
                coordinate_index = 0;
                spawn_y = impact_coordinate_view[coordinate_index];
                impact_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0,
                    spawn_x, spawn_y,
                    zero_flags, sprite_angle, speed_fixed8, zero_flags);
            }
            sprite_offset = sprite_index + BATTLE_PURPLE_RING_EXPLOSION_FIRST_SLOT;
            sprite_offset <<= 2;
            *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)impact_sprite;
            sprite_index = (u8)(sprite_index + 1);
        } while ((u32)sprite_index < BATTLE_PURPLE_RING_COUNT);
        sprite_index = 0;
        {
            register u32 *random_callback_view_2 asm("r6") = &gRandomNumberCallback;
            do {
                register s32 angle_random asm("r4");
                register s32 sprite_angle asm("r5");
                register s32 angle_jitter asm("r0");
                s32 speed_fixed8;
                void *impact_sprite;
                s32 sprite_offset;

                angle_random = CallFunctionR0(*random_callback_view_2);
                sprite_angle = DivideSigned32(sprite_index << 5, 7);
                angle_jitter = (u32)(angle_random * 9) >> 15;
                angle_jitter -= 20;
                sprite_angle += angle_jitter;
                speed_fixed8 = (CallFunctionR0(*random_callback_view_2) * 0x201) >> 15;
                speed_fixed8 += 0x200;
                {
                    register s16 *impact_coordinate_view asm("r2");
                    register s32 coordinate_index asm("r4");
                    register s32 spawn_x asm("r3");
                    register s32 spawn_y asm("r0");
                    impact_coordinate_view = saved_impact_x_view;
                    coordinate_index = 0;
                    spawn_x = impact_coordinate_view[coordinate_index];
                    impact_coordinate_view = saved_impact_y_view;
                    coordinate_index = 0;
                    spawn_y = impact_coordinate_view[coordinate_index];
                    impact_sprite = CreateBattleAngledProjectileSprite(group_bytes, 2, 0,
                        spawn_x, spawn_y,
                        BATTLE_SPRITE_LOOP_ANIMATION, sprite_angle, speed_fixed8, 0);
                }
                sprite_offset = sprite_index + BATTLE_PURPLE_RING_FRAGMENT_FIRST_SLOT;
                sprite_offset <<= 2;
                *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)impact_sprite;
                sprite_index = (u8)(sprite_index + 1);
            } while ((u32)sprite_index < BATTLE_PURPLE_RING_COUNT);
        }
        sprite_index = 0;
        random_callback_slot = &gRandomNumberCallback;
        do {
            register s32 particle_angle asm("r6");
            register s32 horizontal_component_fixed8 asm("r4");
            register s32 vertical_component_fixed8 asm("r5");
            register s32 speed_fixed8 asm("r4");
            s32 speed_reduction_product;

            {
                register u32 *random_callback_view asm("r1") = random_callback_slot;
                register s32 random asm("r0");
                register s32 radial_angle asm("r2");
                register s32 angle_jitter asm("r1");
                random = CallFunctionR0(*random_callback_view);
                radial_angle = sprite_index << 4;
                angle_jitter = (u32)(random * 0x11) >> 15;
                angle_jitter -= 8;
                radial_angle += angle_jitter;
                radial_angle <<= 24;
                particle_angle = (u32)radial_angle >> 24;
            }
            horizontal_component_fixed8 = Cos256(particle_angle);
            __asm__ volatile ("" : "+r" (horizontal_component_fixed8));
            horizontal_component_fixed8 <<= 16;
            horizontal_component_fixed8 >>= 16;
            {
                register s32 vertical_unit_fixed8 asm("r0");
                vertical_unit_fixed8 = Sin256(particle_angle);
                vertical_unit_fixed8 = (s16)vertical_unit_fixed8;
                vertical_component_fixed8 = (vertical_unit_fixed8 * 2) + vertical_unit_fixed8;
            }
            particle_angle = (u32)(BiosArcTan2(horizontal_component_fixed8,
                (s16)vertical_component_fixed8) << 16) >> 24;
            speed_fixed8 = (u16)BiosSqrt(
                (horizontal_component_fixed8 * horizontal_component_fixed8) + (vertical_component_fixed8 * vertical_component_fixed8));
            {
                register u32 *random_callback_view asm("r2") = random_callback_slot;
                speed_reduction_product = speed_fixed8 *
                    ((CallFunctionR0(*random_callback_view) * 0x81) >> 15);
            }
            if (speed_reduction_product < 0) {
                speed_reduction_product += 0xFF;
            }
            speed_fixed8 -= speed_reduction_product >> 8;
            {
                register s16 *x_view asm("r0") = saved_impact_x_view;
                register s16 *y_view asm("r2");
                register s32 coordinate_index asm("r1");
                register s32 spawn_x asm("r3");
                register s32 spawn_y asm("r0");
                coordinate_index = 0;
                spawn_x = x_view[coordinate_index];
                y_view = saved_impact_y_view;
                coordinate_index = 0;
                spawn_y = y_view[coordinate_index];
                /* These sixteen particles have no group slot and can outlive group cleanup. */
                CreateBattleAngledProjectileSprite(group_bytes, 3, 0,
                    spawn_x, spawn_y,
                    (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), particle_angle, speed_fixed8, 0);
            }
            sprite_index = (u8)(sprite_index + 1);
        } while ((u32)sprite_index < BATTLE_PURPLE_RING_RADIAL_PARTICLE_COUNT);
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
        PlayBattleAnimationSound(1);
        {
            register s32 *phase_slot asm("r2") = saved_effect_state;
            *phase_slot = *phase_slot + 1;
        }
    } else {
        register char *group_scan_view asm("r4");

        sprite_index = 0;
        group_scan_view = group_bytes;
        if (*(s32 *)(group_scan_view + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            register s32 *sprite_slots asm("r1") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            s32 sprite_offset;
            do {
                sprite_index = (u8)(sprite_index + 1);
            } while ((u32)sprite_index < BATTLE_PURPLE_RING_TRACKED_SPRITE_COUNT &&
                (sprite_offset = sprite_index << 2,
                 *(s32 *)((char *)sprite_slots + sprite_offset)) == 0);
        }
        if (sprite_index == BATTLE_PURPLE_RING_TRACKED_SPRITE_COUNT) {
            DestroySpriteGroup(group_bytes);
        }
    }
}
