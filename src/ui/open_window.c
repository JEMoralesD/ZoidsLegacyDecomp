#include "m2c_prelude.h"
#include "window.h"


extern struct Window *GetWindow(u8) asm("func_0809716C");
extern void BringWindowToFront(u8) asm("func_080971AC");
extern void DrawWindowText(struct Window *, void *) asm("func_08097DA8");
extern u32 gWindowFrameTileOffset asm("D_02021664");
extern u16 gWindowBgPaletteAttribute asm("D_02021668");
extern u8 gWindowTextItemCounts[] asm("D_0200E6C4");
extern u8 gWindowTextBlockOffsets[] asm("D_0200DE90");

void OpenWindow(s32 window_id, s32 x, s32 y, s32 width, s32 height, volatile s32 flags)
{
    register u32 saved_window_id asm("r9");
    register u32 saved_x asm("r7");
    register u32 saved_y asm("r5");
    register u32 saved_width asm("r4") = width;
    register u32 loaded_height asm("r3") = height;
    register u32 saved_height asm("r8");
    register struct Window *window asm("r6");
    register u16 *tile asm("r2");
    u32 row;
    u32 frame_pad;

    window_id <<= 24;
    window_id = (u32)window_id >> 24;
    saved_window_id = window_id;
    x <<= 24;
    saved_x = (u32)x >> 24;
    y <<= 24;
    saved_y = (u32)y >> 24;
    saved_width <<= 24;
    saved_width = (u32)saved_width >> 24;
    loaded_height <<= 24;
    loaded_height = (u32)loaded_height >> 24;
    saved_height = loaded_height;
    asm volatile("" : : "m"(frame_pad));
    window = GetWindow(window_id);
    window->x = saved_x;
    window->y = saved_y;
    window->width = saved_width;
    window->height = saved_height;
    window->text_row = 0;
    window->text_column = 0;
    window->text_color = 0;
    window->frame_decoration_width = 0;
    window->frame_decoration_x = 0;
    window->prior_held_keys = 0;
    window->key_repeat_frames = 0;
    {
        register s32 one asm("r0") = 1;
        register s32 loaded_flags asm("r5");

        asm volatile("" : "+r"(one));
        loaded_flags = flags;
        asm volatile("" : "+r"(loaded_flags));
        window->flags = loaded_flags | one;
    }

    tile = window->tiles;
    asm volatile("" : "+r"(tile));
    row = 0;
    if (row < saved_height) {
        u32 *base = &gWindowFrameTileOffset;
        u16 *attribute = &gWindowBgPaletteAttribute;

        do {
            u32 column = 0;
            u32 next_row = row + 1;

            asm volatile("" : :
                "r"(row), "r"(row), "r"(row),
                "r"(row), "r"(row), "r"(row),
                "r"(row), "r"(row), "r"(row));
            if (column < window->width) {
                register u32 *row_base asm("r4") = base;
                register u16 *row_attribute asm("r3") = attribute;

                asm volatile("" : "+r"(row_base), "+r"(row_attribute));
                do {
                    register u32 value asm("r0");

                    if (row == 0) {
                        if (column == 0) {
                            value = *row_base + 2;
                        } else if (column != window->width - 1) {
                            value = *row_base + 6;
                        } else {
                            value = *row_base + 3;
                        }
                    } else if (row != window->height - 1) {
                        if (column == 0) {
                            value = *row_base + 8;
                        } else if (column != window->width - 1) {
                            value = *row_base + 1;
                        } else {
                            value = *row_base + 9;
                        }
                    } else if (column == 0) {
                        value = *row_base + 4;
                    } else if (column != window->width - 1) {
                        value = *row_base + 7;
                    } else {
                        value = *row_base + 5;
                    }

                    asm volatile("" : "+r"(value));
                    *tile = value | *row_attribute;
                    tile++;
                    asm volatile("" : "+r"(tile));
                    {
                        u32 next_column = column + 1;

                        column = (u8)next_column;
                    }
                } while (column < window->width);
            }
            row = (u8)next_row;
        } while (row < window->height);
    }

    if (window->flags & WINDOW_FLAG_TEXT_LIST) {
        gWindowTextItemCounts[window->slot] = 0;
        gWindowTextBlockOffsets[window->slot * 0xD2] = 0;
        window->selected_item = 0;
        window->top_item = 0;
        window->prior_top_item = 0;
        window->prior_selected_item = 0xFF;
    }

    if (window->flags & WINDOW_FLAG_YES_NO_PROMPT) {
        register s16 zero16 asm("r4");
        register u8 zero8 asm("r7");
        s32 text_column;

        text_column = (window->width - 2) / 2 - 1;
        zero8 = 0;
        zero16 = 0;
        asm volatile("" : "+r"(zero8), "+r"(zero16));
        window->text_column = text_column;
        window->text_row = window->height - 6;
        DrawWindowText(window, (void *)0x080ED930);

        window->text_column = (window->width - 2) / 2 - 1;
        window->text_row = window->height - 4;
        DrawWindowText(window, (void *)0x080ED938);

        window->text_row = zero16;
        window->text_column = zero16;
        window->selected_item = zero8;
    }

    BringWindowToFront(saved_window_id);
}
