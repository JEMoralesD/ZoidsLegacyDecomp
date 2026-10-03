#include "m2c_prelude.h"
#include "battle.h"
extern s32 *gBattleUnitPopupGroups[] asm("D_02032F64");

s32 AreBattleUnitPopupsFinished(void) asm("func_080CA140");

s32 AreBattleUnitPopupsFinished(void) {
    s32 *popup;
    s32 *first_popup;
    register s32 popup_flags asm("r0");
    u8 unit_index;

    unit_index = 0;
    first_popup = gBattleUnitPopupGroups[0];
    if (first_popup == 0) {
        goto loop_3;
    }
    popup_flags = *first_popup;
    goto block_6;
loop_3:
    unit_index += 1;
    if ((u32) unit_index > 5U) {
        goto block_7;
    }
    popup = gBattleUnitPopupGroups[unit_index];
    if (popup == 0) {
        goto loop_3;
    }
    popup_flags = *popup;
block_6:
    popup_flags &= 1;
    if (popup_flags == 0) {
        goto loop_3;
    }
block_7:
    if (unit_index == 6) {
        return 1;
    }
    return 0;
}
