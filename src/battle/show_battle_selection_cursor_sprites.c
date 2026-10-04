#include "m2c_prelude.h"
#include "battle_display.h"
void ShowBattleSelectionCursorSprites(void) asm("func_080CCA14");

void ShowBattleSelectionCursorSprites(void) {
    s32 *cursor_flags;
    u8 cursor_slot;

    cursor_slot = 0;
    do {
        cursor_flags = M2C_FIELD((cursor_slot * 4), s32 **, BATTLE_SELECTION_CURSOR_SPRITES_RAM);
        *cursor_flags &= ~BATTLE_SPRITE_HIDDEN;
        cursor_slot += 1;
    } while ((u32) cursor_slot <= BATTLE_SELECTION_CURSOR_LAST);
}
