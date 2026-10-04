#include "m2c_prelude.h"
#include "window.h"
void *GetWindow(u8) asm("func_809716C");                                /* extern */

void SetWindowTextPosition(u8 window_id, u16 column, u16 row) asm("func_080981D0");

void SetWindowTextPosition(u8 window_id, u16 column, u16 row) {
    void *window;

    window = GetWindow(window_id);
    WINDOW_FIELD(window, u16, text_column) = column;
    WINDOW_FIELD(window, u16, text_row) = row;
}
