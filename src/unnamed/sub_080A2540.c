#include "m2c_prelude.h"
extern void StopSong(int) asm("func_8092EA0");
extern void SeekEventCommand(int, int, int) asm("func_80A016C");
extern u8 D_02031742;

int sub_080A2540(u8 arg0) {
    StopSong(D_02031742);
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
