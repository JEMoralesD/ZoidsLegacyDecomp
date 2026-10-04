#include "m2c_prelude.h"
#include "../game/player_state.h"

s32 AssignZoidToPlayerTeam(s32 team_slot_index, s32 stored_zoid_slot) asm("func_080E5FA8");

s32 AssignZoidToPlayerTeam(s32 team_slot_index, s32 stored_zoid_slot) {
    register u32 team_slot asm("r5");
    register u32 zoid_slot asm("r6");
    register u8 *zoid asm("r1");
    register s32 zoid_record_offset asm("r0");

    team_slot_index <<= 24;
    team_slot = (u32)team_slot_index >> 24;
    stored_zoid_slot <<= 24;
    zoid_slot = (u32)stored_zoid_slot >> 24;
    zoid_record_offset = zoid_slot << 3;
    zoid_record_offset -= zoid_slot;
    zoid_record_offset <<= 4;
    zoid = (u8 *)0x020218E8;
    asm volatile("" : "+r"(zoid));
    zoid = (u8 *)(zoid_record_offset + (s32)zoid);
    if (zoid[PLAYER_ZOID_OFFSET(pilot_slot)] != 0) {
        register u8 *size_class_address asm("r0");

        size_class_address = zoid;
        size_class_address += PLAYER_ZOID_OFFSET(size_class);
        if (*size_class_address != ZOID_SIZE_CLASS_DOUBLE_ICON || team_slot == 1 || team_slot == 4) {
            register s32 zoid_flags asm("r0");
            register s32 in_team_flag asm("r3");
            register s32 zero_state asm("r4");
            register u8 *player_state_bytes asm("r2");
            register s32 pilot_record_offset asm("r0");
            register u8 *pilot_flags_address asm("r0");
            register s32 pilot_flags_base_offset asm("r1");
            register s32 pilot_flags asm("r1");
            register s32 team_slots_offset asm("r1");
            register u8 *team_slot_address asm("r0");
            register s32 team_reset_state_offset asm("r0");
            register u8 *team_reset_state_address asm("r2");

            zoid_flags = *(u16 *)(zoid + PLAYER_ZOID_OFFSET(flags));
            in_team_flag = PLAYER_RECORD_IN_TEAM;
            zero_state = 0;
            zoid_flags |= in_team_flag;
            *(u16 *)(zoid + PLAYER_ZOID_OFFSET(flags)) = zoid_flags;

            player_state_bytes = (u8 *)0x020218E4;
            asm volatile("" : "+r"(player_state_bytes));
            pilot_record_offset = zoid[PLAYER_ZOID_OFFSET(pilot_slot)];
            pilot_record_offset <<= 6;
            pilot_flags_address = (u8 *)(pilot_record_offset + (s32)player_state_bytes);
            asm volatile("" : "+r"(pilot_flags_address));
            pilot_flags_base_offset = PLAYER_STATE_OFFSET(pilots[0].flags_and_pilot_id);
            asm volatile("" : "+r"(pilot_flags_base_offset));
            pilot_flags_address = (u8 *)((s32)pilot_flags_address + pilot_flags_base_offset);
            pilot_flags = *(u16 *)pilot_flags_address;
            pilot_flags |= in_team_flag;
            *(u16 *)pilot_flags_address = pilot_flags;

            team_slots_offset = PLAYER_STATE_OFFSET(team_zoid_slots);
            asm volatile("" : "+r"(team_slots_offset));
            team_slot_address = (u8 *)((s32)player_state_bytes + team_slots_offset);
            asm volatile("" : "+r"(team_slot_address));
            team_slot_address = (u8 *)(team_slot + (s32)team_slot_address);
            *team_slot_address = zoid_slot;
            team_reset_state_offset = PLAYER_STATE_OFFSET(team_slot_reset_state);
            asm volatile("" : "+r"(team_reset_state_offset));
            team_reset_state_address = (u8 *)((s32)player_state_bytes + team_reset_state_offset);
            asm volatile("" : "+r"(team_reset_state_address));
            team_reset_state_address = (u8 *)(team_slot + (s32)team_reset_state_address);
            *team_reset_state_address = zero_state;
            return 1;
        }
    }
    return 0;
}
