#include "m2c_prelude.h"
#include "screen_effects.h"

extern u8 gScreenDitherMasks[] asm("D_02002740");
extern u16 gScreenTransitionMosaic asm("D_03005F78");
extern u8 gScreenTransitionDuration asm("D_03005F71");
void SetHBlankCallback(s32, s32) asm("func_08094290");
void ClearHBlankCallback(s32) asm("func_080942E0");
void BiosCpuFastSet(s32, s32, s32) asm("func_080ECD28");

void UpdateScreenTransitionRegisters(u8 full_update) asm("func_08096DB0");

void UpdateScreenTransitionRegisters(u8 full_update)
{
    u8 transition_flags = *(u8 *)SCREEN_TRANSITION_ADDRESS(flags);
    u32 transition_kind;

    if (transition_flags == 0) {
        return;
    }
    transition_kind = SCREEN_TRANSITION_KIND_MASK;
    transition_kind &= transition_flags;
    if ((u32)(u8)(transition_kind - 17) <= 1U) {
        if (full_update != 0) {
            u8 *hblank_action = (u8 *)SCREEN_TRANSITION_ADDRESS(hblank_action);
            register u32 action asm("r6") = *hblank_action;
            u32 zero;

            if (action == SCREEN_TRANSITION_HBLANK_INSTALL) {
                register volatile u16 *ime asm("r4") = (u16 *)0x04000208;

                zero = 0;
                *ime = zero;
                SetHBlankCallback(2, SCREEN_TRANSITION_ADDRESS(hblank_callback[0]));
                *ime = action;
                goto clear;
            }
            if (action == SCREEN_TRANSITION_HBLANK_REMOVE) {
                register volatile u16 *ime asm("r4") = (u16 *)0x04000208;

                zero = 0;
                *ime = zero;
                ClearHBlankCallback(2);
                *ime = 1;
clear:
                *hblank_action = zero;
            }
            *(u8 *)SCREEN_TRANSITION_ADDRESS(displayed_bank) ^= 1;
        }
        if (*(u8 *)SCREEN_TRANSITION_ADDRESS(hblank_action) == 0 && gScreenDitherMasks[*(u8 *)SCREEN_TRANSITION_ADDRESS(displayed_bank) * 160] == 0) {
            register volatile u16 *blend_register asm("r1") = (volatile u16 *)0x04000054;

            *blend_register = 16;
            blend_register -= 2;
            *blend_register = 0xFF;
            blend_register -= 4;
            *(volatile u32 *)blend_register = 0x3F3F3F3F;
        }
    }
    if (full_update != 0) {
        u8 current_flags = *(u8 *)SCREEN_TRANSITION_ADDRESS(flags);
        u32 mosaic_flag = SCREEN_TRANSITION_MOSAIC;
        u8 *transition_state;

        mosaic_flag &= current_flags;
        transition_state = (u8 *)SCREEN_TRANSITION_ADDRESS(flags);

        if (mosaic_flag != 0) {
            u16 mosaic = gScreenTransitionMosaic;

            if (mosaic != 0) {
                register volatile u16 *display asm("r1") =
                    (u16 *)0x04000008;
                register u32 mask asm("r2");
                u32 current;

                current = *display;
                mask = 0x40;
                current |= mask;
                *display = current;
                display++;
                current = *display;
                current |= mask;
                *display = current;
                display++;
                current = *display;
                current |= mask;
                *display = current;
                display++;
                current = *display;
                current |= mask;
                *display = current;
                *(u16 *)0x0400004C = mosaic;
            } else {
                register volatile u16 *display asm("r3") =
                    (u16 *)0x04000008;
                register u32 mask asm("r1");
                register u32 current asm("r2");
                register u32 result asm("r0");

                current = *display;
                asm volatile("" : "+r"(current));
                mask = 0xFFBF;
                asm volatile("" : "+r"(mask));
                result = mask;
                asm volatile("" : "+r"(result));
                result &= current;
                *display = result;
                display++;
                current = *display;
                asm volatile("" : "+r"(current));
                result = mask;
                asm volatile("" : "+r"(result));
                result &= current;
                *display = result;
                display++;
                current = *display;
                asm volatile("" : "+r"(current));
                result = mask;
                asm volatile("" : "+r"(result));
                result &= current;
                *display = result;
                {
                    register volatile u16 *last asm("r2") =
                        (u16 *)0x0400000E;
                    register u32 last_value asm("r0") = *last;

                    mask &= last_value;
                    *last = mask;
                }
            }
        }
        if ((*transition_state & SCREEN_TRANSITION_SOFTWARE_PALETTE) != 0) {
            BiosCpuFastSet(0x02000B40, 0x05000000, 0x100);
        }
        {
            u8 *progress = (u8 *)SCREEN_TRANSITION_ADDRESS(progress);
            u32 next_progress = *progress + 1;

            *progress = next_progress;
            /* Duration 255 never completes because the byte counter wraps first. */
            if ((u8)next_progress > gScreenTransitionDuration) {
                *transition_state = 0;
            }
        }
    }
}
