#include "m2c_prelude.h"
#include "battle_popup.h"
extern void CopyBytes(int, int, int) asm("func_080ED038");
extern void FormatNumberText(int, int, int, int) asm("func_08098284");
extern void AppendString(int, int) asm("func_08099F5C");
extern void CreateBattlePopupTextSprites(int, void *, int) asm("func_080C8F48");

void InitializeBattleSensorAccuracyModifierPopup(void *popup) asm("func_080C96AC");

void InitializeBattleSensorAccuracyModifierPopup(void *popup) {
    CopyBytes(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_TEXT_SENSOR_ACCURACY_ROM, 3);
    FormatNumberText(BATTLE_POPUP_FIELD(popup, int, value), 4, NUMBER_TEXT_ASCII | NUMBER_TEXT_SIGN_PREFIX, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    AppendString(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BUFFER_RAM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void InitializeBattleLoadCapacityModifierPopup(void *popup) asm("func_080C96F8");

void InitializeBattleLoadCapacityModifierPopup(void *popup) {
    CopyBytes(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_TEXT_LOAD_CAPACITY_ROM, 3);
    FormatNumberText(BATTLE_POPUP_FIELD(popup, int, value), 4, NUMBER_TEXT_ASCII | NUMBER_TEXT_SIGN_PREFIX, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    AppendString(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BUFFER_RAM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void InitializeBattleInitiativeModifierPopup(void *popup) asm("func_080C9744");

void InitializeBattleInitiativeModifierPopup(void *popup) {
    CopyBytes(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_TEXT_INITIATIVE_ROM, 3);
    FormatNumberText(BATTLE_POPUP_FIELD(popup, int, value), 4, NUMBER_TEXT_ASCII | NUMBER_TEXT_SIGN_PREFIX, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    AppendString(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BUFFER_RAM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void InitializeBattleEvasionScoreModifierPopup(void *popup) asm("func_080C9790");

void InitializeBattleEvasionScoreModifierPopup(void *popup) {
    CopyBytes(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_TEXT_EVASION_SCORE_ROM, 3);
    FormatNumberText(BATTLE_POPUP_FIELD(popup, int, value), 4, NUMBER_TEXT_ASCII | NUMBER_TEXT_SIGN_PREFIX, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    AppendString(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BUFFER_RAM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void InitializeBattleAttackPowerModifierPopup(void *popup) asm("func_080C97DC");

void InitializeBattleAttackPowerModifierPopup(void *popup) {
    CopyBytes(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_TEXT_ATTACK_POWER_ROM, 3);
    FormatNumberText(BATTLE_POPUP_FIELD(popup, int, value), 4, NUMBER_TEXT_ASCII | NUMBER_TEXT_SIGN_PREFIX, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    AppendString(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BUFFER_RAM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}
