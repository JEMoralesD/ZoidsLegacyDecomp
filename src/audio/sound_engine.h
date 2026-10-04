#ifndef SOUND_ENGINE_H
#define SOUND_ENGINE_H

#include "mp2k_command_types.h"

enum AudioVSyncState {
    AUDIO_VSYNC_ENABLED = 0,
    AUDIO_VSYNC_DISABLE_REQUESTED = 1,
    AUDIO_VSYNC_DISABLED = 2,
    AUDIO_VSYNC_ENABLE_REQUESTED = 3
};

enum SoundEngineConstants {
    SOUND_ENGINE_SIGNATURE = 0x68736D53,
    SOUND_ENGINE_SIGNATURE_NEGATION = 0x978C92AD,
    SOUND_ENGINE_VSYNC_DISABLE_OFFSET = 10,
    SOUND_ENGINE_PCM_CHANNEL_COUNT = 12,
    SOUND_ENGINE_PSG_CHANNEL_COUNT = 4,
    SOUND_ENGINE_PCM_BUFFER_SIZE = 0x630,
    SOUND_ENGINE_MIXER_COPY_CONTROL = 0x040000E0,
    SOUND_ENGINE_PCM_CLEAR_CONTROL = 0x0500018C,
    SOUND_MODE_DEFAULT = 0x0099E800,
    SOUND_MODE_GAME = 0x0095F880,
    SOUND_SAMPLE_RATE_TABLE = 0x086A32E4,
    SOUND_CPU_CYCLES_PER_FRAME = 0x44940,
    SOUND_SAMPLE_FREQUENCY_NUMERATOR = 0x91D1B,
    SOUND_SAMPLE_FREQUENCY_ROUNDING = 0x1388,
    SOUND_SAMPLE_FREQUENCY_DENOMINATOR = 0x2710,
    SOUND_SAMPLE_STEP_NUMERATOR = 0x01000000,
    SOUND_TIMER_START_SCANLINE = 159,
    MUSIC_PLAYER_TABLE_WORD_STRIDE = 3,
    MUSIC_PLAYER_TABLE_ROM = 0x086A4694,
    SOUND_ENGINE_PLAYER_MEMORY = 0x03007700,
    SOUND_ENGINE_MIXER_RAM = 0x03007758,
    SOUND_ENGINE_STATE_RAM = 0x03006B30,
    SOUND_ENGINE_PSG_CHANNELS_RAM = 0x03007540
};

enum SoundModeMasks {
    SOUND_MODE_REVERB_MASK = 0xFF,
    SOUND_MODE_REVERB_VALUE_MASK = 0x7F,
    SOUND_MODE_PCM_CHANNEL_COUNT_MASK = 0xF00,
    SOUND_MODE_MASTER_VOLUME_MASK = 0xF000,
    SOUND_MODE_SAMPLE_FREQUENCY_MASK = 0xF0000,
    SOUND_MODE_SAMPLE_FREQUENCY_BYTE_MASK = 0xF0,
    SOUND_MODE_PWM_SETTING_MASK = 0xB00000,
    SOUND_MODE_PWM_VALUE_MASK = 0x300000,
    SOUND_BIAS_LOW_BITS_MASK = 0x3F
};

enum MusicTrackInitialization {
    MUSIC_TRACK_ACTIVE = 0x80,
    MUSIC_TRACK_INITIALIZATION_PENDING = 0x40,
    MUSIC_TRACK_DEFAULT_BEND_RANGE = 2,
    MUSIC_TRACK_DEFAULT_VOLUME_EXTRA = 0x40,
    MUSIC_TRACK_DEFAULT_LFO_SPEED = 0x16,
    MUSIC_TRACK_DEFAULT_TONE_TYPE = 1
};

enum SoundDmaControl {
    SOUND_DMA_REPEAT = 0x02000000,
    SOUND_DMA_STOP_WORD = 0x84400004,
    SOUND_DMA_DISABLED = 0x400,
    SOUND_DMA_FIFO_ENABLED = 0xB600,
    SOUND_TIMER_ENABLED = 0x80
};

struct AudioChannelState {
    u8 flags;
    u8 reserved01[0x2B];
    MusicPlayerTrack *owner_track;
    struct AudioChannelState *previous;
    struct AudioChannelState *next;
    u8 reserved38[8];
};

struct PsgChannelState {
    u8 flags;
    u8 channel_id;
    u8 volume_right;
    u8 volume_left;
    u8 reserved04[2];
    u8 sustain;
    u8 reserved07[3];
    u8 amplitude;
    u8 reserved0B[0xE];
    u8 sustain_level;
    u8 reserved1A;
    u8 output_routing;
    u8 output_pan_mask;
    u8 reserved1D[0x23];
};

struct PcmWaveHeader {
    u16 reserved00;
    u16 loop_flags;
    u32 frequency_fixed10;
    u32 loop_start;
    u32 sample_count;
    s8 samples[0];
};

struct MusicPlayerControlState {
    MusicPlayerInfo player;
    u32 signature;
    void *next_update_callback;
    struct MusicPlayerControlState *next_player;
};

struct SoundEngineState {
    u32 signature;
    u8 dma_vsync_count;
    u8 reverb;
    u8 max_pcm_channels;
    u8 master_volume;
    u8 sample_frequency_index;
    u8 reserved09[2];
    u8 dma_reset_period_vblanks;
    u8 max_scanlines;
    u8 reserved0D[3];
    u32 samples_per_vblank;
    u32 sample_frequency_hz;
    u32 sample_step_factor;
    struct PsgChannelState *psg_channels;
    void *update_music_players;
    MusicPlayerInfo *music_player;
    void *update_psg_channels;
    void *stop_psg_channel;
    void *calculate_psg_frequency;
    void *command_handlers;
    void *play_note;
    void *reserved_callback;
    u8 reserved40[0x10];
    struct AudioChannelState pcm_channels[SOUND_ENGINE_PCM_CHANNEL_COUNT];
    s8 pcm_buffer[SOUND_ENGINE_PCM_BUFFER_SIZE];
};

struct MusicSongDefinition {
    void *song_header;
    u16 player_index;
};

struct MusicPlayerDefinition {
    MusicPlayerInfo *player;
    MusicPlayerTrack *tracks;
    u8 track_count;
    u8 reserved09;
    u16 priority_check_enabled;
};

enum MusicCommandHandlerIndex {
    MUSIC_HANDLER_MEMORY_ACCESS = 8,
    MUSIC_HANDLER_LFO_SPEED = 17,
    MUSIC_HANDLER_MODULATION_DEPTH = 19,
    MUSIC_HANDLER_EXTENDED_COMMAND = 28,
    MUSIC_HANDLER_END_TIE = 29,
    MUSIC_HANDLER_SAMPLE_RATE = 30,
    MUSIC_HANDLER_STOP_TRACK = 31,
    MUSIC_HANDLER_UPDATE_FADE = 32,
    MUSIC_HANDLER_UPDATE_VOLUME_AND_PITCH = 33,
    MUSIC_HANDLER_UNLINK_CHANNEL = 34,
    MUSIC_HANDLER_CLEAR_STATE_PREFIX = 35,
    MUSIC_COMMAND_HANDLER_COUNT = 36
};

enum SoundInitializationConstants {
    SOUND_ENGINE_STATE_CLEAR_CONTROL = 0x05000260,
    SOUND_PSG_CHANNELS_CLEAR_CONTROL = 0x05000040,
    SOUND_INITIAL_MASTER_CONTROL = 0x8F,
    SOUND_INITIAL_MIXER_CONTROL = 0xB0E,
    SOUND_INITIAL_PWM_BITS = 0x40,
    SOUND_INITIAL_MAX_PCM_CHANNELS = 8,
    SOUND_INITIAL_MASTER_VOLUME = 15,
    SOUND_INITIAL_SAMPLE_RATE_MODE = 0x40000,
    SOUND_PLAY_NOTE_THUMB_ENTRY = 0x080EB39D,
    SOUND_NOOP_CALLBACK_THUMB_ENTRY = 0x080ECB79,
    SOUND_MUSIC_PLAYER_UPDATE_THUMB_ENTRY = 0x080EB0C1,
    MUSIC_COMMAND_TABLE_RAM = 0x030074B0,
    MUSIC_UNLINK_CALLBACK_RAM = 0x03007538,
    MUSIC_CLEAR_PREFIX_CALLBACK_RAM = 0x0300753C,
    MUSIC_PLAYER_MAX_TRACKS = 16,
    MUSIC_PLAYER_STOPPED_STATUS = 0x80000000,
    PSG_INITIAL_ENVELOPE = 8,
    PSG_CHANNEL_RESTART = 0x80,
    PSG_INITIAL_OUTPUT_VOLUME = 0x77,
    PSG_CHANNEL_1_PAN_MASK = 0x11,
    PSG_CHANNEL_2_PAN_MASK = 0x22,
    PSG_CHANNEL_3_PAN_MASK = 0x44,
    PSG_CHANNEL_4_PAN_MASK = 0x88
};

enum MusicTrackModulation {
    MUSIC_MODULATION_PITCH = 0,
    MUSIC_MODULATION_VOLUME = 1,
    MUSIC_MODULATION_PAN = 2,
    MUSIC_TRACK_VOLUME_UPDATE_FLAGS = 3,
    MUSIC_TRACK_PITCH_UPDATE_FLAGS = 0xC,
    MUSIC_TRACK_VOLUME_PITCH_UPDATE_FLAGS = 0xF,
    MUSIC_COMMAND_CENTERED_VALUE = 0x40,
    MUSIC_COMMAND_IO_BASE = 0x04000060
};

enum MusicFadeControl {
    MUSIC_FADE_KEEP_TRACK_FLAGS = 1,
    MUSIC_FADE_INCREASE_VOLUME = 2,
    MUSIC_FADE_VOLUME_STEP = 0x10,
    MUSIC_FADE_LAST_PARTIAL_VOLUME = 0xFF,
    MUSIC_FADE_FULL_VOLUME = 0x100
};

enum MusicTrackRefreshFlags {
    MUSIC_TRACK_REFRESH_VOLUME = 1,
    MUSIC_TRACK_REFRESH_PITCH = 4,
    MUSIC_TRACK_RETAIN_REFRESHED_FLAGS = 0xFA
};

enum MusicMemoryOperation {
    MUSIC_MEMORY_SET_IMMEDIATE = 0,
    MUSIC_MEMORY_ADD_IMMEDIATE = 1,
    MUSIC_MEMORY_SUBTRACT_IMMEDIATE = 2,
    MUSIC_MEMORY_COPY_BYTE = 3,
    MUSIC_MEMORY_ADD_BYTE = 4,
    MUSIC_MEMORY_SUBTRACT_BYTE = 5,
    MUSIC_MEMORY_BRANCH_EQUAL_IMMEDIATE = 6,
    MUSIC_MEMORY_BRANCH_NOT_EQUAL_IMMEDIATE = 7,
    MUSIC_MEMORY_BRANCH_GREATER_IMMEDIATE = 8,
    MUSIC_MEMORY_BRANCH_GREATER_EQUAL_IMMEDIATE = 9,
    MUSIC_MEMORY_BRANCH_LESS_EQUAL_IMMEDIATE = 10,
    MUSIC_MEMORY_BRANCH_LESS_IMMEDIATE = 11,
    MUSIC_MEMORY_BRANCH_EQUAL_BYTE = 12,
    MUSIC_MEMORY_BRANCH_NOT_EQUAL_BYTE = 13,
    MUSIC_MEMORY_BRANCH_GREATER_BYTE = 14,
    MUSIC_MEMORY_BRANCH_GREATER_EQUAL_BYTE = 15,
    MUSIC_MEMORY_BRANCH_LESS_EQUAL_BYTE = 16,
    MUSIC_MEMORY_BRANCH_LESS_BYTE = 17
};

enum SoundPitchConstants {
    PCM_WAVE_LOOP_FLAGS_MASK = 0xC000,
    PCM_MAX_KEY = 0xB2,
    PSG_NOISE_CHANNEL = 4,
    PSG_NOISE_KEY_START = 0x15,
    PSG_NOISE_MAX_INDEX = 0x3B,
    PSG_TONE_KEY_START = 0x24,
    PSG_TONE_MAX_INDEX = 0x82,
    PSG_KEY_FREQUENCY_CODES_ROM = 0x086A32FC,
    PSG_FREQUENCY_CURVE_ROM = 0x086A3380,
    PSG_NOISE_FREQUENCIES_ROM = 0x086A3398,
    PSG_OUTPUT_RIGHT = 0x0F,
    PSG_OUTPUT_LEFT = 0xF0,
    PSG_OUTPUT_BOTH = 0xFF,
    PSG_MAX_AMPLITUDE = 0x0F
};

#define PSG_CHANNEL_FIELD(channel, type, field) \
    M2C_FIELD(channel, type, PSG_CHANNEL_OFFSET(field))
#define PCM_WAVE_OFFSET(field) ((s32)&((struct PcmWaveHeader *)0)->field)

#define MUSIC_PLAYER_CONTROL_OFFSET(field) \
    ((s32)&((struct MusicPlayerControlState *)0)->field)
#define PSG_CHANNEL_OFFSET(field) ((s32)&((struct PsgChannelState *)0)->field)
#define MUSIC_PLAYER_CONTROL_FIELD(player, type, field) \
    M2C_FIELD(player, type, MUSIC_PLAYER_CONTROL_OFFSET(field))
#define SOUND_ENGINE_OFFSET(field) ((s32)&((struct SoundEngineState *)0)->field)
#define MUSIC_SONG_OFFSET(field) ((s32)&((struct MusicSongDefinition *)0)->field)
#define MUSIC_PLAYER_OFFSET(field) ((s32)&((MusicPlayerInfo *)0)->field)
#define MUSIC_TRACK_OFFSET(field) ((s32)&((MusicPlayerTrack *)0)->field)
#define MUSIC_PLAYER_DEFINITION_OFFSET(field) \
    ((s32)&((struct MusicPlayerDefinition *)0)->field)

#define SOUND_ENGINE_BYTE(state, field) \
    M2C_FIELD(state, u8 *, SOUND_ENGINE_OFFSET(field))
#define SOUND_ENGINE_SIGNED_BYTE(state, field) \
    M2C_FIELD(state, s8 *, SOUND_ENGINE_OFFSET(field))
#define SOUND_ENGINE_WORD(state, field) \
    M2C_FIELD(state, s32 *, SOUND_ENGINE_OFFSET(field))
#define SOUND_ENGINE_FIELD(state, type, field) \
    M2C_FIELD(state, type, SOUND_ENGINE_OFFSET(field))
#define MUSIC_PLAYER_FIELD(player, type, field) \
    M2C_FIELD(player, type, MUSIC_PLAYER_OFFSET(field))
#define MUSIC_TRACK_FIELD(track, type, field) \
    M2C_FIELD(track, type, MUSIC_TRACK_OFFSET(field))
#define MUSIC_PLAYER_DEFINITION_FIELD(entry, type, field) \
    M2C_FIELD(entry, type, MUSIC_PLAYER_DEFINITION_OFFSET(field))

#endif
