#include "sound_engine.h"

void UpdateMusicTrackVolumeAndPitch(s32 unused_player, void *track) asm("func_080EC02C");

void UpdateMusicTrackVolumeAndPitch(s32 unused_player, void *track) {
    register void *state asm("r2");
    register s32 work asm("r0");
    register s32 value asm("r1");
    register s32 volume_or_flags asm("r3");
    register s32 modulation_type asm("r4");

    state = track;
    work = MUSIC_TRACK_REFRESH_VOLUME;
    value = MUSIC_TRACK_FIELD(state, u8 *, flags);
    work &= value;
    if (work != 0) {
        volume_or_flags = MUSIC_TRACK_FIELD(state, u8 *, volumeExtra);
        asm volatile("" : "+r"(volume_or_flags));
        value = MUSIC_TRACK_FIELD(state, u8 *, volume);
        work = volume_or_flags;
        work *= value;
        volume_or_flags = (u32)work >> 5;
        modulation_type = MUSIC_TRACK_FIELD(state, u8 *, modulationType);
        if (modulation_type == MUSIC_MODULATION_VOLUME) {
            work = MUSIC_TRACK_OFFSET(modulationCalculated);
            work = M2C_FIELD(state, s8 *, work);
            work += 0x80;
            work *= volume_or_flags;
            volume_or_flags = (u32)work >> 7;
        }

        work = MUSIC_TRACK_OFFSET(pan);
        work = M2C_FIELD(state, s8 *, work);
        work <<= 1;
        value = MUSIC_TRACK_OFFSET(panExtra);
        value = M2C_FIELD(state, s8 *, value);
        value = work + value;
        if (modulation_type == MUSIC_MODULATION_PAN) {
            work = MUSIC_TRACK_OFFSET(modulationCalculated);
            work = M2C_FIELD(state, s8 *, work);
            value += work;
        }
        work = 0x80;
        work = -work;
        if (value < work) {
            value = work;
        } else if (value > 0x7F) {
            value = 0x7F;
        }

        work = value;
        work += 0x80;
        work *= volume_or_flags;
        work = (u32)work >> 8;
        MUSIC_TRACK_FIELD(state, u8 *, volumeRight) = work;
        work = 0x7F;
        work -= value;
        work *= volume_or_flags;
        work = (u32)work >> 8;
        MUSIC_TRACK_FIELD(state, u8 *, volumeLeft) = work;
    }

    value = MUSIC_TRACK_FIELD(state, u8 *, flags);
    work = MUSIC_TRACK_REFRESH_PITCH;
    work &= value;
    volume_or_flags = value;
    if (work != 0) {
        work = MUSIC_TRACK_OFFSET(bend);
        work = M2C_FIELD(state, s8 *, work);
        value = MUSIC_TRACK_FIELD(state, u8 *, bendRange);
        work *= value;
        value = MUSIC_TRACK_OFFSET(tune);
        value = M2C_FIELD(state, s8 *, value);
        value += work;
        value <<= 2;
        work = MUSIC_TRACK_OFFSET(keyShift);
        work = M2C_FIELD(state, s8 *, work);
        work <<= 8;
        value += work;
        work = MUSIC_TRACK_OFFSET(keyShiftExtra);
        work = M2C_FIELD(state, s8 *, work);
        work <<= 8;
        value += work;
        work = MUSIC_TRACK_FIELD(state, u8 *, pitchExtra);
        value = work + value;
        work = MUSIC_TRACK_FIELD(state, u8 *, modulationType);
        if (work == MUSIC_MODULATION_PITCH) {
            work = MUSIC_TRACK_OFFSET(modulationCalculated);
            work = M2C_FIELD(state, s8 *, work);
            work <<= 4;
            value += work;
        }
        work = value >> 8;
        MUSIC_TRACK_FIELD(state, u8 *, keyMapped) = work;
        MUSIC_TRACK_FIELD(state, u8 *, pitchMapped) = value;
    }

    work = MUSIC_TRACK_RETAIN_REFRESHED_FLAGS;
    work &= volume_or_flags;
    MUSIC_TRACK_FIELD(state, u8 *, flags) = work;
}
