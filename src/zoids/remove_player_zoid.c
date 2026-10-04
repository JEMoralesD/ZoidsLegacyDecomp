#include "m2c_prelude.h"
#include "../game/player_state.h"
extern u8 gPlayerZoidRecordsBytes[] asm("D_020218E8");
extern u8 gPlayerTeamSlotsFromZoidBaseOffset[] asm("D_off_6908");
extern u8 gPlayerStateBytes[] asm("D_020218E4");
void RemoveZoidFromPlayerTeam(u32) asm("func_080E6020");
void DetachPlayerPilotFromZoid(void) asm("func_080E700C");
void RemovePlayerZoid(u8 stored_zoid_slot) asm("func_080E5C34");

void RemovePlayerZoid(u8 stored_zoid_slot) {
    register u8 zoid_slot_index asm("r3");
    register s32 zoid_or_team_base asm("r1");
    s32 zoid_record_offset;
    u32 team_slot_index;
    u8 *zoid_record;
    zoid_slot_index = stored_zoid_slot;
    zoid_record_offset = zoid_slot_index * sizeof(struct PlayerZoidRecordView);
    zoid_or_team_base = (s32)gPlayerZoidRecordsBytes;
    zoid_record = (u8 *)(zoid_record_offset + zoid_or_team_base);
    team_slot_index = 0;
    zoid_or_team_base += (s32)gPlayerTeamSlotsFromZoidBaseOffset;
    goto check_team_slot;
next_team_slot:
    team_slot_index = (u32) (u8) (team_slot_index + 1);
check_team_slot:
    if (team_slot_index >= PLAYER_TEAM_SLOT_COUNT) goto remove_zoid_record;
    if (*(u8 *)(team_slot_index + zoid_or_team_base) != zoid_slot_index) goto next_team_slot;
    RemoveZoidFromPlayerTeam(team_slot_index);
remove_zoid_record:
    if (M2C_FIELD(zoid_record, u8 *, PLAYER_ZOID_OFFSET(pilot_slot)) != 0) {
        /* Native R0 carries the loaded pilot slot through this partial call signature. */
        DetachPlayerPilotFromZoid();
    }
    M2C_FIELD(zoid_record, s8 *, PLAYER_ZOID_OFFSET(model_id)) = 0;
    gPlayerStateBytes[PLAYER_STATE_OFFSET(stored_zoid_count)] = gPlayerStateBytes[PLAYER_STATE_OFFSET(stored_zoid_count)] - 1;
}
