#include "../sound_engine.h"
void StopMusicTrack(MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EB328");

typedef void (*StopPsgChannelFunc)(u8 channel_id);

void StopMusicTrack(MusicPlayerInfo *player, MusicPlayerTrack *track) {
    register MusicPlayerTrack *track_r5 asm("r5") = track;
    register struct AudioChannelState *channel asm("r4");
    register u32 zero asm("r6");

    if (!(track_r5->flags & MUSIC_TRACK_ACTIVE))
        return;

    channel = track_r5->channel;
    if (channel != NULL) {
        zero = 0;
        do {
            if (channel->flags != 0) {
                register u32 psg_channel asm("r0") = channel->type;
                register u32 mask asm("r3") = TONE_TYPE_PSG_MASK;

                psg_channel &= mask;
                if (psg_channel != 0) {
                    register struct SoundEngineState *engine asm("r3") = SOUND_ENGINE;
                    register StopPsgChannelFunc stop asm("r3") = engine->stop_psg_channel;

                    stop(psg_channel);
                }
                channel->flags = (u8)zero;
            }
            channel->owner_track = (MusicPlayerTrack *)zero;
            channel = channel->next;
        } while (channel != NULL);
    }
    track_r5->channel = channel;
}
