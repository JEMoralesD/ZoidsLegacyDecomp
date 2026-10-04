#include "m2c_prelude.h"
#include "graphics_resources.h"
void LoadCompressedPalette(s32, s32, s32) asm("func_0809A1BC");
void BiosLz77ToVram(s32, s32) asm("func_80ECD34");
extern s32 gPilotPortraitAlternatePalettes[] asm("D_087AF604");
extern u8 gPilotPortraitGraphics[] asm("D_087ADBB8");

void LoadPilotPortraitGraphics(u16 pilot_id, u8 expression_id, u8 alternate_palette_index, u16 tile_offset, s32 palette_bank) asm("func_0809A94C");

void LoadPilotPortraitGraphics(u16 pilot_id, u8 expression_id, u8 alternate_palette_index, u16 tile_offset, s32 palette_bank) {
    u16 destination_palette_bank;
    void *portrait_resource;
    u32 pilot_resource_offset;
    u32 expression_resource_address;

    destination_palette_bank = (u16) palette_bank;
    pilot_resource_offset = pilot_id << 6;
    expression_resource_address = (expression_id << 3) + (u32)gPilotPortraitGraphics;
    portrait_resource = (void *)(pilot_resource_offset + expression_resource_address);
    BiosLz77ToVram(M2C_FIELD(portrait_resource, s32 *, (s32)&((struct CompressedSpriteGraphics *)0)->tiles), (tile_offset << 5) + 0x06010000);
    if (pilot_id != GRAPHICS_PILOT_WITH_ALTERNATE_PORTRAIT_PALETTES) {
        LoadCompressedPalette(M2C_FIELD(portrait_resource, s32 *, (s32)&((struct CompressedSpriteGraphics *)0)->palette), (destination_palette_bank << 5) + 0x05000200, 0x02002880);
        return;
    }
    LoadCompressedPalette(gPilotPortraitAlternatePalettes[alternate_palette_index], (destination_palette_bank << 5) + 0x05000200, 0x02002880);
}
