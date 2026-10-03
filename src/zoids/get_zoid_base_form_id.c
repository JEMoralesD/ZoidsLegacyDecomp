#include "m2c_prelude.h"
#include "../battle/battle.h"
u8 GetZoidBaseFormId(u8 zoid_id) {
    u32 temp_r0;
    register u8 temp_r2 asm("r2");
    temp_r2 = zoid_id;
    temp_r0 = temp_r2 - ZOID_LIGER_ZERO;
    switch (temp_r0) {
    case 0: case 1: case 2: case 3: case 4: case 5: return ZOID_LIGER_ZERO;
    case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13:
    case 14: case 15: case 16: case 17: case 18: case 19: case 20: case 21:
    case 22: case 23: case 24: case 25: case 26: case 27: case 28: case 29:
    case 30: case 31: case 32: case 33: case 34: case 35: case 36: case 37:
    case 38: return temp_r2;
    case 39: case 40: case 41: case 42: return ZOID_BERSERK_FURY;
    default: return temp_r2;
    }
}
