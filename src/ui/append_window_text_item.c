#include "m2c_prelude.h"
#include "window.h"
extern u8 gWindowTextBlockOffsets[] asm("D_0200DE90");
extern u8 gWindowTextItemCounts[] asm("D_0200E6C4");
extern u8 gWindowTextBlocks[] asm("D_0200E6CE");
extern s32 *GetWindow(u8) asm("func_0809716C");
extern void DrawAppendedWindowTextItem(void) asm("func_0809885C");

void AppendWindowTextItem(s32 window_id, u8 *text) asm("func_080988C8");

void AppendWindowTextItem(s32 window_id, u8 *text)
{
    register u8 *text_cursor asm("r2");
    register s32 saved_window_id asm("r5");
    register u8 *block_offsets asm("r10");
    u8 byte_count;
    u8 value;

    text_cursor = text;
    saved_window_id = (u8)((u32)window_id << 24 >> 24);
    byte_count = 0;
    value = *text_cursor;
    block_offsets = gWindowTextBlockOffsets;
    if (value != 0) {
        register u8 *text_blocks asm("r9");
        register u8 *saved_block_offsets asm("r8");
        register u8 *item_count asm("r12");
        s32 lookup_bank_offset;
        register s32 text_bank_offset asm("r6");
        s32 condition;
        s32 lookup_index;

        text_blocks = gWindowTextBlocks;
        saved_block_offsets = block_offsets;
        item_count = gWindowTextItemCounts + saved_window_id;
        lookup_bank_offset = saved_window_id * WINDOW_TEXT_BLOCK_COUNT;
        text_bank_offset = saved_window_id * WINDOW_TEXT_BANK_BYTES;
        do {
            {
                register u8 *index_entry asm("r1");

                index_entry = item_count;
                asm volatile("" : "+r"(index_entry));
                lookup_index = *index_entry + lookup_bank_offset;
            }
            {
                register s32 destination asm("r0");

                destination = saved_block_offsets[lookup_index] * WINDOW_TEXT_BLOCK_BYTES;
                asm volatile("add %0, %1, %0"
                             : "+r"(destination)
                             : "r"(byte_count));
                text_blocks[destination + text_bank_offset] = value;
            }
            condition = *text_cursor;
            if (condition > 3) {
                goto next_character;
            }
            asm volatile("cmp %0, #2\n\tbge 1f"
                         :
                         : "r"(condition)
                         : "cc");
            if (condition != 1) {
                goto next_character;
            }
            text_cursor += 1;
            byte_count += 1;
            {
                register u8 *index_entry asm("r1");
                register s32 destination asm("r0");

                index_entry = item_count;
                asm volatile("" : "+r"(index_entry));
                lookup_index = *index_entry + lookup_bank_offset;
                destination = byte_count + saved_block_offsets[lookup_index] * WINDOW_TEXT_BLOCK_BYTES + text_bank_offset;
                asm volatile("add %0, %1"
                             : "+r"(destination)
                             : "r"(text_blocks));
                *(u8 *)destination = *text_cursor;
            }
            asm volatile("1:");
next_character:
            text_cursor += 1;
            byte_count += 1;
            value = *text_cursor;
        } while (value != 0);
    }
    {
        register u8 *table_base asm("r0");
        register u8 *item_count asm("r3");
        register s32 lookup_bank_offset asm("r2");
        s32 lookup_index;

        table_base = gWindowTextItemCounts;
        asm volatile("add %0, %1, %2"
                     : "=r"(item_count)
                     : "r"(saved_window_id), "r"(table_base));
        lookup_index = *item_count;
        lookup_bank_offset = saved_window_id * WINDOW_TEXT_BLOCK_COUNT;
        lookup_index += lookup_bank_offset;
        {
            register s32 destination asm("r0");

            destination = block_offsets[lookup_index] * WINDOW_TEXT_BLOCK_BYTES;
            asm volatile("add %0, %1, %0"
                         : "+r"(destination)
                         : "r"(byte_count));
            gWindowTextBlocks[destination + saved_window_id * WINDOW_TEXT_BANK_BYTES] = 0;
        }
        if (byte_count <= 0x24) {
            register s32 table_value asm("r0");
            register s32 destination_slot asm("r1");

            table_value = *item_count;
            destination_slot = lookup_bank_offset + 1;
            asm volatile("add %0, %1, %0"
                         : "+r"(destination_slot)
                         : "r"(table_value));
            block_offsets[destination_slot] = block_offsets[table_value + lookup_bank_offset] + 1;
        } else {
            register s32 table_value asm("r0");
            register s32 destination_slot asm("r1");

            table_value = *item_count;
            destination_slot = lookup_bank_offset + 1;
            asm volatile("add %0, %1, %0"
                         : "+r"(destination_slot)
                         : "r"(table_value));
            block_offsets[destination_slot] = block_offsets[table_value + lookup_bank_offset] + 2;
        }
    }
    {
        s32 *window;

        window = GetWindow(saved_window_id);
        DrawAppendedWindowTextItem();
        {
            register u8 *counter asm("r1");

            counter = gWindowTextItemCounts + saved_window_id;
            *counter += 1;
        }
        *window |= WINDOW_FLAG_TILEMAP_DIRTY;
    }
}
