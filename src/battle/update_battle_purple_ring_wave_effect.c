#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern s32 Sin256(u8) asm("func_08092A90");
extern s32 Cos256(u8) asm("func_08092ADC");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern s32 BiosArcTan2(s32, s32) asm("func_080ECD24");
extern u16 BiosSqrt(s32) asm("func_080ECD3C");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattlePurpleRingWaveEffect(struct BattleAnimationGroup *group) asm("func_080D9F6C");

void UpdateBattlePurpleRingWaveEffect(struct BattleAnimationGroup *group) {
    register char *group_bytes asm("r8") = group;
    register s32 *effect_state asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register u32 state asm("r0") = *effect_state;
    u8 sprite_index;

    if (state < BATTLE_PURPLE_RING_COUNT) {
        register s32 *sprite_slots asm("r10");
        s32 * volatile saved_effect_state;
        s32 *sprite_slots_base;
        s32 ring_index;
        s32 sprite_offset;
        register s32 x asm("r3");
        void *ring_sprite;
        register char *group_position_view asm("r1") = group_bytes;

        x = *(s32 *)(group_position_view + BATTLE_ANIMATION_OFFSET(x));
        state *= BATTLE_PURPLE_RING_SPACING_PIXELS;
        x -= state;
        x = (s16)x;
        ring_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0, x,
            (s32)*(s16 *)(group_position_view + BATTLE_ANIMATION_OFFSET(y)), (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), 0, 0);
        ring_index = *effect_state;
        sprite_offset = ring_index << 2;
        sprite_slots_base = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        *(s32 *)((char *)sprite_slots_base + sprite_offset) = (s32)ring_sprite;
        saved_effect_state = effect_state;
        sprite_slots = sprite_slots_base;

        if (ring_index == 0) {
            register u32 *random_callback_slot asm("r9");

            sprite_index = 0;
            random_callback_slot = &gRandomNumberCallback;
            do {
                register s32 particle_angle asm("r6");
                register s32 horizontal_component_or_speed_fixed8 asm("r4");
                register s32 vertical_component_fixed8 asm("r5");
                register s32 speed_reduction_product asm("r0");
                void *particle_sprite;

                {
                    register u32 *random_callback_view_1 asm("r1") = random_callback_slot;
                    register u32 random asm("r0");
                    register s32 radial_angle asm("r2");
                    register s32 angle_jitter asm("r1");
                    random = CallFunctionR0(*random_callback_view_1);
                    radial_angle = sprite_index << 4;
                    angle_jitter = (random * 0x11) >> 15;
                    angle_jitter -= 8;
                    radial_angle += angle_jitter;
                    radial_angle <<= 24;
                    particle_angle = (u32)radial_angle >> 24;
                }
                horizontal_component_or_speed_fixed8 = Cos256(particle_angle);
                __asm__ volatile ("" : "+r" (horizontal_component_or_speed_fixed8));
                horizontal_component_or_speed_fixed8 <<= 16;
                horizontal_component_or_speed_fixed8 >>= 16;
                {
                    register s32 vertical_unit_fixed8 asm("r0");
                    vertical_unit_fixed8 = Sin256(particle_angle);
                    vertical_unit_fixed8 = (s16)vertical_unit_fixed8;
                    vertical_component_fixed8 = (vertical_unit_fixed8 * 2) + vertical_unit_fixed8;
                }
                {
                    register s32 vertical_argument asm("r1") = vertical_component_fixed8;
                    register s32 horizontal_argument asm("r0");
                    vertical_argument = (s16)vertical_argument;
                    __asm__ volatile ("" : "+r" (vertical_argument));
                    horizontal_argument = horizontal_component_or_speed_fixed8;
                    particle_angle = (u32)(BiosArcTan2(horizontal_argument, vertical_argument) << 16) >> 24;
                }
                horizontal_component_or_speed_fixed8 = (u16)BiosSqrt(
                    (horizontal_component_or_speed_fixed8 * horizontal_component_or_speed_fixed8) + (vertical_component_fixed8 * vertical_component_fixed8));
                {
                    register u32 *random_callback_view_2 asm("r2") = random_callback_slot;
                    speed_reduction_product = horizontal_component_or_speed_fixed8 *
                        ((CallFunctionR0(*random_callback_view_2) * 0x81) >> 15);
                }
                if (speed_reduction_product < 0) {
                    speed_reduction_product += 0xFF;
                }
                horizontal_component_or_speed_fixed8 -= speed_reduction_product >> 8;
                {
                    register s16 *group_coordinates asm("r0") = (s16 *)group_bytes;
                    register s32 spawn_x asm("r3");
                    s32 spawn_y;
                    spawn_x = group_coordinates[2];
                    spawn_y = group_coordinates[4];
                    particle_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0, spawn_x,
                        spawn_y, (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), particle_angle, horizontal_component_or_speed_fixed8, 0);
                }
                {
                    register s32 sprite_offset asm("r1") = sprite_index;
                    sprite_offset += BATTLE_PURPLE_RING_COUNT;
                    sprite_offset <<= 2;
                    *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)particle_sprite;
                }
                sprite_index = (u8)(sprite_index + 1);
            } while ((u32)sprite_index < BATTLE_PURPLE_RING_RADIAL_PARTICLE_COUNT);
            PlayBattleAnimationSound(0);
        }
        {
            s32 *phase_slot = saved_effect_state;
            *phase_slot = *phase_slot + 1;
        }
    } else {
        register char *group_scan_view asm("r2");

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
