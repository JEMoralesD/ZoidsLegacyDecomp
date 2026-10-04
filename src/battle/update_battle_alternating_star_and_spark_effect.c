#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void DestroySpriteGroup(void *) asm("func_08095114");
void *CreateBattleAngledProjectileSprite(void *, s32, s32, s32) asm("func_080D2660");
u32 CallFunctionR0(u32) asm("func_080ECD5C");

extern u32 gRandomNumberCallback asm("D_03000010");

struct BattleStarAndSparkEmissionView {
    u32 flags;
    s32 x;
    s32 y;
    void *sprite_slots[BATTLE_ANIMATION_GROUP_SPRITE_COUNT];
    u32 emission_count;
};

void UpdateBattleAlternatingStarAndSparkEffect(struct BattleStarAndSparkEmissionView *group) asm("func_080DC490");

void UpdateBattleAlternatingStarAndSparkEffect(struct BattleStarAndSparkEmissionView *group)
{
    volatile s32 reserve0;
    volatile s32 reserve1;
    volatile s32 reserve2;
    volatile s32 reserve3;
    volatile s32 reserve4;
    register struct BattleStarAndSparkEmissionView *effect_view asm("r6") = group;
    register u32 emission_count asm("r0") = effect_view->emission_count;

    asm volatile("" : "=m"(reserve0), "=m"(reserve1),
                       "=m"(reserve2), "=m"(reserve3),
                       "=m"(reserve4));

    if (emission_count <= 31U) {
        u32 resource_slot = BATTLE_STAR_AND_SPARK_RESOURCE_SPARK;
        register u32 *random_callback_slot asm("r5");
        register s32 x asm("r8");
        register s32 random_x_work asm("r2");
        register s32 y asm("r4");
        register u32 random asm("r0");
        register s32 speed_fixed8 asm("r1");
        register volatile s32 *outgoing asm("sp");
        void *effect_sprite;

        resource_slot &= emission_count;
        random_callback_slot = &gRandomNumberCallback;
        random = CallFunctionR0(*random_callback_slot);
        random_x_work = effect_view->x;
        random_x_work += (random * 0x41) >> 15;
        random_x_work -= 0x20;
        random_x_work = (s16)random_x_work;
        x = random_x_work;
        random = CallFunctionR0(*random_callback_slot);
        y = effect_view->y;
        y += (random * 0x41) >> 15;
        y -= 0x20;
        y = (s16)y;
        random = CallFunctionR0(*random_callback_slot);
        speed_fixed8 = (random * 0x101) >> 15;
        speed_fixed8 += 0x200;

        outgoing[0] = y;
        if (resource_slot == BATTLE_STAR_AND_SPARK_RESOURCE_STAR) {
            outgoing[1] = (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_LOOP_ANIMATION);
        } else {
            outgoing[1] = BATTLE_SPRITE_SEMITRANSPARENT;
        }
        outgoing[2] = 0;
        outgoing[3] = speed_fixed8;
        outgoing[4] = 0;
        {
            register void *call_group asm("r0") = effect_view;
            register s32 call_resource_slot asm("r1") = resource_slot;
            register s32 call_animation_id asm("r2") = 0;
            register s32 call_x asm("r3") = x;

            effect_sprite = CreateBattleAngledProjectileSprite(call_group, call_resource_slot, call_animation_id, call_x);
        }

        {
            register u32 *emission_count_slot asm("r4") = &effect_view->emission_count;
            register u32 sprite_index asm("r2") = *emission_count_slot;
            register u32 sprite_slot_offset asm("r3") = sprite_index << 2;
            register u8 *sprite_slots asm("r1") = (u8 *)&effect_view->sprite_slots[0];

            *(void **)(sprite_slots + sprite_slot_offset) = effect_sprite;
            sprite_index += 1;
            *emission_count_slot = sprite_index;
        }
        return;
    }

    {
        register u32 sprite_index asm("r1") = 0;

        if (effect_view->sprite_slots[0] == 0) {
            register void **sprite_slots asm("r2") = &effect_view->sprite_slots[0];

            do {
                register u32 next_sprite_index_bits asm("r0") = sprite_index + 1;

                next_sprite_index_bits <<= 24;
                sprite_index = next_sprite_index_bits >> 24;
                if (sprite_index > 31U) {
                    break;
                }
                {
                    register u32 sprite_slot_offset asm("r0") = sprite_index << 2;
                    register void **sprite_slot asm("r0");

                    sprite_slot = (void **)((u32)sprite_slots + sprite_slot_offset);
                    asm volatile("" : "+r"(sprite_slot));
                    if (*sprite_slot != 0) {
                        break;
                    }
                }
            } while (1);
        }
        if (sprite_index == 32) {
            DestroySpriteGroup(effect_view);
        }
    }
}
