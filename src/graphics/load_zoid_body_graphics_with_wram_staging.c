#include "m2c_prelude.h"
#include "graphics_resources.h"

extern void BiosLz77ToVram(void *, u32) asm("func_080ECD34");
extern void BiosLz77ToWram(void *, void *) asm("func_080ECD38");
extern void BiosCpuFastSet(void *, u32, u32) asm("func_080ECD28");
extern void LoadCompressedPalette(void *, u32, void *) asm("func_0809A1BC");
extern s32 CreateSpriteGroup(s32, void *, void *) asm("func_08095098");

extern struct ZoidBodyGraphics gZoidBodyGraphicsTable[] asm("D_087A5CF0");
extern u8 gZoidBodyBgPalettes[] asm("D_087A641C");
extern u8 gZoidBodyObjPalettes[] asm("D_087A8F24");
extern u8 gZoidBodySpriteGroupCallbacks[] asm("D_087A3164");

s32 LoadZoidBodyGraphicsWithWramStaging(s32 zoid_id, s32 palette_variant, s32 char_block, s32 screen_block, s32 mirrored, u8 *work_buffer) asm("func_0809A35C");

s32 LoadZoidBodyGraphicsWithWramStaging(s32 zoid_id, s32 palette_variant, s32 char_block, s32 screen_block, s32 mirrored, u8 *work_buffer)
{
    s32 body_zoid_id;
    s32 body_palette_variant;
    s32 destination_char_block;
    register s32 destination_screen_block asm("r5");
    s32 mirror_flag;
    register s32 two asm("r8");
    register s32 body_graphics_offset asm("r7");
    s32 two_saved;
    register u32 base asm("r6");
    u32 tiles;
    register u8 *table asm("r9");
    u8 *bg_palette_table;
    u8 *obj_palette_table;
    u8 *sprite_group_table;
    s32 sprite_group_offset;
    void *sprite_init_callback;
    void *sprite_update_callback;
    void *obj_tiles;
    s32 sprite_group_flags;
    s32 zero;
    register u8 *staging_buffer asm("sl");
    register s32 raw_mirror_flag asm("r2");

    destination_screen_block = screen_block;
    raw_mirror_flag = mirrored;
    staging_buffer = work_buffer;
    asm volatile("" : "+r"(destination_screen_block), "+r"(raw_mirror_flag), "+r"(staging_buffer));
    body_zoid_id = (u8)zoid_id;
    body_palette_variant = (u8)palette_variant;
    destination_char_block = (u8)char_block;
    destination_screen_block = (u8)destination_screen_block;
    mirror_flag = (u8)raw_mirror_flag;

    table = (u8 *)gZoidBodyGraphicsTable;
    two = body_zoid_id * 2;
    body_graphics_offset = (two + body_zoid_id) << 2;
    {
        register u32 table_call asm("r2");
        register u32 entry asm("r0");

        table_call = (u32)table;
        asm volatile("" : "+r"(table_call));
        entry = body_graphics_offset + table_call;
        BiosLz77ToWram(*(void **)(u32)entry, staging_buffer);
    }
    tiles = destination_char_block << 14;
    base = 0x06000000;
    tiles += base;
    BiosCpuFastSet(staging_buffer, tiles, 0x1000);
    bg_palette_table = gZoidBodyBgPalettes;
    LoadCompressedPalette(*(void **)(body_zoid_id * 32 + body_palette_variant * 4 + bg_palette_table), 0x05000000, staging_buffer);

    destination_screen_block <<= 11;
    tiles = destination_screen_block + base;
    if (mirror_flag == 0) {
        u32 t4;

        t4 = (u32)table + (s32)&((struct ZoidBodyGraphics *)0)->bg_tilemap;
        BiosLz77ToVram(*(void **)(body_graphics_offset + t4), tiles);
        two_saved = two;
    } else {
        u32 row, column;
        register u32 next_i asm("r6");
        u32 tilemap_row_offset;
        u32 v;
        register u32 last_column asm("r1");
        register u32 mask_seed asm("r2");
        u32 mask;
        u32 t4b;

        t4b = (u32)table + (s32)&((struct ZoidBodyGraphics *)0)->bg_tilemap;
        BiosLz77ToWram(*(void **)(body_graphics_offset + t4b), staging_buffer);
        row = 0;
        two_saved = two;
        last_column = 31;
        mask_seed = 0x400;
        mask = mask_seed;
        asm volatile("" : "+r"(mask_seed));
        do {
            column = 0;
            next_i = row + 1;
            tilemap_row_offset = row << 6;
            do {
                v = *(u16 *)(tilemap_row_offset + ((last_column - column) * 2 + (u32)staging_buffer));
                v ^= mask;
                *(u16 *)tiles = v;
                tiles += 2;
                column = (u16)(column + 1);
            } while (column <= 31);
            row = (u16)next_i;
            asm volatile("" :: "r"(next_i));
        } while (row <= 15);
    }

    zero = 0;
    BiosCpuFastSet(&zero, destination_screen_block + 0x06000800, 0x01000200);
    {
        u8 *base;
        s32 t;

        base = (u8 *)gZoidBodyGraphicsTable;
        t = (two_saved + body_zoid_id) * 4;
        base += (s32)&((struct ZoidBodyGraphics *)0)->obj_tiles;
        obj_tiles = *(void **)(base + t);
    }
    if (obj_tiles != 0) {
        BiosLz77ToVram(obj_tiles, 0x06010000);
        obj_palette_table = gZoidBodyObjPalettes;
        LoadCompressedPalette(*(void **)(body_zoid_id * 32 + body_palette_variant * 4 + obj_palette_table), 0x05000200, staging_buffer);
        sprite_group_flags = mirror_flag ? 2 : 0;
        sprite_group_table = gZoidBodySpriteGroupCallbacks;
        sprite_group_offset = body_zoid_id * 8;
        sprite_init_callback = *(void **)(sprite_group_table + sprite_group_offset);
        sprite_group_table += 4;
        sprite_update_callback = *(void **)(sprite_group_table + sprite_group_offset);
        return CreateSpriteGroup(sprite_group_flags, sprite_init_callback, sprite_update_callback);
    }
    return 0;
}
