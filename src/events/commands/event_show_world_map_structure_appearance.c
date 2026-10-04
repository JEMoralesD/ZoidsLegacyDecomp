#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"

extern void PlaySong(s32) asm("func_08092E84");
extern s32 CreateSprite(const void *, const void *, s32, u32, u32, s32,
    s32, s32, s32) asm("func_08094484");
extern void DestroySprite(s32) asm("func_08094554");
extern void SetSpriteAnimation(void *, s32) asm("func_08094564");
extern void InitializeEventSpritePool(void) asm("func_0809F850");
extern void LoadEventSpriteGraphics(u8) asm("func_0809F8A0");
extern void CreateEventSprite(s32, u32, s32, u32, u32, s32) asm("func_0809F94C");
extern void DestroyEventSprite(u8) asm("func_0809FB78");
extern void SeekEventCommand(s32, s32, s32) asm("func_080A016C");
extern void CreateWorldMapStructureSprite(void) asm("func_080A6148");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 EventShowWorldMapStructureAppearance(u8 script_slot, struct EventWorldMapStructureCursor *script_cursor) asm("func_080A61B0");

s32 EventShowWorldMapStructureAppearance(u8 script_slot, struct EventWorldMapStructureCursor *script_cursor)
{
    volatile u8 *field_save_state;
    struct EventWorldMapStructureCommand *command;
    register s32 world_x_fixed8 asm("r8");
    register s32 world_y_fixed8 asm("r9");
    register s32 carrier_r5 asm("r5");
    register s32 carrier_r6 asm("r6");
    s32 carrier_r7;
    register s32 carrier_r4 asm("r4");
    register struct WorldMapStructureAppearanceSprites *appearance_sprites asm("r10");
    volatile u8 frame_padding[16];
    volatile s32 saved_script_slot = script_slot;

    field_save_state = (volatile u8 *)0x0202ECF4;

    field_save_state[EVENT_COMMAND_OFFSET(EventFieldMapStateView, world_structure_kind)] = script_cursor->command->structure_variant + 1;
    field_save_state[EVENT_COMMAND_OFFSET(EventFieldMapStateView, world_structure_cell_x)] = script_cursor->command->map_cell_x;
    {
        u8 map_cell_y = script_cursor->command->map_cell_y;

        field_save_state += 32;
        *field_save_state = map_cell_y;
    }
    if (*(u8 *)0x020316F4 == 0) {
        *(u8 *)0x02030664 = 1;
        LoadEventSpriteGraphics(4);
        LoadEventSpriteGraphics(script_cursor->command->structure_variant + 12);

        command = script_cursor->command;
        world_x_fixed8 = command->map_cell_x << 12;
        world_y_fixed8 = command->map_cell_y << 12;
        carrier_r5 = -0x6400;
        carrier_r6 = 0;
        {
            register u32 structure_resource_bits asm("r0") = command->structure_variant + 12;
            register u32 structure_resource_id asm("r1");
            register s32 world_x_fixed8_r3 asm("r3");

            structure_resource_id = (u8)structure_resource_bits;
            world_x_fixed8_r3 = world_x_fixed8;
            asm volatile("" : "+r"(world_x_fixed8_r3));
            carrier_r7 = (u32)world_x_fixed8_r3 >> 8;
            carrier_r4 = world_y_fixed8;
            asm volatile("" : "+r"(carrier_r4));
            carrier_r4 = (u32)carrier_r4 >> 8;
            CreateEventSprite(0, structure_resource_id, 0, carrier_r7, carrier_r4, 1);
        }
        carrier_r7 = CreateSprite((const void *)0x0832BB9C,
            (const void *)0x0832BBA8, 0, carrier_r7, carrier_r4, 0x3B2, 15,
            0x11C8, carrier_r6);
        {
            register s32 limit_copy asm("r0") = -0x800;

            carrier_r4 = limit_copy;

            do {
                register struct WorldMapStructureAppearanceSprites *appearance_sprites_copy asm("r0");
                register struct WorldMapStructureSpriteOffsetView *structure_sprite asm("r1");

                carrier_r6 += 0x40;
                carrier_r5 += carrier_r6;
                if (carrier_r5 > carrier_r4) {
                    carrier_r5 = limit_copy;
                }
                appearance_sprites_copy = (struct WorldMapStructureAppearanceSprites *)0x02031940;
                appearance_sprites = appearance_sprites_copy;
                structure_sprite = appearance_sprites->structure;
                {
                    register s32 rounded_y_offset_fixed8 asm("r0") = carrier_r5;

                    if (carrier_r5 < 0) {
                        rounded_y_offset_fixed8 += 0xFF;
                    }
                    structure_sprite->offset_y = rounded_y_offset_fixed8 >> 8;
                }
                YieldTaskForUpdates(1);
                limit_copy = carrier_r4;
            } while (carrier_r5 < carrier_r4);
        }

        {
            register struct WorldMapStructureAppearanceSprites *appearance_sprites_copy asm("r1") = appearance_sprites;

            SetSpriteAnimation(appearance_sprites_copy->structure, 3);
        }
        DestroySprite(carrier_r7);

        {
            register s32 world_x_fixed8_r3 asm("r3") = world_x_fixed8;
            register s32 world_y_fixed8_r0 asm("r0");
            register s32 dust_x asm("r3");

            asm volatile("" : "+r"(world_x_fixed8_r3));
            carrier_r5 = world_x_fixed8_r3 >> 8;
            dust_x = carrier_r5;
            dust_x -= 12;
            world_y_fixed8_r0 = world_y_fixed8;
            asm volatile("" : "+r"(world_y_fixed8_r0));
            world_y_fixed8_r0 >>= 8;
            asm volatile("" : "+r"(world_y_fixed8_r0));
            carrier_r6 = world_y_fixed8_r0;
            CreateEventSprite(1, 4, 0, dust_x, carrier_r6, ({
                carrier_r7 = 0;
                carrier_r7;
            }));
        }
        {
            register s32 dust_x asm("r3") = carrier_r5 - 4;

            carrier_r4 = carrier_r6 + 4;
            CreateEventSprite(2, 4, 0, dust_x, carrier_r4, carrier_r7);
        }
        CreateEventSprite(3, 4, 0, carrier_r5 + 4, carrier_r4, carrier_r7);
        CreateEventSprite(4, 4, 0, carrier_r5 + 12, carrier_r6,
            carrier_r7);
        PlaySong(124);

        carrier_r6 = 0;
        {
            register struct WorldMapStructureAppearanceSprites *appearance_sprites_check asm("r1") = appearance_sprites;

            if ((appearance_sprites_check->dust->flags & 1) != 0) {
            register struct WorldMapStructureScrollView *background_scroll_offsets asm("r4") =
                (struct WorldMapStructureScrollView *)0x03000054;

            do {
                if (carrier_r6 <= 8) {
                    register volatile s32 *shake_step_table asm("r0") =
                        (volatile s32 *)0x087A18E8;
                    register u32 shake_step_address asm("r2");

                    asm volatile("" : "+r"(shake_step_table));
                    shake_step_address = carrier_r6 << 2;
                    shake_step_address += (u32)shake_step_table;
                    background_scroll_offsets->bg0_y_fixed8 += *(volatile s32 *)shake_step_address;
                    background_scroll_offsets->bg1_y_fixed8 += *(volatile s32 *)shake_step_address;
                    background_scroll_offsets->bg2_y_fixed8 += *(volatile s32 *)shake_step_address;
                    background_scroll_offsets->bg3_y_fixed8 += *(volatile s32 *)shake_step_address;
                    carrier_r6++;
                }
                YieldTaskForUpdates(1);
            } while ((((struct WorldMapStructureAppearanceSprites *)0x02031940)->dust->flags &
                1) != 0);
            }
        }

        {
            register struct WorldMapStructureAppearanceSprites *appearance_sprites_base asm("r4") =
                (struct WorldMapStructureAppearanceSprites *)0x02031940;
            register struct WorldMapStructureSpriteOffsetView *structure_sprite asm("r0");
            register u32 sprite_flags asm("r1");
            register u32 animation_flag_mask asm("r2");

            structure_sprite = appearance_sprites_base->structure;
            sprite_flags = structure_sprite->flags;
            animation_flag_mask = ~0x30;
            sprite_flags &= animation_flag_mask;
            animation_flag_mask = 0x10;
            sprite_flags |= animation_flag_mask;
            structure_sprite->flags = sprite_flags;
            SetSpriteAnimation(structure_sprite, 1);
            PlaySong(79);
            if ((appearance_sprites_base->structure->flags & 4) == 0) {
                register struct WorldMapStructureAppearanceSprites *appearance_sprites_for_wait asm("r5") =
                    appearance_sprites_base;
                register u32 animation_finished_flag asm("r4") = 4;

                do {
                    YieldTaskForUpdates(1);
                } while ((appearance_sprites_for_wait->structure->flags & animation_finished_flag) == 0);
            }
        }

        {
            u8 sprite_slot = 0;

            do {
                DestroyEventSprite(sprite_slot);
                sprite_slot++;
            } while ((u32)sprite_slot <= 15);
        }
        InitializeEventSpritePool();
        CreateWorldMapStructureSprite();
    }

    {
        register s32 next_command_selector asm("r1") = -1;
        register s32 saved_script_slot_r0 asm("r0");

        saved_script_slot_r0 = saved_script_slot;
        SeekEventCommand(saved_script_slot_r0, next_command_selector, 0);
    }
    return 0;
}
