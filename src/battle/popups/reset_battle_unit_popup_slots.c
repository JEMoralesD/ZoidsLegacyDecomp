#include "m2c_prelude.h"
#include "battle_popup.h"
void ResetBattleUnitPopupSlots(void) asm("func_080C9EE0");

void ResetBattleUnitPopupSlots(void) {
    u8 unit_index;

    unit_index = 0;
    do {
        M2C_FIELD((unit_index * 4), s32 *, BATTLE_POPUP_GROUP_SLOTS_RAM) = 0;
        unit_index += 1;
    } while ((u32) unit_index <= 5U);
}
