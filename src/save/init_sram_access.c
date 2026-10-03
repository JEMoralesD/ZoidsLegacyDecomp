#include "m2c_prelude.h"

void InitSramAccess(void) {
    register u32 copy_value asm("r0");
    register u32 remaining_halfwords asm("r1");
    register u16 *source asm("r2");
    register u16 *destination asm("r3");

    source = (u16 *)0x080ECB7D;
    copy_value = 1;
    asm volatile("" : "+r"(source), "+r"(copy_value));
    source = (u16 *)((u32)source & ~copy_value);
    destination = (u16 *)0x03006AB0;
    copy_value = 0x080ECBBD;
    remaining_halfwords = 0x080ECB7D;
    asm volatile("" : "+r"(copy_value), "+r"(remaining_halfwords));
    copy_value -= remaining_halfwords;
    copy_value <<= 15;
    goto test_first;
copy_first:
    copy_value = *source;
    *destination = copy_value;
    source++;
    destination++;
    copy_value = remaining_halfwords - 1;
    copy_value <<= 16;
test_first:
    remaining_halfwords = copy_value >> 16;
    if (remaining_halfwords != 0) {
        goto copy_first;
    }

    *(s32 *)0x03007750 = 0x03006AB1;
    source = (u16 *)0x080ECBFD;
    copy_value = 1;
    asm volatile("" : "+r"(source), "+r"(copy_value));
    source = (u16 *)((u32)source & ~copy_value);
    destination = (u16 *)0x03006A10;
    copy_value = 0x080ECC49;
    remaining_halfwords = 0x080ECBFD;
    asm volatile("" : "+r"(copy_value), "+r"(remaining_halfwords));
    copy_value -= remaining_halfwords;
    copy_value <<= 15;
    goto test_second;
copy_second:
    copy_value = *source;
    *destination = copy_value;
    source++;
    destination++;
    copy_value = remaining_halfwords - 1;
    copy_value <<= 16;
test_second:
    remaining_halfwords = copy_value >> 16;
    if (remaining_halfwords != 0) {
        goto copy_second;
    }

    *(s32 *)0x03007754 = 0x03006A11;
    source = (u16 *)0x04000204;
    copy_value = *source;
    remaining_halfwords = 0xFFFC;
    copy_value &= remaining_halfwords;
    remaining_halfwords = 3;
    copy_value |= remaining_halfwords;
    *source = copy_value;
}
