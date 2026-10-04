#include "m2c_prelude.h"
#include "../event_script.h"
s32 EventJump(int script_slot, u8 **cursor) {
    u8 *p = *cursor;
    *(u32 *)cursor = p[1] | (p[2] << 8) | (p[3] << 16) | (p[4] << 24);
    return EVENT_CONTINUE;
}
