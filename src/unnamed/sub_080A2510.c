#include "m2c_prelude.h"
extern void PlaySong(int) asm("func_8092E84");
extern void SeekEventCommand(int, int, int) asm("func_80A016C");
extern u8 D_02031742;

int sub_080A2510(u8 arg0, u8 **arg1) {
    D_02031742 = (*arg1)[1];
    PlaySong(D_02031742);
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
