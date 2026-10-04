#include "m2c_prelude.h"
M2C_UNK UpdatePerspectiveScanlineBuffers() asm("func_08093B7C");
M2C_UNK SetHBlankCallback(s32, M2C_UNK) asm("func_8094290");
M2C_UNK BiosCpuSet(M2C_UNK, M2C_UNK, M2C_UNK) asm("func_80ECD2C");
extern u8 D_03003430;
extern u16 gPerspectiveCamera[] asm("D_030033C4");

void InitializePerspectiveScanlineBuffers(s32 scanline_buffers, u8 use_hblank_callback) asm("func_08093AE8");

void InitializePerspectiveScanlineBuffers(s32 scanline_buffers, u8 use_hblank_callback) {
    *(u8 *)0x0300342C = 0;
    *(u8 *)0x0300342D = use_hblank_callback;
    if (use_hblank_callback == 1) {
        BiosCpuSet(0x08000638, &D_03003430, 0x0400001C);
    }
    *(s32 *)0x03003428 = scanline_buffers;
    gPerspectiveCamera[22] = gPerspectiveCamera[6] + 1;
    UpdatePerspectiveScanlineBuffers();
    if (use_hblank_callback == 1) {
        *(s16 *)0x04000208 = 0;
        SetHBlankCallback(0, &D_03003430);
        *(s16 *)0x04000208 = (s16)use_hblank_callback;
    }
}
