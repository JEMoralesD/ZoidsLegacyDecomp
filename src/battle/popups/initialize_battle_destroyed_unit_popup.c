#include "m2c_prelude.h"
#include "battle_popup.h"
M2C_UNK CreateBattleUnitPopupEffectSprite() asm("func_080C9024");
M2C_UNK PlaySong() asm("func_8092E84");

void InitializeBattleDestroyedUnitPopup(s32 *popup) asm("func_080C9294");

void InitializeBattleDestroyedUnitPopup(s32 *popup) {
    BATTLE_POPUP_FIELD(popup, s32, sprites[0]) = CreateBattleUnitPopupEffectSprite(popup, BATTLE_POPUP_RESOURCE_EXPLOSION, 0, -16, -16, 0, 0, 0);
    PlaySong(BATTLE_POPUP_SOUND_EXPLOSION);
    BATTLE_POPUP_FIELD(popup, s32, elapsed_updates) = 0;
}
