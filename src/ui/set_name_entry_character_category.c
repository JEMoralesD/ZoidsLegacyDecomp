#include "m2c_prelude.h"
#include "name_entry.h"

s32 IsNameEntrySubtypeGridActive(void) asm("func_0809C434");
void PopulateNameEntryCharacterGrid(void) asm("func_0809C480");
s32 DivideSigned32(s32, s32) asm("func_080ECD98");
u16 ModuloUnsigned32(u16, s32) asm("func_080ECF78");

void SetNameEntryCharacterCategory(s32 character_category, s32 subtype) asm("func_0809C540");

void SetNameEntryCharacterCategory(s32 character_category, s32 subtype) {
    u16 character_count;
    u16 last_row_character_count;
    s32 rounded_character_count;
    register volatile u8 *category_address asm("r4");
    register volatile u8 *total_rows_address asm("r4");

    category_address = (u8 *)NAME_ENTRY_CATEGORY_RAM;
    *category_address = character_category;
    *(u8 *)NAME_ENTRY_SUBTYPE_RAM = subtype;
    if ((IsNameEntrySubtypeGridActive() << 24) == 0) {
        register u8 *category_table_base asm("r2");
        register s32 category_index asm("r1");
        register s32 category_byte_offset asm("r0");
        register u16 *category_count_address asm("r0");

        category_table_base = (u8 *)NAME_ENTRY_CHARACTER_TABLES_ROM;
        asm volatile("" : "+r"(category_table_base));
        category_index = *category_address;
        category_byte_offset = category_index << 5;
        category_byte_offset += category_index;
        category_byte_offset <<= 2;
        category_byte_offset -= category_index;
        category_byte_offset <<= 1;
        category_count_address = (u16 *)(category_byte_offset + (s32)category_table_base);
        character_count = *category_count_address;
    }

    total_rows_address = (u8 *)NAME_ENTRY_TOTAL_ROWS_RAM;
    last_row_character_count = ModuloUnsigned32(character_count, NAME_ENTRY_GRID_COLUMNS);
    if (last_row_character_count != 0) {
        rounded_character_count = character_count + NAME_ENTRY_GRID_COLUMNS;
        rounded_character_count -= last_row_character_count;
    } else {
        rounded_character_count = character_count;
    }
    *total_rows_address = DivideSigned32(rounded_character_count, NAME_ENTRY_GRID_COLUMNS);
    if (*total_rows_address < NAME_ENTRY_GRID_ROWS) {
        *(u8 *)NAME_ENTRY_FIRST_VISIBLE_ROW_RAM = 0;
    } else {
        register volatile u8 *first_visible_row_address asm("r2");
        register s32 first_visible_row asm("r1");
        register s32 last_first_visible_row asm("r0");

        first_visible_row_address = (u8 *)NAME_ENTRY_FIRST_VISIBLE_ROW_RAM;
        first_visible_row = *first_visible_row_address;
        last_first_visible_row = *total_rows_address;
        last_first_visible_row -= NAME_ENTRY_GRID_ROWS;
        if (first_visible_row > last_first_visible_row) {
            *first_visible_row_address = last_first_visible_row;
        }
    }
    PopulateNameEntryCharacterGrid();
}
