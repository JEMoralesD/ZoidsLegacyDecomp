#include "m2c_prelude.h"
#include "../game/player_state.h"
extern void RecalculateZoidStats(void *, void *) asm("func_80E5880");
extern void DetachPlayerPilotFromZoid(u8) asm("func_080E700C");


extern struct PlayerZoidRecordView gPlayerZoidRecords[] asm("D_020218E8");

void AssignPlayerPilotToZoid(u8 stored_pilot_slot, u8 stored_zoid_slot) asm("func_080E6FA0");

void AssignPlayerPilotToZoid(u8 stored_pilot_slot, u8 stored_zoid_slot) {
    struct PlayerZoidRecordView *zoid = &gPlayerZoidRecords[stored_zoid_slot];
    struct PlayerPilotRecordView *pilot = (struct PlayerPilotRecordView *)((char *)gPlayerZoidRecords + PLAYER_STATE_OFFSET(pilots) - PLAYER_STATE_OFFSET(zoids)) + stored_pilot_slot;
    u16 saved_current_hp = zoid->current_hp;
    DetachPlayerPilotFromZoid(stored_pilot_slot);
    if (zoid->pilot_slot != 0) {
        DetachPlayerPilotFromZoid(zoid->pilot_slot);
    }
    zoid->pilot_slot = stored_pilot_slot;
    pilot->zoid_slot = stored_zoid_slot;
    zoid->current_hp = saved_current_hp;
    RecalculateZoidStats(zoid, pilot);
    if (PLAYER_RECORD_IN_TEAM & zoid->flags) {
        pilot->flags_and_pilot_id |= PLAYER_RECORD_IN_TEAM;
    }
}
