#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
#include "../../battle/battle_display.h"

void ConfigureBattleUnitSprites(s32, s32, s32, s32, s32, s32, s32) asm("func_080BAF2C");
void SeekEventCommand(s32, s32, s32) asm("func_080A016C");

s32 EventSetBattleSideModelsFromPlayerTeam(s32 script_slot, struct EventBattlePlayerTeamModelsCommand **script_cursor) asm("func_080A2A58");

s32 EventSetBattleSideModelsFromPlayerTeam(s32 script_slot, struct EventBattlePlayerTeamModelsCommand **script_cursor) {
    struct EventBattlePlayerTeamModelsCommand **saved_script_cursor;
    volatile s32 saved_script_slot;
    u32 unit_slot;
    register struct EventPlayerZoidSlotView *player_zoid_slots asm("r8");
    register u8 *battle_unit_bytes asm("r10");
    register s32 unit_byte_offset asm("r9");
    register u8 *battle_unit_bytes_initial asm("r1");

    saved_script_cursor = script_cursor;
    script_slot <<= 24;
    saved_script_slot = (u32)script_slot >> 24;
    unit_slot = 0;
    player_zoid_slots = (struct EventPlayerZoidSlotView *)EVENT_PLAYER_STATE_RAM;
    battle_unit_bytes_initial = (u8 *)0x02034B4C;
    asm volatile("" : "+r"(battle_unit_bytes_initial));
    battle_unit_bytes = battle_unit_bytes_initial;
    asm volatile("" : "+r"(unit_slot));
    unit_byte_offset = unit_slot;

slot_loop:
    {
        register s32 team_slots_address asm("r0");
        register u8 *team_slot_address asm("r1");
        register s32 stored_zoid_slot asm("r0");

        team_slots_address = 0x690C;
        team_slots_address += (s32)player_zoid_slots;
        team_slot_address = (u8 *)(unit_slot + team_slots_address);
        stored_zoid_slot = *team_slot_address;
        if (stored_zoid_slot == 0) {
            goto next_slot;
        }
        {
            register struct EventBattlePlayerTeamModelsCommand *command asm("r3");
            register s32 sprite_entry_mode asm("r5");

            command = *saved_script_cursor;
            sprite_entry_mode = command->sprite_entry_mode;
            switch (sprite_entry_mode) {
            case EVENT_BATTLE_SPRITES_AT_FORMATION: {
                register s32 stored_zoid_byte_offset asm("r2");
                register struct EventPlayerZoidSlotView *stored_zoid asm("r2");
                register s32 model_id asm("r0");
                register s32 palette_variant asm("r1");
                register s32 size_class asm("r2");

                stored_zoid_slot = *team_slot_address;
                stored_zoid_byte_offset = stored_zoid_slot << 3;
                stored_zoid_byte_offset -= stored_zoid_slot;
                stored_zoid_byte_offset <<= 4;
                stored_zoid_byte_offset += (s32)player_zoid_slots;
                stored_zoid = (struct EventPlayerZoidSlotView *)stored_zoid_byte_offset;
                model_id = stored_zoid->model_id;
                palette_variant = stored_zoid->palette_variant;
                stored_zoid = (struct EventPlayerZoidSlotView *)((s32)stored_zoid + 0x3C);
                size_class = *(u8 *)stored_zoid;
                ConfigureBattleUnitSprites(model_id,
                              palette_variant,
                              size_class,
                              command->side,
                              unit_slot,
                              sprite_entry_mode,
                              sprite_entry_mode);
                break;
            }
            case EVENT_BATTLE_SPRITES_ENTER_FROM_SIDE: {
                s32 side;
                s32 sprite_motion_address;

                side = command->side;
                if (side == 0) {
                    register s32 stored_zoid_byte_offset asm("r2");
                    register struct EventPlayerZoidSlotView *stored_zoid asm("r2");
                    register s32 model_id asm("r0");
                    register s32 palette_variant asm("r1");
                    register s32 size_class asm("r2");

                    stored_zoid_slot = *team_slot_address;
                    stored_zoid_byte_offset = stored_zoid_slot << 3;
                    stored_zoid_byte_offset -= stored_zoid_slot;
                    stored_zoid_byte_offset <<= 4;
                    stored_zoid_byte_offset += (s32)player_zoid_slots;
                    stored_zoid = (struct EventPlayerZoidSlotView *)stored_zoid_byte_offset;
                    model_id = stored_zoid->model_id;
                    palette_variant = stored_zoid->palette_variant;
                    stored_zoid = (struct EventPlayerZoidSlotView *)((s32)stored_zoid + 0x3C);
                    size_class = *(u8 *)stored_zoid;
                    asm volatile("" : "+r"(side));
                    ConfigureBattleUnitSprites(model_id,
                                  palette_variant,
                                  size_class,
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
                    register s32 stored_zoid_byte_offset asm("r2");
                    register struct EventPlayerZoidSlotView *stored_zoid asm("r2");
                    register s32 model_id asm("r0");
                    register s32 palette_variant asm("r1");
                    register s32 size_class asm("r2");

                    stored_zoid_slot = *team_slot_address;
                    stored_zoid_byte_offset = stored_zoid_slot << 3;
                    stored_zoid_byte_offset -= stored_zoid_slot;
                    stored_zoid_byte_offset <<= 4;
                    stored_zoid_byte_offset += (s32)player_zoid_slots;
                    stored_zoid = (struct EventPlayerZoidSlotView *)stored_zoid_byte_offset;
                    model_id = stored_zoid->model_id;
                    palette_variant = stored_zoid->palette_variant;
                    stored_zoid = (struct EventPlayerZoidSlotView *)((s32)stored_zoid + 0x3C);
                    size_class = *(u8 *)stored_zoid;
                    ConfigureBattleUnitSprites(model_id,
                                  palette_variant,
                                  size_class,
                                  1,
                                  unit_slot,
                                  -0x10000,
                                  0);
                    {
                        register s32 marker_nonzero asm("r1");

                        marker_nonzero = 0x02032EF2;
                        asm volatile("" : "+r"(marker_nonzero));
                        sprite_motion_address = unit_slot + marker_nonzero;
                    }
                }
                *(u8 *)sprite_motion_address = sprite_entry_mode;
                break;
            }
            }

            {
                register u8 *team_slot_reload_address asm("r3");

                {
                    register struct EventBattlePlayerTeamModelsCommand *command asm("r0");
                    register s32 side asm("r0");
                    register s32 address asm("r1");
                    register s32 team_slots_address asm("r3");
                    register s32 stored_zoid_slot_value asm("r2");
                    register s32 stored_zoid_byte_offset asm("r0");
                    register s32 palette_variant asm("r0");

                    command = *saved_script_cursor;
                    side = command->side;
                    address = side << 2;
                    address += side;
                    address <<= 3;
                    address -= side;
                    address <<= 7;
                    address += unit_byte_offset;
                    address += (s32)battle_unit_bytes;
                    team_slots_address = 0x690C;
                    team_slots_address += (s32)player_zoid_slots;
                    team_slot_reload_address = (u8 *)(unit_slot + team_slots_address);
                    stored_zoid_slot_value = *team_slot_reload_address;
                    stored_zoid_byte_offset = stored_zoid_slot_value << 3;
                    stored_zoid_byte_offset -= stored_zoid_slot_value;
                    stored_zoid_byte_offset <<= 4;
                    stored_zoid_byte_offset += (s32)player_zoid_slots;
                    palette_variant = ((struct EventPlayerZoidSlotView *)stored_zoid_byte_offset)->model_id;
                    *(u8 *)address = palette_variant;
                }
                {
                    register struct EventBattlePlayerTeamModelsCommand *command asm("r0");
                    register s32 side asm("r0");
                    register s32 address asm("r1");
                    register s32 stored_zoid_slot_value asm("r2");
                    register s32 stored_zoid_byte_offset asm("r0");
                    register s32 palette_variant asm("r0");

                    command = *saved_script_cursor;
                    side = command->side;
                    address = side << 2;
                    address += side;
                    address <<= 3;
                    address -= side;
                    address <<= 7;
                    address += unit_byte_offset;
                    address += (s32)battle_unit_bytes;
                    stored_zoid_slot_value = *team_slot_reload_address;
                    stored_zoid_byte_offset = stored_zoid_slot_value << 3;
                    stored_zoid_byte_offset -= stored_zoid_slot_value;
                    stored_zoid_byte_offset <<= 4;
                    stored_zoid_byte_offset += (s32)player_zoid_slots;
                    palette_variant = ((struct EventPlayerZoidSlotView *)stored_zoid_byte_offset)->palette_variant;
                    *(u8 *)(address + 1) = palette_variant;
                }
                {
                    register struct EventBattlePlayerTeamModelsCommand *command asm("r0");
                    register s32 side asm("r1");
                    register s32 address asm("r0");
                    register s32 zero asm("r3");

                    command = *saved_script_cursor;
                    side = command->side;
                    address = side << 2;
                    address += side;
                    address <<= 3;
                    address -= side;
                    address <<= 7;
                    address += unit_byte_offset;
                    address += (s32)battle_unit_bytes;
                    zero = 0;
                    *(u16 *)(address + 4) = zero;
                }
            }
        }
    }

next_slot:
    {
        register s32 increment asm("r0");

        increment = 0x9C;
        increment <<= 2;
        unit_byte_offset += increment;
    }
    unit_slot++;
    if (unit_slot <= 5) {
        goto slot_loop;
    }

    SeekEventCommand(saved_script_slot, -1, 0);
    return 0;
}
