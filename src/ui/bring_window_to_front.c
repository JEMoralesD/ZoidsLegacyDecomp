#include "m2c_prelude.h"

void CopyBytes(void *, void *, s32) asm("func_080ED038");

void BringWindowToFront(s32 window_slot) {
    u32 window_index;
    u8 *windows;
    s32 window_stride;
    register s32 target_slot asm("r2");
    register u8 *first_window asm("r0");
    register s32 first_slot asm("r1");

    window_slot <<= 24;
    target_slot = (u32)window_slot >> 24;
    window_index = 0;
    first_window = (u8 *)0x0200A8A0;
    first_slot = first_window[0x13];
    windows = first_window;
    if (first_slot != target_slot) {
        register u8 *loop_base asm("r3");
        register s32 search_window_stride asm("r1");

        loop_base = windows;
        search_window_stride = 0x4D0;
        do {
            window_index = (u8)(window_index + 1);
            if (window_index > 9) {
                break;
            }
        } while (loop_base[window_index * search_window_stride + 0x13] != target_slot);
    }

    window_stride = 0x4D0;
    {
        register s32 selected_offset asm("r0");
        register u8 *selected asm("r2");
        register s32 window_flags asm("r0");
        register s32 locked_mask asm("r1");

        selected_offset = window_index;
        selected_offset *= window_stride;
        selected = (u8 *)(selected_offset + (s32)windows);
        window_flags = *(s32 *)selected;
        locked_mask = 0x200;
        window_flags &= locked_mask;
        if (window_flags == 0) {
            {
                register u8 *first_temporary asm("r0");

                first_temporary = (u8 *)0x0200D8C0;
                asm volatile("" : "+r"(first_temporary));
                CopyBytes(first_temporary, selected, window_stride);
            }
            if (window_index != 0) {
                do {
                    register s32 destination_offset asm("r0");
                    register u8 *destination asm("r0");
                    register s32 source_offset asm("r1");
                    register u8 *source asm("r1");

                    destination_offset = window_index;
                    destination_offset *= window_stride;
                    destination = (u8 *)(destination_offset + (s32)windows);
                    window_index -= 1;
                    source_offset = window_index;
                    source_offset *= window_stride;
                    source = (u8 *)(source_offset + (s32)windows);
                    CopyBytes(destination, source, window_stride);
                    window_index <<= 24;
                    window_index >>= 24;
                } while (window_index != 0);
            }
            {
                register u8 *final_base asm("r4");
                register u8 *temporary asm("r1");
                register s32 final_stride asm("r2");
                register s32 final_flags asm("r0");
                register s32 final_mask asm("r1");

                final_base = (u8 *)0x0200A8A0;
                temporary = (u8 *)0x0200D8C0;
                final_stride = 0x4D0;
                CopyBytes(final_base, temporary, final_stride);
                final_flags = *(s32 *)final_base;
                final_mask = 2;
                final_flags |= final_mask;
                *(s32 *)final_base = final_flags;
            }
        }
    }
}
