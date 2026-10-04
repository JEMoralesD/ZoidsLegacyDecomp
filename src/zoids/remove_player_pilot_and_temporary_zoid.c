#include "m2c_prelude.h"
#include "../game/player_state.h"
extern u8 gPlayerPilotRecordsBytes[] asm("D_02027378");
void RemovePlayerZoid(u8) asm("func_080E5C34"); void RemovePlayerPilot(u8) asm("func_080E6E98");
void RemovePlayerPilotAndTemporaryZoid(u8 stored_pilot_slot) asm("func_080E6F6C");

void RemovePlayerPilotAndTemporaryZoid(u8 stored_pilot_slot) {
    s32 pilot_record_offset = stored_pilot_slot << 6;
    u8 *pilot = gPlayerPilotRecordsBytes + pilot_record_offset;
    if (PLAYER_RECORD_TEMPORARY_PAIR & *(u16 *)(pilot + PLAYER_PILOT_OFFSET(flags_and_pilot_id))) {
        RemovePlayerZoid(*(u8 *)(pilot + PLAYER_PILOT_OFFSET(zoid_slot)));
        gPlayerPilotRecordsBytes[pilot_record_offset] = 0;
        return;
    }
    RemovePlayerPilot(stored_pilot_slot);
}
