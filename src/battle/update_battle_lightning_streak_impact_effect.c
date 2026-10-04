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

void UpdateBattleLightningStreakImpactEffect(struct BattleAnimationGroup *group) asm("func_080DAD14");

void UpdateBattleLightningStreakImpactEffect(struct BattleAnimationGroup *group) {
    char *group_bytes = (char *)group;
    register s32 *effect_state asm("r8") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    u32 phase = *effect_state;
    register s32 sprite_index asm("r6");

    if (phase == BATTLE_LIGHTNING_IMPACT_CREATE) {
        register s16 *impact_x_stage asm("r5") = (s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.x));
        register s16 *impact_y_stage asm("r4");
        register s32 *saved_effect_state asm("r10");
        register s16 *impact_x_view asm("r9");
        register s16 *impact_y_view asm("r8");
        register s32 *sprite_slots asm("r5");
        register s32 x asm("r3");
        void *lightning_sprite;

        x = *(s32 *)impact_x_stage;
        x -= BATTLE_LIGHTNING_SPRITE_HALF_WIDTH_PIXELS;
        x <<= 16;
        x >>= 16;
        impact_y_stage = (s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.y));
        lightning_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0, x, (s32)*impact_y_stage,
            0, 0, 1);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = lightning_sprite;
        sprite_index = 0;
        saved_effect_state = effect_state;
        impact_x_view = impact_x_stage;
        impact_y_view = impact_y_stage;
        sprite_slots = (s32 *)((char *)impact_x_stage - (BATTLE_ANIMATION_OFFSET(effect.impact.x) - BATTLE_ANIMATION_OFFSET(sprites[0])));
        do {
            register s32 angle_random asm("r4");
            register s32 sprite_angle_work asm("r0");
            register s32 sprite_angle asm("r4");
            register s32 angle_jitter asm("r1");
            register s32 minimum_speed_fixed8 asm("r2");
            register s32 speed_fixed8 asm("r1");
            register s32 sprite_offset asm("r1");
            register s32 next_sprite_index_bits asm("r2");
            void *effect_sprite;

            {
                register u32 *random_callback_view asm("r3") = &gRandomNumberCallback;
                angle_random = CallFunctionR0(*random_callback_view);
            }
            sprite_angle_work = DivideSigned32(sprite_index << 5, 7);
            sprite_angle_work += 108;
            angle_jitter = (u32)(angle_random * 9) >> 15;
            sprite_angle_work += angle_jitter;
            sprite_angle_work <<= 24;
            sprite_angle = (u32)sprite_angle_work >> 24;
            {
                register u32 *random_callback_view asm("r1") = &gRandomNumberCallback;
                speed_fixed8 = (CallFunctionR0(*random_callback_view) * 0x201) >> 15;
            }
            minimum_speed_fixed8 = 0x200;
            speed_fixed8 += minimum_speed_fixed8;
            {
                register s16 *impact_x_load_view asm("r3") = impact_x_view;
                register s16 *impact_y_load_view asm("r2");
                register s32 impact_x_index asm("r0") = 0;
                register s32 impact_y_index asm("r3");
                register s32 impact_x_value asm("r3");
                register s32 spawn_x asm("r12");
                register s32 spawn_y asm("r0");
                impact_x_value = impact_x_load_view[impact_x_index];
                __asm__ volatile ("" : "+r" (minimum_speed_fixed8));
                spawn_x = impact_x_value;
                impact_y_load_view = impact_y_view;
                impact_y_index = 0;
                __asm__ volatile ("" : "+r" (impact_y_load_view));
                spawn_y = impact_y_load_view[impact_y_index];
                effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0, spawn_x, spawn_y,
                    (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), sprite_angle, speed_fixed8, 0);
            }
            next_sprite_index_bits = sprite_index + 1;
            sprite_offset = next_sprite_index_bits << 2;
            *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)effect_sprite;
            {
                register s32 sprite_rotation asm("r1") = sprite_angle;
                sprite_rotation += 0x80;
                BATTLE_SPRITE_FIELD(effect_sprite, u8, rotation) = sprite_rotation;
            }
            next_sprite_index_bits <<= 24;
            sprite_index = (u32)next_sprite_index_bits >> 24;
        } while ((u32)sprite_index <= 7);
        PlayBattleAnimationSound(0);
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
        {
            register s32 *phase_slot asm("r1") = saved_effect_state;
            *phase_slot = *phase_slot + 1;
        }
    } else if (phase == BATTLE_LIGHTNING_IMPACT_WAIT_FOR_REPEAT_STEP) {
        if (BATTLE_SPRITE_FIELD(*(char **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])), u16, animation_step) == BATTLE_LIGHTNING_IMPACT_REPEAT_ANIMATION_STEP) {
            register s32 *saved_effect_state asm("r10");
            register s16 *impact_x_view asm("r9");
            register s16 *impact_y_view asm("r8");
            register s32 *sprite_slots asm("r5");

            sprite_index = 0;
            saved_effect_state = effect_state;
            {
                register s16 *impact_x_init asm("r2") = (s16 *)BATTLE_ANIMATION_OFFSET(effect.impact.x);
                __asm__ volatile ("" : "+r" (impact_x_init));
                impact_x_init = (s16 *)((s32)impact_x_init + (s32)group_bytes);
                impact_x_view = impact_x_init;
            }
            {
                register s16 *impact_y_init asm("r3") = (s16 *)BATTLE_ANIMATION_OFFSET(effect.impact.y);
                __asm__ volatile ("" : "+r" (impact_y_init));
                impact_y_init = (s16 *)((s32)impact_y_init + (s32)group_bytes);
                impact_y_view = impact_y_init;
            }
            sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            do {
                register s32 angle_random asm("r4");
                register s32 sprite_angle_work asm("r0");
                register s32 sprite_angle asm("r4");
                register s32 angle_jitter asm("r1");
                register s32 minimum_speed_fixed8 asm("r3");
                register s32 speed_fixed8 asm("r1");
                register s32 sprite_offset asm("r1");
                register s32 next_sprite_index_bits asm("r0");
                void *effect_sprite;

                {
                    register u32 *random_callback_view asm("r1") = &gRandomNumberCallback;
                    angle_random = CallFunctionR0(*random_callback_view);
                }
                sprite_angle_work = DivideSigned32(sprite_index << 5, 7);
                sprite_angle_work += 108;
                angle_jitter = (u32)(angle_random * 9) >> 15;
                sprite_angle_work += angle_jitter;
                sprite_angle_work <<= 24;
                sprite_angle = (u32)sprite_angle_work >> 24;
                {
                    register u32 *random_callback_view asm("r2") = &gRandomNumberCallback;
                    speed_fixed8 = (CallFunctionR0(*random_callback_view) * 0x201) >> 15;
                }
                minimum_speed_fixed8 = 0x200;
                speed_fixed8 += minimum_speed_fixed8;
                {
                    register s16 *impact_x_load_view asm("r0") = impact_x_view;
                    register s16 *impact_y_load_view asm("r3");
                    register s32 coordinate_index asm("r2");
                    register s32 impact_x_value asm("r0");
                    register s32 spawn_x asm("r12");
                    register s32 spawn_y asm("r0");
                    coordinate_index = 0;
                    impact_x_value = impact_x_load_view[coordinate_index];
                    __asm__ volatile ("" : "+r" (minimum_speed_fixed8));
                    spawn_x = impact_x_value;
                    impact_y_load_view = impact_y_view;
                    coordinate_index = 0;
                    __asm__ volatile ("" : "+r" (impact_y_load_view));
                    spawn_y = impact_y_load_view[coordinate_index];
                    effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0, spawn_x, spawn_y,
                        (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), sprite_angle, speed_fixed8, 0);
                }
                sprite_offset = sprite_index + 9;
                sprite_offset <<= 2;
                *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)effect_sprite;
                {
                    register s32 sprite_rotation asm("r1") = sprite_angle;
                    sprite_rotation += 0x80;
                    BATTLE_SPRITE_FIELD(effect_sprite, u8, rotation) = sprite_rotation;
                }
                next_sprite_index_bits = sprite_index + 1;
                next_sprite_index_bits <<= 24;
                sprite_index = (u32)next_sprite_index_bits >> 24;
            } while ((u32)sprite_index <= 7);
            {
                register s32 *phase_slot asm("r3") = saved_effect_state;
                *phase_slot = *phase_slot + 1;
            }
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
