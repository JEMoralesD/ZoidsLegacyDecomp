#include "m2c_prelude.h"
#include "window.h"
void *GetWindow() asm("func_0809716C");
M2C_UNK BringWindowToFront(u8) asm("func_080971AC");
M2C_UNK DrawWindowText(void *, M2C_UNK) asm("func_08097DA8");

void PrintWindowTextAt(M2C_UNK text, u8 text_color, u8 window_id, u16 column, u16 row) asm("func_080981F0");

void PrintWindowTextAt(M2C_UNK text, u8 text_color, u8 window_id, u16 column, u16 row) {
    s32 saved_color;
    u8 color;
    void *window;
    color = text_color;
    saved_color = (s32)color;
    window = GetWindow();
    WINDOW_FIELD(window, u16, text_column) = column;
    WINDOW_FIELD(window, u16, text_row) = (u16)row;
    WINDOW_FIELD(window, u8, text_color) = saved_color;
    DrawWindowText(window, text);
    WINDOW_FIELD(window, s32, flags) = (s32)(WINDOW_FIELD(window, s32, flags) | WINDOW_FLAG_TILEMAP_DIRTY);
    BringWindowToFront(window_id);
}
