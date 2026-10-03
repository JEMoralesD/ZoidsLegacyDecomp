#include "m2c_prelude.h"
#include "../game/game_state.h"
extern void func_80ED17C();
extern void SeekEventCommand() asm("func_80A016C");

int sub_080A6688(u8 arg0) {
    *(u8 *)0x02030664 = 1;
    *(s32 *)0x02021690 = GAME_MODE_CREDITS;
    do {
        func_80ED17C(1);
    } while (*(s32 *)0x02021690 != -1);
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
