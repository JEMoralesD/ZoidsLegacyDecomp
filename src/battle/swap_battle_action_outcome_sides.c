#include "m2c_prelude.h"
#include "battle.h"
extern u8 gBattleActionOutcomes[] asm("D_0203ED02");

void SwapBattleActionOutcomeSides(void) asm("func_080E9D48");

void SwapBattleActionOutcomeSides(void) {
    u8 action_index;
    u8 unit_index;
    u8 first_side_outcome;
    u8 *first_side;
    u8 *second_side;
    s32 offset;

    action_index = 0;
    first_side = gBattleActionOutcomes;
    second_side = first_side + 6;
    do {
        unit_index = 0;
        do {
            offset = unit_index + action_index * 0xC;
            first_side_outcome = *(u8 *)((s32)offset + (s32)first_side);
            *(u8 *)((s32)offset + (s32)first_side) = *(u8 *)((s32)offset + (s32)second_side);
            *(u8 *)((s32)offset + (s32)second_side) = first_side_outcome;
            unit_index += 1;
        } while ((u32)unit_index <= 5U);
        action_index += 1;
    } while ((u32)action_index <= 2U);
}
