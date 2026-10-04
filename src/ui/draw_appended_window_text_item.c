#include "m2c_prelude.h"
#include "window.h"

struct WindowTextListView {
    u8 pad0[0xA];
    u16 height;
    s16 text_column;
    u16 text_row;
    u8 pad10[3];
    u8 slot;
    u8 top_item;
};

void DrawWindowText(struct WindowTextListView *, s32) asm("func_08097DA8");

void DrawAppendedWindowTextItem(struct WindowTextListView *window) asm("func_0809885C");

void DrawAppendedWindowTextItem(struct WindowTextListView *window) {
    register u8 *item_counts asm("r0");
    register u8 *item_count asm("r1");
    register s32 slot asm("r3");
    register s32 item_index asm("r0");
    s32 top_item;

    item_counts = (u8 *)0x0200E6C4;
    asm volatile("" : "+r"(item_counts));
    slot = window->slot;
    asm volatile("" : "+r"(slot));
    item_count = (u8 *)(slot + (s32)item_counts);
    asm volatile("" : "+r"(item_count));
    item_index = *item_count;
    asm volatile("" : "+r"(item_index));
    top_item = window->top_item;
    if ((u32)item_index >= (u32)top_item) {
        s32 visible_item_end;
        register s32 saved_item_index asm("r5");

        saved_item_index = item_index;
        asm volatile("" : "+r"(saved_item_index));
        visible_item_end = window->height;
        visible_item_end -= 2;
        visible_item_end += (u32)visible_item_end >> 31;
        visible_item_end >>= 1;
        visible_item_end += top_item;
        if (saved_item_index < visible_item_end) {
            register s32 text_bank_offset asm("r1");
            s32 table_index;
            s32 block_offset;
            register u8 *block_offsets asm("r2");
            register s32 text_blocks asm("r2");

            text_bank_offset = slot * WINDOW_TEXT_BANK_BYTES;
            block_offsets = (u8 *)0x0200DE90;
            asm volatile("" : "+r"(block_offsets));
            table_index = WINDOW_TEXT_BLOCK_COUNT;
            table_index *= slot;
            asm volatile("add %0, %1, %0"
                         : "+r"(table_index)
                         : "r"(saved_item_index));
            table_index += (s32)block_offsets;
            block_offset = *(u8 *)table_index;
            block_offset *= WINDOW_TEXT_BLOCK_BYTES;
            text_blocks = 0x0200E6CE;
            asm volatile("" : "+r"(text_blocks));
            asm volatile("add %0, %0, %1"
                         : "+r"(block_offset)
                         : "r"(text_blocks));
            text_bank_offset += block_offset;
            DrawWindowText(window, text_bank_offset);
            window->text_column = 0;
            window->text_row += 2;
        }
    }
}
