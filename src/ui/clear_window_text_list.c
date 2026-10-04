#include "m2c_prelude.h"
#include "window.h"
extern s8 gWindowTextItemCounts[] asm("D_0200E6C4");
extern s8 gWindowTextBlockOffsets[] asm("D_0200DE90");
M2C_UNK ClearWindow() asm("func_80986B4");
void ClearWindowTextList(u8 window_id) asm("func_08098834");

void ClearWindowTextList(u8 window_id) {
    u32 lookup_offset;
    s8 *block_offsets;
    gWindowTextItemCounts[window_id] = 0;
    block_offsets = gWindowTextBlockOffsets;
    lookup_offset = WINDOW_TEXT_BLOCK_COUNT;
    lookup_offset *= window_id;
    block_offsets[lookup_offset] = 0;
    ClearWindow();
}
