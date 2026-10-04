#include "m2c_prelude.h"
#include "battle_popup.h"
extern void PlaySong(s32) asm("func_08092E84");
extern s32 CreateBattleUnitPopupEffectSprite(void *, s32, s32, s32, s32, s32, s32, s32) asm("func_080C9024");
extern void DestroyBattlePopupWhenSpritesFinish(void *) asm("func_080C9164");

void UpdateBattlePurpleRingPopup(void *popup) asm("func_080C9E94");

void UpdateBattlePurpleRingPopup(void *popup) {
    s32 *elapsed_updates_pointer = &BATTLE_POPUP_FIELD(popup, s32, elapsed_updates);
    s32 elapsed_updates = *elapsed_updates_pointer;
    if (elapsed_updates == 0) {
        BATTLE_POPUP_FIELD(popup, s32, sprites[BATTLE_POPUP_EFFECT_SPRITE_SLOT]) = CreateBattleUnitPopupEffectSprite(popup, BATTLE_POPUP_RESOURCE_PURPLE_RING, 0, 0, elapsed_updates, BATTLE_POPUP_PALETTE_PURPLE_RING, BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1, elapsed_updates);
        PlaySong(BATTLE_POPUP_SOUND_STATUS);
        *elapsed_updates_pointer = *elapsed_updates_pointer + 1;
        return;
    }
    DestroyBattlePopupWhenSpritesFinish(popup);
}
