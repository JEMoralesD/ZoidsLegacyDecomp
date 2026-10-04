#include "m2c_prelude.h"
#include "battle_popup.h"
extern void PlaySong(s32) asm("func_08092E84");
extern void HideBattleUnitSprites(s32, s32) asm("func_080BB05C");
extern struct BattleDisplaySprite *CreateBattleUnitPopupEffectSprite(struct BattleUnitPopupGroupView *, s32, s32, s32, s32, s32, s32, s32) asm("func_080C9024");
extern void DestroyBattlePopupWhenSpritesFinish(struct BattleUnitPopupGroupView *) asm("func_080C9164");
extern s16 DivideSigned32(s32, s32) asm("func_080ECD98");

void UpdateBattleDestroyedUnitPopup(struct BattleUnitPopupGroupView *popup) asm("func_080C92C4");

void UpdateBattleDestroyedUnitPopup(struct BattleUnitPopupGroupView *popup) {
    u32 elapsed_updates = popup->elapsed_updates;
    if (elapsed_updates <= BATTLE_POPUP_THIRD_EXPLOSION_UPDATE) {
        if (elapsed_updates == BATTLE_POPUP_SECOND_EXPLOSION_UPDATE) {
            struct BattleDisplaySprite *second_explosion;
            struct BattleDisplaySprite *second_explosion_view;
            second_explosion = CreateBattleUnitPopupEffectSprite(popup, BATTLE_POPUP_RESOURCE_EXPLOSION, 0, 0, -0x20, 0, BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_2, 0);
            popup->sprites[1] = second_explosion;
            second_explosion->scale = second_explosion->scale * 2;
            second_explosion_view = popup->sprites[1];
            if ((s32) second_explosion_view->scale > 0x100) {
                second_explosion_view->flags = second_explosion_view->flags | BATTLE_SPRITE_DOUBLE_CANVAS;
            }
            PlaySong(BATTLE_POPUP_SOUND_LARGE_EXPLOSION);
        } else if (elapsed_updates == BATTLE_POPUP_THIRD_EXPLOSION_UPDATE) {
            struct BattleDisplaySprite *third_explosion;
            struct BattleDisplaySprite *third_explosion_view;
            third_explosion = CreateBattleUnitPopupEffectSprite(popup, BATTLE_POPUP_RESOURCE_EXPLOSION, 0, 0, -0x40, 0, BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1, 0);
            popup->sprites[2] = third_explosion;
            third_explosion->scale = DivideSigned32(popup->sprites[0]->scale * 0xF, 0xA);
            third_explosion_view = popup->sprites[2];
            if ((s32) third_explosion_view->scale > 0x100) {
                third_explosion_view->flags = third_explosion_view->flags | BATTLE_SPRITE_DOUBLE_CANVAS;
            }
            HideBattleUnitSprites(popup->side, popup->unit_index);
        }
        popup->elapsed_updates = popup->elapsed_updates + 1;
        return;
    }
    DestroyBattlePopupWhenSpritesFinish(popup);
}
