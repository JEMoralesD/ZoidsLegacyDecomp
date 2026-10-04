#include "m2c_prelude.h"
#include "battle_popup.h"
void CreateBattlePopupTextSprites(s32, void *, s32) asm("func_080C8F48");

void InitializeBattlePilotUpArrowPopup(void *popup) asm("func_080C9A2C");

void InitializeBattlePilotUpArrowPopup(void *popup) {
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_PILOT_UP_ARROW_ROM, popup, 0);
    BATTLE_POPUP_FIELD(popup, s32, elapsed_updates) = 0;
}

void InitializeBattleConfusionPopup(void *popup) asm("func_080C9A4C");

void InitializeBattleConfusionPopup(void *popup) {
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_CONFUSION_ROM, popup, 0);
    BATTLE_POPUP_FIELD(popup, s32, elapsed_updates) = 0;
}

void InitializeBattleConfusionEndPopup(void *popup) asm("func_080C9A6C");

void InitializeBattleConfusionEndPopup(void *popup) {
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_CONFUSION_END_ROM, popup, 0);
    BATTLE_POPUP_FIELD(popup, s32, elapsed_updates) = 0;
}
