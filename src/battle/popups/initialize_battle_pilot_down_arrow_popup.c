#include "m2c_prelude.h"
#include "battle_popup.h"
extern void CreateBattlePopupTextSprites() asm("func_080C8F48");
extern u8 gBattlePopupPilotDownArrowText asm("D_081081E0");

void InitializeBattlePilotDownArrowPopup(void *popup) asm("func_080C9A0C");

void InitializeBattlePilotDownArrowPopup(void *popup) {
    CreateBattlePopupTextSprites(&gBattlePopupPilotDownArrowText, popup, 0);
    *(s32 *)((s32)popup + 0x8c) = 0;
}
