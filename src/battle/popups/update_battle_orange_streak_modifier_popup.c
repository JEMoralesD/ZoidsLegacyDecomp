#include "m2c_prelude.h"
#include "battle_popup.h"
extern void PlaySong(s32) asm("func_08092E84");
extern s32 CreateBattleUnitPopupEffectSprite(void *, s32, s32, s32, s32, s32, s32, s32) asm("func_080C9024");
extern void DestroyBattlePopupWhenSpritesFinish(void *) asm("func_080C9164");

void UpdateBattleOrangeStreakModifierPopup(void *popup) asm("func_080C9DCC");

void UpdateBattleOrangeStreakModifierPopup(void *popup) {
    s32 elapsed_updates = BATTLE_POPUP_FIELD(popup, s32, elapsed_updates);
    if (elapsed_updates == 0) {
        if (BATTLE_POPUP_FIELD(popup, s32, value) >= 0) {
            BATTLE_POPUP_FIELD(popup, s32, sprites[BATTLE_POPUP_EFFECT_SPRITE_SLOT]) = CreateBattleUnitPopupEffectSprite(popup, BATTLE_POPUP_RESOURCE_STREAKS, 0, 0, elapsed_updates, BATTLE_POPUP_PALETTE_ORANGE_STREAKS, BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1, elapsed_updates);
            PlaySong(BATTLE_POPUP_SOUND_INCREASE);
        } else {
            BATTLE_POPUP_FIELD(popup, s32, sprites[BATTLE_POPUP_EFFECT_SPRITE_SLOT]) = CreateBattleUnitPopupEffectSprite(popup, BATTLE_POPUP_RESOURCE_DISSOLVING_ORBS, 0, 0, elapsed_updates, BATTLE_POPUP_PALETTE_ORBS, BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1, elapsed_updates);
            PlaySong(BATTLE_POPUP_SOUND_STATUS);
        }
        BATTLE_POPUP_FIELD(popup, s32, elapsed_updates) += 1;
        return;
    }
    DestroyBattlePopupWhenSpritesFinish(popup);
}

void UpdateBattleDissolvingOrbPopup(void *popup) asm("func_080C9E48");

void UpdateBattleDissolvingOrbPopup(void *popup) {
    s32 *p8c = (s32 *)((u8 *)popup + 0x8c);
    s32 elapsed_updates = *p8c;
    if (elapsed_updates == 0) {
        BATTLE_POPUP_FIELD(popup, s32, sprites[BATTLE_POPUP_EFFECT_SPRITE_SLOT]) = CreateBattleUnitPopupEffectSprite(popup, BATTLE_POPUP_RESOURCE_DISSOLVING_ORBS, 0, 0, elapsed_updates, BATTLE_POPUP_PALETTE_ORBS, BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1, elapsed_updates);
        PlaySong(BATTLE_POPUP_SOUND_STATUS);
        *p8c = *p8c + 1;
        return;
    }
    DestroyBattlePopupWhenSpritesFinish(popup);
}
