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

void UpdateBattleTiltedTurquoiseRingParticleEffect(struct BattleAnimationGroup *group) asm("func_080DBEA0");

void UpdateBattleTiltedTurquoiseRingParticleEffect(struct BattleAnimationGroup *group) {
    char *group_bytes = group;
    register s32 *effect_state asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register s32 phase asm("r1") = *effect_state;
    register s32 sprite_index asm("r8");

    if (phase == BATTLE_CONTACT_EFFECT_CREATE) {
        register s32 *sprite_slots asm("r10");
        register u32 *random_callback_slot asm("r9");
        s32 *saved_effect_state;
        void *effect_sprite;
        register s32 zero asm("r0");

        effect_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0,
            *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
            (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), phase, phase);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = effect_sprite;
        zero = 0;
        sprite_index = zero;
        saved_effect_state = effect_state;
        sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        random_callback_slot = &gRandomNumberCallback;
        do {
            register s32 particle_angle asm("r6");
            register s32 vertical_component_fixed8 asm("r5");
            register s32 horizontal_component_or_speed_fixed8 asm("r4");
            register s32 speed_reduction_product asm("r0");
            register s32 base_angle asm("r2");
            register s32 angle_jitter asm("r1");
            void *particle_sprite;

            {
                register u32 *random_callback_init asm("r1") = random_callback_slot;
                register u32 random asm("r0");
                register s32 particle_index_low asm("r1");
                random = CallFunctionR0(*random_callback_init);
                particle_index_low = sprite_index;
                __asm__ volatile ("" : "+r" (particle_index_low));
                base_angle = particle_index_low << 4;
                angle_jitter = (random * 0x11) >> 15;
                angle_jitter -= 8;
                base_angle += angle_jitter;
                base_angle <<= 24;
                particle_angle = (u32)base_angle >> 24;
            }
            {
                register s32 sine_fixed8 asm("r0");
                sine_fixed8 = Sin256(particle_angle);
                sine_fixed8 = (s16)sine_fixed8;
                vertical_component_fixed8 = (sine_fixed8 * 2) + sine_fixed8;
            }
            horizontal_component_or_speed_fixed8 = Cos256(particle_angle);
            __asm__ volatile ("" : "+r" (horizontal_component_or_speed_fixed8));
            horizontal_component_or_speed_fixed8 <<= 16;
            horizontal_component_or_speed_fixed8 >>= 16;
            {
                register s32 half_vertical_component asm("r0");
                half_vertical_component = vertical_component_fixed8 + ((u32)vertical_component_fixed8 >> 31);
                half_vertical_component >>= 1;
                horizontal_component_or_speed_fixed8 -= half_vertical_component;
            }
            particle_angle = (u32)(BiosArcTan2((s16)horizontal_component_or_speed_fixed8,
                (s16)vertical_component_fixed8) << 16) >> 24;
            horizontal_component_or_speed_fixed8 = (u16)BiosSqrt(
                (horizontal_component_or_speed_fixed8 * horizontal_component_or_speed_fixed8) + (vertical_component_fixed8 * vertical_component_fixed8));
            {
                register u32 *random_callback_view asm("r2") = random_callback_slot;
                speed_reduction_product = horizontal_component_or_speed_fixed8 *
                    ((CallFunctionR0(*random_callback_view) * 0x81) >> 15);
            }
            if (speed_reduction_product < 0) {
                speed_reduction_product += 0xFF;
            }
            horizontal_component_or_speed_fixed8 -= speed_reduction_product >> 8;
            particle_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0,
                *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
                (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), particle_angle, horizontal_component_or_speed_fixed8, 0);
            {
                register s32 next_particle_index_bits asm("r2") = sprite_index;
                s32 sprite_offset;
                next_particle_index_bits += 1;
                sprite_offset = next_particle_index_bits << 2;
                *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)particle_sprite;
                next_particle_index_bits <<= 24;
                next_particle_index_bits = (u32)next_particle_index_bits >> 24;
                sprite_index = next_particle_index_bits;
            }
        } while ((u32)sprite_index <= 0xF);
        PlayBattleAnimationSound(0);
        *saved_effect_state = *saved_effect_state + 1;
        return;
    }
    {
        register s32 zero asm("r0") = 0;
        sprite_index = zero;
        if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            s32 *sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            s32 sprite_offset;
            do {
                register s32 next_particle_index_bits asm("r0");
                next_particle_index_bits = sprite_index + 1;
                next_particle_index_bits <<= 24;
                next_particle_index_bits = (u32)next_particle_index_bits >> 24;
                sprite_index = next_particle_index_bits;
                if ((u32)next_particle_index_bits > 0x10) {
                    break;
                }
                sprite_offset = next_particle_index_bits << 2;
            } while (*(s32 *)((char *)sprite_slots + sprite_offset) == 0);
        }
        {
            register s32 scan_result_index asm("r1") = sprite_index;
            if (scan_result_index == 0x11) {
                DestroySpriteGroup(group_bytes);
            }
        }
    }
}
