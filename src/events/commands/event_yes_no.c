#include "m2c_prelude.h"
#include "../event_script.h"
extern void RequestWindowRefresh(void) asm("func_080972C8");
extern void RunMenuScript(s32) asm("func_08098BB4");
extern void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
extern void func_080ED17C(s32);

s32 EventYesNo(u8 script_slot) {
    *(s8 *)0x02030664 = 1;
    if (*(u8 *)0x020324B0 & 4) {
        *(s8 *)0x020314A4 = 0x27;
    }
    RunMenuScript(0x080177FF);
    RequestWindowRefresh();
    if (*(u8 *)0x020324B0 & 4) {
        *(s8 *)0x020314A4 = 0x5F;
    }
    func_080ED17C(1);
    if (*(u8 *)0x0200A882 == 1 && *(u8 *)0x0200A880 == 0) {
        SeekEventCommand(script_slot, EVENT_BRANCH_TRUE, 0);
    } else {
        SeekEventCommand(script_slot, EVENT_BRANCH_FALSE, 0);
    }
    return EVENT_CONTINUE;
}
