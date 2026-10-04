#include "m2c_prelude.h"
#include "battle_popup.h"
extern void CreateBattlePopupTextSprites() asm("func_080C8F48");
extern u8 gBattlePopupAntiAirText asm("D_081081C8");

void ShowBattleAntiAir(int popup) asm("func_080C99CC");

void ShowBattleAntiAir(int popup) {
    CreateBattlePopupTextSprites(&gBattlePopupAntiAirText, popup, 0);
    BATTLE_POPUP_FIELD(popup, u32, elapsed_updates) = 0;
}
