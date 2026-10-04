#include "m2c_prelude.h"
#include "screen_effects.h"
s16 Sin256(s32) asm("func_08092A90"); /* extern */

extern u8 gSceneMosaicState asm("D_02032B9C");
extern u8 gSceneMosaicPhase asm("D_02032B9D");
extern u8 gSceneMosaicDisplayedBank asm("D_02032B9E");
extern s16 gSceneMosaicBuffers[] asm("D_02032BA0");

void BuildSceneScanlineMosaic(void) asm("func_080BA8C0");

void BuildSceneScanlineMosaic(void) {
    s32 bg_mosaic_wave;
    s32 obj_mosaic_size;
    s32 mosaic_size_carrier;
    u8 signed_bg_mosaic_size;
    u8 bg_mosaic_size;
    u8 scanline;
    s16 *mosaic_buffer_base;
    register s16 *mosaic_destination asm("r3");
    s32 mosaic_register_value;

    if (gSceneMosaicState != SCENE_SCANLINE_DISABLED) {
        scanline = 0;
        mosaic_buffer_base = gSceneMosaicBuffers;
        do {
            bg_mosaic_wave = Sin256(gSceneMosaicPhase + scanline);
            if (bg_mosaic_wave < 0) {
                bg_mosaic_wave += 0x1F;
            }
            signed_bg_mosaic_size = (bg_mosaic_wave >> 5) + 7;
            bg_mosaic_size = signed_bg_mosaic_size;
            if ((s8)signed_bg_mosaic_size < 0) {
                bg_mosaic_size = 0;
            }
            if ((s8)bg_mosaic_size > 0xE) {
                bg_mosaic_size = 0xE;
            }
            mosaic_destination = (s16 *)(scanline << 1);
            {
                register s32 bank_or_byte_offset asm("r0");
                register s32 write_bank asm("r1");

                bank_or_byte_offset = gSceneMosaicDisplayedBank;
                write_bank = 1;
                write_bank ^= bank_or_byte_offset;
                bank_or_byte_offset = write_bank << 2;
                bank_or_byte_offset += write_bank;
                bank_or_byte_offset <<= 6;
                mosaic_destination = (s16 *)((u8 *)mosaic_destination + bank_or_byte_offset);
            }
            mosaic_destination = (s16 *)((u8 *)mosaic_destination + (s32)mosaic_buffer_base);
            mosaic_size_carrier = bg_mosaic_size;
            mosaic_size_carrier <<= 24;
            mosaic_size_carrier >>= 24;
            mosaic_register_value = mosaic_size_carrier << 4;
            mosaic_register_value |= mosaic_size_carrier;
            obj_mosaic_size = 0xE;
            obj_mosaic_size -= mosaic_size_carrier;
            mosaic_size_carrier = obj_mosaic_size;
            mosaic_size_carrier <<= 8;
            mosaic_register_value |= mosaic_size_carrier;
            obj_mosaic_size <<= 12;
            mosaic_register_value |= obj_mosaic_size;
            *mosaic_destination = (s16)mosaic_register_value;
            scanline += 1;
        } while (scanline <= 0x9F);
        gSceneMosaicPhase += 4;
    }
}
