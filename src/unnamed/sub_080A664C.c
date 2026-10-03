#include "m2c_prelude.h"
#include "../game/game_state.h"
extern u8 D_02030664;
extern u32 gGameMode;
extern void func_80ED17C(int);
extern void SeekEventCommand(int, int, int) asm("func_80A016C");

int sub_080A664C(u8 arg0) {
    D_02030664 = 1;
    gGameMode = GAME_MODE_CLEAR_DATA_SAVE;
    do {
        func_80ED17C(1);
    } while (gGameMode == GAME_MODE_CLEAR_DATA_SAVE);
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
