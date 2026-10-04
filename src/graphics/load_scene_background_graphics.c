#include "m2c_prelude.h"

M2C_UNK StartTask(s32, M2C_UNK) asm("func_08092D8C");                /* extern */
M2C_UNK ConfigureDisplayWindows(s32, s32, s32, s32, s32, s32, s32, s32) asm("func_0809538C"); /* extern */
M2C_UNK LoadCompressedPalette(s32, M2C_UNK, M2C_UNK) asm("func_0809A1BC");       /* extern */
M2C_UNK LoadSpriteGraphicsFromTable(M2C_UNK, s32, s32, s32) asm("func_0809AA64");      /* extern */
M2C_UNK BiosLz77ToVram(s32, void *) asm("func_080ECD34");                 /* extern */

void LoadSceneBackgroundGraphics(s32 background_id, s32 size_class, s32 graphics_character_block, s32 screen_block, s32 normal_background_character_block) asm("func_0809A5B4");

void LoadSceneBackgroundGraphics(s32 background_id, s32 size_class, s32 graphics_character_block, s32 screen_block, s32 normal_background_character_block) {
    volatile s32 saved_background_character_block;
    s16 *blank_tile_cursor;
    s32 character_block_byte_offset;
    s32 alternate_character_block_byte_offset;
    s32 graphics_record_offset;
    u16 *effect_tilemap_cursor;
    u16 *scene_tilemap_cursor;
    u16 tile_index;
    u32 effect_tilemap_cell;
    u32 scene_tilemap_cell;
    u32 tilemap_row;
    u8 selected_screen_block;
    u8 selected_background_id;
    u32 next_tilemap_row;
    u8 selected_graphics_character_block;
    u8 selected_size_class;
    u8 tilemap_column;

    selected_background_id = background_id;
    selected_size_class = size_class;
    selected_graphics_character_block = graphics_character_block;
    selected_screen_block = screen_block;
    asm volatile("lsl r4, r4, #24\n\tlsr r4, r4, #24" ::: "cc");
    saved_background_character_block = normal_background_character_block;
    if ((u32) selected_background_id <= 0xFCU) {
        {
            register volatile s16 *control asm("r3") = (volatile s16 *)0x0400000C;
            asm volatile("" : "+r"(control));
            asm volatile(
            "mov r0, %1\n\t"
            "lsl r2, r0, #8\n\t"
            "lsl r0, r4, #2\n\t"
            "mov r1, #128\n\t"
            "orr r0, r1\n\t"
            "orr r2, r0\n\t"
            "mov r0, #3\n\t"
            "orr r2, r0\n\t"
            "strh r2, [r3]"
            : "+r"(control)
            : "r"(selected_screen_block)
            : "r0", "r1", "r2", "cc", "memory");
        }
        {
            register u8 *table1 asm("r3") = (u8 *)0x087AF794;
            register s32 computed_offset asm("r2");
            register u32 twice asm("r1");
            asm volatile("" : "+r"(table1));
            twice = selected_background_id * 2;
            asm volatile("" : "+r"(twice));
            asm volatile(
                "add r0, r1, r5\n\t"
                "lsl r2, r0, #3\n\t"
                "mov r5, r1"
                : "=r"(computed_offset)
                : "r"(twice)
                : "r0", "cc");
            if ((u32) selected_size_class > 2U) {
                graphics_record_offset = ((selected_size_class - 2) * 8) + computed_offset;
                asm volatile("" : "+r"(graphics_record_offset));
            } else {
                graphics_record_offset = computed_offset;
                asm volatile("" : "+r"(graphics_record_offset));
            }
            {
                register u8 *address asm("r0") = table1 + graphics_record_offset;
                register s32 source asm("r0");
                register void *dest asm("r1");
                asm volatile("" : "+r"(address));
                source = *(s32 *)address;
                asm volatile("" : "+r"(source));
                asm volatile(
                    "mov r2, r9\n\t"
                    "lsl r1, r2, #14\n\t"
                    "mov r2, #192\n\t"
                    "lsl r2, r2, #19\n\t"
                    "add r1, r1, r2"
                    : "=r"(dest)
                    :
                    : "r2", "cc");
                BiosLz77ToVram(source, dest);
            }
        }
        {
            register u8 *table asm("r2") = (u8 *)0x087AF794;
            register u8 *view asm("r0");
            register s32 table_offset asm("r1");
            asm volatile("" : "+r"(table));
            asm volatile(
                "mov r3, sl\n\t"
                "add r0, r5, r3\n\t"
                "lsl r1, r0, #3"
                : "=r"(table_offset)
                :
                : "r0", "r3", "cc");
            if ((u32) selected_size_class > 2U) {
                asm volatile(
                    "sub r0, r7, #2\n\t"
                    "lsl r0, r0, #3\n\t"
                    "add r1, r0, r1"
                    : "+r"(table_offset)
                    :
                    : "r0", "cc");
            }
            view = table + 4;
            asm volatile("" : "+r"(view));
            LoadCompressedPalette(M2C_FIELD(view, s32 *, table_offset), 0x05000080, 0x02002880);
        }
        {
            register u32 tilemap_high asm("r0") = selected_screen_block;
            register u32 tilemap_offset asm("r1");
            register u32 tilemap_base asm("r0");
            register void *tilemap asm("r4");
            asm volatile("" : "+r"(tilemap_high));
            tilemap_offset = tilemap_high << 0xB;
            asm volatile("" : "+r"(tilemap_offset));
            tilemap_base = 0x06000000;
            asm volatile("" : "+r"(tilemap_base));
            tilemap = (void *)(tilemap_offset + tilemap_base);
            asm volatile("" : "+r"(tilemap));
            {
                register s32 graphics_character_block_value asm("r1") = selected_graphics_character_block;
                register s32 background_character_block_value asm("r2") = saved_background_character_block;
                register s32 character_block_delta asm("r0");
                asm volatile("" : "+r"(graphics_character_block_value));
                asm volatile("" : "+r"(background_character_block_value));
                character_block_delta = graphics_character_block_value - background_character_block_value;
                asm volatile("" : "+r"(character_block_delta));
                tile_index = character_block_delta << 8;
            }
            tilemap_row = 0;
            do {
                tilemap_column = 0;
                next_tilemap_row = tilemap_row + 1;
loop_8:
                if (tilemap_row <= 0xFU) {
                    M2C_FIELD(tilemap, u16 *, 0) = tile_index;
                    M2C_FIELD(tilemap, u16 *, 0x20) = tile_index;
                    tile_index += 1;
                } else {
                    M2C_FIELD(tilemap, u16 *, 0x20) = 0U;
                    M2C_FIELD(tilemap, u16 *, 0) = 0U;
                }
                tilemap += 2;
                tilemap_column += 1;
                if ((u32) tilemap_column <= 0xFU) {
                    goto loop_8;
                }
                tilemap += 0x20;
                tilemap_row = (u8) next_tilemap_row;
            } while (tilemap_row <= 0x1FU);
        }
        return;
    }
    if ((u32) (u8) (selected_background_id + 3) <= 1U) {
        register u32 clear_count asm("r1");
        register s16 *clear_ptr asm("r4");
        register u32 clear_zero asm("r2");
        register u32 fill_end asm("r5");
        register u32 fill_mask asm("r3");
        register u32 fill_threshold asm("r6");
        register u32 fill_value asm("r2");
        register u32 tilemap_offset asm("r5");
        *(s16 *)0x0400000C = ({
            register u32 display_config asm("r0");
            asm volatile(
                "mov r3, %1\n\t"
                "lsl r0, r3, #8\n\t"
                "lsl r1, %2, #2\n\t"
                "orr r0, r1\n\t"
                "mov r1, #3\n\t"
                "orr r0, r1"
                : "=r"(display_config)
                : "r"(selected_screen_block), "r"(selected_graphics_character_block)
                : "r1", "r3", "cc");
            display_config;
        });
        BiosLz77ToVram(0x08421FCC,
                      (void *)((character_block_byte_offset = selected_graphics_character_block << 0xE) + 0x06000000));
        clear_ptr = (s16 *)character_block_byte_offset;
        asm volatile("" : "+r"(clear_ptr));
        {
            register u32 clear_base asm("r0") = 0x06000200;
            asm volatile("" : "+r"(clear_base));
            asm volatile("add r4, r4, r0"
                         : "+r"(clear_ptr)
                         : "r"(clear_base)
                         : "cc");
        }
        clear_count = 0;
        asm volatile("" : "+r"(clear_count));
        {
            register u32 tilemap_high asm("r0") = selected_screen_block;
            asm volatile("" : "+r"(tilemap_high));
            tilemap_offset = tilemap_high << 0xB;
            asm volatile("" : "+r"(tilemap_offset));
        }
        clear_zero = 0;
        asm volatile("" : "+r"(clear_zero));
        do {
            *clear_ptr = clear_zero;
            clear_ptr += 1;
            asm volatile(
                "add r0, r1, #1\n\t"
                "lsl r0, r0, #16\n\t"
                "lsr r1, r0, #16"
                : "+r"(clear_count)
                :
                : "r0", "cc");
        } while (clear_count <= 0xFU);
        BiosLz77ToVram(0x084220B4, (u16 *)0x05000080);
        BiosLz77ToVram(0x084220DC, ({
            effect_tilemap_cursor = (u16 *)0x06000000;
            asm volatile("" : "+r"(effect_tilemap_cursor));
            effect_tilemap_cursor = (u16 *)(tilemap_offset - (0U - (u32)effect_tilemap_cursor));
            effect_tilemap_cursor;
        }));
        effect_tilemap_cell = 0;
        fill_threshold = 0x27F;
        asm volatile("" : "+r"(fill_threshold));
        fill_end = 0x3FF;
        asm volatile("" : "+r"(fill_end));
        asm volatile(
            "mov r2, #128\n\t"
            "lsl r2, r2, #7\n\t"
            "mov r3, r2\n\t"
            "mov r2, #16"
            : "=r"(fill_mask), "=r"(fill_value)
            :
            : "cc");
        do {
            if (effect_tilemap_cell <= fill_threshold) {
                *effect_tilemap_cursor |= fill_mask;
            } else {
                *effect_tilemap_cursor = fill_value;
            }
            effect_tilemap_cursor += 1;
            effect_tilemap_cell = (u32) (u16) (effect_tilemap_cell + 1);
        } while (effect_tilemap_cell <= fill_end);
        {
            register u32 mode asm("r3") = selected_background_id;
            asm volatile("" : "+r"(mode));
            if (mode == 0xFD) {
                LoadSpriteGraphicsFromTable(0x087AF9D4, 9, 0x180, 3);
            }
        }
    } else {
        register u32 mode_ff asm("r0") = selected_background_id;
        asm volatile("" : "+r"(mode_ff));
        if (mode_ff == 0xFF) {
        register u32 clear_count_2 asm("r1");
        register u32 char_block_2 asm("r3");
        {
            register volatile s16 *control asm("r2") = (volatile s16 *)0x0400000C;
            register u32 config asm("r0");
            register u32 high asm("r1") = selected_screen_block;
            asm volatile("" : "+r"(control));
            asm volatile("" : "+r"(high));
            config = high << 8;
            asm volatile("" : "+r"(config));
            char_block_2 = selected_graphics_character_block;
            asm volatile("" : "+r"(char_block_2));
            {
                register u32 low asm("r1");
                low = char_block_2 << 2;
                asm volatile("" : "+r"(low));
                config |= low;
            }
            config |= 3;
            *control = config;
        }
        {
            s32 *state = (s32 *)0x03000054;
            state[1] = 0x2000;
            state[5] = 0xFFFFF000;
        }
        BiosLz77ToVram(0x083C71E0, ({
            register void *graphics_dest asm("r1");
            register s32 graphics_block asm("r4");
            asm volatile(
                "lsl r4, r3, #14\n\t"
                "mov r1, #192\n\t"
                "lsl r1, r1, #19\n\t"
                "add r1, r4, r1"
                : "=r"(graphics_block), "=r"(graphics_dest)
                : "r"(char_block_2)
                : "cc");
            alternate_character_block_byte_offset = graphics_block;
            graphics_dest;
        }));
        blank_tile_cursor = alternate_character_block_byte_offset + 0x060007A0;
        clear_count_2 = 0;
        asm volatile("" : "+r"(clear_count_2));
        asm volatile(
            "mov r0, %0\n\t"
            "lsl r5, r0, #11"
            :
            : "r"(selected_screen_block)
            : "r0", "r5", "cc");
        do {
            *blank_tile_cursor = 0;
            blank_tile_cursor += 1;
            asm volatile(
                "add r0, r1, #1\n\t"
                "lsl r0, r0, #16\n\t"
                "lsr r1, r0, #16"
                : "+r"(clear_count_2)
                :
                : "r0", "cc");
        } while (clear_count_2 <= 0xFU);
        BiosLz77ToVram(0x083C77FC, ({
            scene_tilemap_cursor = (u16 *)0x06000000;
            asm volatile("" : "+r"(scene_tilemap_cursor));
            asm volatile("add r4, r5, r4"
                         : "+r"(scene_tilemap_cursor)
                         :
                         : "cc");
            scene_tilemap_cursor;
        }));
        {
        register u32 fill_end_2 asm("r2");
        register u32 fill_mask_2 asm("r5");
        register u32 fill_value_2 asm("r3");
        scene_tilemap_cell = 0;
        asm volatile(
            "mov r2, #128\n\t"
            "lsl r2, r2, #7\n\t"
            "mov r5, r2\n\t"
            "mov r3, #61"
            : "=r"(fill_mask_2), "=r"(fill_value_2)
            :
            : "r2", "cc");
        fill_end_2 = 0x3FF;
        asm volatile("" : "+r"(fill_end_2));
        do {
            if (scene_tilemap_cell <= 0xFFU) {
                *scene_tilemap_cursor |= fill_mask_2;
            } else {
                *scene_tilemap_cursor = fill_value_2;
            }
            scene_tilemap_cursor += 1;
            scene_tilemap_cell = (u32) (u16) (scene_tilemap_cell + 1);
        } while (scene_tilemap_cell <= fill_end_2);
        }
        *(s16 *)0x05000000 = 0;
        ConfigureDisplayWindows(1, 0xF0, 0x10, 1, 0xF0, 0x5060, 0x808, 0x3F);
        *(s8 *)0x02033F54 = 0;
        StartTask(7, 0x080CFD99);
        }
    }
}
