#include "m2c_prelude.h"
extern int CopyBytes() asm("func_80ED038");
extern int SeekEventCommand() asm("func_80A016C");
extern u8 D_020282EC[];
extern u8 D_020218E4[];
extern u8 D_0202EEC0[];
extern u8 D_0202ECF4[];

int sub_080A6604(u8 arg) {
    CopyBytes(D_020282EC, D_020218E4, 0x6a08);
    CopyBytes(D_0202EEC0, D_0202ECF4, 460);
    SeekEventCommand(arg, -1, 0);
    return 0;
}
