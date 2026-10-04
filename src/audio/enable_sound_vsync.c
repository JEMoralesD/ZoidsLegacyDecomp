#include "sound_engine.h"
void EnableSoundVSync(void) asm("func_080EBD90");

void EnableSoundVSync(void) {
    s32 *sound_state;
    s32 signature;
    sound_state = *(s32 **)0x03007FF0;
    signature = sound_state[0];
    if (signature != SOUND_ENGINE_SIGNATURE) {
        *(u16 *)0x040000C6 = SOUND_DMA_FIFO_ENABLED;
        *(volatile u8 *)((s32)sound_state + SOUND_ENGINE_OFFSET(dma_vsync_count));
        *(u8 *)((s32)sound_state + SOUND_ENGINE_OFFSET(dma_vsync_count)) = 0;
        sound_state[0] = signature - SOUND_ENGINE_VSYNC_DISABLE_OFFSET;
    }
}
