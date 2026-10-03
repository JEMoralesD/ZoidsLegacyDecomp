#include "m2c_prelude.h"
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");                      /* extern */

void sub_080D27D8(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0xC) == 0) {
        DestroySpriteGroup(arg0);
    }
}
