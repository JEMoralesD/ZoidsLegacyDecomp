#include "m2c_prelude.h"
#include "battle_popup.h"
void PlaySong(s32) asm("func_08092E84");
void FormatNumberText(u32, s32, s32, s32) asm("func_08098284");
void CreateBattlePopupTextSprites(s32, void *, s32) asm("func_080C8F48");
s32 CreateBattleUnitPopupEffectSprite(void *, s32, s32, s32, s32, s32, s32, s32) asm("func_080C9024");

void InitializeBattleDamagePopup(struct BattleUnitPopupGroupView *popup) asm("func_080C91A0");

void InitializeBattleDamagePopup(struct BattleUnitPopupGroupView *popup) {
    u32 display_damage;

    BATTLE_POPUP_FIELD(popup, s32, sprites[0]) = CreateBattleUnitPopupEffectSprite(popup, BATTLE_POPUP_RESOURCE_SPARK_IMPACT, 0, 0, -0x20, 0, 0, 0);
    display_damage = BATTLE_POPUP_FIELD(popup, u32, value);
    if (display_damage > BATTLE_POPUP_MAX_DISPLAY_DAMAGE) {
        display_damage = BATTLE_POPUP_MAX_DISPLAY_DAMAGE;
    }
    FormatNumberText(display_damage, BATTLE_POPUP_DAMAGE_DIGITS, NUMBER_TEXT_ASCII | NUMBER_TEXT_LEADING_SPACES, BATTLE_POPUP_TEXT_BUFFER_RAM);
    CreateBattlePopupTextSprites(BATTLE_POPUP_TEXT_BUFFER_RAM, popup, 1);
    PlaySong(BATTLE_POPUP_SOUND_DAMAGE);
    BATTLE_POPUP_FIELD(popup, s32, elapsed_updates) = 0;
}
