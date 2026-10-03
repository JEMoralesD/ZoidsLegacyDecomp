#include "m2c_prelude.h"
extern s16 gPreviousRepeatKeys;
extern s8 gKeyRepeatTimer;
void sub_08096F3C(void) {
    s8 *p2 = &gKeyRepeatTimer;
    s16 *p1 = &gPreviousRepeatKeys;
    *p1 = 0;
    *p2 = 0;
}
