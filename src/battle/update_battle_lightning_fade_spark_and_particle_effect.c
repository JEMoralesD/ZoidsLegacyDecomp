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
extern u16 gBattleBlendControl asm("D_0300004E");
extern u16 gBattleBlendAlpha asm("D_03000050");

void UpdateBattleLightningFadeSparkAndParticleEffect(struct BattleAnimationGroup *group) asm("func_080DCA6C");

void UpdateBattleLightningFadeSparkAndParticleEffect(struct BattleAnimationGroup *group) {
    register char *group_bytes asm("r6") = group;
    register s32 *effect_state_slot asm("r5") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register u32 phase asm("r0") = *effect_state_slot;
    register s32 sprite_index asm("r7");

    switch (phase) {
    case BATTLE_LIGHTNING_FADE_CREATE: {
        register s32 zero asm("r4");
        void *effect_sprite;

        effect_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0,
            (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)) - 0x80),
            (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)), 0x20,
            ({ zero = 0; zero; }), zero);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = effect_sprite;
        gBattleBlendControl = 0x2044;
        gBattleBlendAlpha = 0x1004;
        *(u16 *)0x05000000 = zero;
        PlayBattleAnimationSound(0);
        *effect_state_slot = *effect_state_slot + 1;
        break;
    }
    case BATTLE_LIGHTNING_FADE_BLEND_BACKDROP: {
        register s32 *progress_slot asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.lightning_fade.backdrop_blend_progress));
        register s32 previous_progress asm("r0") = *progress_slot;
        register s32 progress asm("r3") = previous_progress + 2;

        *progress_slot = progress;
        {
            register u16 *blend_alpha_view asm("r2") = &gBattleBlendAlpha;
            *blend_alpha_view = (previous_progress + 6) | 0x1000;
        }
        if (progress == BATTLE_LIGHTNING_FADE_HIDE_BLEND_PROGRESS) {
            s32 *sprite_flags = *(s32 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            *sprite_flags |= BATTLE_SPRITE_HIDDEN;
        }
        if (*progress_slot == BATTLE_LIGHTNING_FADE_END_BLEND_PROGRESS) {
            *effect_state_slot = *effect_state_slot + 1;
        }
        break;
    }
    case BATTLE_LIGHTNING_FADE_START_SPRITE_FADE: {
        register s32 *saved_effect_state asm("r10");
        register s32 *sprite_slots asm("r8");
        register u32 *random_callback_slot asm("r5");

        sprite_index = 0;
        saved_effect_state = effect_state_slot;
        sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        random_callback_slot = &gRandomNumberCallback;
        do {
            register s32 particle_angle asm("r4");
            register s32 random asm("r0");
            register s32 angle_jitter asm("r1");
            register s32 speed_random asm("r0");
            register s32 speed_fixed8 asm("r1");
            register s32 sprite_offset asm("r1");
            register s32 next_index_bits asm("r2");
            void *particle_sprite;

            random = CallFunctionR0(*random_callback_slot);
            particle_angle = sprite_index << 4;
            angle_jitter = (u32)(random * 9) >> 15;
            particle_angle += angle_jitter;
            particle_angle -= 4;
            speed_random = CallFunctionR0(*random_callback_slot);
            speed_fixed8 = speed_random << 8;
            speed_fixed8 += speed_random;
            speed_fixed8 = (u32)speed_fixed8 >> 15;
            speed_fixed8 += 0x200;
            __asm__ volatile ("" : "+r" (sprite_index));
            particle_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0,
                *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
                (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), particle_angle, speed_fixed8, 0);
            next_index_bits = sprite_index + 1;
            sprite_offset = next_index_bits << 2;
            *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)particle_sprite;
            next_index_bits <<= 24;
            sprite_index = (u32)next_index_bits >> 24;
        } while ((u32)sprite_index < BATTLE_LIGHTNING_FADE_SPARK_COUNT);

        /* The second emission loop replaces tracked sparks in slots 9 through 16. */
        sprite_index = 0;
        {
            register u32 *random_callback_init asm("r4") = &gRandomNumberCallback;
            register u32 *second_random_callback asm("r9") = random_callback_init;
            do {
                register s32 angle_random asm("r5");
                register s32 particle_angle asm("r4");
                register s32 scaled_particle_index asm("r0");
                register s32 angle_jitter asm("r0");
                register s32 speed_random asm("r0");
                register s32 speed_fixed8 asm("r1");
                register s32 minimum_speed_fixed8 asm("r0");
                register s32 sprite_offset asm("r1");
                register s32 next_index_bits asm("r0");
                void *particle_sprite;

                {
                    register u32 *random_callback_view asm("r1") = second_random_callback;
                    angle_random = CallFunctionR0(*random_callback_view);
                }
                scaled_particle_index = 107;
                scaled_particle_index *= sprite_index;
                particle_angle = DivideSigned32(scaled_particle_index, 7);
                particle_angle += 81;
                angle_jitter = (u32)(angle_random * 9) >> 15;
                particle_angle += angle_jitter;
                particle_angle = (u8)particle_angle;
                {
                    register u32 *random_callback_view asm("r2") = second_random_callback;
                    speed_random = CallFunctionR0(*random_callback_view);
                }
                speed_fixed8 = speed_random << 8;
                speed_fixed8 += speed_random;
                speed_fixed8 = (u32)speed_fixed8 >> 15;
                minimum_speed_fixed8 = 0x100;
                speed_fixed8 += minimum_speed_fixed8;
                {
                    register s32 spawn_x asm("r3");
                    __asm__ volatile ("" : "+r" (sprite_index));
                    spawn_x = *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x));
                    __asm__ volatile ("" : "+r" (minimum_speed_fixed8));
                    particle_sprite = CreateBattleAngledProjectileSprite(group_bytes, 2, 0,
                        spawn_x, (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
                        (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), particle_angle, speed_fixed8, 0);
                }
                sprite_offset = sprite_index + BATTLE_LIGHTNING_FADE_PARTICLE_FIRST_SLOT;
                sprite_offset <<= 2;
                *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)particle_sprite;
                particle_angle += 0x80;
                *(u8 *)((char *)particle_sprite + BATTLE_SPRITE_OFFSET(rotation)) = particle_angle;
                next_index_bits = sprite_index + 1;
                next_index_bits <<= 24;
                sprite_index = (u32)next_index_bits >> 24;
            } while ((u32)sprite_index < BATTLE_LIGHTNING_FADE_PARTICLE_COUNT);
        }
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
        PlayBattleAnimationSound(1);
        gBattleBlendControl = 0x740;
        gBattleBlendAlpha = 0x1010;
        {
            s32 *sprite_flags = *(s32 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            *sprite_flags = (*sprite_flags & ~BATTLE_SPRITE_HIDDEN) | BATTLE_SPRITE_SEMITRANSPARENT;
        }
        {
            register s32 *state_or_sprite_slot asm("r1") = saved_effect_state;
            *state_or_sprite_slot = *state_or_sprite_slot + 1;
        }
        break;
    }
    case BATTLE_LIGHTNING_FADE_WAIT_FOR_FADE: {
        s32 *progress_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.lightning_fade.fade_updates));
        s32 progress = *progress_slot + 1;
        *progress_slot = progress;
        gBattleBlendAlpha = (16 - ((u32)progress >> 1)) | 0x1000;
        if (progress == BATTLE_LIGHTNING_FADE_UPDATES) {
            DestroySpriteGroup(group_bytes);
        }
        break;
    }
    }
}
