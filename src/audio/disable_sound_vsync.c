#include "sound_engine.h"
extern void BiosCpuSet(void *, void *, s32, s32) asm("func_80ECD2C");
extern s32 *D_03007FF0;
extern s32 D_040000C4;
extern s16 D_040000C6;

void DisableSoundVSync(void) asm("func_080EBD2C");

void DisableSoundVSync(void) {
    s32 zero_sample;
    s32 *sound_state = D_03007FF0;
    s32 signature = *sound_state;
    if ((u32)(signature + SOUND_ENGINE_SIGNATURE_NEGATION) <= 1U) {
        *sound_state = signature + SOUND_ENGINE_VSYNC_DISABLE_OFFSET;
        if (D_040000C4 & SOUND_DMA_REPEAT) {
            D_040000C4 = SOUND_DMA_STOP_WORD;
        }
        D_040000C6 = SOUND_DMA_DISABLED;
        zero_sample = 0;
        BiosCpuSet(&zero_sample, (u8 *)sound_state + SOUND_ENGINE_OFFSET(pcm_buffer), SOUND_ENGINE_PCM_CLEAR_CONTROL, SOUND_DMA_DISABLED);
    }
}
