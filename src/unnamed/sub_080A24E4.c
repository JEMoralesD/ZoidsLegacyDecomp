#include "m2c_prelude.h"
extern u8 D_02030667;
extern void StopSong(int) asm("func_8092EA0");
extern void SeekEventCommand(int, int, int) asm("func_80A016C");

int sub_080A24E4(u8 arg0) {
    StopSong(D_02030667);
    D_02030667 = 0;
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
