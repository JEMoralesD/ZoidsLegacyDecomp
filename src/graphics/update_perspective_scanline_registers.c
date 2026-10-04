#include "m2c_prelude.h"
void BiosCpuSet(s32, s32, s32) asm("func_80ECD2C");

void UpdatePerspectiveScanlineRegisters(u8 full_update) asm("func_08093D9C");

void UpdatePerspectiveScanlineRegisters(u8 full_update) {
    u8 *callback_mode;
    register u8 *displayed_bank asm("r3");
    s32 *scanline_buffers;
    u8 mode;
    volatile u16 *dma_control;
    volatile s32 *dma;

    callback_mode = (u8 *)0x0300342D;
    if (*callback_mode == 0) {
        dma_control = (volatile u16 *)0x040000B0;
        dma_control[5] = 0xC5FF & dma_control[5];
        dma_control[5] = 0x7FFF & dma_control[5];
        dma_control[5];
    }
    displayed_bank = (u8 *)0x0300342C;
    if (full_update != 0) {
        *displayed_bank ^= 1;
    }
    mode = *callback_mode;
    scanline_buffers = (s32 *)0x03003428;
    if (mode == 0) {
        dma = (volatile s32 *)0x040000B0;
        dma[0] = *scanline_buffers + *displayed_bank * 0xA00 + 0x10;
        dma[1] = 0x04000020;
        dma[2] = 0xA6600004;
        dma[2];
    }
    BiosCpuSet(*scanline_buffers + *displayed_bank * 0xA00, 0x04000020, 0x04000004);
}
