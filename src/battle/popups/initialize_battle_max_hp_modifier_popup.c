#include "m2c_prelude.h"
#include "battle_popup.h"
extern void FormatNumberText(s32, s32, s32, s32) asm("func_08098284");
extern void AppendString(s32, s32) asm("func_08099F5C");
extern void CreateBattlePopupTextSprites(s32, void *, s32) asm("func_080C8F48");
extern void CopyBytes(s32, s32, s32) asm("func_080ED038");

void InitializeBattleMaxHpModifierPopup(void *popup) asm("func_080C9400");

void InitializeBattleMaxHpModifierPopup(void *popup) {
    CopyBytes(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_TEXT_MAX_HP_ROM, 6);
    FormatNumberText(BATTLE_POPUP_FIELD(popup, s32, value), 4, NUMBER_TEXT_ASCII | NUMBER_TEXT_SIGN_PREFIX, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    AppendString(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BUFFER_RAM, popup, 0);
    BATTLE_POPUP_FIELD(popup, s32, elapsed_updates) = 0;
}

void InitializeBattleDcpModifierPopup(void *popup) asm("func_080C944C");

void InitializeBattleDcpModifierPopup(void *popup) {
    CopyBytes(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_TEXT_DCP_ROM, 4);
    FormatNumberText(BATTLE_POPUP_FIELD(popup, s32, value), 4, NUMBER_TEXT_ASCII | NUMBER_TEXT_SIGN_PREFIX, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    AppendString(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BUFFER_RAM, popup, 0);
    BATTLE_POPUP_FIELD(popup, s32, elapsed_updates) = 0;
}
