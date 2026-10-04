#include "m2c_prelude.h"
#include "../event_script.h"
extern void StopSong(int) asm("func_8092EA0");
extern void SeekEventCommand(int, int, int) asm("func_80A016C");
extern u8 gEventSongId asm("D_02031742");

int EventStopSong(u8 script_slot) asm("func_080A2540");

int EventStopSong(u8 script_slot) {
    StopSong(gEventSongId);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
