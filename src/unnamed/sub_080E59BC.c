#include "m2c_prelude.h"
s32 LoadZoidEquipmentStats(s32, u8, void *) asm("func_80E58DC");                     /* extern */
M2C_UNK ApplyZoidWeaponWeightPenalty(s32, void *) asm("func_80E59A0");                     /* extern */
M2C_UNK ApplyPilotWeaponModifiers(s32, s32, M2C_UNK, u16, void *) asm("func_080E6994");  /* extern */

s32 sub_080E59BC(s32 arg0, s32 arg1, M2C_UNK arg2, u8 arg3, void *arg4) {
    u8 temp_r6;
    s32 off;

    temp_r6 = arg3;
    if ((LoadZoidEquipmentStats(arg0, temp_r6, arg4) << 0x18) != 0) {
        if (!(1 & M2C_FIELD(arg4, u16 *, 2))) {
            ApplyZoidWeaponWeightPenalty(arg0, arg4);
            off = temp_r6 * 4;
            ApplyPilotWeaponModifiers(arg0, arg1, arg2, M2C_FIELD((arg0 + off), u16 *, 0x52), arg4);
        }
        return 1;
    }
    return 0;
}
