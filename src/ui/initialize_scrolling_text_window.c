#include "m2c_prelude.h"
#include "window.h"
#include "scrolling_text.h"
#include "../battle/battle_display.h"
extern u8 gScrollingTextSource asm("D_02032E60");
extern u8 gScrollingTextWindowId asm("D_02032E64");
extern u8 gScrollingTextTopLine asm("D_02032E65");
extern u8 gScrollingTextArrowSprites asm("D_02032E68");
s32 CreateSprite(M2C_UNK, M2C_UNK, s32, s16, s32, s32, s32, s32, s32) asm("func_8094484"); /* extern */
struct Window *GetWindow(u8) asm("func_809716C");                                /* extern */
M2C_UNK RefreshScrollingTextWindow() asm("func_080E2C24");                                /* extern */

void InitializeScrollingTextWindow(u8 window_id, s32 text_address) asm("func_080E2DCC");

void InitializeScrollingTextWindow(u8 window_id, s32 text_address) {
    u8 saved_window_id;
    struct Window *window;

    saved_window_id = window_id;
    window = GetWindow(saved_window_id);
    *(s32 *)((u32)&gScrollingTextSource) = text_address;
    *(u8 *)((u32)&gScrollingTextWindowId) = saved_window_id;
    *(s8 *)((u32)&gScrollingTextTopLine) = 0;
    M2C_FIELD((void *)((u32)&gScrollingTextArrowSprites), s32 *, 0) = CreateSprite(0x080ED578, 0x080ED5A0, 0, (s16) (((WINDOW_FIELD(window, u16, x) + WINDOW_FIELD(window, u16, width)) - 1) * 8), (s32) (s16) ((WINDOW_FIELD(window, u16, y) + 1) * 8), SCROLLING_TEXT_UP_ARROW_TILE, SCROLLING_TEXT_ARROW_PALETTE, 0x20430, 0);
    M2C_FIELD((void *)((u32)&gScrollingTextArrowSprites), s32 *, 4) = CreateSprite(0x080ED620, 0x080ED648, 0, (s16) (((WINDOW_FIELD(window, u16, x) + WINDOW_FIELD(window, u16, width)) - 1) * 8), (s32) (s16) (((WINDOW_FIELD(window, u16, y) + WINDOW_FIELD(window, u16, height)) - 2) * 8), SCROLLING_TEXT_DOWN_ARROW_TILE, SCROLLING_TEXT_ARROW_PALETTE, 0x20430, 0);
    RefreshScrollingTextWindow();
}
