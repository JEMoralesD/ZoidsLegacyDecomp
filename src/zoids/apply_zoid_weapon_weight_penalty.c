#include "m2c_prelude.h"
#include "../battle/battle.h"
M2C_UNK ApplyWeaponWeightPenalty(s16, s16, s32) asm("func_80E596C");                   /* extern */

void ApplyZoidWeaponWeightPenalty(void *, s32) asm("func_080E59A0");

void ApplyZoidWeaponWeightPenalty(void *zoid, s32 weapon_stats) {
    ApplyWeaponWeightPenalty(M2C_FIELD(zoid, s16 *, 0xE), M2C_FIELD(zoid, s16 *, 0x4C), weapon_stats);
}
