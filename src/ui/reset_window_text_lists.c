#include "m2c_prelude.h"
#include "window.h"
extern u8 gWindowTextItemCounts[] asm("D_0200E6C4"); extern u8 gWindowTextBlockOffsets[] asm("D_0200DE90");
void ResetWindowTextLists(void) asm("func_08098804");

void ResetWindowTextLists(void) {
    u8 window_id;
    for (window_id = 0; window_id < WINDOW_COUNT; window_id++) {
        gWindowTextItemCounts[window_id] = 0;
        gWindowTextBlockOffsets[window_id * WINDOW_TEXT_BLOCK_COUNT] = 0;
    }
}
