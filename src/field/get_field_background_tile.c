#include "m2c_prelude.h"
#include "field_display.h"
extern u16 gFieldMapDimensions[] asm("D_020324A4");
u16 GetFieldBackgroundTile(u8 layer, s32 tile_x, s32 tile_y) asm("func_0809D5F4");

u16 GetFieldBackgroundTile(u8 layer, s32 tile_x, s32 tile_y) {
    s32 tile_x_high_half;
    register u16 *tile_address asm("r3");
    tile_x &= 0x1FF;
    tile_y &= 0x1FF;
    if (*(u16 *)FIELD_CURRENT_MAP_STATE_RAM == 0) {
        { register s32 tile_x_high_carrier asm("r2") = tile_x << 16; asm volatile("" : "+r"(tile_x_high_carrier)); tile_x_high_half = tile_x_high_carrier; }
        if (layer == 1) {
            tile_x = (u32)(tile_x_high_half + 0xFE940000) >> 16;
            if ((u32)tile_x < 80) { s32 world_overlay_y; { register s32 tile_y_carrier asm("r2") = tile_y; asm volatile("" : "+r"(tile_y_carrier)); world_overlay_y = tile_y_carrier; }
                if (world_overlay_y > 0x103) {
                    if (world_overlay_y <= 0x153) { s32 overlay_tile_y = world_overlay_y - 0x104; u32 overlay_metatile_tiles = FIELD_METATILE_TILES_BUFFER_RAM; asm volatile("" : "+r"(overlay_metatile_tiles));
                    { s32 overlay_x_high_half = tile_x << 16; s32 overlay_tile_x = overlay_x_high_half >> 16; s32 metatile_cell_offset = overlay_tile_x & layer; s32 overlay_y_high_half = overlay_tile_y << 16; s32 overlay_y_signed = overlay_y_high_half >> 16; u32 *background_metatile_maps; u16 metatile_id;
                        metatile_cell_offset = (metatile_cell_offset + (overlay_y_signed & layer) * 2) * 2; background_metatile_maps = (u32 *)FIELD_METATILE_MAPS_RAM;
                        { s32 metatile_x = (overlay_tile_x + (s32)((u32)overlay_x_high_half >> 31)) >> 1; s32 metatile_y = (overlay_y_signed + (s32)((u32)overlay_y_high_half >> 31)) >> 1; metatile_id = *(u16 *)(metatile_y * 80 + (metatile_x * 2 + background_metatile_maps[1])); }
                        tile_address = (u16 *)(metatile_cell_offset + metatile_id * 8 + overlay_metatile_tiles); goto done;
                    }
                    }
                }
            }
            return 0;
        }
    } else {
        s32 tile_x_high_carrier = tile_x << 16; asm volatile("" : "+r"(tile_x_high_carrier));
        { s32 signed_tile_x = tile_x_high_carrier >> 16; u16 *dimensions = gFieldMapDimensions; tile_x_high_half = tile_x_high_carrier;
            if (signed_tile_x >= dimensions[0]) return 0;
            if ((s16)tile_y >= dimensions[1]) return 0;
        }
    }
    { u32 metatile_map_array_address = FIELD_METATILE_MAPS_RAM; register u32 *layer_metatile_map asm("r5"); s32 signed_tile_x; s32 metatile_x; s32 tile_y_high_half; s32 signed_tile_y; s32 metatile_y; s32 cell_column; s32 metatile_cell_offset; u32 metatile_tiles_base; u16 metatile_id;
        asm volatile("" : "+r"(metatile_map_array_address)); layer_metatile_map = (u32 *)((layer << 2) + metatile_map_array_address); signed_tile_x = tile_x_high_half >> 16; metatile_x = (signed_tile_x + (s32)((u32)tile_x_high_half >> 31)) >> 1; tile_y_high_half = tile_y << 16; signed_tile_y = tile_y_high_half >> 16; metatile_y = (signed_tile_y + (s32)((u32)tile_y_high_half >> 31)) >> 1;
        metatile_id = *(u16 *)((gFieldMapDimensions[0] >> 1) * metatile_y * 2 + (metatile_x * 2 + *layer_metatile_map)); cell_column = signed_tile_x & 1; metatile_cell_offset = (signed_tile_y & 1) * 2 + cell_column; metatile_tiles_base = *(u32 *)FIELD_METATILE_TILES_RAM; tile_address = (u16 *)(metatile_cell_offset * 2 + (metatile_id * 8 + metatile_tiles_base));
    }
done: return *tile_address;
}
