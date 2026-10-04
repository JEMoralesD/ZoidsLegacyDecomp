#include "m2c_prelude.h"
#include "screen_effects.h"

void SetHBlankCallback(s32, s32) asm("func_08094290");
void ClearHBlankCallback(s32) asm("func_080942E0");

void UpdateSceneScanlineMosaicRegisters(s32 advance_buffer) asm("func_080BA94C");

void UpdateSceneScanlineMosaicRegisters(s32 advance_buffer)
{
    if ((advance_buffer << 24) != 0) {
        register u8 *mosaic_state_address asm("r6") = (u8 *)SCENE_MOSAIC_STATE_RAM;
        register u32 mosaic_state asm("r5") = *mosaic_state_address;

        if (mosaic_state == SCENE_SCANLINE_STARTING) {
            register volatile u16 *interrupt_master_enable asm("r4") =
                (u16 *)0x04000208;

            *interrupt_master_enable = 0;
            SetHBlankCallback(1, SCENE_SCANLINE_CALLBACK_RAM);
            {
                register volatile u16 *bg_control_or_mosaic_register asm("r1") =
                    (u16 *)0x04000008;
                register u32 mask asm("r2");
                u32 current;

                current = *bg_control_or_mosaic_register;
                mask = 0x40;
                current |= mask;
                *bg_control_or_mosaic_register = current;
                bg_control_or_mosaic_register++;
                current = *bg_control_or_mosaic_register;
                current |= mask;
                *bg_control_or_mosaic_register = current;
                bg_control_or_mosaic_register++;
                current = *bg_control_or_mosaic_register;
                current |= mask;
                *bg_control_or_mosaic_register = current;
                bg_control_or_mosaic_register++;
                current = *bg_control_or_mosaic_register;
                current |= mask;
                *bg_control_or_mosaic_register = current;
            }
            *interrupt_master_enable = mosaic_state;
            *mosaic_state_address = SCENE_SCANLINE_ACTIVE;
            goto displayed_bank_address;
        }
        if (mosaic_state == SCENE_SCANLINE_STOPPING) {
            register volatile u16 *interrupt_master_enable asm("r4") =
                (u16 *)0x04000208;

            mosaic_state = 0;
            *interrupt_master_enable = mosaic_state;
            ClearHBlankCallback(1);
            {
                register volatile u16 *bg_control_or_mosaic_register asm("r3") =
                    (u16 *)0x04000008;
                register u32 mask asm("r1");
                register u32 current asm("r2");
                register u32 result asm("r0");

                current = *bg_control_or_mosaic_register;
                asm volatile("" : "+r"(current));
                mask = 0xFFBF;
                asm volatile("" : "+r"(mask));
                result = mask;
                asm volatile("" : "+r"(result));
                result &= current;
                *bg_control_or_mosaic_register = result;
                bg_control_or_mosaic_register++;
                current = *bg_control_or_mosaic_register;
                asm volatile("" : "+r"(current));
                result = mask;
                asm volatile("" : "+r"(result));
                result &= current;
                *bg_control_or_mosaic_register = result;
                bg_control_or_mosaic_register++;
                current = *bg_control_or_mosaic_register;
                asm volatile("" : "+r"(current));
                result = mask;
                asm volatile("" : "+r"(result));
                result &= current;
                *bg_control_or_mosaic_register = result;
                {
                    register volatile u16 *last asm("r2") =
                        (u16 *)0x0400000E;
                    register u32 last_value asm("r0") = *last;

                    mask &= last_value;
                    *last = mask;
                }
            }
            *(u8 *)0x03000074 &= 0xF7;
            *interrupt_master_enable = 1;
            *mosaic_state_address = mosaic_state;
            return;
        }
displayed_bank_address:
        *(u8 *)SCENE_MOSAIC_BANK_RAM ^= 1;
    }

    if (*(u8 *)SCENE_MOSAIC_STATE_RAM == SCENE_SCANLINE_ACTIVE) {
        register volatile u16 *bg_control_or_mosaic_register asm("r3") = (u16 *)0x0400004C;
        register u16 *mosaic_buffer_base asm("r2") = (u16 *)SCENE_MOSAIC_BUFFERS_RAM;
        register u8 *displayed_bank_address asm("r0") = (u8 *)SCENE_MOSAIC_BANK_RAM;
        register u32 displayed_bank asm("r1") = *displayed_bank_address;
        register u32 displayed_buffer_address asm("r0") = displayed_bank << 2;

        asm volatile("" : "+r"(mosaic_buffer_base));
        displayed_buffer_address += displayed_bank;
        displayed_buffer_address <<= 6;
        displayed_buffer_address += (u32)mosaic_buffer_base;
        *bg_control_or_mosaic_register = *(u16 *)displayed_buffer_address;
    }
}
