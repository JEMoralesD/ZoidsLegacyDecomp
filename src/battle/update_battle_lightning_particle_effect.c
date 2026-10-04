#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern s32 DivideSigned32(s32, s32) asm("func_080ECD98");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleLightningParticleEffect(struct BattleAnimationGroup *group) asm("func_080DAA5C");

void UpdateBattleLightningParticleEffect(struct BattleAnimationGroup *group)
{
    register char *group_bytes asm("r6") = (char *)group;
    register s32 *effect_state asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register s32 phase asm("r1") = *effect_state;
    register s32 sprite_index;

    if (phase == BATTLE_CONTACT_EFFECT_CREATE) {
        register s32 *saved_effect_state asm("r10");
        register s32 *sprite_slots asm("r9");
        register u32 *random_callback_slot asm("r8");
        void *lightning_sprite;

        lightning_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0,
            (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)) - BATTLE_LIGHTNING_SPRITE_HALF_WIDTH_PIXELS),
            (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)), phase, phase, 1);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = lightning_sprite;
        sprite_index = 0;
        saved_effect_state = effect_state;
        sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        {
            register u32 *random_callback_init asm("r1") = &gRandomNumberCallback;

            asm volatile("" : "+r"(random_callback_init));
            random_callback_slot = random_callback_init;
        }
        do {
            register s32 angle_random asm("r4");
            register s32 sprite_angle asm("r5");
            register s32 angle_jitter asm("r0");
            register s32 speed_random_or_x_offset asm("r0");
            register s32 speed_fixed8 asm("r1");
            register s32 minimum_speed_fixed8 asm("r2");
            register s32 next_sprite_index_bits asm("r2");
            register s32 sprite_offset asm("r1");
            void *effect_sprite;

            {
                register u32 *random_callback_view asm("r2") = random_callback_slot;

                angle_random = CallFunctionR0(*random_callback_view);
            }
            sprite_angle = DivideSigned32(sprite_index << 5, 7);
            angle_jitter = (u32)(angle_random * 9) >> 15;
            angle_jitter += 108;
            sprite_angle += angle_jitter;
            {
                register u32 *random_callback_view asm("r1") = random_callback_slot;

                speed_random_or_x_offset = CallFunctionR0(*random_callback_view);
            }
            speed_fixed8 = speed_random_or_x_offset << 9;
            speed_fixed8 += speed_random_or_x_offset;
            speed_fixed8 = (u32)speed_fixed8 >> 15;
            minimum_speed_fixed8 = 0x200;
            asm volatile("" : "+r"(minimum_speed_fixed8));
            speed_fixed8 += minimum_speed_fixed8;
            {
                register s32 spawn_x asm("r3");

                speed_random_or_x_offset = 4;
                spawn_x = *(s16 *)(group_bytes + speed_random_or_x_offset);
                asm volatile("" :: "r"(minimum_speed_fixed8));
                {
                    register s32 group_y_offset asm("r2") = 8;
                    register s32 spawn_y asm("r0");

                    spawn_y = *(s16 *)(group_bytes + group_y_offset);
                    effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0, spawn_x, spawn_y,
                        (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), sprite_angle, speed_fixed8, 0);
                }
            }
            next_sprite_index_bits = sprite_index + 1;
            sprite_offset = next_sprite_index_bits << 2;
            *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)effect_sprite;
            next_sprite_index_bits <<= 24;
            sprite_index = (u32)next_sprite_index_bits >> 24;
        } while ((u32)sprite_index <= 7);
        PlayBattleAnimationSound(0);
        {
            register s32 *phase_slot asm("r1") = saved_effect_state;

            *phase_slot = *phase_slot + 1;
        }
        return;
    }

    sprite_index = 0;
    if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
        s32 *sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        s32 sprite_offset;

        do {
            register s32 next_sprite_index_bits asm("r0") = sprite_index + 1;

            next_sprite_index_bits <<= 24;
            sprite_index = (u32)next_sprite_index_bits >> 24;
        } while ((u32)sprite_index <= 8 &&
            (sprite_offset = sprite_index << 2,
             *(s32 *)((char *)sprite_slots + sprite_offset)) == 0);
    }
    if (sprite_index == 9) {
        DestroySpriteGroup(group_bytes);
    }
}
