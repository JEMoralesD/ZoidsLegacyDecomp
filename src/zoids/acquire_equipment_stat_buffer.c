#include "m2c_prelude.h"
#include "../battle/battle.h"
s32 AcquireEquipmentStatBuffer(void) asm("func_080E669C");

s32 AcquireEquipmentStatBuffer(void) {
    register u8 *buffer_depth asm("r3");
    register s32 depth asm("r2");
    register s32 buffers asm("r1");

    buffer_depth = (u8 *)0x02030554;
    depth = *buffer_depth;
    buffers = 0x02030494;
    {
        s32 buffer_address = (depth * 0x18) + buffers;
        *buffer_depth = depth + 1;
        return buffer_address;
    }
}
