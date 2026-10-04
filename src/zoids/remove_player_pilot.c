#include "m2c_prelude.h"
#include "../game/player_state.h"
extern u8 gPlayerPilotRecordsBytes[] asm("D_02027378");
extern u8 gTeamSlotsFromPilotBaseOffset[] asm("D_off_0E78");
void RemoveZoidFromPlayerTeam(u32) asm("func_080E6020");
void DetachPlayerPilotFromZoid(u8) asm("func_080E700C");
void RemovePlayerPilot(u8 stored_pilot_slot) asm("func_080E6E98");

void RemovePlayerPilot(u8 stored_pilot_slot) {
    s32 pilot_record_offset;
    s32 pilot_or_team_base;
    u8 team_slot_index;
    u8 pilot_slot;
    u8 *pilot;
    pilot_slot = stored_pilot_slot;
    pilot_record_offset = pilot_slot << 6;
    pilot_or_team_base = (s32)gPlayerPilotRecordsBytes;
    pilot = (u8 *)(pilot_record_offset + pilot_or_team_base);
    if (*(u16 *)(pilot + PLAYER_PILOT_OFFSET(flags_and_pilot_id)) & PLAYER_RECORD_IN_TEAM) {
        team_slot_index = 0;
        pilot_or_team_base += (s32)gTeamSlotsFromPilotBaseOffset;
        goto test_team_slot;
next_team_slot:
        team_slot_index = team_slot_index + 1;
test_team_slot:
        if (team_slot_index >= PLAYER_TEAM_SLOT_COUNT) goto detach_and_remove_pilot;
        if (pilot[PLAYER_PILOT_OFFSET(zoid_slot)] != *(u8 *)(team_slot_index + pilot_or_team_base)) goto next_team_slot;
        RemoveZoidFromPlayerTeam(team_slot_index);
    }
detach_and_remove_pilot:
    DetachPlayerPilotFromZoid(pilot_slot);
    *(s8 *)pilot = 0;
}
