#include "m2c_prelude.h"
#include "battle_display.h"
M2C_UNK LoadSpriteGraphicsFromTable(M2C_UNK, u8, s32, s32) asm("func_0809AA64");

void LoadBattleStreakParticleGraphics(s32 group_address) asm("func_080CCF3C");

void LoadBattleStreakParticleGraphics(s32 group_address) {
    LoadSpriteGraphicsFromTable(BATTLE_STREAK_GRAPHICS_TABLE_ROM, (u8) (BATTLE_STREAK_GROUP_FIELD((void *)group_address, s32, palette_variant) + BATTLE_STREAK_RESOURCE_SLOT), BATTLE_STREAK_TILE_OFFSET, BATTLE_STREAK_PALETTE_BANK);
}
