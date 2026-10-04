#include "m2c_prelude.h"

void BiosCpuFastSet(void *, void *, s32) asm("func_80ECD28");
void PlaySong(s32) asm("func_8092E84");
void QueueCopy(void *, void *, s32) asm("func_8095208");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

void RunSceneObjPaletteRedPulseTask(void) asm("func_080A6E14");

void RunSceneObjPaletteRedPulseTask(void)
{
    u16 original_obj_palette[16];
    u16 red_pulse_palette[16];
    volatile u32 shifted_spill;
    register u32 pulse_update asm("r4");
    register u16 *red_pulse_palette_base asm("r9");
    register u32 max_red_component asm("r8");
    u8 palette_color_index;

    BiosCpuFastSet((void *)0x05000200, original_obj_palette, 8);
    pulse_update = 0;
    red_pulse_palette_base = red_pulse_palette;
    {
        register u32 max_load asm("r1") = 31;
        asm volatile("" : "+r"(max_load));
        max_red_component = max_load;
    }

outer:
    palette_color_index = 0;
    {
        register u32 next_pulse_update asm("r10");
        register u32 next_load asm("r2") = pulse_update + 1;
        register u32 max_view asm("r0");
        register u32 shifted asm("r3");

        asm volatile("" : "+r"(next_load));
        next_pulse_update = next_load;
        max_view = max_red_component;
        asm volatile("" : "+r"(max_view));
        pulse_update &= max_view;
        shifted = pulse_update << 24;
        do {
            register u32 offset_work asm("r0") = palette_color_index << 1;
            register u8 *source_base asm("r2") = (u8 *)original_obj_palette;
            register u8 *byte_address asm("r1");
            register u32 byte_value asm("r1");
            register u32 offset asm("r6");
            register u32 red_component asm("r5");
            register u32 pulse_strength asm("r4");
            register s32 red_blend_delta asm("r0");

            byte_address = source_base + offset_work;
            byte_value = *byte_address;
            red_component = max_red_component;
            red_component &= byte_value;
            pulse_strength = shifted >> 24;
            offset = offset_work;
            if (pulse_strength == 0) {
                register s32 call_arg asm("r0") = 0x7F;
                asm volatile("" : "+r"(call_arg));
                shifted_spill = shifted;
                PlaySong(call_arg);
                shifted = shifted_spill;
            }

            if ((u8)(pulse_strength - 9) <= 7) {
                pulse_strength = (u8)(16 - pulse_strength);
            } else if (pulse_strength > 16) {
                pulse_strength = 0;
            }

            {
                register u32 max_view asm("r1") = max_red_component;
                asm volatile("" : "+r"(max_view));
                red_blend_delta = max_view - red_component;
            }
            red_blend_delta *= pulse_strength;
            if (red_blend_delta < 0) {
                red_blend_delta += 7;
            }
            red_blend_delta >>= 3;
            asm volatile("add %0, %1, %0"
                         : "+r"(red_blend_delta)
                         : "r"(red_component));
            red_blend_delta <<= 24;
            red_component = (u32)red_blend_delta >> 24;
            {
                register u16 *destination asm("r2");

                {
                    register u16 *output_view asm("r4") = red_pulse_palette_base;
                    asm volatile("" : "+r"(output_view));
                    destination = (u16 *)((u8 *)output_view + offset);
                }
                {
                    register u16 *source_view asm("r1") = original_obj_palette;
                    register u16 *source_address asm("r0");
                    register u32 source_value asm("r0");
                    asm volatile("" : "+r"(source_view));
                    asm volatile("add %0, %1, %2"
                                 : "=r"(source_address)
                                 : "r"(source_view), "r"(offset));
                    source_value = *source_address;
                    {
                        register u32 color_mask asm("r4") = 0xFFE0;
                        register u32 mask_view asm("r1");
                        asm volatile("" : "+r"(color_mask));
                        mask_view = color_mask;
                        asm volatile("" : "+r"(mask_view));
                        source_value &= mask_view;
                    }
                    red_component |= source_value;
                    *destination = red_component;
                }
            }
            palette_color_index++;
        } while (palette_color_index <= 15);

        {
            register u16 *upload_source asm("r0") = red_pulse_palette_base;
            asm volatile("" : "+r"(upload_source));
            QueueCopy(upload_source, (void *)0x05000200, 32);
        }
        {
            register u32 next_view asm("r1") = next_pulse_update;
            register u32 wrapped asm("r0");
            asm volatile("" : "+r"(next_view));
            wrapped = next_view << 24;
            pulse_update = wrapped >> 24;
        }
    }
    YieldTaskForUpdates(1);
    goto outer;
}
