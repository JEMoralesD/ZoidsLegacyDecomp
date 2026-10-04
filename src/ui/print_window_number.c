#include "m2c_prelude.h"
#include "window.h"
M2C_UNK FormatNumberText() asm("func_08098284");
void *GetWindow(u8) asm("func_809716C");
M2C_UNK BringWindowToFront(u8) asm("func_80971AC");
M2C_UNK DrawWindowText(void *, M2C_UNK) asm("func_08097DA8");
void PrintWindowNumber(M2C_UNK value, u8 digit_width, u8 text_color, u8 format_flags, s32 window_id) asm("func_080984C4");

void PrintWindowNumber(M2C_UNK value, u8 digit_width, u8 text_color, u8 format_flags, s32 window_id) {
    u8 saved_window_id = (u8) window_id;
    s32 number_text_buffer = 0x02021676;
    void *window;
    FormatNumberText(value, digit_width, format_flags);
    window = GetWindow(saved_window_id);
    WINDOW_FIELD(window, u8, text_color) = text_color;
    DrawWindowText(window, number_text_buffer);
    WINDOW_FIELD(window, s32, flags) = (s32) (WINDOW_FIELD(window, s32, flags) | WINDOW_FLAG_TILEMAP_DIRTY);
    BringWindowToFront(saved_window_id);
}
