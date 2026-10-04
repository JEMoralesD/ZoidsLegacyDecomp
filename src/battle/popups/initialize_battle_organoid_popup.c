#include "m2c_prelude.h"
#include "battle_popup.h"
extern void CreateBattlePopupTextSprites() asm("func_080C8F48");
extern u8 gBattlePopupOrganoidText[] asm("D_081081D4");

void InitializeBattleOrganoidPopup(int popup) asm("func_080C99EC");

void InitializeBattleOrganoidPopup(int popup) {
    CreateBattlePopupTextSprites(gBattlePopupOrganoidText, popup, 0);
    BATTLE_POPUP_FIELD(popup, u32, elapsed_updates) = 0;
}
