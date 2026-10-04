#include "m2c_prelude.h"
struct WindowClearView { s32 flags; s32 pad4; u16 width; u16 height; s16 text_column; s16 text_row; };
extern struct WindowClearView *GetWindow(u8) asm("func_0809716C");
extern void ReleaseWindowTile(u16) asm("func_08097980");
extern u32 gWindowFrameTileOffset asm("D_02021664");
extern u16 gWindowBgPaletteAttribute asm("D_02021668");
extern s32 gWindowTextTileOffset asm("D_02021670");
void ClearWindow(u8 window_id) {
    struct WindowClearView *window = GetWindow(window_id);
    u16 *tile = (u16 *)(window->width * 2 + (u32)window + 0x20);
    int row = 1;
    while (row < (s32)(window->height - 1)) {
        int column = 1;
        if (column < (s32)(window->width - 1)) {
            do {
                u32 v = *tile & 0x3FF;
                s32 sv = v;
                u32 base = gWindowFrameTileOffset;
                if (v < base || v >= base + 0x40) {
                    ReleaseWindowTile((u16)(sv - gWindowTextTileOffset));
                }
                *tile = (gWindowFrameTileOffset + 1) | gWindowBgPaletteAttribute;
                tile += 1;
                column += 1;
            } while (column < (s32)(window->width - 1));
        }
        tile += 2;
        row += 1;
    }
    window->text_row = 0;
    window->text_column = 0;
    window->flags = window->flags | 2;
}
