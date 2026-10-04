#include "m2c_prelude.h"
#include "../battle/battle.h"
s16 DivideSigned32(s32, s16) asm("func_80ECD98");                            /* extern */

void ApplyWeaponWeightPenalty(s16, s16, void *) asm("func_080E596C");

void ApplyWeaponWeightPenalty(s16 equipment_weight, s16 load_capacity, void *weapon_stats) {
    s16 capacity;
    s16 weight;

    weight = equipment_weight;
    capacity = load_capacity;
    if ((s32) weight > (s32) capacity) {
        if ((s32) (weight - capacity) < (s32) capacity) {
            M2C_FIELD(weapon_stats, s16 *, 0xC) = DivideSigned32(M2C_FIELD(weapon_stats, s16 *, 0xC) * ((capacity * 2) - weight), capacity);
        } else {
            M2C_FIELD(weapon_stats, s16 *, 0xC) = 0;
        }
    }
}
