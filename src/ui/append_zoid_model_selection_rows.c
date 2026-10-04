#include "player_selection.h"
extern u8 gZoidModelSelectionIds[] asm("D_0203237A"); extern s32 gZoidNameTable[];
void AppendWindowTextItem(u8, s32) asm("func_080988C8");
void AppendZoidModelSelectionRows(u8 window_id) asm("func_080AC6B8");

void AppendZoidModelSelectionRows(u8 window_id) {
    u8 selection_index = 0;
    if (selection_index < *(u8 *)PLAYER_MODEL_SELECTION_COUNT_RAM) {
        do {
            AppendWindowTextItem(window_id, gZoidNameTable[gZoidModelSelectionIds[selection_index]]);
            selection_index += 1;
        } while (selection_index < *(u8 *)PLAYER_MODEL_SELECTION_COUNT_RAM);
    }
}
