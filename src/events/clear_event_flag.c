#include "m2c_prelude.h"
extern u8 D_0202ECF4[];
void ClearEventFlag(s32 flag_id) {
    u32 flag_shifted = flag_id << 0x18;
    s32 base = (s32)D_0202ECF4;
    s32 word_offset = (flag_shifted >> 0x1D) * 4;
    s32 *flag_words = (s32 *)(base + 0x150);
    s32 *flag_word = (s32 *)((s32)flag_words + word_offset);
    u32 bit_index = 0x1F000000;
    bit_index &= flag_shifted;
    bit_index >>= 0x18;
    *flag_word &= ~(1 << bit_index);
}
