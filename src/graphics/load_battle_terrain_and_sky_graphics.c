#include "m2c_prelude.h"
#include "graphics_resources.h"

void LoadCompressedPalette(s32, s32, s32) asm("func_0809A1BC");
void BiosLz77ToVram(s32, s32) asm("func_080ECD34");

void LoadBattleTerrainAndSkyGraphics(s32 terrain_id, s32 terrain_char_block, s32 terrain_screen_block, s32 sky_char_block, s32 sky_screen_block) asm("func_0809A848");

void LoadBattleTerrainAndSkyGraphics(s32 terrain_id, s32 terrain_char_block, s32 terrain_screen_block, s32 sky_char_block, s32 sky_screen_block)
{
    register u32 terrain_record_offset asm("r5") = terrain_id;
    register u32 terrain_tilemap_screen_block asm("r4") = terrain_screen_block;
    register u32 sky_tile_char_block asm("r8") = sky_char_block;
    register u32 sky_tilemap_screen_block asm("r9");
    register u32 terrain_graphics_table asm("r6");
    register u32 vram_base asm("sl");
    register u32 stacked_arg asm("r0");
    u16 *tilemap;
    u32 tile_indices;

    asm volatile("" : "+r"(terrain_record_offset));
    asm volatile("" : "+r"(terrain_tilemap_screen_block));
    asm volatile("" : "+r"(sky_tile_char_block));
    stacked_arg = *(volatile u32 *)&sky_screen_block;
    asm volatile("" : "+r"(stacked_arg));
    terrain_record_offset <<= 24;
    terrain_char_block <<= 24;
    terrain_tilemap_screen_block <<= 24;
    terrain_tilemap_screen_block >>= 24;
    {
        register u32 temp asm("r2") = sky_tile_char_block;

        asm volatile("" : "+r"(temp));
        temp <<= 24;
        temp >>= 24;
        sky_tile_char_block = temp;
    }
    stacked_arg <<= 24;
    stacked_arg >>= 24;
    sky_tilemap_screen_block = stacked_arg;

    terrain_graphics_table = 0x087AF614;
    terrain_record_offset >>= 20;
    {
        register u32 resource asm("r0");
        register u32 call_offset asm("r1");
        register u32 base asm("r2");

        resource = terrain_record_offset + terrain_graphics_table;
        resource = *(u32 *)resource;
        asm volatile("" : "+r"(resource));
        call_offset = (u32)terrain_char_block >> 10;
        base = 0xC0;
        base <<= 19;
        asm volatile("" : "+r"(base));
        vram_base = base;
        call_offset += vram_base;
        BiosLz77ToVram(resource, call_offset);
    }
    {
        register u32 resource asm("r0") = terrain_graphics_table;
        register u32 palette asm("r1");
        register u32 work asm("r2");

        resource += (s32)&((struct BattleTerrainGraphics *)0)->terrain_palette;
        resource = terrain_record_offset + resource;
        resource = *(u32 *)resource;
        asm volatile("" : "+r"(resource));
        palette = 0xA0;
        palette <<= 19;
        asm volatile("" : "+r"(palette));
        work = 0x02002880;
        LoadCompressedPalette(resource, palette, work);
    }

    terrain_tilemap_screen_block <<= 11;
    {
        register u32 base asm("r0") = vram_base;

        asm volatile("" : "+r"(base));
        tilemap = (u16 *)(terrain_tilemap_screen_block + base);
    }
    tile_indices = 0x100;
    {
        register u32 outer asm("r1") = 0;
        u32 second_offset;
        register u32 sky_tilemap_offset asm("r8");
        register u32 increment asm("r5");
        register u32 tile_base asm("r4");

        vram_base = terrain_graphics_table;
        terrain_graphics_table = terrain_record_offset;
        {
            register u32 temp asm("r0") = sky_tile_char_block;

            second_offset = temp << 14;
        }
        {
            register u32 temp asm("r0") = sky_tilemap_screen_block;

            sky_tilemap_offset = temp << 11;
        }
        {
            register u32 increment_source asm("r0") = 0x202;

            asm volatile("" : "+r"(increment_source));
            increment = increment_source;
        }

        do {
            register u32 inner asm("r4") = 0;

            do {
                    tilemap[0] = tile_indices;
                    tilemap[4] = tile_indices;
                    tilemap[0x40] = tile_indices;
                    tilemap[0x44] = tile_indices;
                    tilemap++;
                    {
                        register u32 next asm("r0") = tile_indices + increment;

                        next <<= 16;
                        tile_indices = next >> 16;
                    }
                    {
                        register u32 next asm("r0") = inner + 1;

                        next <<= 24;
                        inner = next >> 24;
                    }
                } while (inner <= 3);
            tilemap += 4;
            {
                register u32 next asm("r0") = outer + 1;

                next <<= 24;
                outer = next >> 24;
            }
        } while (outer <= 7);

        {
            register u32 resource asm("r0") = vram_base;
            register u32 target asm("r1");

            resource += (s32)&((struct BattleTerrainGraphics *)0)->sky_tiles;
            resource = terrain_graphics_table + resource;
            resource = *(u32 *)resource;
            asm volatile("" : "+r"(resource));
            tile_base = 0xC0;
            tile_base <<= 19;
            target = second_offset + tile_base;
            BiosLz77ToVram(resource, target);
        }
        {
            register u32 resource asm("r0") = vram_base;
            register u32 palette asm("r1");
            register u32 work asm("r2");

            resource += (s32)&((struct BattleTerrainGraphics *)0)->sky_palette;
            resource = terrain_graphics_table + resource;
            resource = *(u32 *)resource;
            asm volatile("" : "+r"(resource));
            palette = 0x05000080;
            work = 0x02002880;
            LoadCompressedPalette(resource, palette, work);
        }

        {
            register u32 offset asm("r0") = sky_tilemap_offset;

            tilemap = (u16 *)(offset + tile_base);
        }
        tile_indices = 0;
        outer = 0;
        do {
            register u32 inner asm("r4") = 0;

            outer += 1;
            do {
                tilemap[0] = tile_indices;
                tilemap[0x10] = tile_indices;
                tilemap++;
                {
                    register u32 next asm("r0") = tile_indices + 1;

                    next <<= 16;
                    tile_indices = next >> 16;
                }
                {
                    register u32 next asm("r0") = inner + 1;

                    next <<= 24;
                    inner = next >> 24;
                }
            } while (inner <= 0xF);
            tilemap += 0x10;
            {
                register u32 narrowed asm("r0") = outer << 24;

                outer = narrowed >> 24;
            }
        } while (outer <= 0xF);
    }
}
