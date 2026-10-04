#include "m2c_prelude.h"
#include "battle_display.h"
extern void RunMenuScript(int) asm("func_8098BB4");

void CloseQueuedBattleEquipmentWindow(u8 action_index) asm("func_080CCEAC");

void CloseQueuedBattleEquipmentWindow(u8 action_index)
{
    switch (action_index) {
    case 0:
        RunMenuScript(BATTLE_QUEUED_EQUIPMENT_CLOSE_FIRST_MENU);
        return;
    case 1:
        RunMenuScript(BATTLE_QUEUED_EQUIPMENT_CLOSE_SECOND_MENU);
        return;
    case 2:
        RunMenuScript(BATTLE_QUEUED_EQUIPMENT_CLOSE_THIRD_MENU);
        return;
    }
}
