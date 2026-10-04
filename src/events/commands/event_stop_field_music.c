#include "m2c_prelude.h"
#include "../event_script.h"
extern u8 gCurrentFieldSongId asm("D_02030667");
extern void StopSong(int) asm("func_8092EA0");
extern void SeekEventCommand(int, int, int) asm("func_80A016C");

int EventStopFieldMusic(u8 script_slot) asm("func_080A24E4");

int EventStopFieldMusic(u8 script_slot) {
    StopSong(gCurrentFieldSongId);
    gCurrentFieldSongId = 0;
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
