#include "m2c_prelude.h"

typedef union {
    struct {
        u8 type;
        u8 key;
        u8 length;
        u8 panSweep;
        void *wave;
        u8 attack;
        u8 decay;
        u8 sustain;
        u8 release;
    } fields;
    u32 words[3];
} ToneData;

typedef struct {
    u8 flags;
    u8 wait;
    u8 patternLevel;
    u8 repeatCount;
    u8 gateTime;
    u8 key;
    u8 velocity;
    u8 runningStatus;
    u8 keyMapped;
    u8 pitchMapped;
    s8 keyShift;
    s8 keyShiftExtra;
    s8 tune;
    u8 pitchExtra;
    s8 bend;
    u8 bendRange;
    u8 volumeRight;
    u8 volumeLeft;
    u8 volume;
    u8 volumeExtra;
    s8 pan;
    s8 panExtra;
    s8 modulationCalculated;
    u8 modulationDepth;
    u8 modulationType;
    u8 lfoSpeed;
    u8 lfoSpeedCounter;
    u8 lfoDelay;
    u8 lfoDelayCounter;
    u8 priority;
    u8 pseudoEchoVolume;
    u8 pseudoEchoLength;
    void *channel;
    ToneData tone;
    u8 pad30[10];
    u16 timer;
    u32 field3C;
    u8 *command;
    u8 *patternStack[3];
} MusicPlayerTrack;

typedef struct {
    void *songHeader;
    u32 status;
    u8 trackCount;
    u8 priority;
    u8 command;
    u8 priorityCheckEnabled;
    u32 clock;
    u8 pad10[8];
    u8 *memory;
    u16 tempo;
    u16 tempoScale;
    u16 tempoInterval;
    u16 tempoCounter;
    u16 fadeInterval;
    u16 fadeCounter;
    u16 fadeVolume;
    MusicPlayerTrack *tracks;
    ToneData *tone;
} MusicPlayerInfo;

extern void ReadNextMusicCommandByteIntoR3WithAddressGuard(
    MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EAEF4");
extern void ApplyMusicAddressGuardToR3(
    MusicPlayerInfo *player, MusicPlayerTrack *track,
    ToneData *source, u32 value) asm("func_80EAEDA");
extern void ReadMusicCommandByteFromR2WithAddressGuard(
    volatile u8 *io_register, MusicPlayerTrack *track,
    u8 *command_cursor, u32 unused_r3) asm("func_80EAEF6");

#define SAVE_LINK_REGISTER() asm volatile("mov ip, lr" : : : "memory")
#define RETURN_THROUGH_LINK_REGISTER() asm volatile("bx ip")
