#include "m2c_prelude.h"
#include "battle_popup.h"
extern s32 gBattleUnitSprites[][6] asm("D_02032E8C");
extern s32 gBattleDamageShakeOffsets[] asm("D_087A288C");
void DestroyBattlePopupWhenSpritesFinish() asm("func_080C9164");

void UpdateBattleDamagePopup(s32 *popup) asm("func_080C9204");

void UpdateBattleDamagePopup(s32 *popup) {
    s32 *elapsed_updates_pointer = &BATTLE_POPUP_FIELD(popup, s32, elapsed_updates);
    s32 elapsed_updates = *elapsed_updates_pointer;
    if ((u32)elapsed_updates <= BATTLE_POPUP_DAMAGE_SHAKE_LAST_UPDATE) {
        s32 *unit_sprite = (s32 *)gBattleUnitSprites[BATTLE_POPUP_FIELD(popup, s32, side)][BATTLE_POPUP_FIELD(popup, s32, unit_index)];
        BATTLE_SPRITE_FIELD(unit_sprite, s32, user_data.position.x) += gBattleDamageShakeOffsets[elapsed_updates];
        *elapsed_updates_pointer += 1;
    }
    DestroyBattlePopupWhenSpritesFinish(popup);
}
