#include "m2c_prelude.h"
#include "../../game/game_state.h"
#include "../event_script.h"
extern void func_8096308(int, int);
extern void func_80ED17C(int);
extern int SeekEventCommand(int, int, int) asm("func_80A016C");

int EventOpenItemShop(u8 script_slot, u8 **cursor) {
    u8 *p1 = (u8 *)0x02030664;
    *p1 = 1;
    *(u8 *)0x02032B98 = cursor[0][1];
    *(u32 *)0x02021690 = GAME_MODE_ITEM_SHOP;
    func_8096308(0x10, 0);
    *p1 = 2;
    do {
        func_80ED17C(1);
    } while (*p1 != 1);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
