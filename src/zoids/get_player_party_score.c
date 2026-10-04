#include "m2c_prelude.h"
#include "../game/player_state.h"

s32 GetPlayerZoidSellValue(void) asm("func_080E67F0");

s32 GetPlayerPartyScore(void) asm("func_080E13C4");

s32 GetPlayerPartyScore(void) {
    register s32 address_or_value asm("r0");
    register s32 team_slot_or_zoid_slot asm("r1");
    register s32 equipment_slot_or_offset asm("r2");
    register s32 zoid_record_offset asm("r3");
    register u8 *team_entry_or_prices asm("r4");
    register s32 party_score asm("r5");
    register s32 next_team_slot asm("r6");
    u8 *player_state_base;
    register u8 *team_zoid_slots asm("r8");

    party_score = 0;
    team_slot_or_zoid_slot = 0;
    address_or_value = 0x020218E4;
    equipment_slot_or_offset = PLAYER_STATE_OFFSET(team_zoid_slots);
    asm volatile("" : "+r"(address_or_value), "+r"(equipment_slot_or_offset));
    equipment_slot_or_offset += address_or_value;
    team_zoid_slots = (u8 *)equipment_slot_or_offset;
next_team_entry:
    address_or_value = (s32)team_zoid_slots;
    team_entry_or_prices = (u8 *)(team_slot_or_zoid_slot + address_or_value);
    address_or_value = *team_entry_or_prices;
    next_team_slot = team_slot_or_zoid_slot + 1;
    if (address_or_value != 0) {
        /* Native R0 carries the stored Zoid slot through this partial signature. */
        party_score += GetPlayerZoidSellValue();
        equipment_slot_or_offset = 0;
        player_state_base = (u8 *)0x020218E4;
        team_slot_or_zoid_slot = *team_entry_or_prices;
        address_or_value = team_slot_or_zoid_slot << 3;
        address_or_value -= team_slot_or_zoid_slot;
        zoid_record_offset = address_or_value << 4;
        team_entry_or_prices = (u8 *)0x087A2390;
next_equipment_slot:
        address_or_value = equipment_slot_or_offset << 2;
        address_or_value += zoid_record_offset;
        address_or_value += (s32)player_state_base;
        asm volatile("" : "+r"(address_or_value));
        team_slot_or_zoid_slot = address_or_value;
        team_slot_or_zoid_slot += PLAYER_STATE_OFFSET(zoids[0].equipment[0].item_id);
        address_or_value = *(u16 *)team_slot_or_zoid_slot;
        if (address_or_value != 0) {
            address_or_value <<= 2;
            address_or_value += (s32)team_entry_or_prices;
            address_or_value = *(s32 *)address_or_value;
            party_score += address_or_value;
        }
        address_or_value = equipment_slot_or_offset + 1;
        address_or_value <<= 24;
        equipment_slot_or_offset = (u32)address_or_value >> 24;
        if ((u32)equipment_slot_or_offset < 4) {
            goto next_equipment_slot;
        }
    }
    address_or_value = next_team_slot << 24;
    team_slot_or_zoid_slot = (u32)address_or_value >> 24;
    if ((u32)team_slot_or_zoid_slot < PLAYER_TEAM_SLOT_COUNT) {
        goto next_team_entry;
    }
    return party_score;
}
