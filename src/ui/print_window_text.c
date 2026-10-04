#include "m2c_prelude.h"
#include "window.h"
void *GetWindow(u8) asm("func_809716C");                                /* extern */
M2C_UNK BringWindowToFront(u8) asm("func_80971AC");                              /* extern */
M2C_UNK DrawWindowText(void *, M2C_UNK) asm("func_08097DA8");                 /* extern */

void PrintWindowText(M2C_UNK text, u8 text_color, u8 window_id) asm("func_08098248");

void PrintWindowText(M2C_UNK text, u8 text_color, u8 window_id) {
    u8 saved_window_id;
    void *window;

    saved_window_id = window_id;
    window = GetWindow(saved_window_id);
    WINDOW_FIELD(window, u8, text_color) = text_color;
    DrawWindowText(window, text);
    WINDOW_FIELD(window, s32, flags) = (s32) (WINDOW_FIELD(window, s32, flags) | WINDOW_FLAG_TILEMAP_DIRTY);
    BringWindowToFront(saved_window_id);
}
