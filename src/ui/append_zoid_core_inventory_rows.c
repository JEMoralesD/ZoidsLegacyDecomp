#include "player_selection.h"
extern void CopyString(void *, s32) asm("func_80ED128");
extern u8 CountEncodedTextGlyphs(void *) asm("func_08098B58");
extern void AppendString(void *, void *) asm("func_08099F5C");
extern void FormatNumberText(u8, s32, s32, void *) asm("func_08098284");
extern void AppendWindowTextItem(u8, void *) asm("func_080988C8");
extern u8 gZoidCoreSelectionCount asm("D_0203246B");
extern u8 gZoidCoreSelectionIds[] asm("D_02032412");
extern s32 gZoidCoreNameTable[] asm("D_087EEE60");
extern u8 gPlayerZoidCoreQuantities[] asm("D_020217FE");
void AppendZoidCoreInventoryRows(u8 window_id) asm("func_080AC6FC");

void AppendZoidCoreInventoryRows(u8 window_id) {
    u8 selection_index;
    u8 name_columns;
    u8 core_id;
    selection_index = 0;
    if (selection_index < gZoidCoreSelectionCount) {
        register void *row_text asm("r6") = (void *)PLAYER_SELECTION_TEXT_BUFFER_RAM;
        register s32 *core_name_table asm("r10") = gZoidCoreNameTable;
        register void *quantity_text asm("r8");
        {
            register void *quantity_text_buffer asm("r1") = (void *)PLAYER_SELECTION_NUMBER_BUFFER_RAM;
            __asm__ volatile ("" : "+r" (quantity_text_buffer));
            quantity_text = quantity_text_buffer;
        }
        do {
            CopyString(row_text, core_name_table[gZoidCoreSelectionIds[selection_index]]);
            name_columns = CountEncodedTextGlyphs(row_text);
            if (name_columns < PLAYER_SELECTION_CORE_NAME_COLUMNS) {
                do { AppendString((void *)PLAYER_SELECTION_TEXT_BUFFER_RAM, (void *)PLAYER_SELECTION_SPACE_TEXT_ROM); name_columns++; } while (name_columns < PLAYER_SELECTION_CORE_NAME_COLUMNS);
            }
            AppendString(row_text, (void *)PLAYER_SELECTION_QUANTITY_SEPARATOR_ROM);
            core_id = gZoidCoreSelectionIds[selection_index];
            FormatNumberText(gPlayerZoidCoreQuantities[core_id], 2, 2, quantity_text);
            AppendString(row_text, quantity_text);
            AppendWindowTextItem(window_id, row_text);
            selection_index++;
        } while (selection_index < gZoidCoreSelectionCount);
    }
}
