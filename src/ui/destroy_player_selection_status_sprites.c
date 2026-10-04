#include "player_selection.h"
M2C_UNK DestroySprite(s32) asm("func_8094554");
void DestroyPlayerSelectionStatusSprites(u8 visible_rows) asm("func_080ACBA0");

void DestroyPlayerSelectionStatusSprites(u8 visible_rows) {
    u8 visible_row = 0;
    if (visible_row < visible_rows) {
        s32 *team_marker_sprite_addresses = (s32 *)PLAYER_SELECTION_TEAM_SPRITES_RAM;
        do {
            DestroySprite(team_marker_sprite_addresses[visible_row]);
            DestroySprite(*(s32 *)(PLAYER_SELECTION_NEW_SPRITES_RAM + visible_row * 4));
            visible_row++;
        } while (visible_row < visible_rows);
    }
}
