#include "m2c_prelude.h"
#include "../event_script.h"
extern void PlaySong(int) asm("func_8092E84");
extern void SeekEventCommand(int, int, int) asm("func_80A016C");
extern u8 gEventSongId asm("D_02031742");

int EventPlaySong(u8 script_slot, u8 **cursor) asm("func_080A2510");

int EventPlaySong(u8 script_slot, u8 **cursor) {
    gEventSongId = EVENT_COMMAND_BYTE(*cursor, EventSongCommand, song_id);
    PlaySong(gEventSongId);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
