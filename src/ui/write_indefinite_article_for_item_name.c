#include "m2c_prelude.h"
#include "window.h"
s32 WriteIndefiniteArticleForItemName(u8 *destination, u8 *item_name) asm("func_0809F770");

s32 WriteIndefiniteArticleForItemName(u8 *destination, u8 *item_name) {
    u8 initial_latin_code_low;
    destination[0] = SHIFT_JIS_LATIN_HIGH_BYTE;
    destination[1] = SHIFT_JIS_LOWERCASE_A_LOW_BYTE;
    if (item_name[0] == SHIFT_JIS_LATIN_HIGH_BYTE) {
        initial_latin_code_low = item_name[1];
        if (initial_latin_code_low == SHIFT_JIS_UPPERCASE_A_LOW_BYTE || initial_latin_code_low == SHIFT_JIS_UPPERCASE_E_LOW_BYTE || initial_latin_code_low == SHIFT_JIS_UPPERCASE_I_LOW_BYTE || initial_latin_code_low == SHIFT_JIS_UPPERCASE_O_LOW_BYTE ||
            initial_latin_code_low == SHIFT_JIS_UPPERCASE_U_LOW_BYTE || initial_latin_code_low == SHIFT_JIS_LOWERCASE_A_LOW_BYTE || initial_latin_code_low == SHIFT_JIS_LOWERCASE_E_LOW_BYTE || initial_latin_code_low == SHIFT_JIS_LOWERCASE_I_LOW_BYTE ||
            initial_latin_code_low == SHIFT_JIS_LOWERCASE_O_LOW_BYTE || initial_latin_code_low == SHIFT_JIS_LOWERCASE_U_LOW_BYTE) {
            destination[2] = SHIFT_JIS_LATIN_HIGH_BYTE;
            destination[3] = SHIFT_JIS_LOWERCASE_N_LOW_BYTE;
            destination[4] = 0;
            return 2;
        }
    }
    destination[2] = 0;
    return 1;
}
