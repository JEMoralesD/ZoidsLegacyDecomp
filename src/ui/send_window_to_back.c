#include "m2c_prelude.h"

void CopyBytes(void *, void *, s32) asm("func_080ED038");

void SendWindowToBack(s32 window_slot) {
    u8 window_index;
    s32 window_stride;
    u8 *windows;
    u8 *temporary;
    register s32 target_slot asm("r1");
    register u8 *first_window asm("r2");
    register s32 first_slot asm("r0");

    window_slot <<= 24;
    target_slot = (u32)window_slot >> 24;
    window_index = 0;
    first_window = (u8 *)0x0200A8A0;
    first_slot = first_window[0x13];
    temporary = (u8 *)0x0200D8C0;
    asm volatile("" : "+r"(temporary));
    if (first_slot != target_slot) {
        register u8 *loop_base asm("r3");
        register s32 search_window_stride asm("r2");

        loop_base = first_window;
        search_window_stride = 0x4D0;
        do {
            window_index = (u8)(window_index + 1);
            if (window_index > 9) {
                break;
            }
        } while (loop_base[window_index * search_window_stride + 0x13] != target_slot);
    }

    windows = (u8 *)0x0200A8A0;
    window_stride = 0x4D0;
    CopyBytes(temporary, windows + window_index * window_stride, window_stride);
    {
        register u32 current asm("r1");

        current = window_index;
        if (current <= 8) {
            do {
                u32 next;
                register s32 destination_offset asm("r0");
                register u8 *destination asm("r0");
                register s32 source_offset asm("r1");
                register u8 *source asm("r1");

                destination_offset = current;
                destination_offset *= window_stride;
                destination = (u8 *)(destination_offset + (s32)windows);
                asm volatile("" : "+r"(destination));
                next = current + 1;
                source_offset = next;
                source_offset *= window_stride;
                source = (u8 *)(source_offset + (s32)windows);
                CopyBytes(destination, source, window_stride);
                next <<= 24;
                current = next >> 24;
            } while (current <= 8);
        }
    }
    {
        register u8 *final_base asm("r0");
        register s32 final_offset asm("r1");
        register u8 *final_temporary asm("r1");
        register s32 final_stride asm("r2");

        final_base = (u8 *)0x0200A8A0;
        asm volatile("" : "+r"(final_base));
        final_offset = 0x2B50;
        asm volatile("" : "+r"(final_offset));
        final_base = (u8 *)((s32)final_base + final_offset);
        final_temporary = (u8 *)0x0200D8C0;
        final_stride = 0x4D0;
        CopyBytes(final_base, final_temporary, final_stride);
    }
}
