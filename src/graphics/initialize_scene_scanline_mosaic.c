#include "m2c_prelude.h"
#include "screen_effects.h"
M2C_UNK BiosCpuSet(M2C_UNK, M2C_UNK, M2C_UNK) asm("func_080ECD2C");
extern u8 gSceneMosaicState asm("D_02032B9C");
extern u8 gSceneMosaicDisplayedBank asm("D_02032B9E");
extern u8 gSceneMosaicPhase asm("D_02032B9D");
extern u8 gDisplayUpdateFlags asm("D_03000074");

void InitializeSceneScanlineMosaic(void) asm("func_080BA878");

void InitializeSceneScanlineMosaic(void) {
    gSceneMosaicState = SCENE_SCANLINE_STARTING;
    gSceneMosaicDisplayedBank = 0;
    gSceneMosaicPhase = 0;
    BiosCpuSet((void *)SCENE_MOSAIC_CALLBACK_CODE_ROM, (void *)SCENE_SCANLINE_CALLBACK_RAM, (void *)0x04000014);
    gDisplayUpdateFlags |= 8;
}
