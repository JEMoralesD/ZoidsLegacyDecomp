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

void UpdateBattleLightningParticleAndStreakEffect(struct BattleAnimationGroup *group) asm("func_080DAB58");

void UpdateBattleLightningParticleAndStreakEffect(struct BattleAnimationGroup *group) {
    register char *group_bytes asm("r6") = (char *)group;
    register s32 *effect_state asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register s32 phase asm("r1") = *effect_state;
    u8 sprite_index;

    if (phase == BATTLE_CONTACT_EFFECT_CREATE) {
        register s32 *saved_effect_state asm("sl");
        register s32 *sprite_slots asm("r9");
        register u32 *random_callback_slot asm("r8");

        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAnimationSprite(group_bytes, 0, 0,
            (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)) - BATTLE_LIGHTNING_SPRITE_HALF_WIDTH_PIXELS), *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
            phase, phase, 1);
        sprite_index = 0;
        saved_effect_state = effect_state;
        {
            register s32 *sprite_slots_init asm("r0") = (s32 *)BATTLE_ANIMATION_OFFSET(sprites[0]);
            asm volatile("" : "+r"(sprite_slots_init));
            sprite_slots_init = (s32 *)((char *)sprite_slots_init + (s32)group_bytes);
            sprite_slots = sprite_slots_init;
        }
        {
            register u32 *random_callback_init asm("r1") = &gRandomNumberCallback;
            asm volatile("" : "+r"(random_callback_init));
            random_callback_slot = random_callback_init;
        }
        do {
            register s32 random asm("r4");
            register s32 sprite_angle asm("r5");
            register s32 angle_jitter asm("r0");
            register s32 speed_fixed8 asm("r1");
            register s32 minimum_speed_fixed8 asm("r2");
            register s32 next_sprite_index_bits asm("r2");
            register s32 sprite_slot_address_or_offset asm("r1");
            void *effect_sprite;

            {
                register u32 *random_callback_view asm("r2") = random_callback_slot;
                random = CallFunctionR0(*random_callback_view);
            }
            sprite_angle = DivideSigned32(sprite_index << 5, 7);
            angle_jitter = (u32)(random * 9) >> 15;
            angle_jitter += 0x6C;
            sprite_angle += angle_jitter;
            {
                register u32 *random_callback_view asm("r1") = random_callback_slot;
                speed_fixed8 = (u32)(CallFunctionR0(*random_callback_view) * 0x201) >> 15;
            }
            minimum_speed_fixed8 = 0x200;
            asm volatile("" : "+r"(minimum_speed_fixed8));
            speed_fixed8 += minimum_speed_fixed8;
            {
                register s32 group_x_offset asm("r0");
                register s32 group_x asm("r3");

                asm volatile(
                    "mov %0, #4\n\t"
                    "ldrsh %1, [%2, %0]"
                    : "=r"(group_x_offset), "=r"(group_x)
                    : "r"(group_bytes));
                effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0,
                    group_x, *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
                    (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), sprite_angle, speed_fixed8, 0);
            }
            next_sprite_index_bits = sprite_index + 1;
            sprite_slot_address_or_offset = next_sprite_index_bits << 2;
            asm volatile("add %0, %1" : "+r"(sprite_slot_address_or_offset)
                         : "r"(sprite_slots));
            *(void **)sprite_slot_address_or_offset = effect_sprite;
            next_sprite_index_bits <<= 24;
            sprite_index = (u32)next_sprite_index_bits >> 24;
        } while (sprite_index <= 7);

        sprite_index = 0;
        random_callback_slot = &gRandomNumberCallback;
        do {
            register s32 random asm("r5");
            register s32 sprite_angle asm("r4");
            register s32 angle_jitter asm("r0");
            register s32 speed_fixed8 asm("r1");
            register s32 minimum_speed_fixed8 asm("r0");
            register s32 sprite_slot_address_or_offset asm("r1");
            void *effect_sprite;

            {
                register u32 *random_callback_view asm("r1") = random_callback_slot;
                random = CallFunctionR0(*random_callback_view);
            }
            sprite_angle = DivideSigned32(sprite_index << 5, 7);
            sprite_angle += 0x6C;
            angle_jitter = (u32)(random * 9) >> 15;
            sprite_angle += angle_jitter;
            sprite_angle = (u8)sprite_angle;
            {
                register u32 *random_callback_view asm("r2") = random_callback_slot;
                speed_fixed8 = (u32)(CallFunctionR0(*random_callback_view) * 0x201) >> 15;
            }
            minimum_speed_fixed8 = 0x200;
            asm volatile("" : "+r"(minimum_speed_fixed8));
            speed_fixed8 += minimum_speed_fixed8;
            effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 2, 0,
                *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
                (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), sprite_angle, speed_fixed8, 0);
            sprite_slot_address_or_offset = sprite_index + 9;
            sprite_slot_address_or_offset <<= 2;
            asm volatile("add %0, %1" : "+r"(sprite_slot_address_or_offset)
                         : "r"(sprite_slots));
            *(void **)sprite_slot_address_or_offset = effect_sprite;
            asm volatile("add %0, #128" : "+r"(sprite_angle));
            BATTLE_SPRITE_FIELD(effect_sprite, s8, rotation) = sprite_angle;
            sprite_index = (u8)(sprite_index + 1);
        } while (sprite_index <= 7);

        PlayBattleAnimationSound(0);
        {
            register s32 *phase_slot asm("r1") = saved_effect_state;
            *phase_slot = *phase_slot + 1;
        }
    } else {
        sprite_index = 0;
        if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            register char *sprite_slots_view asm("r1") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
            do {
                sprite_index = (u8)(sprite_index + 1);
            } while (sprite_index <= 0x10 &&
                *(s32 *)(sprite_slots_view + (sprite_index << 2)) == 0);
        }
        if (sprite_index == 0x11) {
            DestroySpriteGroup(group_bytes);
        }
    }
}
