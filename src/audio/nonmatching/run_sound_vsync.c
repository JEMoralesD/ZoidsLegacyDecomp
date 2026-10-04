#include "../sound_engine.h"
void RunSoundVSync(void) asm("func_080EB084");

#define DMA1_SOURCE ((volatile u8 *)0x040000BC)
#define DMA_CONTROL_WORD 8
#define DMA_CONTROL_HIGH 10

void RunSoundVSync(void) {
    register struct SoundEngineState *engine asm("r0") = SOUND_ENGINE;
    register u32 signature asm("r2") = SOUND_ENGINE_SIGNATURE;
    register u32 lock_state asm("r3") = engine->signature - signature;
    register s32 value asm("r1");
    register volatile u8 *dma asm("r2");

    if (lock_state > 1)
        return;

    value = engine->dma_vsync_count - 1;
    engine->dma_vsync_count = value;
    if (value > 0)
        return;

    value = engine->dma_reset_period_vblanks;
    engine->dma_vsync_count = value;
    dma = DMA1_SOURCE;
    value = *(volatile u32 *)(dma + DMA_CONTROL_WORD);
    if (value & SOUND_DMA_REPEAT) {
        value = SOUND_DMA_STOP_WORD;
        *(volatile u32 *)(dma + DMA_CONTROL_WORD) = value;
    }
    value = SOUND_DMA_DISABLED;
    *(volatile u16 *)(dma + DMA_CONTROL_HIGH) = value;
    value = SOUND_DMA_FIFO_ENABLED;
    *(volatile u16 *)(dma + DMA_CONTROL_HIGH) = value;
}
