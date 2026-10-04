#include "m2c_prelude.h"
#include "../battle/battle.h"
extern u8 TestEventFlag(int) asm("func_809F818");
extern u32 gPilotNameTable[] asm("D_087EDFB4");

int GetPilotDisplayName(u8 pilot_id) asm("func_080E7B64");

int GetPilotDisplayName(u8 pilot_id) {
    if (pilot_id == 1) {
        return 0x02021774;
    }
    if (pilot_id == 2 || pilot_id == 0x60 || pilot_id == 0x61) {
        if (TestEventFlag(PILOT_NAME_REVEAL_MYSTERY_WARRIOR) == 0) {
            return 0x08109414;
        }
    }
    if ((u8)(pilot_id - 0x36) <= 1) {
        if (TestEventFlag(PILOT_NAME_REVEAL_MYSTERY_WOMAN) == 0) {
            return 0x08109434;
        }
    }
    return gPilotNameTable[pilot_id];
}
