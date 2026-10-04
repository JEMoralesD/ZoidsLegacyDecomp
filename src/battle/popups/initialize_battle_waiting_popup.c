#include "m2c_prelude.h"
#include "battle_popup.h"
extern void CreateBattlePopupTextSprites(void *a, void *b, int c) asm("func_080C8F48");
extern u8 gBattlePopupWaitingText[] asm("D_08108124");

void InitializeBattleWaitingPopup(void *popup) asm("func_080C9274");

void InitializeBattleWaitingPopup(void *popup) {
    CreateBattlePopupTextSprites(gBattlePopupWaitingText, popup, 0);
    BATTLE_POPUP_FIELD(popup, s32, elapsed_updates) = 0;
}
