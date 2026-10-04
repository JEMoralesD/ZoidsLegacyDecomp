#include "m2c_prelude.h"
#include "window.h"
extern s16 gPreviousRepeatKeys;
extern s8 gKeyRepeatTimer;
void ResetMenuKeyRepeat(void) asm("func_08096F3C");

void ResetMenuKeyRepeat(void) {
    s8 *repeat_timer = &gKeyRepeatTimer;
    s16 *previous_keys = &gPreviousRepeatKeys;
    *previous_keys = 0;
    *repeat_timer = 0;
}
