#include "m2c_prelude.h"

/* Newlib srand stores _REENT->_new._reent._rand_next. */
struct LibcRandomStateView {
    u8 reserved00[0x58];
    s32 random_seed;
};

enum LibcRandomResources {
    LIBC_REENT_POINTER_ROM = 0x087F3128
};

void SeedLibcRandom(s32 seed) asm("func_080ED098");

void SeedLibcRandom(s32 seed) {
    (*(struct LibcRandomStateView **)LIBC_REENT_POINTER_ROM)->random_seed = seed;
}
