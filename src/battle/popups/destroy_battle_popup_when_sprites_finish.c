#include "m2c_prelude.h"
#include "battle_popup.h"

extern void DestroySpriteGroup() asm("func_8095114");

void DestroyBattlePopupWhenSpritesFinish(struct BattleUnitPopupGroupView *popup) asm("func_080C9164");

void DestroyBattlePopupWhenSpritesFinish(struct BattleUnitPopupGroupView *popup) {
    u8 sprite_slot;
    struct BattleDisplaySprite *sprite;
    for (sprite_slot = 0; sprite_slot < BATTLE_POPUP_SPRITE_COUNT; sprite_slot++) {
        sprite = popup->sprites[sprite_slot];
        if (sprite != 0 && (BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_ACTIVE))
            break;
    }
    if (sprite_slot == BATTLE_POPUP_SPRITE_COUNT)
        DestroySpriteGroup(popup);
}
