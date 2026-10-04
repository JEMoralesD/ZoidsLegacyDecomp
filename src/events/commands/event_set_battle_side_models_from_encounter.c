#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
#include "../../battle/battle_display.h"

void ConfigureBattleUnitSprites(s32, s32, s32, s32, s32, s32, s32) asm("func_080BAF2C");
void SeekEventCommand(s32, s32, s32) asm("func_080A016C");

s32 EventSetBattleSideModelsFromEncounter(s32 script_slot, struct EventBattleEncounterModelsCommand **script_cursor) asm("func_080A2BB0");

s32 EventSetBattleSideModelsFromEncounter(s32 script_slot, struct EventBattleEncounterModelsCommand **script_cursor) {
    struct EventBattleEncounterModelsCommand **saved_script_cursor;
    volatile s32 saved_script_slot;
    u32 unit_slot;
    register u8 *lookup_table_base asm("r4");
    register s32 unit_byte_offset asm("r9");
    register s32 encounter_unit_byte_offset asm("r8");
    register u8 *battle_unit_bytes asm("r10");
    register u8 *battle_unit_bytes_initial asm("r0");

    saved_script_cursor = script_cursor;
    script_slot <<= 24;
    saved_script_slot = (u32)script_slot >> 24;
    unit_slot = 0;
    asm volatile("" : "+r"(unit_slot));
    lookup_table_base = (u8 *)EVENT_ENCOUNTER_RECORDS_ROM;
    unit_byte_offset = unit_slot;
    encounter_unit_byte_offset = unit_slot;
    battle_unit_bytes_initial = (u8 *)0x02034B4C;
    asm volatile("" : "+r"(battle_unit_bytes_initial));
    battle_unit_bytes = battle_unit_bytes_initial;

slot_loop:
    {
        register struct EventBattleEncounterModelsCommand *command asm("r3");
        register s32 formation_index asm("r0");
        register s32 encounter_record_bytes asm("r1");
        register s32 address asm("r2");
        register s32 encounter_group asm("r1");
        register s32 side_byte_offset asm("r0");
        register struct EventEncounterModelSlotView *entry asm("r2");
        register s32 model_id asm("r0");

        command = *saved_script_cursor;
        formation_index = command->formation_index;
        asm volatile("" : "+r"(formation_index));
        encounter_record_bytes = 100;
        address = formation_index;
        address *= encounter_record_bytes;
        address += encounter_unit_byte_offset;
        encounter_group = command->encounter_group;
        side_byte_offset = encounter_group << 5;
        side_byte_offset -= encounter_group;
        side_byte_offset <<= 2;
        side_byte_offset += encounter_group;
        side_byte_offset <<= 4;
        address += side_byte_offset;
        address += (s32)lookup_table_base;
        entry = (struct EventEncounterModelSlotView *)address;
        model_id = entry->model_id;
        if (model_id == 0) {
            goto next_slot;
        }

        {
            register s32 sprite_entry_mode asm("r5");

            sprite_entry_mode = command->sprite_entry_mode;
            switch (sprite_entry_mode) {
            case EVENT_BATTLE_SPRITES_AT_FORMATION: {
                register s32 model_id asm("r0");
                register s32 palette_variant asm("r1");
                register s32 size_class_or_table asm("r2");
                register u8 *zoid_base_records asm("r4");
                register s32 side asm("r3");

                model_id = entry->model_id;
                palette_variant = entry->palette_variant;
                size_class_or_table = model_id << 3;
                size_class_or_table -= model_id;
                size_class_or_table <<= 3;
                zoid_base_records = (u8 *)EVENT_ZOID_BASE_RECORDS_ROM;
                asm volatile("" : "+r"(zoid_base_records));
                size_class_or_table += (s32)zoid_base_records;
                size_class_or_table = *(u8 *)(size_class_or_table + 2);
                side = command->side;
                ConfigureBattleUnitSprites(model_id,
                              palette_variant,
                              size_class_or_table,
                              side,
                              unit_slot,
                              sprite_entry_mode,
                              sprite_entry_mode);
                break;
            }
            case EVENT_BATTLE_SPRITES_ENTER_FROM_SIDE: {
                s32 sprite_motion_address;
                register s32 side asm("r4");

                side = command->side;
                if (side == 0) {
                    register s32 model_id asm("r0");
                    register s32 palette_variant asm("r1");
                    register s32 size_class_or_table asm("r2");
                    register u8 *zoid_base_records asm("r3");

                    model_id = entry->model_id;
                    palette_variant = entry->palette_variant;
                    size_class_or_table = model_id << 3;
                    size_class_or_table -= model_id;
                    size_class_or_table <<= 3;
                    zoid_base_records = (u8 *)EVENT_ZOID_BASE_RECORDS_ROM;
                    asm volatile("" : "+r"(zoid_base_records));
                    size_class_or_table += (s32)zoid_base_records;
                    size_class_or_table = *(u8 *)(size_class_or_table + 2);
                    ConfigureBattleUnitSprites(model_id,
                                  palette_variant,
                                  size_class_or_table,
                                  0,
                                  unit_slot,
                                  0x10000,
                                  side);
                    {
                        register s32 marker_zero asm("r0");

                        marker_zero = EVENT_BATTLE_SPRITE_MOTION_RAM;
                        asm volatile("" : "+r"(marker_zero));
                        sprite_motion_address = unit_slot + marker_zero;
                    }
                } else {
                    register s32 model_id asm("r0");
                    register s32 palette_variant asm("r1");
                    register s32 size_class_or_table asm("r2");
                    register u8 *zoid_base_records asm("r4");

                    model_id = entry->model_id;
                    palette_variant = entry->palette_variant;
                    size_class_or_table = model_id << 3;
                    size_class_or_table -= model_id;
                    size_class_or_table <<= 3;
                    zoid_base_records = (u8 *)EVENT_ZOID_BASE_RECORDS_ROM;
                    asm volatile("" : "+r"(zoid_base_records));
                    size_class_or_table += (s32)zoid_base_records;
                    size_class_or_table = *(u8 *)(size_class_or_table + 2);
                    ConfigureBattleUnitSprites(model_id,
                                  palette_variant,
                                  size_class_or_table,
                                  1,
                                  unit_slot,
                                  -0x10000,
                                  0);
                    {
                        register s32 marker_nonzero asm("r4");

                        marker_nonzero = 0x02032EF2;
                        asm volatile("" : "+r"(marker_nonzero));
                        sprite_motion_address = unit_slot + marker_nonzero;
                    }
                }
                *(u8 *)sprite_motion_address = sprite_entry_mode;
                break;
            }
            }
        }

        {
            register s32 encounter_record_bytes asm("r4");

            {
                register struct EventBattleEncounterModelsCommand *current_command asm("r2");
                register s32 side asm("r0");
                register s32 destination asm("r3");
                register s32 formation_index asm("r0");
                register s32 source_address asm("r1");
                register s32 encounter_group asm("r2");
                register s32 side_byte_offset asm("r0");
                register u8 *base asm("r0");
                register s32 palette_variant asm("r0");

                current_command = *saved_script_cursor;
                side = current_command->side;
                destination = side << 2;
                destination += side;
                destination <<= 3;
                destination -= side;
                destination <<= 7;
                destination += unit_byte_offset;
                destination += (s32)battle_unit_bytes;
                formation_index = current_command->formation_index;
                asm volatile("" : "+r"(formation_index));
                encounter_record_bytes = 100;
                source_address = formation_index;
                source_address *= encounter_record_bytes;
                source_address += encounter_unit_byte_offset;
                encounter_group = current_command->encounter_group;
                side_byte_offset = encounter_group << 5;
                side_byte_offset -= encounter_group;
                side_byte_offset <<= 2;
                side_byte_offset += encounter_group;
                side_byte_offset <<= 4;
                source_address += side_byte_offset;
                base = (u8 *)EVENT_ENCOUNTER_RECORDS_ROM;
                asm volatile("" : "+r"(base));
                source_address += (s32)base;
                palette_variant = ((struct EventEncounterModelSlotView *)source_address)->model_id;
                *(u8 *)destination = palette_variant;
            }
            {
                register struct EventBattleEncounterModelsCommand *current_command asm("r2");
                register s32 side asm("r0");
                register s32 destination asm("r3");
                register s32 formation_index asm("r0");
                register s32 source_address asm("r1");
                register s32 encounter_group asm("r2");
                register s32 side_byte_offset asm("r0");
                register u8 *base asm("r4");
                register s32 palette_variant asm("r0");

                current_command = *saved_script_cursor;
                side = current_command->side;
                destination = side << 2;
                destination += side;
                destination <<= 3;
                destination -= side;
                destination <<= 7;
                destination += unit_byte_offset;
                destination += (s32)battle_unit_bytes;
                formation_index = current_command->formation_index;
                asm volatile("" : "+r"(formation_index));
                source_address = formation_index;
                source_address *= encounter_record_bytes;
                source_address += encounter_unit_byte_offset;
                encounter_group = current_command->encounter_group;
                side_byte_offset = encounter_group << 5;
                side_byte_offset -= encounter_group;
                side_byte_offset <<= 2;
                side_byte_offset += encounter_group;
                side_byte_offset <<= 4;
                source_address += side_byte_offset;
                base = (u8 *)EVENT_ENCOUNTER_RECORDS_ROM;
                asm volatile("" : "+r"(base));
                source_address += (s32)base;
                palette_variant = ((struct EventEncounterModelSlotView *)source_address)->palette_variant;
                *(u8 *)(destination + 1) = palette_variant;
            }
        }
        {
            register struct EventBattleEncounterModelsCommand *current_command asm("r0");
            register s32 side asm("r1");
            register s32 destination asm("r0");
            register s32 zero asm("r1");

            current_command = *saved_script_cursor;
            side = current_command->side;
            destination = side << 2;
            destination += side;
            destination <<= 3;
            destination -= side;
            destination <<= 7;
            destination += unit_byte_offset;
            destination += (s32)battle_unit_bytes;
            zero = 0;
            *(u16 *)(destination + 4) = zero;
        }
    }

next_slot:
    {
        register s32 column_increment asm("r3");
        register s32 entry_increment asm("r0");

        column_increment = 0x9C;
        column_increment <<= 2;
        asm volatile("" : "+r"(column_increment));
        unit_byte_offset += column_increment;
        entry_increment = 0x10;
        encounter_unit_byte_offset += entry_increment;
    }
    unit_slot++;
    if (unit_slot <= 5) {
        goto slot_loop;
    }

    SeekEventCommand(saved_script_slot, -1, 0);
    return 0;
}
