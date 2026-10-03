#include "m2c_prelude.h"
#include "../game/game_state.h"
extern void func_08096308(int, int);
extern void func_080ED17C(int);
extern int SeekEventCommand(int, int, int) asm("func_080A016C");

extern u8 D_02030664;
extern u32 gGameMode;

int sub_080A242C(u8 arg0)
{
    D_02030664 = 1;
    gGameMode = GAME_MODE_ZOIDS_LAB;
    func_08096308(16, 0);
    D_02030664 = 2;
    do {
        func_080ED17C(1);
    } while (D_02030664 != 1);
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
