#include "../sound_engine.h"
void UpdateMusicChannelVolume(struct AudioChannelState *channel, MusicPlayerTrack *track) asm("func_080EB36C");

void UpdateMusicChannelVolume(struct AudioChannelState *channel, MusicPlayerTrack *track) {
    register struct AudioChannelState *channel_r4 asm("r4") = channel;
    register MusicPlayerTrack *track_r5 asm("r5") = track;
    register u32 velocity asm("r1") = channel_r4->velocity;
    register s32 pan asm("r2") = (s8)channel_r4->rhythm_pan;
    register s32 scale asm("r3");
    register s32 volume asm("r0");

    scale = 0x80;
    scale = scale + pan;
    scale *= velocity;
    volume = track_r5->volumeRight * scale;
    volume >>= 14;
    if ((u32)volume > 0xFF)
        volume = 0xFF;
    channel_r4->volume_right = volume;

    scale = 0x7F;
    scale -= pan;
    scale *= velocity;
    volume = track_r5->volumeLeft * scale;
    volume >>= 14;
    if ((u32)volume > 0xFF)
        volume = 0xFF;
    channel_r4->volume_left = volume;
}
