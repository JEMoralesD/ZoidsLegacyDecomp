#include "player_selection.h"
void HidePlayerSelectionStatusSprites(u8 visible_rows) asm("func_080ACB5C");

void HidePlayerSelectionStatusSprites(u8 visible_rows) {
    u8 visible_row;
    for (visible_row = 0; visible_row < visible_rows; visible_row++) {
        (*(struct PlayerSelectionSpriteFlagsView **)(PLAYER_SELECTION_TEAM_SPRITES_RAM + visible_row * 4))->flags |= PLAYER_SELECTION_SPRITE_HIDDEN;
        (*(struct PlayerSelectionSpriteFlagsView **)(PLAYER_SELECTION_NEW_SPRITES_RAM + visible_row * 4))->flags |= PLAYER_SELECTION_SPRITE_HIDDEN;
    }
}
