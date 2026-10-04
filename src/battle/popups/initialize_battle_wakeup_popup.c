#include "m2c_prelude.h"
#include "battle_popup.h"
extern void CreateBattlePopupTextSprites(int, void *, int) asm("func_080C8F48");

void InitializeBattleWakeupPopup(void *popup) asm("func_080C9B40");

void InitializeBattleWakeupPopup(void *popup) {
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_WAKEUP_ROM, popup, 0);
    BATTLE_POPUP_FIELD(popup, s32, elapsed_updates) = 0;
}
