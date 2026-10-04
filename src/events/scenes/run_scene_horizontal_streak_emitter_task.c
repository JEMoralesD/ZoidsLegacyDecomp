#include "m2c_prelude.h"

extern s32 gRandomCallback asm("D_03000010");
extern u8 gBattleSceneSide asm("D_02033F36");

s32 CallFunctionR0(s32) asm("func_080ECD5C");
s32 *CreateSpriteFromTable(s32, u8, s32, s16, s32, s32, s32, s32, s32) asm("func_08094374");
void PlaySong(s32) asm("func_08092E84");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

void RunSceneHorizontalStreakEmitterTask(void) asm("func_080A829C");

void RunSceneHorizontalStreakEmitterTask(void)
{
    s32 updates_since_spawn;
    s32 spawn_interval;
    u32 sprite_slot_index;

    {
        s32 *elapsed_updates_address;
        s32 zero;
        s32 **streak_sprite_slots;

        sprite_slot_index = 0;
        elapsed_updates_address = (s32 *)0x02031C10;
        asm volatile("" : "+r"(elapsed_updates_address));
        zero = 0;
        streak_sprite_slots = (s32 **)0x02031C14;
        do {
            *streak_sprite_slots++ = (s32 *)zero;
            sprite_slot_index++;
        } while (sprite_slot_index <= 31);
        *elapsed_updates_address = 0;
    }

    updates_since_spawn = 0;
    spawn_interval = 18;
mainloop:
    {
        s32 one;
        s32 **streak_sprite_slot;

        sprite_slot_index = 0;
        one = 1;
        streak_sprite_slot = (s32 **)0x02031C14;
        do {
            s32 v = (s32)*streak_sprite_slot;

            if (v != 0) {
                v = *(s32 *)v;
                v &= one;
                if (v == 0) {
                    *streak_sprite_slot = (s32 *)v;
                }
            }
            streak_sprite_slot++;
            asm volatile("" : : "r"(streak_sprite_slot), "r"(streak_sprite_slot), "r"(streak_sprite_slot), "r"(streak_sprite_slot), "r"(streak_sprite_slot), "r"(streak_sprite_slot), "r"(streak_sprite_slot));
            sprite_slot_index++;
        } while (sprite_slot_index <= 31);
    }

    if (updates_since_spawn == spawn_interval || spawn_interval == 0) {
        s32 *rng;
        register s32 **streak_sprite_slot asm("r5");
        s32 offset;

        sprite_slot_index = 0;
        rng = &gRandomCallback;
        streak_sprite_slot = (s32 **)0x02031C14;
        offset = 0;
        do {
            s32 *entry = *streak_sprite_slot;

            if (entry == 0) {
                register s32 animation_variant asm("r8");
                register s32 spawn_y asm("r6");
                s32 random_flip_selector;
                s32 sprite_flags;
                s32 *created_streak;
                s32 side_or_spawn_x;
                s32 *stored_streak;
                s32 streak_speed_fixed8;

                animation_variant = ((u32)(CallFunctionR0(*rng) * 3) >> 15) + 2;
                spawn_y = (CallFunctionR0(*rng) << 8) >> 16;
                random_flip_selector = (u32)(CallFunctionR0(*rng) * 10) >> 15;
                sprite_flags = 0x348;
                if (random_flip_selector != 0) {
                    sprite_flags += 0x80;
                }
                created_streak = CreateSpriteFromTable(0x087ACDD8, 0x77, (u16)animation_variant, 0, spawn_y,
                                       (s32)entry, (s32)entry, sprite_flags,
                                       0x080CCEF1);
                *streak_sprite_slot = created_streak;
                side_or_spawn_x = gBattleSceneSide;
                if (side_or_spawn_x != 0) {
                    side_or_spawn_x = 0xF000;
                }
                created_streak[10] = side_or_spawn_x;
                {
                    u8 *streak_slot_table = (u8 *)0x02031C14;

                    asm volatile("" : "+r"(streak_slot_table));
                    stored_streak = *(s32 **)(streak_slot_table + offset);
                }
                streak_speed_fixed8 = animation_variant + 1;
                streak_speed_fixed8 <<= 10;
                stored_streak[11] = streak_speed_fixed8;
                goto created;
            }
            streak_sprite_slot++;
            offset += 4;
            sprite_slot_index++;
        } while (sprite_slot_index <= 31);
created:
        updates_since_spawn = 0;
        if (spawn_interval != 0) {
            spawn_interval--;
            if (spawn_interval == 0) {
                PlaySong(0x68);
            }
        }
    } else {
        updates_since_spawn++;
    }

    {
        s32 elapsed_updates = *(s32 *)0x02031C10;

        if ((u32)elapsed_updates <= 0x12B) {
            *(s32 *)0x02031C10 = elapsed_updates + 1;
        }
    }
    YieldTaskForUpdates(1);
    goto mainloop;
}
