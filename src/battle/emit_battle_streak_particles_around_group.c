#include "m2c_prelude.h"
#include "battle_display.h"

extern s32 D_03000010;
extern u8 gBattleSceneSide asm("D_02033F36");

s32 CallFunctionR0(s32) asm("func_080ECD5C");
void *CreateSpriteFromTable(s32, u8, s32, s16, s32, s32, s32, s32, s32) asm("func_08094374");

void EmitBattleStreakParticlesAroundGroup(void *group) asm("func_080CCF60");

void EmitBattleStreakParticlesAroundGroup(void *group)
{
    register char *group_bytes asm("r5") = group;
    void **sprite_slots;
    register s32 *random_callback_address asm("r6");
    register s32 slot_index asm("r2");

    slot_index = 0;
    sprite_slots = (void **)(group_bytes + BATTLE_STREAK_GROUP_OFFSET(sprite_slots));
    random_callback_address = &D_03000010;
    do {
        register s32 slot_offset asm("r8");
        register s32 slot_offset_source asm("r0");
        register void *existing_sprite asm("r1");

        slot_offset_source = slot_index * 4;
        existing_sprite = *(void **)((u8 *)sprite_slots + slot_offset_source);
        slot_offset = slot_offset_source;
        if (existing_sprite == 0) {
            register s32 animation_index asm("r4");
            register s32 random asm("r0");
            register s32 spawn_y asm("r2");
            register void **sprite_slot asm("r4");
            void *sprite;

            random = CallFunctionR0(*random_callback_address);
            animation_index = (u32)(random * 4) >> 16;
            random = CallFunctionR0(*random_callback_address);
            spawn_y = BATTLE_STREAK_GROUP_FIELD(group_bytes, s32, y);
            spawn_y += (u32)(random * 0x41) >> 15;
            spawn_y -= 0x20;
            spawn_y = (s16)spawn_y;
            sprite = CreateSpriteFromTable(BATTLE_STREAK_SPRITE_TABLE_ROM, BATTLE_STREAK_RESOURCE_SLOT, animation_index, 0, spawn_y,
                BATTLE_STREAK_TILE_OFFSET, 10, gBattleSceneSide != 0 ? 0x9048 : 0x1048,
                BATTLE_STREAK_UPDATE_CALLBACK);
            sprite_slot = (void **)((u8 *)sprite_slots + slot_offset);
            *sprite_slot = sprite;

            if (gBattleSceneSide == 0) {
                register s32 *sprite_words_r2 asm("r2");
                register s32 spawn_x_fixed8 asm("r0");
                register s32 random_x_offset asm("r1");

                random = CallFunctionR0(*random_callback_address);
                sprite_words_r2 = *sprite_slot;
                random_x_offset = random << 6;
                random_x_offset += random;
                random_x_offset = (u32)random_x_offset >> 15;
                spawn_x_fixed8 = BATTLE_STREAK_GROUP_FIELD(group_bytes, s32, x);
                spawn_x_fixed8 += random_x_offset;
                spawn_x_fixed8 -= 0x20;
                spawn_x_fixed8 <<= 8;
                sprite_words_r2[10] = spawn_x_fixed8;
            } else {
                register s32 *sprite_words_r2 asm("r2");
                register s32 spawn_x_fixed8 asm("r1");

                random = CallFunctionR0(*random_callback_address);
                sprite_words_r2 = *sprite_slot;
                spawn_x_fixed8 = random << 6;
                spawn_x_fixed8 += random;
                spawn_x_fixed8 = (u32)spawn_x_fixed8 >> 15;
                spawn_x_fixed8 -= BATTLE_STREAK_GROUP_FIELD(group_bytes, s32, x);
                spawn_x_fixed8 += 0xD0;
                spawn_x_fixed8 <<= 8;
                sprite_words_r2[10] = spawn_x_fixed8;
            }

            random = CallFunctionR0(*random_callback_address);
            {
                register s32 slot_offset_view asm("r2") = slot_offset;
                register void **sprite_slot_view asm("r1");
                register s32 *sprite_words_r2 asm("r2");
                register s32 speed_fixed8 asm("r1");

                asm volatile("" : "+r"(slot_offset_view));
                sprite_slot_view = (void **)((u8 *)sprite_slots + slot_offset_view);
                sprite_words_r2 = *sprite_slot_view;
                speed_fixed8 = random << 8;
                speed_fixed8 += random;
                speed_fixed8 = (u32)speed_fixed8 >> 15;
                speed_fixed8 += 0x200;
                sprite_words_r2[11] = speed_fixed8;
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
