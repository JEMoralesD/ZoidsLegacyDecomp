#include "m2c_prelude.h"
#include "battle_display.h"
M2C_UNK DestroySprite(s32) asm("func_8094554");

void DestroyBattleSelectionCursorSprites(void) asm("func_080CC9F0");

void DestroyBattleSelectionCursorSprites(void) {
    u8 cursor_slot = 0;
    s32 *cursor_sprites = (s32 *)BATTLE_SELECTION_CURSOR_SPRITES_RAM;
    for (; cursor_slot <= BATTLE_SELECTION_CURSOR_LAST; cursor_slot++) {
        DestroySprite(cursor_sprites[cursor_slot]);
    }
}
