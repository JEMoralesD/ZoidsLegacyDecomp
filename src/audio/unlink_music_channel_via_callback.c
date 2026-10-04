#include "sound_engine.h"
M2C_UNK CallFunctionR1(M2C_UNK, s32) asm("func_80ECD60");

void UnlinkMusicChannelViaCallback(M2C_UNK channel) asm("func_080EBAA8");

void UnlinkMusicChannelViaCallback(M2C_UNK channel) {
    CallFunctionR1(channel, *(s32 *)MUSIC_UNLINK_CALLBACK_RAM);
}
