#include "m2c_prelude.h"
#include "battle_popup.h"
void LoadSpriteGraphicsFromTable(void *, s32, s32, s32) asm("func_0809AA64");
void BiosLz77ToVram(void *, s32) asm("func_080ECD34");
extern u8 gBattleDamagePopupGraphics asm("D_087AD1B8");
extern u8 gBattleDestroyedUnitPopupGraphics asm("D_087AD1C0");
extern u8 gBattleBlueParticlePopupGraphics asm("D_087AD1C8");
extern u8 gBattlePopupGlyphGraphics asm("D_087AC9E8");
extern u8 gBattlePopupSparkTiles asm("D_081076AC");
extern u8 gBattlePopupStreakTiles asm("D_08107970");
extern u8 gBattlePopupDissolvingOrbTiles asm("D_08107A68");
extern u8 gBattlePopupPurpleRingTiles asm("D_08107D24");
extern u8 gBattlePopupGoldSparkPalette asm("D_08108024");
extern u8 gBattlePopupTurquoiseSparkPalette asm("D_08108040");
extern u8 gBattlePopupPurpleSparkPalette asm("D_0810805C");
extern u8 gBattlePopupGreenStreakPalette asm("D_08108078");
extern u8 gBattlePopupPurpleStreakPalette asm("D_08108094");
extern u8 gBattlePopupOrangeStreakPalette asm("D_081080B0");
extern u8 gBattlePopupOrbPalette asm("D_081080CC");
extern u8 gBattlePopupPurpleRingPalette asm("D_081080F4");
extern u8 gBattlePopupSideOneTileMemory asm("D_06010000");
extern u8 gBattlePopupSideZeroTileMemory asm("D_06013000");
extern u8 gBattlePopupSideOneStreakTileMemory asm("D_06010400");
extern u8 gBattlePopupSideZeroStreakTileMemory asm("D_06013400");
extern u8 gBattlePopupSideOneOrbTileMemory asm("D_06010700");
extern u8 gBattlePopupSideZeroOrbTileMemory asm("D_06013700");
extern u8 gBattlePopupSideOneRingTileMemory asm("D_06010B00");
extern u8 gBattlePopupSideZeroRingTileMemory asm("D_06013B00");
extern u8 gBattlePopupSpritePaletteBank0 asm("D_05000200");
extern u8 gBattlePopupSpritePaletteBank1 asm("D_05000220");
extern u8 gBattlePopupSpritePaletteBank2 asm("D_05000240");
extern u8 gBattlePopupSpritePaletteBank3 asm("D_05000260");
extern u8 gBattlePopupSpritePaletteBank4 asm("D_05000280");
extern u8 gBattlePopupSpritePaletteBank5 asm("D_050002A0");
extern u8 gBattlePopupSpritePaletteBank12 asm("D_05000380");
extern u8 gBattlePopupSpritePaletteBank13 asm("D_050003A0");
extern u8 gBattlePopupSpritePaletteBank6 asm("D_050002C0");
extern u8 gBattlePopupSpritePaletteBank7 asm("D_050002E0");
extern u8 gBattlePopupSpritePaletteBank8 asm("D_05000300");
extern u8 gBattlePopupSpritePaletteBank9 asm("D_05000320");
extern u8 gBattlePopupSpritePaletteBank10 asm("D_05000340");
extern u8 gBattlePopupSpritePaletteBank11 asm("D_05000360");

void LoadBattlePopupGraphics(u8 side, u8 graphics_bank) asm("func_080C9F00");

void LoadBattlePopupGraphics(u8 side, u8 graphics_bank) {
    s32 tile_offset, palette_bank;
    switch (graphics_bank) {
    case BATTLE_POPUP_GRAPHICS_DAMAGE:
    {
        void *damage_graphics = &gBattleDamagePopupGraphics;
        tile_offset = 0; if (side == 0) tile_offset = BATTLE_POPUP_SIDE_ZERO_TILE_OFFSET;
        palette_bank = 0; if (side == 0) palette_bank = BATTLE_POPUP_SIDE_ZERO_PALETTE_OFFSET;
        LoadSpriteGraphicsFromTable(damage_graphics, 0, tile_offset, palette_bank);
        goto load_popup_glyph_graphics;
    }
    case BATTLE_POPUP_GRAPHICS_DESTROYED_UNIT:
    {
        void *explosion_graphics = &gBattleDestroyedUnitPopupGraphics;
        tile_offset = 0; if (side == 0) tile_offset = BATTLE_POPUP_SIDE_ZERO_TILE_OFFSET;
        palette_bank = 0; if (side == 0) palette_bank = BATTLE_POPUP_SIDE_ZERO_PALETTE_OFFSET;
        LoadSpriteGraphicsFromTable(explosion_graphics, 0, tile_offset, palette_bank);
        return;
    }
    case BATTLE_POPUP_GRAPHICS_MODIFIERS:
        BiosLz77ToVram(&gBattlePopupSparkTiles, side == 0 ? (s32)&gBattlePopupSideZeroTileMemory : (s32)&gBattlePopupSideOneTileMemory);
        BiosLz77ToVram(&gBattlePopupStreakTiles, side == 0 ? (s32)&gBattlePopupSideZeroStreakTileMemory : (s32)&gBattlePopupSideOneStreakTileMemory);
        BiosLz77ToVram(&gBattlePopupDissolvingOrbTiles, side == 0 ? (s32)&gBattlePopupSideZeroOrbTileMemory : (s32)&gBattlePopupSideOneOrbTileMemory);
        BiosLz77ToVram(&gBattlePopupPurpleRingTiles, side == 0 ? (s32)&gBattlePopupSideZeroRingTileMemory : (s32)&gBattlePopupSideOneRingTileMemory);
        BiosLz77ToVram(&gBattlePopupGoldSparkPalette, side == 0 ? (s32)&gBattlePopupSpritePaletteBank0 + 0xC0 : (s32)&gBattlePopupSpritePaletteBank0);
        BiosLz77ToVram(&gBattlePopupTurquoiseSparkPalette, side == 0 ? (s32)&gBattlePopupSpritePaletteBank1 + 0xC0 : (s32)&gBattlePopupSpritePaletteBank1);
        BiosLz77ToVram(&gBattlePopupPurpleSparkPalette, side == 0 ? (s32)&gBattlePopupSpritePaletteBank2 + 0xC0 : (s32)&gBattlePopupSpritePaletteBank2);
        BiosLz77ToVram(&gBattlePopupGreenStreakPalette, side == 0 ? (s32)&gBattlePopupSpritePaletteBank3 + 0xC0 : (s32)&gBattlePopupSpritePaletteBank3);
        BiosLz77ToVram(&gBattlePopupPurpleStreakPalette, side == 0 ? (s32)&gBattlePopupSpritePaletteBank4 + 0xC0 : (s32)&gBattlePopupSpritePaletteBank4);
        BiosLz77ToVram(&gBattlePopupOrangeStreakPalette, side == 0 ? (s32)&gBattlePopupSpritePaletteBank5 + 0xC0 : (s32)&gBattlePopupSpritePaletteBank5);
        BiosLz77ToVram(&gBattlePopupOrbPalette, (s32)&gBattlePopupSpritePaletteBank12);
        BiosLz77ToVram(&gBattlePopupPurpleRingPalette, (s32)&gBattlePopupSpritePaletteBank13);
load_popup_glyph_graphics:
        LoadSpriteGraphicsFromTable(&gBattlePopupGlyphGraphics, 0, BATTLE_POPUP_GLYPH_TILE_OFFSET, BATTLE_POPUP_GLYPH_PALETTE_BANK);
        return;
    case BATTLE_POPUP_GRAPHICS_BLUE_PARTICLES:
    {
        void *blue_particle_graphics = &gBattleBlueParticlePopupGraphics;
        tile_offset = 0; if (side == 0) tile_offset = BATTLE_POPUP_SIDE_ZERO_TILE_OFFSET;
        palette_bank = 0; if (side == 0) palette_bank = BATTLE_POPUP_SIDE_ZERO_PALETTE_OFFSET;
        LoadSpriteGraphicsFromTable(blue_particle_graphics, 0, tile_offset, palette_bank);
        return;
    }
    }
}
