#include "m2c_prelude.h"

s32 FindFirstMismatchingByte(u8 *source_bytes, u8 *compared_bytes, s32 byte_count) asm("func_080ECBFC");

s32 FindFirstMismatchingByte(u8 *source_bytes, u8 *compared_bytes, s32 byte_count) {
    register s32 bytes_remaining asm("r3");
    register s32 sentinel asm("r0");

    bytes_remaining = byte_count;
    {
        register u16 *wait_control asm("r2");
        register u16 wait_control_value asm("r0");
        u16 preserved_wait_bits;

        wait_control = (u16 *)0x04000204;
        asm volatile("" : "+r"(wait_control));
        wait_control_value = *wait_control;
        asm volatile("" : "+r"(wait_control_value));
        preserved_wait_bits = 0xFFFC;
        wait_control_value &= preserved_wait_bits;
        wait_control_value |= 3;
        *wait_control = wait_control_value;
    }
    bytes_remaining -= 1;
    sentinel = 1;
    sentinel = -sentinel;
    if (bytes_remaining != sentinel) {
        register s32 loop_sentinel asm("r2");

        loop_sentinel = sentinel;
        asm volatile("" : "+r"(loop_sentinel));
        do {
            u8 compared_byte;
            u8 source_byte;

            compared_byte = *compared_bytes;
            source_byte = *source_bytes;
            source_bytes += 1;
            compared_bytes += 1;
            if (compared_byte != source_byte) {
                return (s32)(compared_bytes - 1);
            }
            bytes_remaining -= 1;
        } while (bytes_remaining != loop_sentinel);
    }
    return 0;
}
