#include "../sound_engine.h"
void MusicCommandEndTrack(MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EAE90");
void UnlinkMusicChannel(struct AudioChannelState *channel) asm("func_080EAE70");

void MusicCommandEndTrack(MusicPlayerInfo *player, MusicPlayerTrack *track) {
    struct AudioChannelState *channel = track->channel;

    if (channel != NULL) {
        do {
            u8 flags = channel->flags;

            if (flags & SOUND_CHANNEL_ON)
                channel->flags = flags | SOUND_CHANNEL_STOP;
            UnlinkMusicChannel(channel);
            channel = channel->next;
        } while (channel != NULL);
    }
    track->flags = 0;
}
