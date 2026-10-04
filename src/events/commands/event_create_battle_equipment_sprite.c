#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"

extern void LoadSpriteGraphicsFromTable(u8 *, u32, u32, u32) asm("func_0809AA64");
extern s32 CreateMirroredSpriteFromTable(u8 *, u32, s32, s32, s32, u32, u32, u32, s32, u32) asm("func_080D22B4");
extern void SeekEventCommand(u32, s32, s32) asm("func_080A016C");

s32 EventCreateBattleEquipmentSprite(int script_slot, u8 **script_cursor) asm("func_080A3510");

s32 EventCreateBattleEquipmentSprite(int script_slot, u8 **script_cursor)
{
    register u32 saved_script_slot asm("r9");
    register u8 *sprite_table asm("sl");
    register u32 sprite_resource_index asm("r8");
    register u8 *mount_position_table asm("r4");
    register u8 *command asm("r3");
    register u32 equipment_slot_or_offset asm("r5");
    register s32 mount_x_corrected asm("r5");
    register s32 mount_x asm("r6");
    register u8 *zoid_model_address asm("ip");
    register u8 *scene_side_address asm("r1");

    saved_script_slot = (u8)script_slot;
    {
        register u8 *graphics_table asm("r4");
        register u8 *command_r0 asm("r0");
        register u32 resource_variant asm("r1");
        register u32 resource_pair_base asm("r2");
        register u32 equipment_slot asm("r0");
        register u32 resource_index_bits asm("r0");
        graphics_table = (u8 *)0x087ABC6C;
        asm volatile("" : "+r"(graphics_table));
        command_r0 = *script_cursor;
        resource_variant = command_r0[2];
        resource_pair_base = resource_variant << 1;
        equipment_slot = command_r0[1];
        if (equipment_slot != 0) {
            resource_index_bits = (resource_pair_base + 1) << 24;
        } else {
            resource_index_bits = resource_variant << 25;
        }
        {
            register u32 graphics_resource_index asm("r1");
            register u8 *command_reloaded asm("r0");
            register u32 equipment_slot_r3 asm("r3");
            graphics_resource_index = resource_index_bits >> 24;
            command_reloaded = *script_cursor;
            equipment_slot_r3 = command_reloaded[1];
            LoadSpriteGraphicsFromTable(graphics_table, graphics_resource_index, equipment_slot_r3 << 7, equipment_slot_r3);
        }
    }
    {
        register u8 *sprite_table_r0 asm("r0");
        sprite_table_r0 = (u8 *)0x087AC2BC;
        asm volatile("" : "+r"(sprite_table_r0));
        sprite_table = sprite_table_r0;
    }
    {
        register u8 *command_for_resource asm("r0");
        register u32 sprite_variant asm("r1");
        register u32 sprite_pair_base asm("r2");
        register u32 equipment_slot_r0 asm("r0");
        command_for_resource = *script_cursor;
        sprite_variant = command_for_resource[2];
        sprite_pair_base = sprite_variant << 1;
        equipment_slot_r0 = command_for_resource[1];
        if (equipment_slot_r0 != 0) {
            register u32 paired_resource_index asm("r0");
            paired_resource_index = sprite_pair_base + 1;
            asm volatile("" : "+r"(paired_resource_index));
            sprite_resource_index = paired_resource_index;
        } else {
            sprite_variant <<= 1;
            sprite_resource_index = sprite_variant;
        }
    }
    {
        register u8 *zoid_model_address_r2 asm("r2");
        register u32 mount_position_offset asm("r1");
        register u32 zoid_model_or_record_offset asm("r0");
        mount_position_table = (u8 *)0x087EC38C;
        asm volatile("" : "+r"(mount_position_table));
        command = *script_cursor;
        equipment_slot_or_offset = command[1];
        mount_position_offset = equipment_slot_or_offset << 2;
        zoid_model_address_r2 = (u8 *)0x020317D6;
        asm volatile("" : "+r"(zoid_model_address_r2));
        zoid_model_or_record_offset = *zoid_model_address_r2;
        zoid_model_or_record_offset <<= 5;
        mount_position_offset += zoid_model_or_record_offset;
        mount_position_offset += (u32)mount_position_table;
        mount_x = *(s16 *)mount_position_offset;
        zoid_model_address = zoid_model_address_r2;
        if (equipment_slot_or_offset == 2) {
            register u8 *variant_x_adjustments asm("r0");
            register u32 variant_offset asm("r1");
            register s32 variant_x_adjustment asm("r0");
            variant_x_adjustments = (u8 *)0x087AC90C;
            asm volatile("" : "+r"(variant_x_adjustments));
            variant_offset = command[2];
            variant_offset <<= 1;
            variant_offset += (u32)variant_x_adjustments;
            variant_x_adjustment = *(s16 *)variant_offset;
            mount_x_corrected = (s16)(mount_x - variant_x_adjustment);
        } else {
            mount_x_corrected = mount_x;
        }
    }
    {
        register s32 sprite_address asm("r0");
        register u8 *equipment_sprite_slots asm("r2");
        sprite_address = CreateMirroredSpriteFromTable(
            sprite_table,
            sprite_resource_index,
            0,
            mount_x_corrected,
            ({
                register u32 equipment_slot_or_offset asm("r1");
                register u8 *zoid_model_address_for_y asm("r2");
                register u32 model_record_offset asm("r0");
                register u8 *mount_y_address asm("r0");
                register s32 mount_y asm("r0");
                equipment_slot_or_offset = command[1];
                equipment_slot_or_offset <<= 2;
                zoid_model_address_for_y = zoid_model_address;
                asm volatile("" : "+r"(zoid_model_address_for_y));
                model_record_offset = *zoid_model_address_for_y;
                model_record_offset <<= 5;
                equipment_slot_or_offset += model_record_offset;
                mount_y_address = mount_position_table + 2;
                equipment_slot_or_offset += (u32)mount_y_address;
                mount_y = *(s16 *)equipment_slot_or_offset;
                mount_y;
            }),
            ({
                register u32 tile_offset asm("r0");
                tile_offset = command[1];
                tile_offset <<= 7;
                tile_offset;
            }),
            ({
                register u32 palette_bank asm("r0");
                palette_bank = command[1];
                palette_bank;
            }),
            ({
                register u8 *mount_priority_table asm("r2");
                register u8 *zoid_model_address_for_flags asm("r0");
                register u32 zoid_model asm("r1");
                register u32 priority_offset asm("r0");
                register u32 equipment_slot asm("r3");
                register u32 mount_priority asm("r0");
                register u32 sprite_flags asm("r2");
                mount_priority_table = (u8 *)0x087ED68C;
                asm volatile("" : "+r"(mount_priority_table));
                zoid_model_address_for_flags = zoid_model_address;
                asm volatile("" : "+r"(zoid_model_address_for_flags));
                zoid_model = *zoid_model_address_for_flags;
                priority_offset = zoid_model << 1;
                priority_offset += zoid_model;
                equipment_slot = command[1];
                priority_offset += equipment_slot;
                priority_offset += (u32)mount_priority_table;
                mount_priority = *(u8 *)priority_offset;
                sprite_flags = mount_priority << 6;
                sprite_flags |= 0x1318;
                scene_side_address = (u8 *)0x02033F36;
                asm volatile("" : "+r"(scene_side_address));
                if (*scene_side_address != 0) {
                    sprite_flags |= 0x8000;
                }
                sprite_flags;
            }),
            0,
            ({
                register u32 scene_side asm("r0");
                scene_side = *scene_side_address;
                scene_side;
            }));
        equipment_sprite_slots = (u8 *)0x02033F40;
        asm volatile("" : "+r"(equipment_sprite_slots));
        {
            register u8 *command_for_storage asm("r1");
            register u32 sprite_slot_offset asm("r1");
            command_for_storage = *script_cursor;
            sprite_slot_offset = command_for_storage[1];
            sprite_slot_offset <<= 2;
            sprite_slot_offset += (u32)equipment_sprite_slots;
            *(s32 *)sprite_slot_offset = sprite_address;
        }
    }
    SeekEventCommand(saved_script_slot, -1, 0);
    return 0;
}
