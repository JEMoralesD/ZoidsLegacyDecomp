#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
#include "../../battle/battle_display.h"

u8 DivideUnsigned32(u8, s32) asm("func_080ECF00");
s32 ModuloUnsigned32(u8, s32) asm("func_080ECF78");
void LoadZoidIconGraphics(u8, u8, s32, u8) asm("func_0809A4CC");
struct EventBattleIconPositionView *CreateSprite(s32, s32, s32, s32) asm("func_08094484");
void SeekEventCommand(s32, s32, s32) asm("func_080A016C");

s32 EventCreateBattleUnitSpriteAtPosition(s32 script_slot, struct EventBattleUnitSpritePositionCommand **script_cursor) asm("func_080A26A4");

s32 EventCreateBattleUnitSpriteAtPosition(s32 script_slot, struct EventBattleUnitSpritePositionCommand **script_cursor) {
    register struct EventBattleUnitSpritePositionCommand **saved_script_cursor asm("r9");
    register s32 sprite_setup_carrier_r4 asm("r4");
    struct EventBattleUnitSpritePositionCommand *command;
    register u32 side asm("r6");
    register u32 unit_slot asm("r8");
    struct EventBattleIconPositionView *unit_sprite;
    volatile s32 reserve0;
    volatile s32 reserve1;
    volatile s32 reserve2;
    volatile s32 reserve3;
    volatile s32 reserve4;
    volatile s32 saved_script_slot;

    asm volatile("" : "=m"(reserve0),
                       "=m"(reserve1),
                       "=m"(reserve2),
                       "=m"(reserve3),
                       "=m"(reserve4));
    saved_script_cursor = script_cursor;
    script_slot <<= 24;
    saved_script_slot = (u32)script_slot >> 24;
    sprite_setup_carrier_r4 = (s32)*script_cursor;
    command = (struct EventBattleUnitSpritePositionCommand *)sprite_setup_carrier_r4;
    {
        register u32 sprite_index asm("r5");
        register u32 result asm("r0");

        sprite_index = command->sprite_index;
        result = DivideUnsigned32(sprite_index, 6);
        result <<= 24;
        side = result >> 24;
        result = ModuloUnsigned32(sprite_index, 6);
        result <<= 24;
        result >>= 24;
        unit_slot = result;
        LoadZoidIconGraphics(command->model_id, command->palette_variant, sprite_index << 6, sprite_index);
    }

    {
        register u8 *zoid_base_records asm("r2");
        register s32 model_id asm("r1");
        register s32 metadata_offset asm("r0");
        register s32 size_class_or_table asm("r0");
        s32 asset_a;
        register s32 asset_b asm("r10");
        s32 callback;
        register struct EventBattleUnitSpritePositionCommand *call_input asm("r5");
        register volatile s32 *outgoing asm("sp");

        zoid_base_records = (u8 *)EVENT_ZOID_BASE_RECORDS_ROM;
        asm volatile("" : "+r"(zoid_base_records));
        call_input = *saved_script_cursor;
        model_id = call_input->model_id;
        metadata_offset = model_id << 3;
        metadata_offset -= model_id;
        metadata_offset <<= 3;
        metadata_offset += (s32)zoid_base_records;
        size_class_or_table = *(u8 *)(metadata_offset + 2);

        if (size_class_or_table == ZOID_SIZE_CLASS_DOUBLE_ICON) {
            register s32 asset_b_init asm("r1");

            asset_a = 0x0821024C;
            asset_b_init = 0x08210258;
            asm volatile("" : "+r"(asset_b_init));
            asset_b = asset_b_init;
            outgoing[0] = 0;
            outgoing[1] = call_input->sprite_index << 6;
            outgoing[2] = call_input->sprite_index;
            sprite_setup_carrier_r4 = 3;
            sprite_setup_carrier_r4 -= ModuloUnsigned32(unit_slot, 3);
            sprite_setup_carrier_r4 <<= 6;
            sprite_setup_carrier_r4 |= 0x00080308;
            if (call_input->flip_x != 0) {
                sprite_setup_carrier_r4 |= BATTLE_SPRITE_FLIP_X;
            }
            outgoing[3] = sprite_setup_carrier_r4;
            callback = 0x080BAE19;
        } else {
            register s32 asset_b_init asm("r2");

            asset_a = 0x0821024C;
            asset_b_init = 0x08210258;
            asm volatile("" : "+r"(asset_b_init));
            asset_b = asset_b_init;
            outgoing[0] = 0;
            outgoing[1] = call_input->sprite_index << 6;
            outgoing[2] = call_input->sprite_index;
            sprite_setup_carrier_r4 = 3;
            sprite_setup_carrier_r4 -= ModuloUnsigned32(unit_slot, 3);
            sprite_setup_carrier_r4 <<= 6;
            sprite_setup_carrier_r4 |= 0x00000308;
            if (call_input->flip_x != 0) {
                sprite_setup_carrier_r4 |= BATTLE_SPRITE_FLIP_X;
            }
            outgoing[3] = sprite_setup_carrier_r4;
            callback = 0x080BADD5;
        }
        outgoing[4] = callback;
        unit_sprite = CreateSprite(asset_a,
                               asset_b,
                               0,
                               0);
    }

    {
        struct EventBattleIconPositionView **unit_sprite_slots;
        register u32 slot_copy asm("r1");
        register s32 slot_offset asm("r2");
        register s32 group_double asm("r3");
        register s32 unit_sprite_offset asm("r1");
        register struct EventBattleIconPositionView **unit_sprite_address asm("r1");
        register s32 retained_slot_offset asm("r7");
        register s32 retained_group_double asm("r5");

        sprite_setup_carrier_r4 = EVENT_BATTLE_UNIT_SPRITES_RAM;
        asm volatile("" : "+r"(sprite_setup_carrier_r4));
        unit_sprite_slots = (struct EventBattleIconPositionView **)sprite_setup_carrier_r4;
        slot_copy = unit_slot;
        slot_offset = slot_copy << 2;
        group_double = side << 1;
        unit_sprite_offset = group_double + side;
        unit_sprite_offset <<= 3;
        unit_sprite_offset = slot_offset + unit_sprite_offset;
        unit_sprite_address = (struct EventBattleIconPositionView **)(unit_sprite_offset + (s32)unit_sprite_slots);
        *unit_sprite_address = unit_sprite;
        retained_slot_offset = slot_offset;
        retained_group_double = group_double;
        asm volatile("" : "+r"(retained_slot_offset),
                             "+r"(retained_group_double));

        {
            register struct EventBattleUnitSpritePositionCommand *fresh_input asm("r2");
            register s32 high asm("r1");
            register s32 mask asm("r0");
            struct EventBattleUnitSpritePositionCommand *retained_input;
            register s32 sprite_or_slots_carrier_r3 asm("r3");
            register struct EventBattleIconPositionView *unit_sprite_for_x asm("r2");
            s32 coordinate;

            fresh_input = *saved_script_cursor;
            high = fresh_input->world_x_high;
            mask = 0x80;
            mask &= high;
            sprite_setup_carrier_r4 = (s32)fresh_input;
            retained_input = (struct EventBattleUnitSpritePositionCommand *)sprite_setup_carrier_r4;
            if (mask == 0) {
                register s32 offset asm("r0");
                register struct EventBattleIconPositionView **address asm("r0");
                register s32 upper asm("r0");
                register s32 lower asm("r1");

                sprite_or_slots_carrier_r3 = EVENT_BATTLE_UNIT_SPRITES_RAM;
                offset = retained_group_double + side;
                offset <<= 3;
                offset = retained_slot_offset + offset;
                address = (struct EventBattleIconPositionView **)(offset + sprite_or_slots_carrier_r3);
                unit_sprite_for_x = *address;
                asm volatile("" : : : "memory");
                upper = retained_input->world_x_high;
                upper <<= 8;
                lower = retained_input->world_x_low;
                coordinate = upper + lower;
            } else {
                register s32 offset asm("r0");
                register struct EventBattleIconPositionView **address asm("r0");
                register s32 upper asm("r0");
                register s32 lower asm("r1");

                sprite_or_slots_carrier_r3 = EVENT_BATTLE_UNIT_SPRITES_RAM;
                offset = retained_group_double + side;
                offset <<= 3;
                offset = retained_slot_offset + offset;
                address = (struct EventBattleIconPositionView **)(offset + sprite_or_slots_carrier_r3);
                unit_sprite_for_x = *address;
                asm volatile("" : : : "memory");
                upper = retained_input->world_x_high;
                upper <<= 8;
                asm volatile("" : "+r"(upper));
                lower = retained_input->world_x_low;
                upper += lower;
                lower = -0x10000;
                coordinate = upper + lower;
            }
            unit_sprite_for_x->world_x_fixed8 = coordinate << 8;
            asm volatile("" : "+r"(side) : "m"(unit_sprite_for_x->world_x_fixed8) : "memory");

            {
                register s32 offset asm("r0");
                register struct EventBattleIconPositionView **address asm("r0");

                offset = retained_group_double + side;
                offset <<= 3;
                offset = retained_slot_offset + offset;
                address = (struct EventBattleIconPositionView **)(offset + sprite_or_slots_carrier_r3);
                sprite_or_slots_carrier_r3 = (s32)*address;
            }
            asm volatile("" : "+r"(retained_group_double));
            ((struct EventBattleIconPositionView *)sprite_or_slots_carrier_r3)->world_y_fixed8 = 0;

            {
                register s32 high asm("r1");
                register s32 mask asm("r0");
                s32 coordinate;

                high = retained_input->world_z_high;
                mask = 0x80;
                mask &= high;
                if (mask == 0) {
                    register s32 upper asm("r0");
                    register s32 lower asm("r1");

                    asm volatile("" : : : "memory");
                    upper = retained_input->world_z_high;
                    upper <<= 8;
                    lower = retained_input->world_z_low;
                    coordinate = upper + lower;
                    asm volatile("" : "+r"(coordinate));
                } else {
                    register s32 upper asm("r0");
                    register s32 lower asm("r1");
                    register s32 sign asm("r2");

                    asm volatile("" : : : "memory");
                    upper = retained_input->world_z_high;
                    upper <<= 8;
                    asm volatile("" : "+r"(upper));
                    lower = retained_input->world_z_low;
                    upper += lower;
                    sign = -0x10000;
                    asm volatile("" : "+r"(sign));
                    coordinate = upper + sign;
                }
                ((struct EventBattleIconPositionView *)sprite_or_slots_carrier_r3)->world_z_fixed8 = coordinate << 8;
            }
            asm volatile("" : "+r"(retained_group_double));
        }

        {
            register u8 *marker_base asm("r1");
            register s32 marker_offset asm("r0");
            register s32 zero asm("r3");

            marker_base = (u8 *)EVENT_BATTLE_SPRITE_MOTION_RAM;
            asm volatile("" : "+r"(marker_base));
            marker_offset = retained_group_double + side;
            marker_offset <<= 1;
            marker_offset += unit_slot;
            marker_offset += (s32)marker_base;
            zero = 0;
            *(u8 *)marker_offset = zero;

            {
                register u8 *grid_base asm("r2");
                register u32 slot_copy asm("r0");
                register s32 unit_byte_offset asm("r1");
                register s32 side_byte_offset asm("r0");
                register u8 *grid_address asm("r1");
                register struct EventBattleUnitSpritePositionCommand **slot_address asm("r2");
                register struct EventBattleUnitSpritePositionCommand *current_input asm("r0");
                register s32 palette_variant asm("r0");

                grid_base = (u8 *)0x02034B4C;
                asm volatile("" : "+r"(grid_base));
                slot_copy = unit_slot;
                unit_byte_offset = retained_slot_offset + slot_copy;
                unit_byte_offset <<= 3;
                unit_byte_offset -= slot_copy;
                unit_byte_offset <<= 4;
                side_byte_offset = side << 2;
                side_byte_offset += side;
                side_byte_offset <<= 3;
                side_byte_offset -= side;
                side_byte_offset <<= 7;
                unit_byte_offset += side_byte_offset;
                grid_address = (u8 *)(unit_byte_offset + (s32)grid_base);
                slot_address = saved_script_cursor;
                current_input = *slot_address;
                palette_variant = current_input->model_id;
                grid_address[0] = palette_variant;
                current_input = *slot_address;
                palette_variant = current_input->palette_variant;
                grid_address[1] = palette_variant;
                *(u16 *)(grid_address + 4) = zero;
            }
        }
    }

    {
        register s32 callback_arg asm("r0");
        register s32 minus_one asm("r1");
        register s32 zero asm("r2");

        minus_one = 1;
        minus_one = -minus_one;
        asm volatile("" : "+r"(minus_one));
        callback_arg = saved_script_slot;
        zero = 0;
        SeekEventCommand(callback_arg, minus_one, zero);
    }
    return 0;
}
