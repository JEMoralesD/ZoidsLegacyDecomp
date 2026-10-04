#include "m2c_prelude.h"
#include "battle_display.h"

void *CreateSprite(s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484");
void LoadZoidIconGraphics(u8, u8, s32, u8) asm("func_0809A4CC");
u8 IsBattleUnitActive(u8, u8) asm("func_080E9D88");
u8 DivideUnsigned32(u8, s32) asm("func_080ECF00");
u8 ModuloUnsigned32(u8, s32) asm("func_080ECF78");

extern void *gBattleTargetSelectionIcons[] asm("D_02033FB4");

void CreateBattleTargetSelectionIcons(u8 target_side) asm("func_080CCA6C");

void CreateBattleTargetSelectionIcons(u8 target_side)
{
    u8 unit_slot;
    s32 icon_x, icon_y;
    u32 tile_offset;
    s32 palette_bank;
    u8 unit_active;
    u8 *battle_unit;
    void *icon_sprite;

    unit_slot = 0;
    do {
        unit_active = IsBattleUnitActive(target_side, unit_slot);
        if (unit_active != 0) {
            icon_x = (u16)(((1 - DivideUnsigned32(unit_slot, 3)) << 5) + 0x18);
            icon_y = (u32)(((2 - ModuloUnsigned32(unit_slot, 3)) << 0x15) + 0x280000) >> 0x10;
            if (target_side == 0) {
                s32 mirrored_position_bits;
                mirrored_position_bits = 0x50;
                mirrored_position_bits -= (s16)icon_x;
                mirrored_position_bits <<= 16;
                icon_x = (u32)mirrored_position_bits >> 16;
                mirrored_position_bits = 0x90;
                mirrored_position_bits -= (s16)icon_y;
                mirrored_position_bits <<= 16;
                icon_y = (u32)mirrored_position_bits >> 16;
            }
            battle_unit = (u8 *)(unit_slot * (s32)sizeof(struct BattleUnit) + target_side * (s32)sizeof(struct BattleSide) + BATTLE_STATE_RAM);
            LoadZoidIconGraphics(BATTLE_UNIT_FIELD(battle_unit, u8, zoid_id), BATTLE_UNIT_FIELD(battle_unit, u8, palette_variant),
                tile_offset = (u32)((unit_slot << 0x16) + 0x1800000) >> 0x10,
                palette_bank = unit_slot + 3);
            icon_sprite = CreateSprite(BATTLE_TARGET_SELECTION_ICON_FRAMES_ROM, BATTLE_TARGET_SELECTION_ICON_ANIMATIONS_ROM, 0, (s16)icon_x, (s16)icon_y, tile_offset, palette_bank,
                (target_side != 0) ? 0x8148 : 0x148, 0);
            gBattleTargetSelectionIcons[unit_slot] = icon_sprite;
            *(u16 *)((u32)icon_sprite + BATTLE_SPRITE_OFFSET(scale)) = BATTLE_TARGET_SELECTION_ICON_SCALE;
        } else {
            gBattleTargetSelectionIcons[unit_slot] = (void *)unit_active;
        }
        unit_slot++;
    } while (unit_slot < BATTLE_ACTIVE_UNIT_COUNT);
}
