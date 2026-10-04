#include "m2c_prelude.h"
#include "battle_popup.h"
void PlaySong(s32) asm("func_08092E84");
s32 CreateBattleUnitPopupEffectSprite(void *, s32, s32, s32, s32, s32, s32, s32) asm("func_080C9024");
void DestroyBattlePopupWhenSpritesFinish(void *) asm("func_080C9164");

void UpdateBattlePurpleSparkModifierPopup(void *popup) asm("func_080C9C58");

void UpdateBattlePurpleSparkModifierPopup(void *popup) {
    s32 elapsed_updates;

    elapsed_updates = BATTLE_POPUP_FIELD(popup, s32, elapsed_updates);
    if (elapsed_updates == 0) {
        if (BATTLE_POPUP_FIELD(popup, s32, value) >= 0) {
            BATTLE_POPUP_FIELD(popup, s32, sprites[BATTLE_POPUP_EFFECT_SPRITE_SLOT]) = CreateBattleUnitPopupEffectSprite(popup, BATTLE_POPUP_RESOURCE_SPARKS, 0, 0, elapsed_updates, BATTLE_POPUP_PALETTE_PURPLE_SPARKS, BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1, elapsed_updates);
            PlaySong(BATTLE_POPUP_SOUND_INCREASE);
        } else {
            BATTLE_POPUP_FIELD(popup, s32, sprites[BATTLE_POPUP_EFFECT_SPRITE_SLOT]) = CreateBattleUnitPopupEffectSprite(popup, BATTLE_POPUP_RESOURCE_DISSOLVING_ORBS, 0, 0, elapsed_updates, BATTLE_POPUP_PALETTE_ORBS, BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1, elapsed_updates);
            PlaySong(BATTLE_POPUP_SOUND_STATUS);
        }
        BATTLE_POPUP_FIELD(popup, s32, elapsed_updates) = BATTLE_POPUP_FIELD(popup, s32, elapsed_updates) + 1;
        return;
    }
    DestroyBattlePopupWhenSpritesFinish(popup);
}
