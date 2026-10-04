#include "m2c_prelude.h"
#include "../game/player_state.h"

s32 RemoveZoidFromPlayerTeam(s32 team_slot_index) asm("func_080E6020");

s32 RemoveZoidFromPlayerTeam(s32 team_slot_index) {
    register u32 normalized_team_slot asm("r0");
    register u32 team_slot asm("r6");
    register u8 *player_state_bytes asm("r4");
    register s32 team_slots_offset asm("r1");
    register u8 *team_slots_address asm("r0");
    register u8 *team_slot_address asm("r3");
    register s32 zoid_or_pilot_slot asm("r0");

    normalized_team_slot = team_slot_index << 24;
    normalized_team_slot >>= 24;
    asm volatile("" : "+r"(normalized_team_slot));
    team_slot = normalized_team_slot;
    player_state_bytes = (u8 *)0x020218E4;
    team_slots_offset = PLAYER_STATE_OFFSET(team_zoid_slots);
    asm volatile("" : "+r"(player_state_bytes));
    asm volatile("" : "+r"(team_slots_offset));
    team_slots_address = (u8 *)((s32)player_state_bytes + team_slots_offset);
    asm volatile("" : "+r"(team_slots_address));
    team_slot_address = (u8 *)(team_slot + (s32)team_slots_address);
    zoid_or_pilot_slot = *team_slot_address;
    if (zoid_or_pilot_slot != 0) {
        register s32 zoid_slot_copy asm("r1");
        register s32 zoid_record_offset asm("r0");
        register u8 *zoid_records_base asm("r1");
        register u8 *zoid asm("r2");
        register s32 zoid_flags asm("r1");
        register s32 clear_in_team_mask asm("r5");
        register s32 updated_zoid_flags asm("r0");
        s32 zero_state;

        zoid_slot_copy = zoid_or_pilot_slot;
        zoid_record_offset = zoid_slot_copy << 3;
        zoid_record_offset -= zoid_slot_copy;
        zoid_record_offset <<= 4;
        zoid_records_base = player_state_bytes + 4;
        zoid = (u8 *)(zoid_record_offset + (s32)zoid_records_base);
        zoid_flags = *(u16 *)(zoid + PLAYER_ZOID_OFFSET(flags));
        clear_in_team_mask = 0xFFFB;
        updated_zoid_flags = clear_in_team_mask;
        updated_zoid_flags &= zoid_flags;
        zero_state = 0;
        *(u16 *)(zoid + PLAYER_ZOID_OFFSET(flags)) = updated_zoid_flags;
        zoid_or_pilot_slot = zoid[PLAYER_ZOID_OFFSET(pilot_slot)];
        if (zoid_or_pilot_slot != 0) {
            register u8 *pilot_flags_address asm("r0");
            register s32 pilot_flags_base_offset asm("r1");
            register s32 pilot_flags asm("r2");
            register s32 updated_pilot_flags asm("r1");

            zoid_or_pilot_slot <<= 6;
            pilot_flags_address = (u8 *)(zoid_or_pilot_slot + (s32)player_state_bytes);
            pilot_flags_base_offset = PLAYER_STATE_OFFSET(pilots[0].flags_and_pilot_id);
            asm volatile("" : "+r"(pilot_flags_base_offset));
            pilot_flags_address = (u8 *)((s32)pilot_flags_address + pilot_flags_base_offset);
            pilot_flags = *(u16 *)pilot_flags_address;
            updated_pilot_flags = clear_in_team_mask;
            updated_pilot_flags &= pilot_flags;
            *(u16 *)pilot_flags_address = updated_pilot_flags;
        }
        *team_slot_address = zero_state;
        {
            register s32 team_reset_state_offset asm("r1");
            register u8 *team_reset_state_address asm("r0");

            team_reset_state_offset = PLAYER_STATE_OFFSET(team_slot_reset_state);
            asm volatile("" : "+r"(team_reset_state_offset));
            team_reset_state_address = (u8 *)((s32)player_state_bytes + team_reset_state_offset);
            team_reset_state_address = (u8 *)(team_slot + (s32)team_reset_state_address);
            *team_reset_state_address = zero_state;
        }
        return 1;
    }
    return 0;
}
