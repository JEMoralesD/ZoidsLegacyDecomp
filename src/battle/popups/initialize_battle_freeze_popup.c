#include "m2c_prelude.h"
#include "battle_popup.h"
extern void CreateBattlePopupTextSprites(void *a, int b, int c) asm("func_080C8F48");
extern u8 gBattlePopupFreezeText[] asm("D_0810812C");

void InitializeBattleFreezePopup(int popup) asm("func_080C9394");

void InitializeBattleFreezePopup(int popup) {
    CreateBattlePopupTextSprites(gBattlePopupFreezeText, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}
