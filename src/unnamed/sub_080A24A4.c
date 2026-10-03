#include "m2c_prelude.h"
M2C_UNK PlayOrContinueSong(u16) asm("func_8092E74");
M2C_UNK SeekEventCommand(u8, s32, s32) asm("func_80A016C");
extern u8 D_02030667;
extern u16 D_0202ECF4;

typedef struct { u8 pad[30]; u8 f; u8 pad2; } S;
extern S D_087C4434[];

int sub_080A24A4(u8 arg0) {
    D_02030667 = D_087C4434[D_0202ECF4].f;
    PlayOrContinueSong(D_02030667);
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
