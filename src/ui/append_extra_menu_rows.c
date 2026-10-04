#include "m2c_prelude.h"
#include "name_entry.h"
#include "window.h"

void AppendWindowTextItem(s32, s32) asm("func_080988C8");
void CopyBytes(void *, s32, s32) asm("func_080ED038");

void AppendExtraMenuRows(void) asm("func_0809D094");

void AppendExtraMenuRows(void) {
    u8 transfer_save_status;
    u8 challenge_save_status;
    u16 *row_text;

    if (*(u8 *)0x0202169C == 1 && *(u8 *)0x0202169D == 1 &&
        (challenge_save_status = *(u8 *)0x0202169E) == 1) {
        *(u16 *)NAME_ENTRY_GLYPH_TEXT_RAM = challenge_save_status;
    } else {
        register u16 *row_style_address asm("r1") = (u16 *)NAME_ENTRY_GLYPH_TEXT_RAM;
        register s32 disabled_row_style asm("r0") = EXTRA_MENU_ROW_DISABLED;
        *row_style_address = disabled_row_style;
    }

    row_text = (u16 *)EXTRA_MENU_ROW_TEXT_RAM;
    CopyBytes(row_text, EXTRA_MENU_CHALLENGE_TEXT_ROM, 33);
    row_text -= 1;
    {
        register s32 zero asm("r1") = 0;
        row_text[17] = WINDOW_TEXT_SET_COLOR;
        row_text[18] = zero;
    }
    AppendWindowTextItem(0, (s32)row_text);
    AppendWindowTextItem(0, EXTRA_MENU_MULTIPLAY_TEXT_ROM);

    if (*(u8 *)0x0202169C == 1 && *(u8 *)0x0202169D == 1 &&
        (transfer_save_status = *(u8 *)0x0202169E) == 1) {
        *(u16 *)NAME_ENTRY_GLYPH_TEXT_RAM = transfer_save_status;
    } else {
        register u16 *row_style_address asm("r1") = (u16 *)NAME_ENTRY_GLYPH_TEXT_RAM;
        register s32 disabled_row_style asm("r0") = EXTRA_MENU_ROW_DISABLED;
        *row_style_address = disabled_row_style;
    }

    row_text = (u16 *)EXTRA_MENU_ROW_TEXT_RAM;
    CopyBytes(row_text, EXTRA_MENU_TRANSFER_TEXT_ROM, 17);
    row_text -= 1;
    {
        register s32 zero asm("r1") = 0;
        row_text[9] = WINDOW_TEXT_SET_COLOR;
        row_text[10] = zero;
    }
    AppendWindowTextItem(0, (s32)row_text);
    AppendWindowTextItem(0, EXTRA_MENU_DATABASE_TEXT_ROM);
    AppendWindowTextItem(0, EXTRA_MENU_CONFIG_TEXT_ROM);

    if (*(u8 *)0x02021699 <= 1 || *(u8 *)0x0202169A <= 1 ||
        *(u8 *)0x0202169B <= 1 || *(u8 *)0x0202169C <= 1 ||
        *(u8 *)0x0202169D <= 1 || *(u8 *)0x0202169E <= 1 ||
        *(u8 *)0x0202169F <= 1) {
        *(u16 *)NAME_ENTRY_GLYPH_TEXT_RAM = EXTRA_MENU_ROW_ENABLED;
    } else {
        register s32 disabled_row_style asm("r0") = EXTRA_MENU_ROW_DISABLED;
        *(u16 *)NAME_ENTRY_GLYPH_TEXT_RAM = disabled_row_style;
    }

    row_text = (u16 *)EXTRA_MENU_ROW_TEXT_RAM;
    CopyBytes(row_text, EXTRA_MENU_DELETE_SAVE_TEXT_ROM, 31);
    row_text -= 1;
    AppendWindowTextItem(0, (s32)row_text);
}
