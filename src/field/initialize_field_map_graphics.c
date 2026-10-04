#include "m2c_prelude.h"
#include "field_display.h"

M2C_UNK ClearTransferQueue() asm("func_080951E8");
M2C_UNK InitializeWindowGraphics(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08096FBC");
M2C_UNK LoadCompressedPalette(s32, s32, M2C_UNK) asm("func_0809A1BC");
s16 GetFieldBackgroundTile(u8, s16, s16) asm("func_0809D5F4");
M2C_UNK ResetFieldMapDecorationSprites() asm("func_0809D700");
s32 TestEventFlag(s32) asm("func_0809F818");
M2C_UNK CreateWorldMapStructureSprite() asm("func_080A6148");
M2C_UNK BiosLz77ToVram(s32, M2C_UNK) asm("func_080ECD34");
M2C_UNK BiosLz77ToWram(s32, s32) asm("func_080ECD38");

extern u8 gFieldMapDefinitions[] asm("D_087C4434");
extern s32 gFieldBackgroundTilemaps[] asm("D_02032E88");
extern s32 gFieldMetatileTiles asm("D_02032E90");
extern s32 gFieldMapCollisionCells asm("D_02032E94");
extern s32 gWorldMapStructureSprite asm("D_020314A0");
extern s16 gFieldStreamedTileOrigins[] asm("D_0203249C");
extern u16 gFieldMapDimensions[] asm("D_020324A4");
extern u8 gFieldMapModeFlags asm("D_020324B0");
extern u16 D_0300004C;
extern u8 gFieldCameraScrollOffsets[] asm("D_03000054");
extern u8 gFieldMapState[] asm("D_0202ECF4");
extern u8 gGameCompletionFlag asm("D_02021770");

void InitializeFieldMapGraphics(s32 map_id, s32 center_x_fixed8, s32 center_y_fixed8) asm("func_0809D938");

void InitializeFieldMapGraphics(s32 map_id, s32 center_x_fixed8, s32 center_y_fixed8) {
    u8 *map_definition_bytes;
    u8 *tile_origin_bytes;
    u8 *background_scroll_bytes;
    s32 saved_map_id;
    s32 scroll_x_fixed8;
    s32 scroll_y_fixed8;
    s32 tile_origin_x_bits;
    s32 tile_origin_y_bits;
    s32 tile_origin_x_signed;
    s32 tile_origin_y_signed;
    s32 next_layer;
    s32 row_high_half;
    s32 world_tile_y;
    s32 world_tile_x;
    s16 column_signed;
    s16 column;
    s16 row;
    s32 maximum_scroll_x;
    s32 bg0_metatile_map_source;
    s32 map_definition_offset_reloaded;
    s32 display_control;
    s32 tile_origin_x;
    s32 tile_origin_y;
    s32 bg1_metatile_map_source;
    s32 centered_scroll_x;
    s32 map_definition_offset;
    s32 clamped_scroll_y;
    s32 screen_row_offset;
    register s32 world_tile_y_high_half asm("r8");
    register s32 screen_tile_mask asm("r10");
    s32 map_definition_offset_carrier;
    register s32 screen_map_base asm("r9");
    u16 next_column;
    u32 next_row_high_half;
    u8 layer;

    saved_map_id = map_id;
    scroll_x_fixed8 = center_x_fixed8;
    scroll_y_fixed8 = center_y_fixed8;
    ClearTransferQueue();
    background_scroll_bytes = gFieldCameraScrollOffsets;
    InitializeWindowGraphics(3, 0, 0x100, 0xC0, 0x1C0, 3, 0xE, 0, 0x3E6, 0xF);
    map_definition_bytes = gFieldMapDefinitions;
    map_definition_offset = map_id << 5;
    { u8 *q4; q4 = map_definition_bytes + 4; BiosLz77ToVram(*(s32 *)(map_definition_offset + q4), 0x06004000); }
    { u8 *q8; q8 = map_definition_bytes + 8; LoadCompressedPalette(*(s32 *)(map_definition_offset + q8), 0x05000000, 0x02002880); }
    if (saved_map_id == 0) {
        gFieldBackgroundTilemaps[0] = (s32) *(s32 *)(map_definition_bytes + 0xC);
        gFieldBackgroundTilemaps[1] = saved_map_id;
        gFieldMetatileTiles = *(s32 *)(map_definition_bytes + 0x14);
        gFieldMapCollisionCells = *(s32 *)(map_definition_bytes + 0x18);
        if (gFieldMapState[0x1E] != 0) {
            CreateWorldMapStructureSprite();
            map_definition_offset_carrier = 0;
        } else {
            gWorldMapStructureSprite = saved_map_id;
            map_definition_offset_carrier = 0;
        }
    } else {
        { u8 *qC; qC = map_definition_bytes + 0xC; bg0_metatile_map_source = *(s32 *)(map_definition_offset + qC); }
        if (bg0_metatile_map_source != 0) {
            BiosLz77ToWram(bg0_metatile_map_source, 0x02032E98);
            gFieldBackgroundTilemaps[0] = 0x02032E98;
        } else {
            gFieldBackgroundTilemaps[0] = bg0_metatile_map_source;
        }
        { u8 *b2; u8 *q10; b2 = gFieldMapDefinitions; map_definition_offset_reloaded = saved_map_id << 5; q10 = b2 + 0x10; bg1_metatile_map_source = *(s32 *)(map_definition_offset_reloaded + q10); }
        map_definition_offset_carrier = map_definition_offset_reloaded;
        if (bg1_metatile_map_source != 0) {
            BiosLz77ToWram(bg1_metatile_map_source, 0x02034E98);
            gFieldBackgroundTilemaps[1] = 0x02034E98;
        } else {
            gFieldBackgroundTilemaps[1] = bg1_metatile_map_source;
        }
        { u8 *b3; u8 *q14; u8 *q18; b3 = gFieldMapDefinitions; q14 = b3 + 0x14; BiosLz77ToWram(*(s32 *)(map_definition_offset_carrier + q14), FIELD_METATILE_TILES_BUFFER_RAM); q18 = b3 + 0x18; BiosLz77ToWram(*(s32 *)(map_definition_offset_carrier + q18), 0x0203AE98); }
        gFieldMetatileTiles = FIELD_METATILE_TILES_BUFFER_RAM;
        gFieldMapCollisionCells = 0x0203AE98;
        gWorldMapStructureSprite = 0;
    }
    if ((saved_map_id == 0) || (saved_map_id == 0x40)) {
        D_0300004C = (u16) ((0xE000 & D_0300004C) | 0x1940);
        if (1 & gFieldMapModeFlags) {
            BiosLz77ToVram(0x08498FA0, 0x0600C000);
            BiosLz77ToVram(0x08499A44, 0x06000800);
            D_0300004C = (u16) (D_0300004C | 0x200);
            *(s16 *)0x0400000A = 0x10E;
        }
        if (saved_map_id == 0) {
            BiosLz77ToVram(0x08499C1C, 0x06001000);
            D_0300004C = (u16) (D_0300004C | 0x400);
            *(s16 *)0x0400000C = 0x207;
            if ((1 & gFieldMapModeFlags) && ((TestEventFlag(0) << 0x18) != 0) && (gGameCompletionFlag == 0)) {
                BiosLz77ToWram(0x08499D38, 0x02032E98);
                gFieldBackgroundTilemaps[1] = 0x02032E98;
                BiosLz77ToWram(0x0849A044, FIELD_METATILE_TILES_BUFFER_RAM);
                *(s16 *)0x0400000E = 0x1F0E;
                { u8 *d; d = gFieldCameraScrollOffsets; *(s32 *)(d + 0x18) = scroll_x_fixed8; *(s32 *)(d + 0x1C) = scroll_y_fixed8; }
                gFieldMapModeFlags = (u8) (gFieldMapModeFlags | 4);
            }
        }
        *(s16 *)0x04000008 = 7;
        ResetFieldMapDecorationSprites();
    } else {
        display_control = (0xE000 & D_0300004C) | 0x1840;
        D_0300004C = display_control;
        if (gFieldBackgroundTilemaps[0] != 0) {
            D_0300004C = display_control | 0x100;
            *(s16 *)0x04000008 = 7;
        }
        if (gFieldBackgroundTilemaps[1] != 0) {
            D_0300004C |= 0x200;
            *(s16 *)0x0400000A = 0x106;
        }
        gFieldMapModeFlags = 0;
    }
    { u16 *d; u8 *w; d = gFieldMapDimensions; w = gFieldMapDefinitions + map_definition_offset_carrier; d[0] = (u16) *(u16 *)w; d[1] = (u16) *(u16 *)(w + 2); }
    centered_scroll_x = scroll_x_fixed8 + 0xFFFF8800;
    scroll_x_fixed8 = centered_scroll_x;
    scroll_y_fixed8 += 0xFFFFB000;
    if (saved_map_id != 0) {
        if (centered_scroll_x < 0) {
            scroll_x_fixed8 = 0;
        } else {
            maximum_scroll_x = (gFieldMapDimensions[0] - 0x1E) << 0xB;
            if (scroll_x_fixed8 > maximum_scroll_x) {
                scroll_x_fixed8 = maximum_scroll_x;
            }
        }
        if (scroll_y_fixed8 < 0) {
            clamped_scroll_y = 0;
            goto block_39;
        }
        clamped_scroll_y = (gFieldMapDimensions[1] - 0x14) << 0xB;
        if (scroll_y_fixed8 > clamped_scroll_y) {
block_39:
            scroll_y_fixed8 = clamped_scroll_y;
        }
    }
    tile_origin_bytes = (u8 *)gFieldStreamedTileOrigins;
    tile_origin_x = scroll_x_fixed8 >> 0xB;
    *(s16 *)(tile_origin_bytes + 4) = (s16) tile_origin_x;
    *(s16 *)(tile_origin_bytes + 0) = (s16) tile_origin_x;
    tile_origin_x_bits = (s32) (u16) tile_origin_x;
    tile_origin_y = scroll_y_fixed8 >> 0xB;
    *(s16 *)(tile_origin_bytes + 6) = (s16) tile_origin_y;
    *(s16 *)(tile_origin_bytes + 2) = (s16) tile_origin_y;
    tile_origin_y_bits = (s32) (u16) tile_origin_y;
    layer = 0;
    do {
        { s32 c; c = gFieldBackgroundTilemaps[layer]; next_layer = layer + 1;
        if (c != 0) {
            if ((saved_map_id != 0) || (layer == 0)) {
                screen_map_base = (layer << 0xB) + 0x06000000;
            } else {
                screen_map_base = 0x0600F800;
            }
            *(s32 *)(background_scroll_bytes + (layer * 8)) = scroll_x_fixed8;
            *(s32 *)(background_scroll_bytes + (({ s32 ix; ix = (layer * 2) + 1; ix; }) * 4)) = scroll_y_fixed8;
            row = 0;
            { s32 t0; s32 t1; t0 = tile_origin_x_bits << 0x10; t1 = tile_origin_y_bits << 0x10; asm volatile("" : "+r"(layer)); next_layer = layer + 1; tile_origin_x_signed = t0 >> 0x10; tile_origin_y_signed = t1 >> 0x10; }
            screen_tile_mask = 0x1F;
            do {
                column = 0;
                row_high_half = row << 0x10;
                world_tile_y = tile_origin_y_signed + (row_high_half >> 0x10);
                world_tile_y_high_half = world_tile_y << 0x10;
                screen_row_offset = (world_tile_y & screen_tile_mask) << 6;
                do {
                    column_signed = column;
                    world_tile_x = tile_origin_x_signed + column_signed;
                    { u8 z; s32 a1; a1 = (s32) (s16) world_tile_x; z = layer; asm volatile("" : "+r"(z));
                    *(s16 *)(screen_row_offset + (((world_tile_x & screen_tile_mask) * 2) + screen_map_base)) = GetFieldBackgroundTile(z, a1, (s16) (world_tile_y_high_half >> 0x10)); }
                    next_column = column_signed + 1;
                    column = (s16) next_column;
                } while ((s32) (s16) next_column <= 0x1E);
                next_row_high_half = row_high_half + 0x10000;
                row = (s16) (next_row_high_half >> 0x10);
            } while ((s32) ((s32) next_row_high_half >> 0x10) <= 0x14);
        }
        }
        layer = (u8) next_layer;
    } while ((u32) layer <= 1U);
}
