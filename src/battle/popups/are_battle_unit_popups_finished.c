#include "m2c_prelude.h"
#include "battle_popup.h"
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
        goto check_next_popup;
    }
    popup_flags = *first_popup;
    goto check_popup_flags;
check_next_popup:
    unit_index += 1;
    if ((u32) unit_index > BATTLE_ACTIVE_UNIT_COUNT - 1) {
        goto check_result;
    }
    popup = gBattleUnitPopupGroups[unit_index];
    if (popup == 0) {
        goto check_next_popup;
    }
    popup_flags = *popup;
check_popup_flags:
    popup_flags &= BATTLE_POPUP_GROUP_ACTIVE;
    if (popup_flags == 0) {
        goto check_next_popup;
    }
check_result:
    if (unit_index == BATTLE_ACTIVE_UNIT_COUNT) {
        return 1;
    }
    return 0;
}
