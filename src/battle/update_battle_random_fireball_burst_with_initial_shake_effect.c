#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void DestroySpriteGroup(void *) asm("func_08095114");
/* The caller supplies the remaining four arguments through its outgoing stack area. */
void *CreateBattleAnimationSprite(void *, s32, s32, s32) asm("func_080D2450");
void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
void PlayBattleAnimationSound(s32) asm("func_080D2790");
u32 CallFunctionR0(u32) asm("func_080ECD5C");

extern u32 gRandomNumberCallback asm("D_03000010");


void UpdateBattleRandomFireballBurstWithInitialShakeEffect(struct BattleRandomFireballBurstGroupView *group) asm("func_080DFF84");

void UpdateBattleRandomFireballBurstWithInitialShakeEffect(struct BattleRandomFireballBurstGroupView *group)
{
    volatile s32 outgoing_y_reserve;
    volatile s32 outgoing_flags_reserve;
    volatile s32 outgoing_callback_reserve;
    volatile s32 outgoing_facing_reserve;
    register struct BattleRandomFireballBurstGroupView *effect_view asm("r6") = group;
    register s32 *effect_state_slot asm("r8") = &effect_view->phase;
    s32 phase = *effect_state_slot;

    asm volatile("" : "=m"(outgoing_y_reserve), "=m"(outgoing_flags_reserve),
                       "=m"(outgoing_callback_reserve), "=m"(outgoing_facing_reserve));

    if (phase == BATTLE_RANDOM_FIREBALL_BURST_EMIT) {
        register u32 *random_callback_slot asm("r5") = &gRandomNumberCallback;
        register s32 fireball_x asm("r4");
        register s32 fireball_y asm("r2");
        void *effect_sprite;
        u32 *emission_count_slot;
        register u32 sprite_slot_index asm("r3");
        register u32 random_bits asm("r0");
        register volatile s32 *outgoing_arguments asm("sp");

        random_bits = CallFunctionR0(*random_callback_slot);
        fireball_x = effect_view->x - 0x20;
        fireball_x += (random_bits * 0x41) >> 15;
        fireball_x = (s16)fireball_x;
        random_bits = CallFunctionR0(*random_callback_slot);
        fireball_y = effect_view->y - 0x20;
        fireball_y += (random_bits * 0x41) >> 15;
        fireball_y = (s16)fireball_y;
        outgoing_arguments[0] = fireball_y;
        outgoing_arguments[1] = phase;
        outgoing_arguments[2] = phase;
        outgoing_arguments[3] = phase;
        {
            register void *group_argument asm("r0") = effect_view;
            register s32 resource_slot_zero asm("r1") = 0;
            register s32 animation_zero asm("r2") = 0;
            register s32 fireball_x_argument asm("r3");

            asm volatile("" : "+r"(group_argument));
            asm volatile("" : "+r"(resource_slot_zero));
            asm volatile("" : "+r"(animation_zero));
            fireball_x_argument = fireball_x;
            asm volatile("" : "+r"(fireball_x_argument));
            effect_sprite = CreateBattleAnimationSprite(group_argument, resource_slot_zero, animation_zero, fireball_x_argument);
        }

        emission_count_slot = &effect_view->emission_count;
        sprite_slot_index = *emission_count_slot;
        {
            register s32 sprite_slot_offset asm("r2") = sprite_slot_index << 2;
            register u8 *sprite_slots asm("r1") = (u8 *)&effect_view->sprite_slots[0];
            asm volatile("" : "+r"(sprite_slots), "+r"(sprite_slot_offset), "+r"(sprite_slot_index));
            *(void **)(sprite_slots + sprite_slot_offset) = effect_sprite;
        }
        if (sprite_slot_index == 0) {
            SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
            PlayBattleAnimationSound(0);
        }
        *emission_count_slot += 1;
        if (*emission_count_slot == BATTLE_RANDOM_FIREBALL_BURST_SPRITE_COUNT) {
            register s32 *state_view asm("r1") = effect_state_slot;
            register s32 state_value asm("r0");

            asm volatile("" : "+r"(state_view));
            state_value = *state_view;
            state_value += 1;
            *state_view = state_value;
        }
        return;
    }

    {
        register u32 sprite_slot_index asm("r1") = 0;

        if (effect_view->sprite_slots[0] == 0) {
            register void **sprite_slots asm("r2") = &effect_view->sprite_slots[0];

            asm volatile("" : "+r"(sprite_slots));
            do {
                register u32 next_index_bits asm("r0") = sprite_slot_index + 1;

                next_index_bits <<= 24;
                sprite_slot_index = next_index_bits >> 24;
                if (sprite_slot_index >= BATTLE_RANDOM_FIREBALL_BURST_SPRITE_COUNT) {
                    break;
                }
                {
                    register u32 sprite_slot_offset asm("r0") = sprite_slot_index << 2;
                    register void **sprite_slot asm("r0");

                    sprite_slot = (void **)((u32)sprite_slots + sprite_slot_offset);
                    asm volatile("" : "+r"(sprite_slot));
                    if (*sprite_slot != 0) {
                        break;
                    }
                }
            } while (1);
        }
        if (sprite_slot_index == BATTLE_RANDOM_FIREBALL_BURST_SPRITE_COUNT) {
            DestroySpriteGroup(effect_view);
        }
    }
}
