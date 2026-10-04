#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
#include "../../battle/battle_display.h"

extern struct ZoidBaseRecordView gZoidBaseStatTable[];

void ConfigureBattleUnitSprites(s32, s32, s32, s32, s32, s32, s32) asm("func_080BAF2C");
void SeekEventCommand(u8, s32, s32) asm("func_080A016C");

s32 EventSetBattleUnitModel(s32 script_slot, struct EventBattleUnitModelCommand **script_cursor) asm("func_080A2568");

s32 EventSetBattleUnitModel(s32 script_slot, struct EventBattleUnitModelCommand **script_cursor) {
    struct EventBattleUnitModelCommand *volatile *saved_script_cursor;
    register u32 saved_script_slot asm("r8");
    register struct EventBattleUnitModelCommand *command asm("r4");
    register s32 sprite_entry_mode asm("r5");

    saved_script_cursor = script_cursor;
    script_slot <<= 24;
    saved_script_slot = (u32)script_slot >> 24;
    command = *saved_script_cursor;
    sprite_entry_mode = command->sprite_entry_mode;
    switch (sprite_entry_mode) {
    case EVENT_BATTLE_SPRITES_AT_FORMATION: {
        register u32 model_id asm("r0");
        register u32 palette_variant asm("r1");
        register s32 size_class_or_offset asm("r2");
        register struct ZoidBaseRecordView *size_class_or_table asm("r3");

        model_id = command->model_id;
        palette_variant = command->palette_variant;
        size_class_or_table = (struct ZoidBaseRecordView *)EVENT_ZOID_BASE_RECORDS_ROM;
        asm volatile("" : "+r"(size_class_or_table));
        size_class_or_offset = model_id << 3;
        size_class_or_offset -= model_id;
        size_class_or_offset <<= 3;
        size_class_or_offset += (s32)size_class_or_table;
        size_class_or_offset = *(u8 *)(size_class_or_offset + 2);
        ConfigureBattleUnitSprites(model_id,
                      palette_variant,
                      size_class_or_offset,
                      command->side,
                      command->unit_slot,
                      sprite_entry_mode,
                      sprite_entry_mode);
        break;
    }
    case EVENT_BATTLE_SPRITES_ENTER_FROM_SIDE: {
        register s32 side asm("r6");
        register s32 flag_left asm("r0");
        register s32 flag_right asm("r1");

        side = command->side;
        if (side == 0) {
            register u32 model_id asm("r0");
            register u32 palette_variant asm("r1");
            register s32 size_class_or_offset asm("r2");
            register struct ZoidBaseRecordView *size_class_or_table asm("r3");

            model_id = command->model_id;
            palette_variant = command->palette_variant;
            size_class_or_table = (struct ZoidBaseRecordView *)EVENT_ZOID_BASE_RECORDS_ROM;
            asm volatile("" : "+r"(size_class_or_table));
            size_class_or_offset = model_id << 3;
            size_class_or_offset -= model_id;
            size_class_or_offset <<= 3;
            size_class_or_offset += (s32)size_class_or_table;
            size_class_or_offset = *(u8 *)(size_class_or_offset + 2);
            ConfigureBattleUnitSprites(model_id,
                          palette_variant,
                          size_class_or_offset,
                          0,
                          command->unit_slot,
                          0x10000,
                          side);
            flag_right = EVENT_BATTLE_SPRITE_MOTION_RAM;
            asm volatile("" : "+r"(flag_right));
            flag_left = (s32)*saved_script_cursor;
            flag_left = ((struct EventBattleUnitModelCommand *)flag_left)->unit_slot;
        } else {
            register u32 model_id asm("r0");
            register u32 palette_variant asm("r1");
            register s32 size_class_or_offset asm("r2");
            register struct ZoidBaseRecordView *size_class_or_table asm("r3");

            model_id = command->model_id;
            palette_variant = command->palette_variant;
            size_class_or_table = (struct ZoidBaseRecordView *)EVENT_ZOID_BASE_RECORDS_ROM;
            asm volatile("" : "+r"(size_class_or_table));
            size_class_or_offset = model_id << 3;
            size_class_or_offset -= model_id;
            size_class_or_offset <<= 3;
            size_class_or_offset += (s32)size_class_or_table;
            size_class_or_offset = *(u8 *)(size_class_or_offset + 2);
            ConfigureBattleUnitSprites(model_id,
                          palette_variant,
                          size_class_or_offset,
                          1,
                          command->unit_slot,
                          -0x10000,
                          0);
            flag_left = EVENT_BATTLE_SPRITE_MOTION_RAM;
            asm volatile("" : "+r"(flag_left));
            flag_right = (s32)*saved_script_cursor;
            flag_left += 6;
            flag_right = ((struct EventBattleUnitModelCommand *)flag_right)->unit_slot;
        }
        flag_left += flag_right;
        *(u8 *)flag_left = sprite_entry_mode;
        break;
    }
    }

    {
        register u8 *battle_unit_bytes asm("r4");

        battle_unit_bytes = (u8 *)0x02034B4C;
        {
            register struct EventBattleUnitModelCommand *current_command asm("r3");
            register s32 unit_slot asm("r0");
            register s32 offset asm("r1");
            register s32 side asm("r2");
            register s32 side_byte_offset asm("r0");
            register s32 model_or_palette asm("r0");

            current_command = *saved_script_cursor;
            unit_slot = current_command->unit_slot;
            offset = unit_slot << 2;
            offset += unit_slot;
            offset <<= 3;
            offset -= unit_slot;
            offset <<= 4;
            side = current_command->side;
            side_byte_offset = side << 2;
            side_byte_offset += side;
            side_byte_offset <<= 3;
            side_byte_offset -= side;
            side_byte_offset <<= 7;
            offset += side_byte_offset;
            offset += (s32)battle_unit_bytes;
            model_or_palette = current_command->model_id;
            sprite_entry_mode = 0;
            *(u8 *)offset = model_or_palette;
        }
        {
            register struct EventBattleUnitModelCommand *current_command asm("r3");
            register s32 unit_slot asm("r0");
            register s32 offset asm("r1");
            register s32 side asm("r2");
            register s32 side_byte_offset asm("r0");
            register s32 model_or_palette asm("r0");

            current_command = *saved_script_cursor;
            unit_slot = current_command->unit_slot;
            offset = unit_slot << 2;
            offset += unit_slot;
            offset <<= 3;
            offset -= unit_slot;
            offset <<= 4;
            side = current_command->side;
            side_byte_offset = side << 2;
            side_byte_offset += side;
            side_byte_offset <<= 3;
            side_byte_offset -= side;
            side_byte_offset <<= 7;
            offset += side_byte_offset;
            offset += (s32)battle_unit_bytes;
            model_or_palette = current_command->palette_variant;
            *(u8 *)(offset + 1) = model_or_palette;
        }
        {
            register struct EventBattleUnitModelCommand *current_command asm("r2");
            register s32 unit_slot asm("r0");
            register s32 offset asm("r1");
            register s32 side asm("r2");
            register s32 side_byte_offset asm("r0");

            current_command = *saved_script_cursor;
            unit_slot = current_command->unit_slot;
            offset = unit_slot << 2;
            offset += unit_slot;
            offset <<= 3;
            offset -= unit_slot;
            offset <<= 4;
            side = current_command->side;
            side_byte_offset = side << 2;
            side_byte_offset += side;
            side_byte_offset <<= 3;
            side_byte_offset -= side;
            side_byte_offset <<= 7;
            offset += side_byte_offset;
            offset += (s32)battle_unit_bytes;
            *(u16 *)(offset + 4) = sprite_entry_mode;
        }
    }

    {
        register s32 minus_one asm("r1");

        minus_one = -1;
        SeekEventCommand(saved_script_slot, minus_one, 0);
    }
    return 0;
}
