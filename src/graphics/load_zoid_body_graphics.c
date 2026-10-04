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

s32 LoadZoidBodyGraphics(s32 zoid_id, s32 palette_variant, u32 char_block, s32 screen_block, s32 mirrored, u8 *work_buffer) asm("func_0809A1F8");

s32 LoadZoidBodyGraphics(s32 zoid_id, s32 palette_variant, u32 char_block, s32 screen_block, s32 mirrored, u8 *work_buffer)
{
    s32 body_zoid_id;
    s32 body_palette_variant;
    u32 destination_char_block;
    s32 mirror_flag;
    s32 two;
    s32 body_graphics_offset;
    s32 two_saved;
    u32 vrambase;
    u8 *table;
    u8 *bg_palette_table;
    u8 *obj_palette_table;
    u8 *sprite_group_table;
    s32 sprite_group_offset;
    void *sprite_init_callback;
    void *sprite_update_callback;
    void *obj_tiles;
    s32 sprite_group_flags;
    s32 zero;

    body_zoid_id = (u8)zoid_id;
    body_palette_variant = (u8)palette_variant;
    destination_char_block = char_block << 24;
    screen_block = screen_block << 24;
    screen_block = (u32)screen_block >> 24;
    mirror_flag = (u8)mirrored;
    asm volatile("" :: "r"(screen_block), "r"(screen_block), "r"(screen_block));

    table = (u8 *)gZoidBodyGraphicsTable;
    two = body_zoid_id * 2;
    body_graphics_offset = (two + body_zoid_id) << 2;
    BiosLz77ToVram(*(void **)(table + body_graphics_offset), (destination_char_block >> 10) + (vrambase = 0x06000000));
    bg_palette_table = gZoidBodyBgPalettes;
    LoadCompressedPalette(*(void **)(body_zoid_id * 32 + body_palette_variant * 4 + bg_palette_table), 0x05000000, work_buffer);

    screen_block <<= 11;
    vrambase += screen_block;
    if (mirror_flag == 0) {
        u8 *t4;

        t4 = table + (s32)&((struct ZoidBodyGraphics *)0)->bg_tilemap;
        BiosLz77ToVram(*(void **)(t4 + body_graphics_offset), vrambase);
        two_saved = two;
    } else {
        u32 row, column;
        u32 next_i;
        u32 tilemap_row_offset;
        u32 v;
        u32 last_column;
        u32 m0;
        u32 mask;

        u8 *t4b;

        t4b = table + (s32)&((struct ZoidBodyGraphics *)0)->bg_tilemap;
        BiosLz77ToWram(*(void **)(t4b + body_graphics_offset), work_buffer);
        row = 0;
        two_saved = two;
        last_column = 31;
        {
            register u32 m0r asm("r3");
            m0r = 0x400;
            asm volatile("" : "+r"(m0r));
            mask = m0r;
        }
        do {
            column = 0;
            next_i = row + 1;
            tilemap_row_offset = row << 6;
            do {
                v = *(u16 *)(tilemap_row_offset + ((last_column - column) * 2 + (u32)work_buffer));
                v ^= mask;
                *(u16 *)vrambase = v;
                vrambase += 2;
                column = (u16)(column + 1);
            } while (column <= 31);
            row = (u16)next_i;
        } while (row <= 15);
        asm volatile("");
        asm volatile("");
        asm volatile("");
        asm volatile("");
        asm volatile("");
        asm volatile("");
        asm volatile("");
        asm volatile("" :: "r"(mask));
    }

    zero = 0;
    {
        register u32 k asm("r1");
        k = 0x06000400;
        asm volatile("" : "+r"(k));
        BiosCpuFastSet(&zero, screen_block + k, 0x01000300);
    }
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
        LoadCompressedPalette(*(void **)(body_zoid_id * 32 + body_palette_variant * 4 + obj_palette_table), 0x05000200, work_buffer);
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
