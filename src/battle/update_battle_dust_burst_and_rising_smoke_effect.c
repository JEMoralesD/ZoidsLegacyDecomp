#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void DestroySpriteGroup(void *) asm("func_08095114");
void *CreateBattleAnimationSprite(void *, s32, s32, s32) asm("func_080D2450");
void *CreateBattleAngledProjectileSprite(void *, s32, s32, s32) asm("func_080D2660");
void PlayBattleAnimationSound(s32) asm("func_080D2790");
u32 CallFunctionR0(u32) asm("func_080ECD5C");

extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleDustBurstAndRisingSmokeEffect(struct BattleAnimationGroup *group) asm("func_080DB7CC");

void UpdateBattleDustBurstAndRisingSmokeEffect(struct BattleAnimationGroup *group)
{
    volatile s32 reserve0;
    volatile s32 reserve1;
    volatile s32 reserve2;
    volatile s32 reserve3;
    volatile s32 reserve4;
    register char *group_bytes = group;
    register u32 *saved_effect_state asm("r8") = (u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register u32 phase asm("r0") = *saved_effect_state;

    asm volatile("" : "=m"(reserve0), "=m"(reserve1),
                       "=m"(reserve2), "=m"(reserve3),
                       "=m"(reserve4));

    switch (phase) {
    case BATTLE_DUST_BURST_CREATE: {
        register volatile s32 *outgoing asm("sp");
        register s32 call_x asm("r3") = *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x));

        asm volatile("" : "+r"(call_x));
        outgoing[0] = *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y));
        outgoing[1] = BATTLE_SPRITE_SEMITRANSPARENT;
        outgoing[2] = 0;
        outgoing[3] = 0;
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAnimationSprite(group_bytes, BATTLE_DUST_BURST_RESOURCE_DUST, 0, call_x);
        PlayBattleAnimationSound(0);
        goto advance;
    }

    case BATTLE_DUST_BURST_EMIT_RISING_SMOKE: {
        register u32 *smoke_count_slot asm("r6") = (u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.dust_burst.smoke_count));
        register u32 *random_callback_slot asm("r5");
        register s32 x asm("r4");
        register u32 random asm("r0");
        register s32 speed_fixed8 asm("r1");
        register volatile s32 *outgoing asm("sp");
        void *effect_sprite;

        *smoke_count_slot = *smoke_count_slot + 1;
        random_callback_slot = &gRandomNumberCallback;
        asm volatile("" : "+r"(random_callback_slot));
        random = CallFunctionR0(*random_callback_slot);
        x = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)) - 0x40;
        x += (random * 0x81) >> 15;
        x = (s16)x;

        random = CallFunctionR0(*random_callback_slot);
        speed_fixed8 = (random * 0x101) >> 15;
        speed_fixed8 += 0x80;

        outgoing[0] = (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)) - 0xF);
        outgoing[1] = BATTLE_SPRITE_SEMITRANSPARENT;
        outgoing[2] = 0xC0;
        outgoing[3] = speed_fixed8;
        outgoing[4] = 0;
        effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, BATTLE_DUST_BURST_RESOURCE_SMOKE, 0, x);

        {
            register u32 sprite_index asm("r3") = *smoke_count_slot;
            register u32 offset asm("r2") = sprite_index << 2;
            register u8 *sprite_slots asm("r1") = (u8 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));

            *(void **)(sprite_slots + offset) = effect_sprite;
            if (sprite_index != 31) {
                break;
            }
        }

advance: {
            register u32 *effect_state_view asm("r1") = saved_effect_state;
            register u32 next_effect_state asm("r0");

            asm volatile("" : "+r"(effect_state_view));
            next_effect_state = *effect_state_view;
            next_effect_state += 1;
            *effect_state_view = next_effect_state;
        }
        break;
    }

    case BATTLE_DUST_BURST_WAIT_FOR_SPRITES: {
        register u32 sprite_index asm("r1") = 0;

        if (*(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            register void **sprite_slots asm("r2") = (void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));

            do {
                register u32 next_sprite_index_bits asm("r0") = sprite_index + 1;

                next_sprite_index_bits <<= 24;
                sprite_index = next_sprite_index_bits >> 24;
                if (sprite_index > 31U) {
                    break;
                }
                {
                    register u32 offset asm("r0") = sprite_index << 2;
                    register void **sprite_slot asm("r0");

                    sprite_slot = (void **)((u32)sprite_slots + offset);
                    asm volatile("" : "+r"(sprite_slot));
                    if (*sprite_slot != 0) {
                        break;
                    }
                }
            } while (1);
        }
        if (sprite_index == 32) {
            DestroySpriteGroup(group_bytes);
        }
        break;
    }
    }
}
