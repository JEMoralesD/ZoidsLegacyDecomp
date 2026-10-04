#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern s32 DivideSigned32(s32, s32) asm("func_080ECD98");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleFacingHorizontalSlashFragmentAndParticleEffect(struct BattleAnimationGroup *group) asm("func_080DA8B4");

void UpdateBattleFacingHorizontalSlashFragmentAndParticleEffect(struct BattleAnimationGroup *group) {
    register char *group_bytes asm("r6") = (char *)group;
    register s32 *effect_state asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    u32 phase = *effect_state;
    register s32 sprite_index asm("r5");

    if (phase == BATTLE_CONTACT_EFFECT_CREATE) {
        register s32 *saved_effect_state asm("r10");
        register s32 *sprite_slots asm("r8");
        u32 *random_callback_slot;
        register s32 last_emission_index asm("r9");
        void *strike_sprite;

        strike_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0,
            *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
            BATTLE_SPRITE_SEMITRANSPARENT, 0, 1);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = strike_sprite;
        sprite_index = 0;
        saved_effect_state = effect_state;
        sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        random_callback_slot = &gRandomNumberCallback;
        last_emission_index = 7;
        do {
            register s32 angle_random asm("r4");
            register s32 sprite_angle_work asm("r0");
            register s32 sprite_angle asm("r4");
            register s32 angle_jitter asm("r1");
            register s32 speed_random asm("r0");
            register s32 speed_random_product asm("r1");
            register s32 speed_jitter_fixed8 asm("r2");
            register s32 speed_fixed8 asm("r1");
            register s32 sprite_offset asm("r1");
            register s32 next_sprite_index_bits asm("r2");
            void *effect_sprite;

            angle_random = CallFunctionR0(*random_callback_slot);
            sprite_angle_work = DivideSigned32(sprite_index << 5, 7);
            angle_jitter = (u32)(angle_random * 5) >> 15;
            angle_jitter += 110;
            sprite_angle = sprite_angle_work + angle_jitter;
            speed_random = CallFunctionR0(*random_callback_slot);
            speed_random_product = speed_random << 8;
            speed_random_product += speed_random;
            speed_jitter_fixed8 = (u32)speed_random_product >> 15;
            if ((u32)sprite_index <= 3) {
                register s32 speed_step_fixed8 asm("r1") = sprite_index << 8;
                register s32 minimum_speed_fixed8 asm("r3") = 0x300;
                register s32 jittered_minimum_speed_fixed8 asm("r0") = speed_jitter_fixed8 + minimum_speed_fixed8;
                speed_fixed8 = speed_step_fixed8 + jittered_minimum_speed_fixed8;
            } else {
                register s32 mirrored_emission_index asm("r0") = last_emission_index - sprite_index;
                register s32 minimum_speed_fixed8 asm("r3") = 0x300;
                mirrored_emission_index <<= 8;
                speed_fixed8 = speed_jitter_fixed8 + minimum_speed_fixed8;
                speed_fixed8 = mirrored_emission_index + speed_fixed8;
            }
            effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0,
                *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
                BATTLE_SPRITE_LOOP_ANIMATION, sprite_angle, speed_fixed8, 0);
            next_sprite_index_bits = sprite_index + 1;
            sprite_offset = next_sprite_index_bits << 2;
            *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)effect_sprite;
            next_sprite_index_bits <<= 24;
            sprite_index = (u32)next_sprite_index_bits >> 24;
        } while ((u32)sprite_index <= 7);

        sprite_index = 0;
        random_callback_slot = &gRandomNumberCallback;
        last_emission_index = 7;
        do {
            register s32 angle_random asm("r4");
            register s32 sprite_angle_work asm("r0");
            register s32 sprite_angle asm("r4");
            register s32 angle_jitter asm("r1");
            register s32 speed_random asm("r0");
            register s32 speed_random_product asm("r1");
            register s32 speed_jitter_fixed8 asm("r2");
            register s32 speed_fixed8 asm("r1");
            register s32 sprite_offset asm("r1");
            register s32 next_sprite_index_bits asm("r0");
            void *effect_sprite;

            angle_random = CallFunctionR0(*random_callback_slot);
            sprite_angle_work = DivideSigned32(sprite_index << 5, 7);
            angle_jitter = (u32)(angle_random * 5) >> 15;
            angle_jitter += 110;
            sprite_angle = sprite_angle_work + angle_jitter;
            speed_random = CallFunctionR0(*random_callback_slot);
            speed_random_product = speed_random << 8;
            speed_random_product += speed_random;
            speed_jitter_fixed8 = (u32)speed_random_product >> 15;
            if ((u32)sprite_index <= 3) {
                register s32 speed_step_fixed8 asm("r1") = sprite_index << 8;
                register s32 minimum_speed_fixed8 asm("r3") = 0x300;
                register s32 jittered_minimum_speed_fixed8 asm("r0") = speed_jitter_fixed8 + minimum_speed_fixed8;
                speed_fixed8 = speed_step_fixed8 + jittered_minimum_speed_fixed8;
            } else {
                register s32 mirrored_emission_index asm("r0") = last_emission_index - sprite_index;
                register s32 minimum_speed_fixed8 asm("r3") = 0x300;
                mirrored_emission_index <<= 8;
                speed_fixed8 = speed_jitter_fixed8 + minimum_speed_fixed8;
                speed_fixed8 = mirrored_emission_index + speed_fixed8;
            }
            effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 2, 0,
                *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
                BATTLE_SPRITE_SEMITRANSPARENT, sprite_angle, speed_fixed8, 0);
            sprite_offset = sprite_index + 9;
            sprite_offset <<= 2;
            *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)effect_sprite;
            next_sprite_index_bits = sprite_index + 1;
            next_sprite_index_bits <<= 24;
            sprite_index = (u32)next_sprite_index_bits >> 24;
        } while ((u32)sprite_index <= 7);
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE_AND_RECOIL_SCROLL, 0);
        PlayBattleAnimationSound(0);
        {
            register s32 *phase_slot asm("r3") = saved_effect_state;
            *phase_slot = *phase_slot + 1;
        }
    } else {
        sprite_index = 0;
        if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            s32 *sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            s32 sprite_offset;
            do {
                register s32 next_sprite_index_bits asm("r0") = sprite_index + 1;
                next_sprite_index_bits <<= 24;
                sprite_index = (u32)next_sprite_index_bits >> 24;
            } while ((u32)sprite_index <= 0x10 &&
                (sprite_offset = sprite_index << 2,
                 *(s32 *)((char *)sprite_slots + sprite_offset)) == 0);
        }
        if (sprite_index == 0x11) {
            DestroySpriteGroup(group_bytes);
        }
    }
}
