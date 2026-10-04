#include "m2c_prelude.h"
#include "../game/player_state.h"

extern u8 gPlayerStateBytes[] asm("D_020218E4");
extern u8 gPlayerPilotFlagsBaseOffset[] asm("D_off_00005A96");

s32 AddPlayerZoid(u8, u8) asm("func_080E5A18");
s32 AddOrReactivatePlayerPilot(u8) asm("func_080E6C78");
void AssignPlayerPilotToZoid(s32, s32) asm("func_080E6FA0");

u8 AddTemporaryPlayerZoidWithPilot(u8 zoid_model_id, u8 palette_variant, u8 pilot_id) asm("func_080E6EEC");

u8 AddTemporaryPlayerZoidWithPilot(u8 zoid_model_id, u8 palette_variant, u8 pilot_id) {
    register s32 stored_zoid_slot asm("r6");
    s32 stored_pilot_slot;
    s32 allocation_result;
    u32 allocations_valid;
    s32 zoid_slot_failure_test;
    s32 zoid_slot_valid_bits;
    register u16 *zoid_header_view asm("r0");
    register u8 *player_state_bytes asm("r3");
    u16 zoid_flags;
    u16 temporary_pair_flag;
    u8 created_zoid_slot;

    allocation_result = AddPlayerZoid(zoid_model_id, palette_variant);
    allocation_result <<= 24;
    stored_zoid_slot = (u32)allocation_result >> 24;
    zoid_slot_failure_test = PLAYER_STORAGE_SLOT_NOT_FOUND;
    zoid_slot_failure_test ^= stored_zoid_slot;
    zoid_slot_valid_bits = -zoid_slot_failure_test;
    zoid_slot_valid_bits |= zoid_slot_failure_test;
    allocations_valid = (u32)zoid_slot_valid_bits >> 31;
    allocation_result = AddOrReactivatePlayerPilot(pilot_id);
    stored_pilot_slot = (u32)(allocation_result << 24) >> 24;
    if (stored_pilot_slot == PLAYER_STORAGE_SLOT_NOT_FOUND) {
        allocations_valid = 0;
    }
    if (allocations_valid == 0) {
        created_zoid_slot = 0;
        goto return_created_slot;
    }

    {
        register s32 pilot_slot_argument asm("r0");
        pilot_slot_argument = stored_pilot_slot;
        asm volatile("" : "+r"(pilot_slot_argument));
        AssignPlayerPilotToZoid(pilot_slot_argument, stored_zoid_slot);
    }
    player_state_bytes = gPlayerStateBytes;
    asm volatile("" : "+r"(player_state_bytes));
    zoid_header_view = (u16 *)(stored_zoid_slot * 0x70);
    zoid_header_view = (u16 *)((u8 *)zoid_header_view + (s32)player_state_bytes);
    zoid_header_view[PLAYER_STATE_OFFSET(zoids[0].current_hp) / sizeof(u16)] = zoid_header_view[PLAYER_STATE_OFFSET(zoids[0].max_hp) / sizeof(u16)];
    zoid_header_view[PLAYER_STATE_OFFSET(zoids[0].current_ep) / sizeof(u16)] = zoid_header_view[PLAYER_STATE_OFFSET(zoids[0].max_ep) / sizeof(u16)];
    zoid_flags = zoid_header_view[PLAYER_STATE_OFFSET(zoids[0].flags) / sizeof(u16)];
    temporary_pair_flag = PLAYER_RECORD_TEMPORARY_PAIR;
    zoid_flags |= temporary_pair_flag;
    zoid_header_view[PLAYER_STATE_OFFSET(zoids[0].flags) / sizeof(u16)] = zoid_flags;

    {
        register u16 *pilot_flags_address asm("r0");
        register s32 pilot_flags_base_offset asm("r1");
        pilot_flags_address = (u16 *)(stored_pilot_slot << 6);
        pilot_flags_address = (u16 *)((u8 *)pilot_flags_address + (s32)player_state_bytes);
        asm volatile("" : "+r"(pilot_flags_address));
        asm volatile("" :: "r"(stored_pilot_slot));
        pilot_flags_base_offset = (s32)gPlayerPilotFlagsBaseOffset;
        asm volatile("" : "+r"(pilot_flags_base_offset));
        pilot_flags_address = (u16 *)((u8 *)pilot_flags_address + pilot_flags_base_offset);
        temporary_pair_flag |= *pilot_flags_address;
        *pilot_flags_address = temporary_pair_flag;
    }
    created_zoid_slot = stored_zoid_slot;

return_created_slot:
    return created_zoid_slot;
}
