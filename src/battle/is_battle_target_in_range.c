#include "m2c_prelude.h"
#include "battle.h"
u8 GetBattleRowDistance(u8, u8, u8, u8) asm("func_080E7BCC");

s32 IsBattleTargetInRange(u8 attacker_side, u8 attacker_unit_slot, u8 target_side, u8 target_unit_slot, u8 range_kind) asm("func_080E8324");

s32 IsBattleTargetInRange(u8 attacker_side, u8 attacker_unit_slot, u8 target_side, u8 target_unit_slot, u8 range_kind) {
    u8 row_distance;
    s32 ret;

    row_distance = GetBattleRowDistance(attacker_side, attacker_unit_slot, target_side, target_unit_slot);
    ret = 0;
    switch (range_kind) {
    case EQUIPMENT_RANGE_0_TO_1:
        if (row_distance <= 1) ret = 1;
        break;
    case EQUIPMENT_RANGE_0_TO_2:
        if (row_distance <= 2) ret = 1;
        break;
    case EQUIPMENT_RANGE_0_TO_3:
        if (row_distance <= 3) ret = 1;
        break;
    case EQUIPMENT_RANGE_EXACTLY_2:
        if (row_distance == 2) ret = 1;
        break;
    case EQUIPMENT_RANGE_2_TO_3:
        if ((u8) (row_distance - 2) <= 1) ret = 1;
        break;
    case EQUIPMENT_RANGE_EXACTLY_3:
        if (row_distance == 3) ret = 1;
        break;
    }
    return ret;
}
