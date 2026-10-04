#include "m2c_prelude.h"
#include "graphics_resources.h"
extern void BiosLz77ToVram(s32, s32) asm("func_80ECD34");
extern void LoadCompressedPalette(s32, s32, s32) asm("func_0809A1BC");
extern s32 gZoidIconTiles[] asm("D_087AA70C");
extern s32 gZoidIconPalettes[][8] asm("D_087AA96C");

void LoadZoidIconGraphics(u8 zoid_id, u8 palette_variant, int tile_offset, int palette_bank) asm("func_0809A4CC");

void LoadZoidIconGraphics(u8 zoid_id, u8 palette_variant, int tile_offset, int palette_bank)
{
    tile_offset = tile_offset << 0x10;
    palette_bank = palette_bank << 0x18;
    palette_bank = (u32)palette_bank >> 0x18;
    BiosLz77ToVram(gZoidIconTiles[zoid_id], ((u32)tile_offset >> 0xB) + 0x06010000);
    LoadCompressedPalette(gZoidIconPalettes[zoid_id][palette_variant], (palette_bank << 5) + 0x05000200, 0x02002880);
}
