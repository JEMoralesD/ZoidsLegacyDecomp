#include "m2c_prelude.h"
#include "battle_popup.h"
extern void CopyBytes(int, int, int) asm("func_080ED038");
extern void FormatNumberText(int, int, int, int) asm("func_08098284");
extern void AppendString(int, int) asm("func_08099F5C");
extern void CreateBattlePopupTextSprites(int, void *, int) asm("func_080C8F48");

void InitializeBattleSpeedModifierPopup(void *popup) asm("func_080C957C");

void InitializeBattleSpeedModifierPopup(void *popup) {
    CopyBytes(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_TEXT_SPEED_ROM, 3);
    FormatNumberText(BATTLE_POPUP_FIELD(popup, int, value), 4, NUMBER_TEXT_ASCII | NUMBER_TEXT_SIGN_PREFIX, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    AppendString(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BUFFER_RAM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void InitializeBattleMobilityModifierPopup(void *popup) asm("func_080C95C8");

void InitializeBattleMobilityModifierPopup(void *popup) {
    CopyBytes(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_TEXT_MOBILITY_ROM, 3);
    FormatNumberText(BATTLE_POPUP_FIELD(popup, int, value), 4, NUMBER_TEXT_ASCII | NUMBER_TEXT_SIGN_PREFIX, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    AppendString(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BUFFER_RAM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void InitializeBattleDefenseModifierPopup(void *popup) asm("func_080C9614");

void InitializeBattleDefenseModifierPopup(void *popup) {
    CopyBytes(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_TEXT_DEFENSE_ROM, 3);
    FormatNumberText(BATTLE_POPUP_FIELD(popup, int, value), 4, NUMBER_TEXT_ASCII | NUMBER_TEXT_SIGN_PREFIX, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    AppendString(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BUFFER_RAM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void InitializeBattleArmorRateModifierPopup(void *popup) asm("func_080C9660");

void InitializeBattleArmorRateModifierPopup(void *popup) {
    CopyBytes(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_TEXT_ARMOR_RATE_ROM, 3);
    FormatNumberText(BATTLE_POPUP_FIELD(popup, int, value), 4, NUMBER_TEXT_ASCII | NUMBER_TEXT_SIGN_PREFIX, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    AppendString(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BUFFER_RAM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}
