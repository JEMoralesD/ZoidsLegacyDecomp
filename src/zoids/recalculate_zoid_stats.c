#include "m2c_prelude.h"
M2C_UNK ApplyEquipmentStatBonuses(void *) asm("func_80E542C");                          /* extern */
M2C_UNK LoadZoidBaseStats() asm("func_80E5538");                                /* extern */
M2C_UNK ApplyEquipmentWeightPenalty(void *) asm("func_80E5674");                          /* extern */
M2C_UNK CalculateZoidDerivedStats(void *, s32, s32) asm("func_80E570C");                /* extern */
M2C_UNK func_80E57D0(void *);                          /* extern */
M2C_UNK func_80E6830(void *, s32, s32);                /* extern */

void RecalculateZoidStats(void *zoid, s32 pilot) {
    LoadZoidBaseStats();
    ApplyEquipmentStatBonuses(zoid);
    ApplyEquipmentWeightPenalty(zoid);
    func_80E6830(zoid, pilot, 0);
    func_80E57D0(zoid);
    CalculateZoidDerivedStats(zoid, pilot, 0);
    func_80E57D0(zoid);
    if ((s32) M2C_FIELD(zoid, s16 *, 6) > (s32) (s16) M2C_FIELD(zoid, u16 *, 0x3A)) {
        M2C_FIELD(zoid, s16 *, 6) = (s16) M2C_FIELD(zoid, u16 *, 0x3A);
    }
    if ((s32) M2C_FIELD(zoid, s16 *, 8) > (s32) (s16) M2C_FIELD(zoid, u16 *, 0x3E)) {
        M2C_FIELD(zoid, s16 *, 8) = (s16) M2C_FIELD(zoid, u16 *, 0x3E);
    }
}
