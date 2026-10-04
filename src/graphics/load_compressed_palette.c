#include "m2c_prelude.h"
#include "graphics_resources.h"
M2C_UNK BiosCpuFastSet(u16 *, u16 *, u32) asm("func_80ECD28");               /* extern */
M2C_UNK BiosLz77ToWram(void *, u16 *) asm("func_80ECD38");                   /* extern */

void LoadCompressedPalette(void *compressed_palette, u16 *palette_destination, u16 *work_buffer) asm("func_0809A1BC");

void LoadCompressedPalette(void *compressed_palette, u16 *palette_destination, u16 *work_buffer) {
    BiosLz77ToWram(compressed_palette, work_buffer);
    if (palette_destination == (u16 *)0x05000000) {
        *work_buffer = *palette_destination;
    }
    BiosCpuFastSet(work_buffer, palette_destination, (u32) ((M2C_FIELD(compressed_palette, u8 *, 1) | (M2C_FIELD(compressed_palette, u8 *, 2) << 8) | (M2C_FIELD(compressed_palette, u8 *, 3) << 0x10)) << 9) >> 0xB);
}
