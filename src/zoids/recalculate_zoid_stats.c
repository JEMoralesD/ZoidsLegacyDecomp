#include "m2c_prelude.h"
M2C_UNK ApplyEquipmentStatBonuses(void *) asm("func_80E542C");                          /* extern */
M2C_UNK LoadZoidBaseStats() asm("func_80E5538");                                /* extern */
M2C_UNK ApplyEquipmentWeightPenalty(void *) asm("func_80E5674");                          /* extern */
M2C_UNK CalculateZoidDerivedStats(void *, s32, s32) asm("func_80E570C");                /* extern */
M2C_UNK ClampZoidStats(void *) asm("func_080E57D0");                          /* extern */
M2C_UNK ApplyPilotZoidStatModifiers(void *, s32, s32) asm("func_080E6830");                /* extern */

void RecalculateZoidStats(void *zoid, s32 pilot) {
    LoadZoidBaseStats();
    ApplyEquipmentStatBonuses(zoid);
    ApplyEquipmentWeightPenalty(zoid);
    ApplyPilotZoidStatModifiers(zoid, pilot, 0);
    ClampZoidStats(zoid);
    CalculateZoidDerivedStats(zoid, pilot, 0);
    ClampZoidStats(zoid);
    if ((s32) M2C_FIELD(zoid, s16 *, 6) > (s32) (s16) M2C_FIELD(zoid, u16 *, 0x3A)) {
        M2C_FIELD(zoid, s16 *, 6) = (s16) M2C_FIELD(zoid, u16 *, 0x3A);
    }
    if ((s32) M2C_FIELD(zoid, s16 *, 8) > (s32) (s16) M2C_FIELD(zoid, u16 *, 0x3E)) {
        M2C_FIELD(zoid, s16 *, 8) = (s16) M2C_FIELD(zoid, u16 *, 0x3E);
    }
}
