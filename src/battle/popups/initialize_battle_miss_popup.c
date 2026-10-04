#include "m2c_prelude.h"
#include "battle_popup.h"
void CreateBattlePopupTextSprites() asm("func_080C8F48");

void InitializeBattleMissPopup(int popup) asm("func_080C9254");

void InitializeBattleMissPopup(int popup) {
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_MISS_ROM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}
