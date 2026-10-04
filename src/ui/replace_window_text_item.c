#include "m2c_prelude.h"
#include "window.h"

typedef struct {
    u32 flags;
    u16 x;
    u16 y;
    u16 width;
    u16 height;
    u16 text_column;
    u16 text_row;
    u8 data10[2];
    u8 text_color;
    u8 slot;
    u8 top_item;
} WindowTextItemView;

extern u8 gWindowTextBlockOffsets[] asm("D_0200DE90");
extern u8 gWindowTextBlocks[] asm("D_0200E6CE");
extern WindowTextItemView *GetWindow(s32 window_id) asm("func_0809716C");
extern void DrawWindowText(WindowTextItemView *window, void *data) asm("func_08097DA8");
extern u8 CountEncodedTextGlyphs(void *data) asm("func_08098B58");

void ReplaceWindowTextItem(s32 window_id, s32 item_index, u8 *text) asm("func_080989EC");

void ReplaceWindowTextItem(s32 window_id, s32 item_index, u8 *text)
{
    register s32 saved_window_id asm("r12");
    s32 saved_item_index;
    register u8 *text_cursor asm("r2");
    register s32 index asm("r5");
    register u8 value asm("r3");
    register u8 *text_blocks asm("r10");
    register u8 *block_offsets asm("r9");

    {
        register s32 window_id_input asm("r0") = window_id;

        asm volatile("lsl %0, %0, #24\n\tlsr %0, %0, #24"
                     : "+r"(window_id_input));
        saved_window_id = window_id_input;
    }
    saved_item_index = (u8)item_index;
    text_cursor = text;
    index = 0;
    value = *text_cursor;
    text_blocks = gWindowTextBlocks;
    block_offsets = gWindowTextBlockOffsets;

    /* The native copy stops when a byte is no greater than its index. */
    if ((u32)index < (u32)value) {
        register u8 *grid asm("r8") = text_blocks;
        register u8 *row_data asm("r6");
        register s32 grid_offset asm("r4");
        register s32 row_offset asm("r0");
        register u8 *row_base_copy asm("r1");

        row_offset = saved_window_id * WINDOW_TEXT_BLOCK_COUNT;
        asm volatile("add %0, %1, %0"
                     : "+r"(row_offset)
                     : "r"(saved_item_index));
        row_base_copy = block_offsets;
        asm volatile("" : "+r"(row_base_copy));
        asm volatile("add %0, %1, %2"
                     : "=r"(row_data)
                     : "r"(row_offset), "r"(row_base_copy));
        grid_offset = saved_window_id * WINDOW_TEXT_BANK_BYTES;
        do {
            s32 condition;
            register s32 destination asm("r0");

            destination = row_data[0] * WINDOW_TEXT_BLOCK_BYTES;
            asm volatile("add %0, %1, %0"
                         : "+r"(destination)
                         : "r"(index));
            destination += grid_offset;
            destination += (s32)grid;
            *(u8 *)destination = value;
            condition = *text_cursor;
            if (condition <= 3) {
                asm volatile("cmp %0, #2\n\tbge 1f"
                             :
                             : "r"(condition)
                             : "cc");
                if (condition == 1) {
                    register s32 destination asm("r0");

                    text_cursor++;
                    {
                        register s32 next_index asm("r0") = index + 1;

                        asm volatile("" : "+r"(next_index));
                        index = (u8)next_index;
                    }
                    destination = row_data[0] * WINDOW_TEXT_BLOCK_BYTES;
                    asm volatile("add %0, %1, %0"
                                 : "+r"(destination)
                                 : "r"(index));
                    destination += grid_offset;
                    destination += (s32)grid;
                    *(u8 *)destination = *text_cursor;
                }
                asm volatile("1:");
            }
            text_cursor++;
            {
                register s32 next_index asm("r0") = index + 1;

                asm volatile("" : "+r"(next_index));
                index = (u8)next_index;
            }
            value = *text_cursor;
        } while ((u32)index < (u32)value);
    }

    {
        register s32 zero asm("r8");
        register s32 row_stride asm("r6");
        s32 grid_stride;
        register s32 destination asm("r0");

        row_stride = WINDOW_TEXT_BLOCK_COUNT;
        destination = saved_window_id * row_stride;
        asm volatile("add %0, %1, %0"
                     : "+r"(destination)
                     : "r"(saved_item_index));
        destination += (s32)block_offsets;
        destination = *(u8 *)destination * WINDOW_TEXT_BLOCK_BYTES;
        asm volatile("add %0, %1, %0"
                     : "+r"(destination)
                     : "r"(index));
        grid_stride = WINDOW_TEXT_BANK_BYTES;
        destination += saved_window_id * grid_stride;
        destination += (s32)text_blocks;
        zero = 0;
        *(u8 *)destination = zero;

        {
            register WindowTextItemView *window asm("r4") = GetWindow(saved_window_id);

            if ((u32)saved_item_index >= (u32)window->top_item) {
                u8 top_item = window->top_item;
                u32 half_span = window->height - 2;

                if ((s32)saved_item_index < (s32)(top_item + ((s32)(half_span + (half_span >> 31)) >> 1))) {
                    u32 saved_position[2];
                    u8 result;

                    {
                        register u32 text_column asm("r1") = window->text_column;

                        asm volatile("" : "+r"(text_column));
                        saved_position[0] = text_column;
                    }
                    {
                        register u32 text_row asm("r0") = window->text_row;

                        asm volatile("" : "+r"(text_row));
                        saved_position[1] = text_row;
                    }
                    {
                        register s32 zero_copy asm("r1") = zero;

                        asm volatile("" : "+r"(zero_copy));
                        window->text_column = zero_copy;
                    }
                    window->text_row = (saved_item_index - top_item) * 2;
                    {
                        register s32 row_address asm("r0") = window->slot;
                        register s32 grid_address asm("r1") = row_address;
                        register s32 tile asm("r2");

                        asm volatile("" : "+r"(row_address), "+r"(grid_address));
                        grid_address *= grid_stride;
                        row_address *= row_stride;
                        asm volatile("add %0, %1, %0"
                                     : "+r"(row_address)
                                     : "r"(saved_item_index));
                        row_address += (s32)block_offsets;
                        tile = *(u8 *)row_address;
                        row_address = tile * WINDOW_TEXT_BLOCK_BYTES;
                        row_address += (s32)text_blocks;
                        grid_address += row_address;
                        DrawWindowText(window, (void *)grid_address);
                    }
                    {
                        register s32 row_address asm("r1") = window->slot;
                        register s32 grid_address asm("r0") = row_address;
                        register s32 tile asm("r2");

                        asm volatile("" : "+r"(row_address), "+r"(grid_address));
                        grid_address *= grid_stride;
                        row_address *= row_stride;
                        asm volatile("add %0, %1, %0"
                                     : "+r"(row_address)
                                     : "r"(saved_item_index));
                        row_address += (s32)block_offsets;
                        tile = *(u8 *)row_address;
                        row_address = tile * WINDOW_TEXT_BLOCK_BYTES;
                        row_address += (s32)text_blocks;
                        grid_address += row_address;
                        result = CountEncodedTextGlyphs((void *)grid_address);
                    }
                    while ((s32)result < (s32)(window->width - 2)) {
                        DrawWindowText(window, (void *)0x080ED940);
                        result++;
                    }
                    {
                        register u16 restored_c asm("r0");

                        asm volatile("mov %0, sp\n\t"
                                     "ldrh %0, [%0, #0]"
                                     : "=r"(restored_c)
                                     : "m"(saved_position[0]));
                        window->text_column = restored_c;
                    }
                    {
                        register u16 restored_e asm("r1");

                        asm volatile("mov %0, sp\n\t"
                                     "ldrh %0, [%0, #4]"
                                     : "=r"(restored_e)
                                     : "m"(saved_position[1]));
                        window->text_row = restored_e;
                    }
                    window->flags |= WINDOW_FLAG_TILEMAP_DIRTY;
                }
            }
        }
    }
}
