#include "m2c_prelude.h"
#include "../../game/game_state.h"
#include "../event_script.h"
extern u8 gBattleSetup[];
extern void func_0809F850(void);
extern void func_080ED17C();
extern int SeekEventCommand() asm("func_080A016C");

s32 EventStartBattle(u8 script_slot, struct EventBattleCommand **cursor) {
    *(u8 *)0x02030664 = 2;
    *(u8 *)0x02030666 = 0;
    *(s32 *)0x02031744 = 0;
    func_0809F850();
    gBattleSetup[3] = (*cursor)->encounter_id;
    gBattleSetup[4] = (*cursor)->formation_id;
    gBattleSetup[1] = (*cursor)->terrain_id;
    gBattleSetup[5] = (*cursor)->options;
    *(s32 *)0x02030558 = 0;
    *(s32 *)0x02021690 = GAME_MODE_BATTLE;
    if (*(u8 *)0x02030664 == 2) {
        do {
            func_080ED17C(1);
        } while (*(u8 *)0x02030664 == 2);
    }
    *(s8 *)0x02030667 = 0;
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
