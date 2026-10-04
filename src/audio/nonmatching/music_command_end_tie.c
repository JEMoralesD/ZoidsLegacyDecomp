#include "../sound_engine.h"
void MusicCommandEndTie(MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EB59C");

void MusicCommandEndTie(MusicPlayerInfo *player, MusicPlayerTrack *track) {
    register u8 *command asm("r2") = track->command;
    register u32 key asm("r3") = *command;
    register struct AudioChannelState *channel asm("r1");
    register u32 playing asm("r4");
    register u32 stopping asm("r5");

    if (key < 0x80) {
        track->key = key;
        track->command = command + 1;
    } else {
        key = track->key;
    }

    channel = track->channel;
    if (channel == NULL)
        return;

    playing = SOUND_CHANNEL_START | SOUND_CHANNEL_ENVELOPE_MASK;
    stopping = SOUND_CHANNEL_STOP;
    do {
        register u32 flags asm("r2") = channel->flags;

        if ((flags & playing) && !(flags & stopping) && channel->midi_key == key) {
            channel->flags = flags | SOUND_CHANNEL_STOP;
            return;
        }
        channel = channel->next;
    } while (channel != NULL);
}
