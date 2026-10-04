#include "m2c_prelude.h"
#include "battle_display.h"
M2C_UNK DestroySprite() asm("func_8094554");

void DestroyBattleTargetSelectionIcons(void) asm("func_080CCBAC");

void DestroyBattleTargetSelectionIcons(void) {
    u8 unit_slot = 0;
    s32 *target_icon_sprites = (s32 *)BATTLE_TARGET_SELECTION_ICONS_RAM;
    for (; unit_slot < BATTLE_ACTIVE_UNIT_COUNT; unit_slot++) {
        if (target_icon_sprites[unit_slot] != 0) {
            DestroySprite();
        }
    }
}
