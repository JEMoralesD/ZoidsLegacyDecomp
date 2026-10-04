#include "m2c_prelude.h"
void UpdateLinkDriverPackets(void) asm("func_08092834");
void UpdateAudioAndVSyncState(void) asm("func_08092FA0");

void WaitForFrameUpdates(u8 requested_frames) asm("func_0809223C");

void WaitForFrameUpdates(u8 requested_frames) {
    volatile u8 *update_request;
    u8 elapsed_vblanks;
    int remaining_frames;

    elapsed_vblanks = *(u8 *)0x0300067D;
    if (requested_frames > elapsed_vblanks) {
        requested_frames = requested_frames - elapsed_vblanks;
    } else {
        requested_frames = 1;
    }
    remaining_frames = (u8)(requested_frames - 1);
    if (remaining_frames != 0xFF) {
        update_request = (volatile u8 *)0x0300067C;
        do {
            if (remaining_frames != 0) {
                *update_request = 2;
            } else {
                *update_request = 1;
            }
            remaining_frames -= 1;
            do { } while (*update_request != 0);
            remaining_frames = (u8)remaining_frames;
        } while (remaining_frames != 0xFF);
    }
    UpdateLinkDriverPackets();
    UpdateAudioAndVSyncState();
    *(u8 *)0x0300067D = 0;
}
