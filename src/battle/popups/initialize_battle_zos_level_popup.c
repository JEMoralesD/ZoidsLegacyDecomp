#include "m2c_prelude.h"
#include "battle_popup.h"
extern void CopyBytes(int, int, int) asm("func_080ED038");
extern void FormatNumberText(int, int, int, int) asm("func_08098284");
extern void AppendString(int, int) asm("func_08099F5C");
extern void CreateBattlePopupTextSprites(int, void *, int) asm("func_080C8F48");

void InitializeBattleZosLevelPopup(void *popup) asm("func_080C9A8C");

void InitializeBattleZosLevelPopup(void *popup) {
    CopyBytes(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_TEXT_ZOS_LEVEL_ROM, 4);
    FormatNumberText(BATTLE_POPUP_FIELD(popup, int, value), 4, NUMBER_TEXT_ASCII | NUMBER_TEXT_SIGN_PREFIX, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    AppendString(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BUFFER_RAM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void InitializeBattleDecoyPopup(void *popup) asm("func_080C9AD8");

void InitializeBattleDecoyPopup(void *popup) {
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_DECOY_ROM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void InitializeBattleEffectOnlyPopup(void *popup) asm("func_080C9AF8");

void InitializeBattleEffectOnlyPopup(void *popup) {
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void InitializeBattleZosOffPopup(void *popup) asm("func_080C9B00");

void InitializeBattleZosOffPopup(void *popup) {
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_ZOS_OFF_ROM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void InitializeBattleBerserkPopup(void *popup) asm("func_080C9B20");

void InitializeBattleBerserkPopup(void *popup) {
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BERSERK_ROM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}
