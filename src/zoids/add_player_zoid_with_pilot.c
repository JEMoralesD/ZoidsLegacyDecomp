#include "m2c_prelude.h"
#include "../game/player_state.h"
extern u8 gPlayerStateBytes[] asm("D_020218E4");
u8 AddPlayerZoid(u8, u8) asm("func_080E5A18");
u8 AddOrReactivatePlayerPilot(u8) asm("func_080E6C78");
M2C_UNK AssignPlayerPilotToZoid(u8, u8) asm("func_080E6FA0");
s32 AddPlayerZoidWithPilot(u8 zoid_model_id, u8 palette_variant, u8 pilot_id) asm("func_080E5C8C");

s32 AddPlayerZoidWithPilot(u8 zoid_model_id, u8 palette_variant, u8 pilot_id) {
    u8 stored_zoid_slot = AddPlayerZoid(zoid_model_id, palette_variant);
    /* Both allocations run before either result is checked. */
    u8 stored_pilot_slot = AddOrReactivatePlayerPilot(pilot_id);
    if ((stored_zoid_slot != PLAYER_STORAGE_SLOT_NOT_FOUND) && (stored_pilot_slot != PLAYER_STORAGE_SLOT_NOT_FOUND)) {
        AssignPlayerPilotToZoid(stored_pilot_slot, stored_zoid_slot);
        {
        register s32 player_state_address asm("r1");
        void *zoid_header_view;
        player_state_address = (s32)gPlayerStateBytes;
        /* This pointer starts four bytes before the Zoid record, matching the native header-based address. */
        zoid_header_view = (void *)(stored_zoid_slot * sizeof(struct PlayerZoidRecordView) + player_state_address);
        M2C_FIELD(zoid_header_view, u16 *, PLAYER_STATE_OFFSET(zoids[0].current_hp)) = M2C_FIELD(zoid_header_view, u16 *, PLAYER_STATE_OFFSET(zoids[0].max_hp));
        M2C_FIELD(zoid_header_view, u16 *, PLAYER_STATE_OFFSET(zoids[0].current_ep)) = M2C_FIELD(zoid_header_view, u16 *, PLAYER_STATE_OFFSET(zoids[0].max_ep));
        }
        return 1;
    }
    return 0;
}
