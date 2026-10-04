#include "sound_engine.h"
extern void ClearMusicStatePrefixViaCallback(void *) asm("func_080EBABC");

void InitializeMusicPlayer(void *player, s8 *tracks, int track_count) asm("func_080EBDC8");

void InitializeMusicPlayer(void *player, s8 *tracks, int track_count) {
    s32 previous_update_callback;
    s32 signature;
    register s8 *track_pointer asm("r6");
    u8 tracks_remaining;
    register void *sound_state asm("r5");

    track_pointer = tracks;
    track_count = track_count << 0x18;
    tracks_remaining = (u32)track_count >> 0x18;
    if (tracks_remaining != 0) {
        if ((u32) tracks_remaining > MUSIC_PLAYER_MAX_TRACKS) {
            tracks_remaining = MUSIC_PLAYER_MAX_TRACKS;
        }
        sound_state = *(void **)0x03007FF0;
        signature = SOUND_ENGINE_WORD(sound_state, signature);
        if (signature == SOUND_ENGINE_SIGNATURE) {
            SOUND_ENGINE_WORD(sound_state, signature) = (s32) (signature + 1);
            ClearMusicStatePrefixViaCallback(player);
            MUSIC_PLAYER_FIELD(player, s8 **, tracks) = track_pointer;
            MUSIC_PLAYER_FIELD(player, u8 *, trackCount) = tracks_remaining;
            MUSIC_PLAYER_FIELD(player, s32 *, status) = MUSIC_PLAYER_STOPPED_STATUS;
            if (tracks_remaining != 0) {
                do {
                    *track_pointer = 0;
                    tracks_remaining -= 1;
                    track_pointer += sizeof(MusicPlayerTrack);
                } while (tracks_remaining != 0);
            }
            previous_update_callback = SOUND_ENGINE_WORD(sound_state, update_music_players);
            if (previous_update_callback != 0) {
                MUSIC_PLAYER_CONTROL_FIELD(player, s32 *, next_update_callback) = previous_update_callback;
                MUSIC_PLAYER_CONTROL_FIELD(player, void **, next_player) = (void *) SOUND_ENGINE_FIELD(sound_state, void **, music_player);
                SOUND_ENGINE_WORD(sound_state, update_music_players) = 0;
            }
            SOUND_ENGINE_FIELD(sound_state, void **, music_player) = player;
            SOUND_ENGINE_WORD(sound_state, update_music_players) = SOUND_MUSIC_PLAYER_UPDATE_THUMB_ENTRY;
            SOUND_ENGINE_WORD(sound_state, signature) = SOUND_ENGINE_SIGNATURE;
            MUSIC_PLAYER_CONTROL_FIELD(player, s32 *, signature) = SOUND_ENGINE_SIGNATURE;
        }
    }
}
