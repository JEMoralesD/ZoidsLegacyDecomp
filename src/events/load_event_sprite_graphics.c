#include "m2c_prelude.h"
#include "event_script.h"

extern u8 gEventSpriteResourceCount asm("D_020317DA");
extern u16 gEventSpriteResourceIds[] asm("D_020317DC");
extern u16 gEventSpriteTileOffsets[] asm("D_020317FC");
extern u16 gEventSpritePaletteBanks[] asm("D_0203181C");
extern u16 gEventSpriteTileBoundary asm("D_0203183C");
extern u16 gEventSpritePaletteBoundary asm("D_0203183E");

void LoadSpriteGraphicsFromTable(s32, s32, s32, s32) asm("func_0809AA64");


void LoadEventSpriteGraphics(u8 resource_id) asm("func_0809F8A0");

void LoadEventSpriteGraphics(u8 resource_id)
{
    register u32 resource_id_r6 asm("r6") = resource_id;
    register u32 loaded_resource_count asm("r0") = gEventSpriteResourceCount;

    if (loaded_resource_count <= 15) {
        u8 resource_index = 0;

        if (resource_index < loaded_resource_count) {
            register u16 *loaded_resource_ids asm("r4") = gEventSpriteResourceIds;
            register u32 requested_resource_id asm("r3") = resource_id_r6;
            register u32 loaded_resource_limit asm("r2") = loaded_resource_count;

            do {
                if (loaded_resource_ids[resource_index] == requested_resource_id) {
                    return;
                }
                resource_index++;
            } while (resource_index < loaded_resource_limit);
        }

        asm volatile("" : "+r"(resource_id_r6));

        {
            const struct EventSpriteGraphicsRecord *graphics_table =
                (const struct EventSpriteGraphicsRecord *)EVENT_SPRITE_GRAPHICS_TABLE_ROM;
            const u8 *compressed_graphics = graphics_table[resource_id_r6].compressed_graphics;
            u32 graphics_bytes_or_tile_count;

            graphics_bytes_or_tile_count = compressed_graphics[1];
            graphics_bytes_or_tile_count |= compressed_graphics[2] << 8;
            graphics_bytes_or_tile_count |= compressed_graphics[3] << 16;
            {
                register u16 *tile_boundary asm("r4") = &gEventSpriteTileBoundary;
                graphics_bytes_or_tile_count >>= 5;
                *tile_boundary -= graphics_bytes_or_tile_count;
                {
                    register u16 *palette_boundary asm("r5") = &gEventSpritePaletteBoundary;
                    *palette_boundary -= 1;

                    LoadSpriteGraphicsFromTable((s32)graphics_table, resource_id_r6,
                        *tile_boundary, *palette_boundary);
                    gEventSpriteResourceIds[gEventSpriteResourceCount] = resource_id_r6;
                    gEventSpriteTileOffsets[gEventSpriteResourceCount] = *tile_boundary;
                    gEventSpritePaletteBanks[gEventSpriteResourceCount] = *palette_boundary;
                    gEventSpriteResourceCount++;
                }
            }
        }
    }
}
