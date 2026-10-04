#include "m2c_prelude.h"
#include "../../graphics/screen_effects.h"

extern s16 Sin256(s16) asm("func_08092A90");
extern void QueueCopy(void *, void *, s32) asm("func_08095208");
extern u8 ModuloSigned32(s32, s32) asm("func_080ECE30");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");

extern u8 gSceneBg1DisplayedBank asm("D_02031C99");
extern u8 gSceneBg1ScrollBuffers[] asm("D_02031C9A");

void RunSceneBg1DistortionTask(void) asm("func_080A8FAC");

void RunSceneBg1DistortionTask(void)
{
    register u32 split_elapsed_updates asm("r6");
    register s32 palette_frame_or_scanline_offset asm("r4");
    register u32 scanline asm("r5");
    /* The native task inherits its initial phase from R8. */
    register s32 wave_phase asm("r8");
    register s32 next_wave_phase asm("r10");
    register u8 *scroll_buffer_base asm("r9");
    u8 *displayed_bank_address;

    split_elapsed_updates = 0;
    palette_frame_or_scanline_offset = 0;
    scroll_buffer_base = gSceneBg1ScrollBuffers;
    displayed_bank_address = &gSceneBg1DisplayedBank;
    asm volatile("" : "+r"(scroll_buffer_base), "+r"(displayed_bank_address));
loop:
    scanline = 0;
    next_wave_phase = wave_phase + 1;
    {
        s32 palette_update_spills[2];
        register s32 upload_view asm("r0");

        upload_view = palette_frame_or_scanline_offset << 5;
        asm volatile("" : "+r"(upload_view));
        palette_update_spills[1] = upload_view;
        palette_update_spills[0] = palette_frame_or_scanline_offset + 1;
        asm volatile("" : : "m"(palette_update_spills[0]), "m"(palette_update_spills[1]));

    do {
        u8 mode = *(u8 *)SCENE_BG1_DISTORTION_PHASE_RAM;
        if (mode == SCENE_BG1_DISTORTION_VERTICAL_WAVE) {
            goto zero;
        }
        if (mode == SCENE_BG1_DISTORTION_SPLIT_SCANLINES) {
            goto one;
        }
        palette_frame_or_scanline_offset = scanline << 2;
        goto sine;
zero:
        {
            register s32 bank asm("r1");
            register s32 zero_offset asm("r2") = scanline << 2;
            register u32 address asm("r0");

            asm volatile("ldrb r0, [%1]\n\tmov %0, #1\n\teor %0, r0"
                         : "=r"(bank)
                         : "r"(displayed_bank_address)
                         : "r0");
            address = bank << 2;
            address += bank;
            address <<= 7;
            asm volatile("add %0, %1, %0" : "+r"(address) : "r"(zero_offset));
            address += (u32)scroll_buffer_base;
            *(u16 *)address = mode;
            palette_frame_or_scanline_offset = zero_offset;
        }
        goto sine;
one:
        {
            s32 split_start_update = 0x9F - scanline;
            palette_frame_or_scanline_offset = scanline << 2;
            if (split_elapsed_updates >= split_start_update) {
                s32 value = split_elapsed_updates;
                u16 split_x_offset;
                register s32 bank asm("r1");
                register u32 address asm("r0");

                value -= 0x9F;
                value += scanline;
                split_x_offset = value * 4;
                if ((s16)(value * 4) > 0x100) {
                    split_x_offset = 0x100;
                }
                bank = 1;
                if (mode & scanline) {
                    split_x_offset = (u32)(0 - ((u32)split_x_offset << 16)) >> 16;
                }
                asm volatile("ldrb r0, [%1]\n\teor %0, r0"
                             : "+r"(bank)
                             : "r"(displayed_bank_address)
                             : "r0");
                address = bank * 0x280;
                asm volatile("add %0, %1, %0" : "+r"(address) : "r"(palette_frame_or_scanline_offset));
                address += (u32)scroll_buffer_base;
                *(u16 *)address = split_x_offset;
            }
        }
sine:

        {
            register s16 sine asm("r3") = Sin256((s16)((scanline + wave_phase) * 2));
            register u16 *sine_destination asm("r2");

            {
                register s32 bank asm("r1");
                register u32 address asm("r0");
                register u8 *sine_base asm("r1");

                asm volatile("ldrb r0, [%1]\n\tmov %0, #1\n\teor %0, r0"
                             : "=r"(bank)
                             : "r"(displayed_bank_address)
                             : "r0");
                address = bank * 0x280;
                asm volatile("add %0, %1, %0" : "+r"(address) : "r"(palette_frame_or_scanline_offset));
                sine_base = (u8 *)(SCENE_BG1_SCROLL_BUFFERS_RAM + SCENE_BG1_SCROLL_OFFSET(y));
                asm volatile("" : "+r"(sine_base));
                sine_destination = (u16 *)(address + (u32)sine_base);
                asm volatile("" : "+r"(sine_destination));
            }
            {
                register s32 bg1_y_fixed8 asm("r0") = *(s32 *)SCENE_BG1_SCROLL_Y_FIXED8_RAM;

                if (bg1_y_fixed8 < 0) {
                    bg1_y_fixed8 += 0xFF;
                }
                {
                    register s32 bg1_y_pixels asm("r1") = bg1_y_fixed8 >> 8;
                    register s32 sine_value asm("r0") = (s16)sine;
                    register s32 result asm("r0");

                    if (sine_value < 0) {
                        sine_value += 0x3F;
                    }
                    result = bg1_y_pixels + (sine_value >> 6);
                    *sine_destination = result;
                }
            }
        }
        {
            register s32 next_column asm("r0");

            next_column = scanline + 1;
            scanline = (u8)next_column;
        }
    } while (scanline <= 0x9F);

    {
        register s32 angle_view asm("r1");
        register s32 narrowed_angle asm("r0");

        angle_view = next_wave_phase;
        asm volatile("" : "+r"(angle_view));
        narrowed_angle = (u8)angle_view;
        asm volatile("" : "+r"(narrowed_angle));
        wave_phase = narrowed_angle;
    }
    if (*(u8 *)SCENE_BG1_DISTORTION_PHASE_RAM != 0) {
        split_elapsed_updates++;
        if (split_elapsed_updates == 0xE0) {
            *(u8 *)SCENE_BG1_DISTORTION_PHASE_RAM = SCENE_BG1_DISTORTION_SPLIT_COMPLETE;
        }
    }
    {
        register u8 *upload_source asm("r0");
        register s32 upload_view asm("r1");

        upload_source = (u8 *)SCENE_BG1_SCROLL_PALETTE_FRAMES_ROM;
        asm volatile("" : "+r"(upload_source));
        upload_view = palette_update_spills[1];
        asm volatile("" : "+r"(upload_view));
        upload_source = (u8 *)((u32)upload_view + (u32)upload_source);
        QueueCopy(upload_source, (void *)0x05000180, 0x20);
    }
    palette_frame_or_scanline_offset = (u8)ModuloSigned32(palette_update_spills[0], 0x1E);
    YieldTaskForUpdates(1);
    }
    goto loop;
}
