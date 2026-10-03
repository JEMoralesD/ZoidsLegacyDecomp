#include "m2c_prelude.h"
#include "../game/game_state.h"
M2C_UNK SeekEventCommand() asm("func_80A016C");
extern u32 gGameMode;
extern u8 gEventMapId;
extern u8 D_020324B1;

s32 sub_080A5FF4(u8 arg0) {
    u8 v;
    if (gGameMode == GAME_MODE_FIELD) {
        v = gEventMapId;
        if (v == 0 || v == 0x40) {
            D_020324B1 = 0;
        }
    }
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
