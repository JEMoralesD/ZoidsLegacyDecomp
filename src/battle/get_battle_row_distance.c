#include "m2c_prelude.h"
#include "battle.h"
u8 GetBattleRowDistance(u8, u8, u8, u8) asm("func_080E7BCC");

u8 GetBattleRowDistance(u8 attacker_side, u8 attacker_unit_slot, u8 target_side, u8 target_unit_slot) {
    u8 row_distance;
    if (attacker_unit_slot <= 5) {
        row_distance = 0;
        if (attacker_side != target_side) {
            row_distance = 1;
            if (target_unit_slot > 2) {
                row_distance = 2;
            }
            if (attacker_unit_slot <= 2) {
                goto ret_r;
            }
            goto add_one;
        }
        if (attacker_unit_slot <= 2) {
            if (target_unit_slot > 2) {
                goto add_one;
            }
            goto ret_r;
        }
        if (target_unit_slot > 2) {
            goto ret_r;
        }
add_one:
        row_distance = row_distance + 1;
        return row_distance;
ret_r:
        return row_distance;
    }
    return 1;
}
