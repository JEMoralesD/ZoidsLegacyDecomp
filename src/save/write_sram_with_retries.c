#include "m2c_prelude.h"
M2C_UNK func_80ECBBC(s32, s32, s32);
s32 CallFunctionR3(s32, s32, s32, s32) asm("func_80ECD68");

s32 WriteSramWithRetries(s32 source, s32 destination, s32 byte_count, s32 initial_result) {
    s32 result;
    u32 attempt;

    result = initial_result;
    for (attempt = 0; attempt <= 2U; attempt = (u32) (u8) (attempt + 1)) {
        func_80ECBBC(source, destination, byte_count);
        result = CallFunctionR3(source, destination, byte_count, *(s32 *)0x03007754);
        if (result == 0) {
            break;
        }
    }
    return result;
}
