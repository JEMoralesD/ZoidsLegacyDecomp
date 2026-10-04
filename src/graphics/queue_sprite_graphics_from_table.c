#include "m2c_prelude.h"
#include "graphics_resources.h"


void QueueCopy(s32, s32, s32) asm("func_08095208");
void BiosLz77ToWram(void *, void *) asm("func_080ECD38");

void QueueSpriteGraphicsFromTable(s32 resource_table, s32 resource_id, s32 tile_offset, s32 palette_bank, u8 * volatile work_buffer) asm("func_0809AAA8");

void QueueSpriteGraphicsFromTable(s32 resource_table, s32 resource_id, s32 tile_offset, s32 palette_bank, u8 * volatile work_buffer)
{
    register s32 resource_address asm("r9");
    register u32 tile_destination asm("r6");
    register u32 palette_destination asm("r8");
    register u8 *staging_buffer asm("r7");
    register u32 tile_bytes asm("r4");
    register u32 palette_bytes asm("r5");
    u8 *tiles;
    u8 *palette;
    u64 save_marker;
    register u32 shared_high asm("r2");

    resource_address = resource_table;
    asm volatile("" : : "r"(resource_address));
    tile_destination = tile_offset;
    asm volatile("" : : "r"(tile_destination));
    palette_destination = palette_bank;
    asm volatile("" : : "r"(palette_destination));
    staging_buffer = work_buffer;
    resource_id <<= 24;
    tile_destination <<= 16;
    tile_destination >>= 16;
    {
        register u32 normalized_second asm("r0") = palette_destination;
        asm volatile("" : "+r"(normalized_second));
        normalized_second <<= 16;
        normalized_second >>= 16;
        palette_destination = normalized_second;
    }
    resource_id = (u32)resource_id >> 21;
    resource_address += resource_id;

    {
        register struct CompressedSpriteGraphics *view asm("r1") =
            (struct CompressedSpriteGraphics *)resource_address;
        tiles = view->tiles;
    }
    tile_bytes = tiles[1];
    {
        register u32 byte asm("r1") = tiles[2];
        byte <<= 8;
        tile_bytes |= byte;
    }
    {
        register struct CompressedSpriteGraphics *view asm("r2") =
            (struct CompressedSpriteGraphics *)resource_address;
        register u8 *next_palette asm("r1") = view[1].palette;
        /* Both high length bytes come from the next resource's palette header. */
        shared_high = next_palette[3];
        shared_high <<= 16;
    }
    tile_bytes |= shared_high;

    {
        register struct CompressedSpriteGraphics *view asm("r3") =
            (struct CompressedSpriteGraphics *)resource_address;
        palette = view->palette;
    }
    palette_bytes = palette[1];
    {
        register u32 byte asm("r1") = palette[2];
        byte <<= 8;
        palette_bytes |= byte;
    }
    palette_bytes |= shared_high;

    BiosLz77ToWram(tiles, staging_buffer);
    {
        register struct CompressedSpriteGraphics *view asm("r1") =
            (struct CompressedSpriteGraphics *)resource_address;
        register u8 *loaded_palette asm("r0") = view->palette;
        register s32 next_source asm("r2");
        asm volatile("" : "+r"(loaded_palette));
        next_source = (s32)(staging_buffer + tile_bytes);
        asm volatile("" : "+r"(next_source));
        resource_address = next_source;
        BiosLz77ToWram(loaded_palette, resource_address);
    }

    tile_destination <<= 5;
    {
        register u32 first_base asm("r3") = 0x06010000;
        asm volatile("" : "+r"(first_base));
        tile_destination += first_base;
    }
    tile_bytes <<= 16;
    tile_bytes >>= 16;
    asm volatile("" : : "r"(tile_bytes));
    QueueCopy((s32)staging_buffer, tile_destination, tile_bytes);
    asm volatile("" : "=l"(save_marker));

    {
        register u32 shifted_target asm("r0") = palette_destination;
        register u32 second_base asm("r1");
        asm volatile("" : "+r"(shifted_target));
        shifted_target <<= 5;
        palette_destination = shifted_target;
        second_base = 0x05000200;
        asm volatile("" : "+r"(second_base));
        palette_destination += second_base;
    }
    palette_bytes <<= 16;
    palette_bytes >>= 16;
    asm volatile("" : : "r"(palette_bytes));
    QueueCopy(resource_address, palette_destination, palette_bytes);
    asm volatile("" : : "l"(save_marker));
}
