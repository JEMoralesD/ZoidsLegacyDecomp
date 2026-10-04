#include "sound_engine.h"
void MusicCommandReturnFromPattern(s32 unused_player, void *track) asm("func_080EAF3C");

void MusicCommandReturnFromPattern(s32 unused_player, void *track) {
    register s32 level_or_return_cursor asm("r2");
    register void *stack_base asm("r3");

    level_or_return_cursor = MUSIC_TRACK_FIELD(track, u8 *, patternLevel);
    if (level_or_return_cursor != 0) {
        level_or_return_cursor--;
        MUSIC_TRACK_FIELD(track, u8 *, patternLevel) = level_or_return_cursor;
        stack_base = track + level_or_return_cursor * 4;
        level_or_return_cursor = MUSIC_TRACK_FIELD(stack_base, s32 *, patternStack);
        MUSIC_TRACK_FIELD(track, s32 *, command) = level_or_return_cursor;
    }
}
