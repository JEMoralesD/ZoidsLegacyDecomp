#include "m2c_prelude.h"
#include "window.h"
void *GetWindow(u8) asm("func_0809716C");                            /* extern */
M2C_UNK BringWindowToFront(u8) asm("func_080971AC");                          /* extern */
M2C_UNK DrawWindowText(void *, M2C_UNK) asm("func_08097DA8");             /* extern */
M2C_UNK FormatNumberText(M2C_UNK, u8, u8) asm("func_08098284");             /* extern */

void PrintWindowNumberAt(M2C_UNK value, u8 digit_width, u8 text_color, u8 format_flags, u8 window_id, u16 column, u16 row) asm("func_0809844C");

void PrintWindowNumberAt(M2C_UNK value, u8 digit_width, u8 text_color, u8 format_flags, u8 window_id, u16 column, u16 row) {
    u8 saved_window_id;
    void *window;
    register void *number_text_buffer asm("r10");

    saved_window_id = (u8) window_id;
    number_text_buffer = (void *)0x02021676;
    asm volatile("" : "+r"(number_text_buffer));
    FormatNumberText(value, digit_width, format_flags);
    window = GetWindow(saved_window_id);
    WINDOW_FIELD(window, u16, text_column) = (u16) column;
    WINDOW_FIELD(window, u16, text_row) = (u16) row;
    WINDOW_FIELD(window, u8, text_color) = text_color;
    DrawWindowText(window, number_text_buffer);
    WINDOW_FIELD(window, s32, flags) = (s32) (WINDOW_FIELD(window, s32, flags) | WINDOW_FLAG_TILEMAP_DIRTY);
    BringWindowToFront(saved_window_id);
}
