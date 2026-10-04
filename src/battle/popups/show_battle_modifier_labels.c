#include "m2c_prelude.h"
#include "battle_popup.h"
extern void CopyBytes(int, int, int) asm("func_080ED038");
extern void FormatNumberText(int, int, int, int) asm("func_08098284");
extern void AppendString(int, int) asm("func_08099F5C");
extern void CreateBattlePopupTextSprites(int, void *, int) asm("func_080C8F48");

void ShowBattleEvasionBonus(void *popup) asm("func_080C9874");

void ShowBattleEvasionBonus(void *popup) {
    CopyBytes(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_TEXT_EVASION_BONUS_ROM, 8);
    FormatNumberText(BATTLE_POPUP_FIELD(popup, int, value), 4, NUMBER_TEXT_ASCII | NUMBER_TEXT_SIGN_PREFIX, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    AppendString(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BUFFER_RAM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void ShowBattleDoubleAttackPower(void *popup) asm("func_080C98C0");

void ShowBattleDoubleAttackPower(void *popup) {
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_DOUBLE_ATTACK_POWER_ROM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void ShowBattleHalfEvasionRate(void *popup) asm("func_080C98E0");

void ShowBattleHalfEvasionRate(void *popup) {
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_HALF_EVASION_RATE_ROM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void ShowBattleHalfAttackPower(void *popup) asm("func_080C9900");

void ShowBattleHalfAttackPower(void *popup) {
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_HALF_ATTACK_POWER_ROM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void ShowBattleDoubleEvasionRate(void *popup) asm("func_080C9920");

void ShowBattleDoubleEvasionRate(void *popup) {
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_DOUBLE_EVASION_RATE_ROM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void ShowBattleSensorAccuracyOverride(void *popup) asm("func_080C9940");

void ShowBattleSensorAccuracyOverride(void *popup) {
    CopyBytes(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_TEXT_SENSOR_ACCURACY_OVERRIDE_ROM, 4);
    FormatNumberText(BATTLE_POPUP_FIELD(popup, int, value), 4, NUMBER_TEXT_ASCII, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    AppendString(BATTLE_POPUP_TEXT_BUFFER_RAM, BATTLE_POPUP_NUMBER_BUFFER_RAM);
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BUFFER_RAM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void ShowBattleEnergyShieldOn(void *popup) asm("func_080C998C");

void ShowBattleEnergyShieldOn(void *popup) {
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_ENERGY_SHIELD_ON_ROM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}

void ShowBattleEnergyShieldOff(void *popup) asm("func_080C99AC");

void ShowBattleEnergyShieldOff(void *popup) {
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_ENERGY_SHIELD_OFF_ROM, popup, 0);
    BATTLE_POPUP_FIELD(popup, int, elapsed_updates) = 0;
}
