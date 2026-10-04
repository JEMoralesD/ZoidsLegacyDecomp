#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
#include "../../battle/battle_display.h"

void ConfigureBattleUnitSprites(s32, s32, s32, s32, s32, s32, s32) asm("func_080BAF2C");
void SeekEventCommand(s32, s32, s32) asm("func_080A016C");

s32 EventSetBattleFuzorComponentModels(s32 script_slot, struct EventBattleFuzorModelsCommand **script_cursor) asm("func_080A2D40");

s32 EventSetBattleFuzorComponentModels(s32 script_slot, struct EventBattleFuzorModelsCommand **script_cursor) {
    struct EventBattleFuzorModelsCommand **saved_script_cursor;
    volatile s32 saved_script_slot;
    u32 unit_slot;
    register u32 next_unit_slot asm("r10");

    saved_script_cursor = script_cursor;
    script_slot <<= 24;
    saved_script_slot = (u32)script_slot >> 24;
    unit_slot = 0;
outer_loop:
    {
        register u8 *component_models asm("r0");
        register u32 model_id asm("r0");
        register u32 next_unit_slot_value asm("r1");

        component_models = (u8 *)EVENT_FUZOR_COMPONENT_MODELS_ROM;
        asm volatile("" : "+r"(component_models));
        model_id = *(u8 *)(unit_slot + (s32)component_models);
        next_unit_slot_value = unit_slot + 1;
        asm volatile("" : "+r"(next_unit_slot_value));
        next_unit_slot = next_unit_slot_value;
        if (model_id != 0) {
            u32 stored_zoid_slot;
            register struct EventPlayerZoidSlotView *player_zoid_slots asm("r4");
            register s32 zero asm("r9");
            register u8 *battle_unit_bytes asm("r8");
            s32 stored_zoid_byte_offset;

            stored_zoid_slot = 0;
            player_zoid_slots = (struct EventPlayerZoidSlotView *)EVENT_PLAYER_STATE_RAM;
            zero = 0;
            battle_unit_bytes = (u8 *)0x02034B4C;
            stored_zoid_byte_offset = 0;
inner_loop:
            {
                register struct EventPlayerZoidSlotView *stored_zoid asm("r2");

                stored_zoid = (struct EventPlayerZoidSlotView *)
                    (stored_zoid_byte_offset + (s32)player_zoid_slots);
                if (stored_zoid->model_id == ((u8 *)EVENT_FUZOR_COMPONENT_MODELS_ROM)[unit_slot] &&
                    (stored_zoid->flags & EVENT_FUZOR_COMPONENT_REQUIRED_FLAG) != 0) {
                    s32 sprite_entry_mode;

                    sprite_entry_mode = (*saved_script_cursor)->sprite_entry_mode;
                    switch (sprite_entry_mode) {
                    case EVENT_BATTLE_SPRITES_AT_FORMATION:
                        ConfigureBattleUnitSprites(stored_zoid->model_id,
                                      stored_zoid->palette_variant,
                                      stored_zoid->size_class,
                                      0,
                                      unit_slot,
                                      sprite_entry_mode,
                                      sprite_entry_mode);
                        break;
                    case EVENT_BATTLE_SPRITES_ENTER_FROM_SIDE:
                        ConfigureBattleUnitSprites(stored_zoid->model_id,
                                      stored_zoid->palette_variant,
                                      stored_zoid->size_class,
                                      0,
                                      unit_slot,
                                      0x10000,
                                      zero);
                        {
                            register u8 *marker asm("r0");

                            marker = (u8 *)EVENT_BATTLE_SPRITE_MOTION_RAM;
                            asm volatile("" : "+r"(marker));
                            marker = (u8 *)(unit_slot + (s32)marker);
                            *marker = sprite_entry_mode;
                        }
                        break;
                    }

                    {
                        register s32 unit_byte_offset asm("r2");
                        register struct EventBattleFuzorModelsCommand *command asm("r0");
                        register s32 side asm("r1");
                        register s32 side_byte_offset asm("r0");
                        register s32 address asm("r0");
                        register struct EventPlayerZoidSlotView *matched_zoid asm("r3");
                        register s32 model_or_palette_variant asm("r1");

                        unit_byte_offset = unit_slot << 2;
                        unit_byte_offset += unit_slot;
                        unit_byte_offset <<= 3;
                        unit_byte_offset -= unit_slot;
                        unit_byte_offset <<= 4;
                        command = *saved_script_cursor;
                        side = EVENT_FUZOR_DESTINATION_SIDE(command);
                        side_byte_offset = side << 2;
                        side_byte_offset += side;
                        side_byte_offset <<= 3;
                        side_byte_offset -= side;
                        side_byte_offset <<= 7;
                        address = unit_byte_offset + side_byte_offset;
                        address += (s32)battle_unit_bytes;
                        matched_zoid = (struct EventPlayerZoidSlotView *)EVENT_PLAYER_STATE_RAM;
                        asm volatile("" : "+r"(matched_zoid));
                        matched_zoid = (struct EventPlayerZoidSlotView *)
                            (stored_zoid_byte_offset + (s32)matched_zoid);
                        model_or_palette_variant = matched_zoid->model_id;
                        *(u8 *)address = model_or_palette_variant;

                        command = *saved_script_cursor;
                        side = EVENT_FUZOR_DESTINATION_SIDE(command);
                        side_byte_offset = side << 2;
                        side_byte_offset += side;
                        side_byte_offset <<= 3;
                        side_byte_offset -= side;
                        side_byte_offset <<= 7;
                        address = unit_byte_offset + side_byte_offset;
                        address += (s32)battle_unit_bytes;
                        model_or_palette_variant = matched_zoid->palette_variant;
                        *(u8 *)(address + 1) = model_or_palette_variant;

                        command = *saved_script_cursor;
                        side = EVENT_FUZOR_DESTINATION_SIDE(command);
                        side_byte_offset = side << 2;
                        side_byte_offset += side;
                        side_byte_offset <<= 3;
                        side_byte_offset -= side;
                        side_byte_offset <<= 7;
                        unit_byte_offset += side_byte_offset;
                        unit_byte_offset += (s32)battle_unit_bytes;
                        {
                            register s32 zero_value asm("r0");

                            zero_value = zero;
                            *(u16 *)(unit_byte_offset + 4) = zero_value;
                        }
                    }
                    goto outer_continue;
                }
            }
            stored_zoid_byte_offset += 0x70;
            stored_zoid_slot++;
            if (stored_zoid_slot <= 0xCE) {
                goto inner_loop;
            }
        }
    }
outer_continue:
    unit_slot = next_unit_slot;
    if (unit_slot <= 5) {
        goto outer_loop;
    }
    SeekEventCommand(saved_script_slot, -1, 0);
    return 0;
}
