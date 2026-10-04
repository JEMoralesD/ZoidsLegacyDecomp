#include "m2c_prelude.h"
#include "battle_combination.h"
extern u8 gBattleCombinationPresentation[] asm("D_02032F7C");
void ResetBattleCombinationPresentation(void) asm("func_080C2DB0");

void ResetBattleCombinationPresentation(void) {
    u8 unit_slot;
    for (unit_slot = 0; unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1; unit_slot++) {
        gBattleCombinationPresentation[unit_slot] = BATTLE_COMBINATION_UNCHANGED_UNIT;
    }
}
