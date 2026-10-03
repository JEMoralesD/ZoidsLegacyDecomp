#include "m2c_prelude.h"
#include "battle.h"
extern u8 gBattleState[];
s32 IsBattleUnitActive(u8 side, u8 unit_slot) {
    u8 *unit = gBattleState + (side * 0x1380) + (unit_slot * 0x270);
    if ((*(u8 *)(unit + 0) != 0) && !(8 & *(u16 *)(unit + 4))) {
        return 1;
    }
    return 0;
}
