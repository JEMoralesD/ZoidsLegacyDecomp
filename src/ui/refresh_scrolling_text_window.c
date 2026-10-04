#include "m2c_prelude.h"
#include "window.h"
#include "scrolling_text.h"
#include "../battle/battle_display.h"

extern u8 gScrollingTextSource[] asm("D_02032E60");
extern u8 gScrollingTextWindowId[] asm("D_02032E64");
extern u8 gScrollingTextTopLine[] asm("D_02032E65");
extern u32 gScrollingTextArrowSprites[] asm("D_02032E68");
extern u8 gScrollingTextLineBuffer[] asm("D_02030564");

extern struct Window *GetWindow(u8) asm("func_0809716C");
extern void RequestWindowRefresh(void) asm("func_080972C8");
extern void PrintWindowTextAt(u8 *, s32, u8, s32, s32) asm("func_080981F0");
extern void ClearWindow(u8) asm("func_080986B4");

void RefreshScrollingTextWindow(void) asm("func_080E2C24");

void RefreshScrollingTextWindow(void) {
    struct Window *window;
    register s32 width asm("r9");
    register s32 window_height asm("r8");
    u8 *line_output;
    u8 *text_cursor;
    u8 line_columns;
    u8 visible_line;
    u8 logical_line;
    u8 text_byte;
    u32 interior_height;

    window = GetWindow(gScrollingTextWindowId[0]);
    width = window->width;
    window_height = window->height;
    ClearWindow(gScrollingTextWindowId[0]);
    {
        register u32 *up_arrow_flags asm("r2");
        register u32 flags asm("r0");

        if (gScrollingTextTopLine[0] == 0) {
            up_arrow_flags = (u32 *)gScrollingTextArrowSprites[SCROLLING_TEXT_UP_ARROW];
            flags = *up_arrow_flags | BATTLE_SPRITE_HIDDEN;
        } else {
            up_arrow_flags = (u32 *)gScrollingTextArrowSprites[SCROLLING_TEXT_UP_ARROW];
            flags = *up_arrow_flags & ~BATTLE_SPRITE_HIDDEN;
        }
        *up_arrow_flags = flags;
    }

    text_cursor = *(u8 **)gScrollingTextSource;
    line_output = gScrollingTextLineBuffer;
    line_columns = 0;
    visible_line = 0;
    logical_line = 0;
    text_byte = *text_cursor;
    if (text_byte != 0) {
        do {
            if (text_byte <= 31) {
                switch (*text_cursor) {
                case WINDOW_TEXT_SET_COLOR:
                    text_cursor += 2;
                    break;
                case WINDOW_TEXT_SET_POSITION:
                    text_cursor += 3;
                    break;
                case WINDOW_TEXT_PLAYER_NAME:
                    text_cursor += 1;
                    break;
                case WINDOW_TEXT_NEWLINE:
                    text_cursor += 1;
                    if (logical_line >= gScrollingTextTopLine[0]) {
                        *line_output = 0;
                        PrintWindowTextAt(gScrollingTextLineBuffer, 0, gScrollingTextWindowId[0], 0, visible_line * SCROLLING_TEXT_LINE_HEIGHT);
                        visible_line += 1;
                    }
                    line_output = gScrollingTextLineBuffer;
                    line_columns = 0;
                    logical_line = (u8)(logical_line + 1);
                    break;
                }
            } else {
                line_columns += 1;
                if ((s32)line_columns > (s32)(width - SCROLLING_TEXT_BORDER_COLUMNS)) {
                    if (logical_line >= gScrollingTextTopLine[0]) {
                        *line_output = 0;
                        PrintWindowTextAt(gScrollingTextLineBuffer, 0, gScrollingTextWindowId[0], 0, visible_line * SCROLLING_TEXT_LINE_HEIGHT);
                        visible_line += 1;
                    }
                    line_output = gScrollingTextLineBuffer;
                    /* The native wrap counter excludes the glyph that starts the next line. */
                    line_columns = 0;
                    logical_line = (u8)(logical_line + 1);
                }
                line_output[0] = text_cursor[0];
                text_cursor += 1;
                line_output += 1;
                line_output[0] = text_cursor[0];
                text_cursor += 1;
                line_output += 1;
            }
            interior_height = window_height - 2;
            if (visible_line == ((s32)(interior_height + (interior_height >> 31)) >> 1)) {
                *(u32 *)gScrollingTextArrowSprites[SCROLLING_TEXT_DOWN_ARROW] &= ~BATTLE_SPRITE_HIDDEN;
                RequestWindowRefresh();
                return;
            }
            text_byte = *text_cursor;
        } while (text_byte != 0);
    }

    *line_output = 0;
    PrintWindowTextAt(gScrollingTextLineBuffer, 0, gScrollingTextWindowId[0], 0, visible_line * SCROLLING_TEXT_LINE_HEIGHT);
    *(u32 *)gScrollingTextArrowSprites[SCROLLING_TEXT_DOWN_ARROW] |= BATTLE_SPRITE_HIDDEN;
    RequestWindowRefresh();
}
