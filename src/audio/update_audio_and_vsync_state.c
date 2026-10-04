#include "sound_engine.h"
void UpdateSoundMixer(void) asm("func_080EB748");
void DisableSoundVSync(void) asm("func_080EBD2C");
void EnableSoundVSync(void) asm("func_080EBD90");

void UpdateAudioAndVSyncState(void) asm("func_08092FA0");

void UpdateAudioAndVSyncState(void) {
    u8 *vsync_state;
    u8 current_state;
    UpdateSoundMixer();
    vsync_state = (u8 *)0x03003170;
    current_state = *vsync_state;
    if (current_state == AUDIO_VSYNC_DISABLE_REQUESTED) {
        DisableSoundVSync();
        *vsync_state = AUDIO_VSYNC_DISABLED;
    } else if (current_state == AUDIO_VSYNC_ENABLE_REQUESTED) {
        EnableSoundVSync();
        *vsync_state = AUDIO_VSYNC_ENABLED;
    }
}

u8 *RequestAudioVSyncState(u8 enabled) asm("func_08092FD0");

u8 *RequestAudioVSyncState(u8 enabled) {
    register u8 requested_state asm("r1") = enabled;
    if (requested_state == 0) {
        register u8 *state_pointer asm("r2") = (u8 *)0x03003170;
        if (*state_pointer == AUDIO_VSYNC_ENABLED) {
            *state_pointer = AUDIO_VSYNC_DISABLE_REQUESTED;
            return (u8 *)0x03003170;
        }
    }
    if (requested_state == 1) {
        if (*(u8 *)0x03003170 == AUDIO_VSYNC_DISABLED) {
            *(u8 *)0x03003170 = AUDIO_VSYNC_ENABLE_REQUESTED;
        }
    }
    return (u8 *)0x03003170;
}
