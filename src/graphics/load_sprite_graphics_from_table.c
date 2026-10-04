#include "m2c_prelude.h"
#include "graphics_resources.h"
M2C_UNK LoadCompressedPalette(s32, s32, M2C_UNK) asm("func_0809A1BC");
M2C_UNK BiosLz77ToVram(s32, s32) asm("func_080ECD34");
extern u32 D_06010000;
extern u32 D_05000200;
extern u32 gSaveBuffer;

void LoadSpriteGraphicsFromTable(s32 resource_table, s32 resource_id, s32 tile_offset, s32 palette_bank) asm("func_0809AA64");

void LoadSpriteGraphicsFromTable(s32 resource_table, s32 resource_id, s32 tile_offset, s32 palette_bank) {
    void *resource;

    resource_id = resource_id << 0x18;
    tile_offset = tile_offset << 0x10;
    palette_bank = palette_bank << 0x10;
    palette_bank = (u32)palette_bank >> 0x10;
    resource = resource_table + ((u32)resource_id >> 0x15);
    BiosLz77ToVram(M2C_FIELD(resource, s32 *, (s32)&((struct CompressedSpriteGraphics *)0)->tiles), ((u32)tile_offset >> 0xB) + (s32)&D_06010000);
    LoadCompressedPalette(M2C_FIELD(resource, s32 *, (s32)&((struct CompressedSpriteGraphics *)0)->palette), (palette_bank << 5) + (s32)&D_05000200, (s32)&gSaveBuffer);
}
