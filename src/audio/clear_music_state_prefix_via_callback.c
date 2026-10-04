#include "sound_engine.h"
M2C_UNK CallFunctionR1(M2C_UNK, s32) asm("func_80ECD60");

void ClearMusicStatePrefixViaCallback(M2C_UNK music_state) asm("func_080EBABC");

void ClearMusicStatePrefixViaCallback(M2C_UNK music_state) {
    CallFunctionR1(music_state, *(s32 *)MUSIC_CLEAR_PREFIX_CALLBACK_RAM);
}
