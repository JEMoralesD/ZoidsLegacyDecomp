#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
extern u8 gPlayerState[] asm("D_020218E4");
s32 SeekEventCommand(u8, s32, s32) asm("func_080A016C");
s32 IsPlayerCombinationFormationValid(s32, s32) asm("func_080C1070");

s32 EventIfPlayerTeamHasPilot(u8 script_slot, void **script_cursor) asm("func_080A103C");

s32 EventIfPlayerTeamHasPilot(u8 script_slot, void **script_cursor) {
    u8 saved_script_slot;
    u8 requested_pilot_id;
    u8 *player_state;
    u8 *team_slots;
    u32 team_slot_index;

    saved_script_slot = script_slot;
    team_slot_index = 0;
    player_state = gPlayerState;
    team_slots = player_state + PLAYER_STATE_OFFSET(team_zoid_slots);
    requested_pilot_id = *((u8 *)*script_cursor + 1);
    do {
        u8 zoid_storage_slot = *(u8 *)(team_slot_index + (u32)team_slots);
        u8 pilot_storage_slot = *(u8 *)(player_state + zoid_storage_slot * 0x70 + 6);
        u8 pilot_id = *(u8 *)(player_state + pilot_storage_slot * 0x40 + 0x5A94);
        if (pilot_id == requested_pilot_id) {
            break;
        }
        team_slot_index += 1;
    } while (team_slot_index <= 5);
    if (team_slot_index <= 5) {
        SeekEventCommand(saved_script_slot, 0xF, 0);
    } else {
        SeekEventCommand(saved_script_slot, 0x10, 0);
    }
    return 0;
}

s32 EventIfPlayerCombinationFormationValid(u8 script_slot, void **script_cursor) asm("func_080A10A0");

s32 EventIfPlayerCombinationFormationValid(u8 script_slot, void **script_cursor) {
    u8 saved_script_slot;

    saved_script_slot = script_slot;
    switch (*((u8 *)*script_cursor + 1)) {
    case 0x47:
        if ((IsPlayerCombinationFormationValid(4, 0) != 0) || (IsPlayerCombinationFormationValid(4, 1) != 0) || (IsPlayerCombinationFormationValid(4, 2) != 0)) {
            SeekEventCommand(saved_script_slot, 0xF, 0);
        } else {
            SeekEventCommand(saved_script_slot, 0x10, 0);
        }
        break;
    case 0x76:
        if (IsPlayerCombinationFormationValid(5, 1) != 0) {
            SeekEventCommand(saved_script_slot, 0xF, 0);
        } else {
            SeekEventCommand(saved_script_slot, 0x10, 0);
        }
        break;
    case 0x80:
        if ((IsPlayerCombinationFormationValid(1, 0) != 0) || (IsPlayerCombinationFormationValid(1, 1) != 0)) {
            SeekEventCommand(saved_script_slot, 0xF, 0);
        } else {
            SeekEventCommand(saved_script_slot, 0x10, 0);
        }
        break;
    case 0x81:
        if ((IsPlayerCombinationFormationValid(2, 0) != 0) || (IsPlayerCombinationFormationValid(2, 1) != 0)) {
            SeekEventCommand(saved_script_slot, 0xF, 0);
        } else {
            SeekEventCommand(saved_script_slot, 0x10, 0);
        }
        break;
    case 0x82:
        if (IsPlayerCombinationFormationValid(3, 0) != 0) {
            SeekEventCommand(saved_script_slot, 0xF, 0);
        } else {
            SeekEventCommand(saved_script_slot, 0x10, 0);
        }
        break;
    case 0x83:
        if ((IsPlayerCombinationFormationValid(0, 0) != 0) || (IsPlayerCombinationFormationValid(0, 1) != 0) || (IsPlayerCombinationFormationValid(0, 3) != 0) || (IsPlayerCombinationFormationValid(0, 4) != 0)) {
            SeekEventCommand(saved_script_slot, 0xF, 0);
        } else {
            SeekEventCommand(saved_script_slot, 0x10, 0);
        }
        break;
    case 0x84:
        if ((IsPlayerCombinationFormationValid(6, 3) != 0) || (IsPlayerCombinationFormationValid(6, 4) != 0) || (IsPlayerCombinationFormationValid(6, 5) != 0)) {
            SeekEventCommand(saved_script_slot, 0xF, 0);
        } else {
            SeekEventCommand(saved_script_slot, 0x10, 0);
        }
        break;
    case 0x97:
        if ((IsPlayerCombinationFormationValid(7, 0) != 0) || (IsPlayerCombinationFormationValid(7, 1) != 0) || (IsPlayerCombinationFormationValid(7, 3) != 0) || (IsPlayerCombinationFormationValid(7, 4) != 0)) {
            SeekEventCommand(saved_script_slot, 0xF, 0);
        } else {
            SeekEventCommand(saved_script_slot, 0x10, 0);
        }
        break;
    default:
        SeekEventCommand(saved_script_slot, 0x10, 0);
        break;
    }
    return 0;
}
