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

void UpdateBattleFacingStrikeParticleAndDelayedFragmentEffect(struct BattleAnimationGroup *group) asm("func_080DA5B0");

void UpdateBattleFacingStrikeParticleAndDelayedFragmentEffect(struct BattleAnimationGroup *group) {
    char *group_bytes = (char *)group;
    register s32 *effect_state asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register u32 phase asm("r0") = *effect_state;
    register s32 sprite_index asm("r6");

    switch (phase) {
    case BATTLE_DELAYED_STRIKE_FRAGMENT_CREATE: {
        register s32 *saved_effect_state asm("r10");
        register s32 *sprite_slots asm("r8");
        register u32 *random_callback_slot asm("r5");
        void *strike_sprite;

        strike_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0,
            *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
            BATTLE_SPRITE_SEMITRANSPARENT, 0, 1);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = strike_sprite;
        sprite_index = 0;
        saved_effect_state = effect_state;
        sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        random_callback_slot = &gRandomNumberCallback;
        do {
            register s32 sprite_angle asm("r4");
            register s32 random asm("r0");
            register s32 angle_jitter asm("r1");
            register s32 minimum_speed_fixed8 asm("r0");
            register s32 sprite_offset asm("r1");
            register s32 next_sprite_index_bits asm("r2");
            s32 speed_fixed8;
            void *effect_sprite;

            random = CallFunctionR0(*random_callback_slot);
            sprite_angle = sprite_index << 4;
            angle_jitter = (u32)(random * 0x11) >> 15;
            angle_jitter += 56;
            sprite_angle += angle_jitter;
            speed_fixed8 = (CallFunctionR0(*random_callback_slot) * 0x101) >> 15;
            minimum_speed_fixed8 = 0x100;
            speed_fixed8 += minimum_speed_fixed8;
            effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0,
                *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
                (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), sprite_angle, speed_fixed8, 0);
            next_sprite_index_bits = sprite_index + 1;
            sprite_offset = next_sprite_index_bits << 2;
            *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)effect_sprite;
            next_sprite_index_bits <<= 24;
            sprite_index = (u32)next_sprite_index_bits >> 24;
        } while ((u32)sprite_index <= 7);
        sprite_index = 0;
        random_callback_slot = &gRandomNumberCallback;
        do {
            register s32 sprite_angle asm("r4");
            register s32 random asm("r0");
            register s32 angle_jitter asm("r1");
            register s32 sprite_offset asm("r1");
            register s32 next_sprite_index_bits asm("r0");
            s32 speed_fixed8;
            void *effect_sprite;

            random = CallFunctionR0(*random_callback_slot);
            sprite_angle = sprite_index << 3;
            angle_jitter = (u32)(random * 9) >> 15;
            angle_jitter -= 68;
            sprite_angle += angle_jitter;
            speed_fixed8 = (CallFunctionR0(*random_callback_slot) * 0x201) >> 15;
            speed_fixed8 += 0x200;
            effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0,
                *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
                (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), sprite_angle, speed_fixed8, 0);
            sprite_offset = sprite_index + 9;
            sprite_offset <<= 2;
            *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)effect_sprite;
            next_sprite_index_bits = sprite_index + 1;
            next_sprite_index_bits <<= 24;
            sprite_index = (u32)next_sprite_index_bits >> 24;
        } while ((u32)sprite_index <= 0xF);
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE_AND_RECOIL_SCROLL, 0);
        PlayBattleAnimationSound(0);
        {
            register s32 *phase_slot asm("r1") = saved_effect_state;
            *phase_slot = *phase_slot + 1;
        }
        break;
    }
    case BATTLE_DELAYED_STRIKE_FRAGMENT_WAIT_FOR_ANIMATION_STEP: {
        if (BATTLE_SPRITE_FIELD(*(char **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])), u16, animation_step) == BATTLE_FACING_STRIKE_FRAGMENT_ANIMATION_STEP) {
            register s32 *saved_effect_state asm("r10");
            register s32 *sprite_slots asm("r8");
            register u32 *random_callback_slot asm("r9");

            sprite_index = 0;
            saved_effect_state = effect_state;
            {
                register s32 *sprite_slots_init asm("r2") = (s32 *)BATTLE_ANIMATION_OFFSET(sprites[0]);
                __asm__ volatile ("" : "+r" (sprite_slots_init));
                sprite_slots_init = (s32 *)((s32)sprite_slots_init + (s32)group_bytes);
                sprite_slots = sprite_slots_init;
            }
            {
                register u32 *random_callback_init asm("r0") = &gRandomNumberCallback;
                random_callback_slot = random_callback_init;
            }
            do {
                register s32 angle_random asm("r4");
                register s32 sprite_angle asm("r5");
                register s32 angle_jitter asm("r0");
                register s32 minimum_speed_fixed8 asm("r0");
                register s32 sprite_offset asm("r1");
                register s32 next_sprite_index_bits asm("r0");
                s32 speed_fixed8;
                void *effect_sprite;

                {
                    register u32 *random_callback_view asm("r1") = random_callback_slot;
                    angle_random = CallFunctionR0(*random_callback_view);
                }
                sprite_angle = DivideSigned32(sprite_index << 6, 6);
                angle_jitter = (u32)(angle_random * 9) >> 15;
                angle_jitter += 108;
                sprite_angle += angle_jitter;
                {
                    register u32 *random_callback_view asm("r2") = random_callback_slot;
                    speed_fixed8 = (CallFunctionR0(*random_callback_view) * 0x201) >> 15;
                }
                minimum_speed_fixed8 = 0x200;
                speed_fixed8 += minimum_speed_fixed8;
                {
                    register s32 spawn_x asm("r3");
                    spawn_x = *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x));
                    __asm__ volatile ("" : "+r" (minimum_speed_fixed8));
                    effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 2, 0,
                        spawn_x, (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
                        BATTLE_SPRITE_LOOP_ANIMATION, sprite_angle, speed_fixed8, 0);
                }
                sprite_offset = sprite_index + 25;
                sprite_offset <<= 2;
                *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)effect_sprite;
                next_sprite_index_bits = sprite_index + 1;
                next_sprite_index_bits <<= 24;
                sprite_index = (u32)next_sprite_index_bits >> 24;
            } while ((u32)sprite_index <= 6);
            {
                register s32 *phase_slot asm("r1") = saved_effect_state;
                *phase_slot = *phase_slot + 1;
            }
        }
        break;
    }
    case BATTLE_DELAYED_STRIKE_FRAGMENT_WAIT_FOR_SPRITES: {
        sprite_index = 0;
        if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            s32 *sprite_slots_scan = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            s32 sprite_offset;
            do {
                register s32 next_sprite_index_bits asm("r0") = sprite_index + 1;
                next_sprite_index_bits <<= 24;
                sprite_index = (u32)next_sprite_index_bits >> 24;
            } while ((u32)sprite_index <= 0x1F &&
                (sprite_offset = sprite_index << 2,
                 *(s32 *)((char *)sprite_slots_scan + sprite_offset)) == 0);
        }
        if (sprite_index == 0x20) {
            DestroySpriteGroup(group_bytes);
        }
        break;
    }
    }
}
