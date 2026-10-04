#include "m2c_prelude.h"
#include "name_entry.h"
extern int IsNameEntrySubtypeGridActive(void) asm("func_0809C434");
extern void PrintWindowTextAt(void *, s32, s32, s32, s32) asm("func_080981F0");
extern u8 gNameEntryCharacterCategory asm("D_020216F5");
extern u8 gNameEntryFirstVisibleRow asm("D_02021708");
extern u16 gNameEntryFirstCategoryCharacter asm("D_087A11DA");

extern struct NameEntryGlyphText gNameEntryGlyphText asm("D_02030564");

void PopulateNameEntryCharacterGrid(void) asm("func_0809C480");

void PopulateNameEntryCharacterGrid(void) {
    u16 *character_cursor;
    u16 remaining_characters;
    u8 grid_row, grid_column;

    if ((IsNameEntrySubtypeGridActive() << 24) == 0) {
        register int category_byte_offset asm("r0") = gNameEntryCharacterCategory * NAME_ENTRY_CATEGORY_BYTES;
        u8 *category_characters = (u8 *)&gNameEntryFirstCategoryCharacter + category_byte_offset;
        character_cursor = (u16 *)(category_characters + (NAME_ENTRY_GRID_COLUMNS * 2) * gNameEntryFirstVisibleRow);
        remaining_characters = *(u16 *)((u8 *)&gNameEntryFirstCategoryCharacter + category_byte_offset - 2) - NAME_ENTRY_GRID_COLUMNS * gNameEntryFirstVisibleRow;
    }
    grid_row = 0;
    do {
        grid_column = 0;
        do {
            if (remaining_characters != 0) {
                gNameEntryGlyphText.character = *character_cursor;
                remaining_characters--;
            } else {
                gNameEntryGlyphText.character = NAME_ENTRY_SPACE_CHARACTER;
            }
            gNameEntryGlyphText.terminator = 0;
            PrintWindowTextAt(&gNameEntryGlyphText, 0, 2, grid_column * 2 + 1, (s16)(grid_row * 2));
            character_cursor++;
            grid_column++;
        } while (grid_column < NAME_ENTRY_GRID_COLUMNS);
        grid_row++;
    } while (grid_row < NAME_ENTRY_GRID_ROWS);
    IsNameEntrySubtypeGridActive();
}
