#include "m2c_prelude.h"
#include "battle.h"
extern s32 TestEventFlag(s32) asm("func_809F818");
extern s32 gPilotNameTable[] asm("D_087EDFB4");

s32 GetBattlePilotDisplayName(u8 side, u8 pilot_id) asm("func_080E9924");

s32 GetBattlePilotDisplayName(u8 side, u8 pilot_id) {
    if (pilot_id == 1) {
        return side * 0x12 + 0x0203EDE8;
    }
    if (pilot_id == 2 || pilot_id == 0x60 || pilot_id == 0x61) {
        if ((TestEventFlag(PILOT_NAME_REVEAL_MYSTERY_WARRIOR) << 24) == 0) {
            return 0x0810948C;
        }
    }
    if ((u8)(pilot_id - 0x36) <= 1 && (TestEventFlag(PILOT_NAME_REVEAL_MYSTERY_WOMAN) << 24) == 0) {
        return 0x081094AC;
    }
    return gPilotNameTable[pilot_id];
}
