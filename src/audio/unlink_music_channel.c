#include "sound_engine.h"

void UnlinkMusicChannel(struct AudioChannelState *channel) asm("func_080EAE70");

void UnlinkMusicChannel(struct AudioChannelState *channel)
{
    register struct AudioChannelState *channel_state asm("r0") = channel;
    register MusicPlayerTrack *owner_track asm("r3") = channel_state->owner_track;

    if (owner_track != 0) {
        register struct AudioChannelState *next asm("r1") = channel_state->next;
        register struct AudioChannelState *previous asm("r2") = channel_state->previous;

        if (previous != 0) {
            previous->next = next;
        } else {
            owner_track->channel = next;
        }
        if (next != 0) {
            next->previous = previous;
        }
        channel_state->owner_track = 0;
    }
}
