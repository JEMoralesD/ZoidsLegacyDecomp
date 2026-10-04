#include "m2c_prelude.h"

s32 GetWindow(s32 window_slot) {
    u8 *windows;
    u8 *retained_windows;
    s32 window_index;
    s32 first_slot;
    register s32 target_slot asm("r3");

    target_slot = (u8)window_slot;
    asm volatile("" : "+r"(target_slot));
    window_index = 0;
    windows = (u8 *)0x0200A8A0;
    first_slot = windows[0x13];
    {
        register u8 *captured_windows asm("r5");
        captured_windows = windows;
        asm volatile("" : "+&r"(captured_windows) : "r"(windows));
        retained_windows = captured_windows;
    }
    if (first_slot != target_slot) {
        register u8 *scan_windows asm("r4");
        s32 window_stride;

        scan_windows = retained_windows;
        asm volatile("" : "+r"(scan_windows));
        window_stride = 0x4D0;
        do {
            window_index = (u8)(window_index + 1);
            if ((u32)window_index > 9U) {
                break;
            }
        } while (scan_windows[window_index * window_stride + 0x13] != target_slot);
    }
    {
        s32 result;
        result = window_index * 0x4D0;
        result += (s32)retained_windows;
        return result;
    }
}
