#include "player_selection.h"
extern void CopyString(void *, s32) asm("func_80ED128");
extern u8 CountEncodedTextGlyphs(void *) asm("func_08098B58");
extern void AppendString(void *, void *) asm("func_08099F5C");
extern void FormatNumberText(u8, s32, s32, void *) asm("func_08098284");
extern void AppendWindowTextItem(u8, void *) asm("func_080988C8");
extern u8 gRecoveryItemSelectionCount asm("D_020322B1");
extern u8 gRecoveryItemSelectionIds[] asm("D_020322A8");
extern s32 gRecoveryItemNameTable[] asm("D_087EEE10");
extern u8 gPlayerRecoveryItemQuantities[] asm("D_020217F4");

void AppendRecoveryItemInventoryRows(u8 window_id) asm("func_080AC7BC");

void AppendRecoveryItemInventoryRows(u8 window_id) {
    u8 selection_index;
    u8 name_columns;
    selection_index = 0;
    while (selection_index < gRecoveryItemSelectionCount) {
        CopyString((void *)PLAYER_SELECTION_TEXT_BUFFER_RAM, gRecoveryItemNameTable[gRecoveryItemSelectionIds[selection_index]]);
        name_columns = CountEncodedTextGlyphs((void *)PLAYER_SELECTION_TEXT_BUFFER_RAM);
        if (name_columns < PLAYER_SELECTION_RECOVERY_NAME_COLUMNS) {
            do {
                AppendString((void *)PLAYER_SELECTION_TEXT_BUFFER_RAM, (void *)PLAYER_SELECTION_SPACE_TEXT_ROM);
                name_columns++;
            } while (name_columns < PLAYER_SELECTION_RECOVERY_NAME_COLUMNS);
        }
        AppendString((void *)PLAYER_SELECTION_TEXT_BUFFER_RAM, (void *)PLAYER_SELECTION_QUANTITY_SEPARATOR_ROM);
        FormatNumberText(gPlayerRecoveryItemQuantities[gRecoveryItemSelectionIds[selection_index]], 2, 2, (void *)PLAYER_SELECTION_NUMBER_BUFFER_RAM);
        AppendString((void *)PLAYER_SELECTION_TEXT_BUFFER_RAM, (void *)PLAYER_SELECTION_NUMBER_BUFFER_RAM);
        AppendWindowTextItem(window_id, (void *)PLAYER_SELECTION_TEXT_BUFFER_RAM);
        selection_index++;
    }
}
