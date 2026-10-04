#include "sound_engine.h"

void MusicCommandSetTrackPriority(
    MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EAF80");

__attribute__((naked)) void MusicCommandSetTrackPriority(
    MusicPlayerInfo *player, MusicPlayerTrack *track)
{
    register MusicPlayerInfo *live_player asm("r0") = player;
    register MusicPlayerTrack *live_track asm("r1") = track;
    register u32 value asm("r3");

    SAVE_LINK_REGISTER();
    ReadNextMusicCommandByteIntoR3WithAddressGuard(live_player, live_track);
    asm volatile("" : "=r"(live_player), "=r"(live_track), "=r"(value));
    live_track->priority = value;
    RETURN_THROUGH_LINK_REGISTER();
}

void MusicCommandSetPlayerTempo(
    MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EAF8C");

__attribute__((naked)) void MusicCommandSetPlayerTempo(
    MusicPlayerInfo *player, MusicPlayerTrack *track)
{
    register MusicPlayerInfo *live_player asm("r0") = player;
    register MusicPlayerTrack *live_track asm("r1") = track;
    register u32 value asm("r3");

    SAVE_LINK_REGISTER();
    ReadNextMusicCommandByteIntoR3WithAddressGuard(live_player, live_track);
    asm volatile("" : "=r"(live_player), "=r"(live_track), "=r"(value));
    value <<= 1;
    live_player->tempo = value;
    {
        register u32 scale asm("r2") = live_player->tempoScale;

        value *= scale;
    }
    value >>= 8;
    live_player->tempoInterval = value;
    RETURN_THROUGH_LINK_REGISTER();
}

void MusicCommandSetTrackKeyShift(
    MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EAFA0");

__attribute__((naked)) void MusicCommandSetTrackKeyShift(
    MusicPlayerInfo *player, MusicPlayerTrack *track)
{
    register MusicPlayerInfo *live_player asm("r0") = player;
    register MusicPlayerTrack *live_track asm("r1") = track;
    register u32 value asm("r3");

    SAVE_LINK_REGISTER();
    ReadNextMusicCommandByteIntoR3WithAddressGuard(live_player, live_track);
    asm volatile("" : "=r"(live_player), "=r"(live_track), "=r"(value));
    live_track->keyShift = value;
    {
        register u32 flags asm("r3") = live_track->flags;
        register u32 mask asm("r2") = MUSIC_TRACK_PITCH_UPDATE_FLAGS;

        flags |= mask;
        live_track->flags = flags;
    }
    RETURN_THROUGH_LINK_REGISTER();
}

void MusicCommandLoadTrackTone(
    MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EAFB4");

__attribute__((naked)) void MusicCommandLoadTrackTone(
    MusicPlayerInfo *player, MusicPlayerTrack *track)
{
    register MusicPlayerInfo *live_player asm("r0") = player;
    register MusicPlayerTrack *live_track asm("r1") = track;
    register ToneData *source asm("r2");
    register u32 value asm("r3");

    SAVE_LINK_REGISTER();
    {
        register u8 *command asm("r2") = live_track->command;
        register u32 voice asm("r3") = *command;
        register u32 offset asm("r2");
        register ToneData *tone asm("r3");

        command++;
        live_track->command = command;
        offset = voice << 1;
        offset += voice;
        offset <<= 2;
        tone = live_player->tone;
        offset += (u32)tone;
        source = (ToneData *)offset;
    }

    value = source->words[0];
    ApplyMusicAddressGuardToR3(live_player, live_track, source, value);
    asm volatile("" : "=r"(live_player), "=r"(live_track),
                          "=r"(source), "=r"(value));
    live_track->tone.words[0] = value;

    value = source->words[1];
    ApplyMusicAddressGuardToR3(live_player, live_track, source, value);
    asm volatile("" : "=r"(live_player), "=r"(live_track),
                          "=r"(source), "=r"(value));
    live_track->tone.words[1] = value;

    value = source->words[2];
    ApplyMusicAddressGuardToR3(live_player, live_track, source, value);
    asm volatile("" : "=r"(live_player), "=r"(live_track),
                          "=r"(source), "=r"(value));
    live_track->tone.words[2] = value;
    RETURN_THROUGH_LINK_REGISTER();
}

void MusicCommandSetTrackVolume(
    MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EAFE4");

__attribute__((naked)) void MusicCommandSetTrackVolume(
    MusicPlayerInfo *player, MusicPlayerTrack *track)
{
    register MusicPlayerInfo *live_player asm("r0") = player;
    register MusicPlayerTrack *live_track asm("r1") = track;
    register u32 value asm("r3");

    SAVE_LINK_REGISTER();
    ReadNextMusicCommandByteIntoR3WithAddressGuard(live_player, live_track);
    asm volatile("" : "=r"(live_player), "=r"(live_track), "=r"(value));
    live_track->volume = value;
    {
        register u32 flags asm("r3") = live_track->flags;
        register u32 mask asm("r2") = MUSIC_TRACK_VOLUME_UPDATE_FLAGS;

        flags |= mask;
        live_track->flags = flags;
    }
    RETURN_THROUGH_LINK_REGISTER();
}

void MusicCommandSetTrackPan(
    MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EAFF8");

__attribute__((naked)) void MusicCommandSetTrackPan(
    MusicPlayerInfo *player, MusicPlayerTrack *track)
{
    register MusicPlayerInfo *live_player asm("r0") = player;
    register MusicPlayerTrack *live_track asm("r1") = track;
    register u32 value asm("r3");

    SAVE_LINK_REGISTER();
    ReadNextMusicCommandByteIntoR3WithAddressGuard(live_player, live_track);
    asm volatile("" : "=r"(live_player), "=r"(live_track), "=r"(value));
    value -= MUSIC_COMMAND_CENTERED_VALUE;
    live_track->pan = value;
    {
        register u32 flags asm("r3") = live_track->flags;
        register u32 mask asm("r2") = MUSIC_TRACK_VOLUME_UPDATE_FLAGS;

        flags |= mask;
        live_track->flags = flags;
    }
    RETURN_THROUGH_LINK_REGISTER();
}

void MusicCommandSetTrackPitchBend(
    MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EB00C");

__attribute__((naked)) void MusicCommandSetTrackPitchBend(
    MusicPlayerInfo *player, MusicPlayerTrack *track)
{
    register MusicPlayerInfo *live_player asm("r0") = player;
    register MusicPlayerTrack *live_track asm("r1") = track;
    register u32 value asm("r3");

    SAVE_LINK_REGISTER();
    ReadNextMusicCommandByteIntoR3WithAddressGuard(live_player, live_track);
    asm volatile("" : "=r"(live_player), "=r"(live_track), "=r"(value));
    value -= MUSIC_COMMAND_CENTERED_VALUE;
    live_track->bend = value;
    {
        register u32 flags asm("r3") = live_track->flags;
        register u32 mask asm("r2") = MUSIC_TRACK_PITCH_UPDATE_FLAGS;

        flags |= mask;
        live_track->flags = flags;
    }
    RETURN_THROUGH_LINK_REGISTER();
}

void MusicCommandSetTrackPitchBendRange(
    MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EB020");

__attribute__((naked)) void MusicCommandSetTrackPitchBendRange(
    MusicPlayerInfo *player, MusicPlayerTrack *track)
{
    register MusicPlayerInfo *live_player asm("r0") = player;
    register MusicPlayerTrack *live_track asm("r1") = track;
    register u32 value asm("r3");

    SAVE_LINK_REGISTER();
    ReadNextMusicCommandByteIntoR3WithAddressGuard(live_player, live_track);
    asm volatile("" : "=r"(live_player), "=r"(live_track), "=r"(value));
    live_track->bendRange = value;
    {
        register u32 flags asm("r3") = live_track->flags;
        register u32 mask asm("r2") = MUSIC_TRACK_PITCH_UPDATE_FLAGS;

        flags |= mask;
        live_track->flags = flags;
    }
    RETURN_THROUGH_LINK_REGISTER();
}

void MusicCommandSetTrackLfoDelay(
    MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EB034");

__attribute__((naked)) void MusicCommandSetTrackLfoDelay(
    MusicPlayerInfo *player, MusicPlayerTrack *track)
{
    register MusicPlayerInfo *live_player asm("r0") = player;
    register MusicPlayerTrack *live_track asm("r1") = track;
    register u32 value asm("r3");

    SAVE_LINK_REGISTER();
    ReadNextMusicCommandByteIntoR3WithAddressGuard(live_player, live_track);
    asm volatile("" : "=r"(live_player), "=r"(live_track), "=r"(value));
    live_track->lfoDelay = value;
    RETURN_THROUGH_LINK_REGISTER();
}

void MusicCommandSetTrackModulationType(
    MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EB040");

__attribute__((naked)) void MusicCommandSetTrackModulationType(
    MusicPlayerInfo *player, MusicPlayerTrack *track)
{
    register MusicPlayerInfo *live_player asm("r0") = player;
    register MusicPlayerTrack *live_track asm("r1") = track;
    register u32 value asm("r3");

    SAVE_LINK_REGISTER();
    ReadNextMusicCommandByteIntoR3WithAddressGuard(live_player, live_track);
    asm volatile("" : "=r"(live_player), "=r"(live_track), "=r"(value));
    {
        register u32 current asm("r0") = live_track->modulationType;

        if (current != value) {
            live_track->modulationType = value;
            {
                register u32 flags asm("r3") = live_track->flags;
                register u32 mask asm("r2") = MUSIC_TRACK_VOLUME_PITCH_UPDATE_FLAGS;

                flags |= mask;
                live_track->flags = flags;
            }
        }
    }
    RETURN_THROUGH_LINK_REGISTER();
}

void MusicCommandSetTrackTuning(
    MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EB058");

__attribute__((naked)) void MusicCommandSetTrackTuning(
    MusicPlayerInfo *player, MusicPlayerTrack *track)
{
    register MusicPlayerInfo *live_player asm("r0") = player;
    register MusicPlayerTrack *live_track asm("r1") = track;
    register u32 value asm("r3");

    SAVE_LINK_REGISTER();
    ReadNextMusicCommandByteIntoR3WithAddressGuard(live_player, live_track);
    asm volatile("" : "=r"(live_player), "=r"(live_track), "=r"(value));
    value -= MUSIC_COMMAND_CENTERED_VALUE;
    live_track->tune = value;
    {
        register u32 flags asm("r3") = live_track->flags;
        register u32 mask asm("r2") = MUSIC_TRACK_PITCH_UPDATE_FLAGS;

        flags |= mask;
        live_track->flags = flags;
    }
    RETURN_THROUGH_LINK_REGISTER();
}

void MusicCommandWriteIoRegister(
    MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EB06C");

__attribute__((naked)) void MusicCommandWriteIoRegister(
    MusicPlayerInfo *player, MusicPlayerTrack *track)
{
    register MusicPlayerTrack *live_track asm("r1") = track;
    register u8 *command asm("r2");
    register u32 register_offset asm("r3");
    register volatile u8 *port asm("r0");

    SAVE_LINK_REGISTER();
    command = live_track->command;
    register_offset = *command;
    command++;
    port = (volatile u8 *)MUSIC_COMMAND_IO_BASE;
    asm volatile("" : "+r"(port));
    port += register_offset;
    ReadMusicCommandByteFromR2WithAddressGuard(port, live_track, command, register_offset);
    {
        register u32 register_value asm("r3");

        asm volatile("" : "=r"(port), "=r"(live_track),
                              "=r"(command), "=r"(register_value));
        *port = register_value;
    }
    RETURN_THROUGH_LINK_REGISTER();
}
