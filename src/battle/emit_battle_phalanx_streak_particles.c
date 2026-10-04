#include "m2c_prelude.h"
#include "battle_display.h"

extern s32 D_03000010;
extern u8 gBattleSceneSide asm("D_02033F36");

s32 CallFunctionR0(s32) asm("func_080ECD5C");
void *CreateSpriteFromTable(s32, u8, s32, s16, s32, s32, s32, s32, s32) asm("func_08094374");

void EmitBattlePhalanxStreakParticles(s32 group_address) asm("func_080CD05C");

void EmitBattlePhalanxStreakParticles(s32 group_address) {
    void **sprite_slots;
    register s32 *random_callback_address asm("r5");
    register s32 slot_index asm("r2");

    slot_index = 0;
    sprite_slots = (void **)(group_address + BATTLE_STREAK_GROUP_OFFSET(sprite_slots));
    random_callback_address = &D_03000010;
    do {
        register s32 slot_offset asm("r6");
        register s32 slot_offset_source asm("r0");
        register void *existing_sprite asm("r1");

        slot_offset_source = slot_index * 4;
        existing_sprite = *(void **)((u8 *)sprite_slots + slot_offset_source);
        slot_offset = slot_offset_source;
        if (existing_sprite == 0) {
            register s32 animation_index asm("r4");
            register s32 animation_index_bits asm("r1");
            s32 spawn_y;
            s32 side_or_start_x_fixed8;
            register s32 *sprite_words asm("r1");

            animation_index_bits = (u32)(CallFunctionR0(*random_callback_address) * 5) >> 15;
            animation_index = (u8)animation_index_bits;
            spawn_y = (CallFunctionR0(*random_callback_address) * 0x102) >> 16;
            sprite_words = CreateSpriteFromTable(BATTLE_STREAK_SPRITE_TABLE_ROM, BATTLE_STREAK_RESOURCE_SLOT, animation_index, 0, spawn_y,
                                    BATTLE_STREAK_TILE_OFFSET, 10,
                                    gBattleSceneSide != 0 ? 0x83C8 : 0x3C8,
                                    BATTLE_STREAK_UPDATE_CALLBACK);
            *(void **)((u8 *)sprite_slots + slot_offset) = sprite_words;
            side_or_start_x_fixed8 = gBattleSceneSide;
            if (side_or_start_x_fixed8 != 0) {
                side_or_start_x_fixed8 = 0xF000;
            }
            sprite_words[10] = side_or_start_x_fixed8;
            {
                register s32 **sprite_slot asm("r0");
                register s32 *sprite_words_reloaded asm("r1");
                register s32 speed_fixed8 asm("r0");

                asm volatile("add %0, %1, %2"
                             : "=r"(sprite_slot)
                             : "r"(sprite_slots), "r"(slot_offset));
                sprite_words_reloaded = *sprite_slot;
                speed_fixed8 = animation_index + 1;
                speed_fixed8 <<= 10;
                sprite_words_reloaded[11] = speed_fixed8;
            }
            return;
        }
        {
            register u8 next_slot_index asm("r0");

            next_slot_index = slot_index + 1;
            slot_index = next_slot_index;
        }
    } while ((u32)slot_index <= 0x1F);
}
