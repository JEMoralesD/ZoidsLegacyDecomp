#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
#include "../../battle/battle_display.h"

void ConfigureBattleUnitSprites(s32, s32, s32, s32, s32, s32, s32) asm("func_080BAF2C");
void SeekEventCommand(s32, s32, s32) asm("func_080A016C");

s32 EventSetBattleUnitModelFromPlayerPilot(s32 script_slot, struct EventBattleUnitPilotModelCommand **script_cursor) asm("func_080A2890");

s32 EventSetBattleUnitModelFromPlayerPilot(s32 script_slot, struct EventBattleUnitPilotModelCommand **script_cursor) {
    register struct EventBattleUnitPilotModelCommand *volatile *saved_script_cursor asm("r6");
    register u32 saved_script_slot asm("r9");
    register struct PlayerPilotRecordView *matched_pilot asm("r8");
    u32 pilot_slot;
    struct PlayerPilotRecordView *lookup_table_base;
    s32 pilot_byte_offset;

    saved_script_cursor = script_cursor;
    script_slot <<= 24;
    saved_script_slot = (u32)script_slot >> 24;
    pilot_slot = 1;
    lookup_table_base = (struct PlayerPilotRecordView *)EVENT_PLAYER_PILOTS_RAM;
    pilot_byte_offset = 0x40;
    goto scan_entry;

scan_next:
    pilot_slot++;
    if (pilot_slot > 0x34) {
        goto done;
    }
    pilot_byte_offset = pilot_slot << 6;

scan_entry:
    pilot_byte_offset += (s32)lookup_table_base;
    matched_pilot = (struct PlayerPilotRecordView *)pilot_byte_offset;
    if (matched_pilot->active_pilot_id != (*saved_script_cursor)->pilot_id) {
        goto scan_next;
    }
    if (pilot_slot > 0x34) {
        goto done;
    }
    {
        register struct PlayerPilotRecordView *matched_pilot_check asm("r1");
        register s32 matched_zoid_slot asm("r0");

        matched_pilot_check = matched_pilot;
        asm volatile("" : "+r"(matched_pilot_check));
        matched_zoid_slot = matched_pilot_check->zoid_slot;
        if (matched_zoid_slot == 0) {
            goto done;
        }
    }

    {
        register struct EventBattleUnitPilotModelCommand *command asm("r4");
        register s32 sprite_entry_mode asm("r5");

        command = *saved_script_cursor;
        sprite_entry_mode = command->sprite_entry_mode;
        switch (sprite_entry_mode) {
        case EVENT_BATTLE_SPRITES_AT_FORMATION: {
            register struct EventPlayerZoidSlotView *player_zoid_slots asm("r2");
            register struct PlayerPilotRecordView *entry asm("r1");
            register s32 zoid_slot asm("r0");
            register s32 stored_zoid_address asm("r1");
            register u32 model_id asm("r0");
            register u32 palette_variant asm("r1");
            register s32 size_class_or_offset asm("r2");
            register u8 *size_class asm("r3");

            player_zoid_slots = (struct EventPlayerZoidSlotView *)EVENT_PLAYER_STATE_RAM;
            asm volatile("" : "+r"(player_zoid_slots));
            entry = matched_pilot;
            zoid_slot = entry->zoid_slot;
            stored_zoid_address = zoid_slot << 3;
            stored_zoid_address -= zoid_slot;
            stored_zoid_address <<= 4;
            stored_zoid_address += (s32)player_zoid_slots;
            model_id = ((struct EventPlayerZoidSlotView *)stored_zoid_address)->model_id;
            palette_variant = ((struct EventPlayerZoidSlotView *)stored_zoid_address)->palette_variant;
            size_class = (u8 *)EVENT_ZOID_BASE_RECORDS_ROM;
            asm volatile("" : "+r"(size_class));
            size_class_or_offset = model_id << 3;
            size_class_or_offset -= model_id;
            size_class_or_offset <<= 3;
            size_class_or_offset += (s32)size_class;
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
            s32 side;
            register s32 flag_left asm("r0");
            register s32 flag_right asm("r1");

            side = command->side;
            if (side == 0) {
                register struct EventPlayerZoidSlotView *player_zoid_slots asm("r2");
                register struct PlayerPilotRecordView *entry asm("r1");
                register s32 zoid_slot asm("r0");
                register s32 stored_zoid_address asm("r1");
                register u32 model_id asm("r0");
                register u32 palette_variant asm("r1");
                register s32 size_class_or_offset asm("r2");
                register u8 *size_class asm("r3");

                asm volatile("" : "+r"(side));
                player_zoid_slots = (struct EventPlayerZoidSlotView *)EVENT_PLAYER_STATE_RAM;
                asm volatile("" : "+r"(player_zoid_slots));
                entry = matched_pilot;
                zoid_slot = entry->zoid_slot;
                stored_zoid_address = zoid_slot << 3;
                stored_zoid_address -= zoid_slot;
                stored_zoid_address <<= 4;
                stored_zoid_address += (s32)player_zoid_slots;
                model_id = ((struct EventPlayerZoidSlotView *)stored_zoid_address)->model_id;
                palette_variant = ((struct EventPlayerZoidSlotView *)stored_zoid_address)->palette_variant;
                size_class = (u8 *)EVENT_ZOID_BASE_RECORDS_ROM;
                asm volatile("" : "+r"(size_class));
                size_class_or_offset = model_id << 3;
                size_class_or_offset -= model_id;
                size_class_or_offset <<= 3;
                size_class_or_offset += (s32)size_class;
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
                flag_left = ((struct EventBattleUnitPilotModelCommand *)flag_left)->unit_slot;
            } else {
                register struct EventPlayerZoidSlotView *player_zoid_slots asm("r2");
                register struct PlayerPilotRecordView *entry asm("r1");
                register s32 zoid_slot asm("r0");
                register s32 stored_zoid_address asm("r1");
                register u32 model_id asm("r0");
                register u32 palette_variant asm("r1");
                register s32 size_class_or_offset asm("r2");
                register u8 *size_class asm("r3");

                player_zoid_slots = (struct EventPlayerZoidSlotView *)EVENT_PLAYER_STATE_RAM;
                asm volatile("" : "+r"(player_zoid_slots));
                entry = matched_pilot;
                zoid_slot = entry->zoid_slot;
                stored_zoid_address = zoid_slot << 3;
                stored_zoid_address -= zoid_slot;
                stored_zoid_address <<= 4;
                stored_zoid_address += (s32)player_zoid_slots;
                model_id = ((struct EventPlayerZoidSlotView *)stored_zoid_address)->model_id;
                palette_variant = ((struct EventPlayerZoidSlotView *)stored_zoid_address)->palette_variant;
                size_class = (u8 *)EVENT_ZOID_BASE_RECORDS_ROM;
                asm volatile("" : "+r"(size_class));
                size_class_or_offset = model_id << 3;
                size_class_or_offset -= model_id;
                size_class_or_offset <<= 3;
                size_class_or_offset += (s32)size_class;
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
                flag_right = ((struct EventBattleUnitPilotModelCommand *)flag_right)->unit_slot;
            }
            flag_left += flag_right;
            *(u8 *)flag_left = sprite_entry_mode;
            break;
        }
        }

        {
            register u8 *battle_unit_bytes asm("r3");
            register struct EventPlayerZoidSlotView *player_zoid_slots asm("r4");

            battle_unit_bytes = (u8 *)0x02034B4C;
            {
                register struct EventBattleUnitPilotModelCommand *current_command asm("r2");
                register s32 unit_slot asm("r0");
                register s32 offset asm("r1");
                register s32 side_value asm("r2");
                register s32 side_byte_offset asm("r0");
                register struct PlayerPilotRecordView *entry asm("r0");
                register s32 zoid_slot asm("r2");
                register s32 stored_zoid_byte_offset asm("r0");
                register s32 model_or_palette_variant asm("r0");

                current_command = *saved_script_cursor;
                unit_slot = current_command->unit_slot;
                offset = unit_slot << 2;
                offset += unit_slot;
                offset <<= 3;
                offset -= unit_slot;
                offset <<= 4;
                side_value = current_command->side;
                side_byte_offset = side_value << 2;
                side_byte_offset += side_value;
                side_byte_offset <<= 3;
                side_byte_offset -= side_value;
                side_byte_offset <<= 7;
                offset += side_byte_offset;
                offset += (s32)battle_unit_bytes;
                player_zoid_slots = (struct EventPlayerZoidSlotView *)EVENT_PLAYER_STATE_RAM;
                entry = matched_pilot;
                zoid_slot = entry->zoid_slot;
                stored_zoid_byte_offset = zoid_slot << 3;
                stored_zoid_byte_offset -= zoid_slot;
                stored_zoid_byte_offset <<= 4;
                stored_zoid_byte_offset += (s32)player_zoid_slots;
                model_or_palette_variant = ((struct EventPlayerZoidSlotView *)stored_zoid_byte_offset)->model_id;
                sprite_entry_mode = 0;
                *(u8 *)offset = model_or_palette_variant;
            }
            {
                register struct EventBattleUnitPilotModelCommand *current_command asm("r2");
                register s32 unit_slot asm("r0");
                register s32 offset asm("r1");
                register s32 side_value asm("r2");
                register s32 side_byte_offset asm("r0");
                register struct PlayerPilotRecordView *entry asm("r0");
                register s32 zoid_slot asm("r2");
                register s32 stored_zoid_byte_offset asm("r0");
                register s32 model_or_palette_variant asm("r0");

                current_command = *saved_script_cursor;
                unit_slot = current_command->unit_slot;
                offset = unit_slot << 2;
                offset += unit_slot;
                offset <<= 3;
                offset -= unit_slot;
                offset <<= 4;
                side_value = current_command->side;
                side_byte_offset = side_value << 2;
                side_byte_offset += side_value;
                side_byte_offset <<= 3;
                side_byte_offset -= side_value;
                side_byte_offset <<= 7;
                offset += side_byte_offset;
                offset += (s32)battle_unit_bytes;
                entry = matched_pilot;
                zoid_slot = entry->zoid_slot;
                stored_zoid_byte_offset = zoid_slot << 3;
                stored_zoid_byte_offset -= zoid_slot;
                stored_zoid_byte_offset <<= 4;
                stored_zoid_byte_offset += (s32)player_zoid_slots;
                model_or_palette_variant = ((struct EventPlayerZoidSlotView *)stored_zoid_byte_offset)->palette_variant;
                *(u8 *)(offset + 1) = model_or_palette_variant;
            }
            {
                register struct EventBattleUnitPilotModelCommand *current_command asm("r2");
                register s32 unit_slot asm("r0");
                register s32 offset asm("r1");
                register s32 side_value asm("r2");
                register s32 side_byte_offset asm("r0");

                current_command = *saved_script_cursor;
                unit_slot = current_command->unit_slot;
                offset = unit_slot << 2;
                offset += unit_slot;
                offset <<= 3;
                offset -= unit_slot;
                offset <<= 4;
                side_value = current_command->side;
                side_byte_offset = side_value << 2;
                side_byte_offset += side_value;
                side_byte_offset <<= 3;
                side_byte_offset -= side_value;
                side_byte_offset <<= 7;
                offset += side_byte_offset;
                offset += (s32)battle_unit_bytes;
                *(u16 *)(offset + 4) = sprite_entry_mode;
            }
        }
    }

done:
    {
        register s32 minus_one asm("r1");

        minus_one = -1;
        SeekEventCommand(saved_script_slot, minus_one, 0);
    }
    return 0;
}
