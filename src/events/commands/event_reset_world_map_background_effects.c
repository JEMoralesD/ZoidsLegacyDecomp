#include "m2c_prelude.h"
#include "../../field/field_display.h"
#include "../../game/game_state.h"
extern void SeekEventCommand() asm("func_80A016C");

s32 EventResetWorldMapBackgroundEffects(u8 script_slot) asm("func_080A5F88");

s32 EventResetWorldMapBackgroundEffects(u8 script_slot) {
    u8 t;
    s32 *q;
    if (*(s32 *)0x02021690 == GAME_MODE_FIELD) {
        t = *(u8 *)0x020316F4;
        if (t == 0 || t == 0x40) {
            *(s8 *)0x020324B0 = 0;
            *(u16 *)0x0300004C &= 0xFDFF;
            *(u16 *)0x0400000E = 0x300;
            q = (s32 *)0x03000054;
            q[7] = 0;
            q[6] = 0;
        }
    }
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
