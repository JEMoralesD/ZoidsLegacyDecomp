#include "m2c_prelude.h"
#include "battle.h"
#include "../game/player_state.h"
s32 FindAbilityValue(void *, s32, s32) asm("func_080E74F0");                /* extern */
u16 BiosSqrt(s16) asm("func_080ECD3C");                             /* extern */
s32 CallFunctionR0(s32) asm("func_080ECD5C");                             /* extern */

s32 RollBattleStatusResistance(u8 side, u8 unit_slot, u16 base_chance) asm("func_080BE488");

s32 RollBattleStatusResistance(u8 side, u8 unit_slot, u16 base_chance) {
    s32 unit_offset;
    s32 side_offset;
    s32 side_stride_units;
    s32 unit_stride_units;
    s32 flag_boost;
    u16 resistance_chance;
    void *pilot_record;
    register s32 battle_state_address asm("r8");

    side_stride_units = side * 0x27;
    side_offset = side_stride_units << 7;
    unit_stride_units = unit_slot * 0x27;
    unit_offset = unit_stride_units << 4;
    battle_state_address = 0x02034B4C;
    pilot_record = side_offset + (unit_offset + battle_state_address) + 0x70;
    {
        s32 signed_base_chance;
        s32 pilot_dcp_bonus;

        pilot_dcp_bonus = BiosSqrt(M2C_FIELD(pilot_record, s16 *, PLAYER_PILOT_OFFSET(dcp_bonus_percent))) * 3;
        signed_base_chance = (s16)base_chance;
        resistance_chance = signed_base_chance + pilot_dcp_bonus;
    }
    if ((FindAbilityValue(pilot_record, PILOT_ABILITY_STATUS_RESISTANCE_BONUS, 0) << 0x10) != 0) {
        resistance_chance += 0x1E;
    }
    if ((FindAbilityValue(pilot_record, PILOT_ABILITY_STATUS_RESISTANCE_PENALTY, 0) << 0x10) != 0) {
        resistance_chance -= 0x1E;
    }
    if (((u32) resistance_chance <= 0x5FU) && (0x40 & BATTLE_UNIT_FIELD((unit_offset + side_offset + battle_state_address), u16, flags))) {
        flag_boost = (0x64 - resistance_chance) * 3;
        if (flag_boost < 0) {
            flag_boost += 3;
        }
        resistance_chance += flag_boost >> 2;
    }
    if ((u32) resistance_chance > 0x63U) {
        resistance_chance = 0x63;
    }
    if ((s32) ((u32) (CallFunctionR0(*(s32 *)0x03000010) * 0x64) >> 0xF) <= (s32) resistance_chance) {
        return 1;
    }
    return 0;
}
