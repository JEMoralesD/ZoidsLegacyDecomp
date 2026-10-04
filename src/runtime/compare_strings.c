#include "m2c_prelude.h"

s32 CompareStrings(u8 *left_string, u8 *right_string)
{
    u8 *left = left_string;
    u8 *right = right_string;

    if (!(((u32)left_string | (u32)right) & 3) &&
        *(u32 *)left == *(u32 *)right) {
        for (;;) {
            u32 word = *(u32 *)left;
            if ((word + 0xFEFEFEFF) & ~word & 0x80808080)
                return 0;
            left += 4;
            right += 4;
            if (*(u32 *)left != *(u32 *)right)
                break;
        }
    }

    while (*left && *left == *right) {
        left++;
        right++;
    }
    {
        register u8 *left_reg asm("r2") = left;
        register u8 *right_reg asm("r3") = right;
        register s32 result asm("r0");

        asm volatile(
            "ldrb r2, [r2, #0]\n"
            "ldrb r3, [r3, #0]\n"
            "sub r0, r2, r3"
            : "=r"(result), "+r"(left_reg), "+r"(right_reg));
        return result;
    }
}
