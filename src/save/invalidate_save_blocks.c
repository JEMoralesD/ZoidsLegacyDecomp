#include "m2c_prelude.h"

struct SaveBlock {
    s32 data;
    s32 size;
};

extern struct SaveBlock gSaveBlockTable[];

void WriteSramWithRetries(s32, s32, s32) asm("func_080ECCE4");

void InvalidateSaveBlocks(void) {
    s32 block_address;
    u8 block_index;

    block_index = 0;
    block_address = 0x0E000004;
    if ((gSaveBlockTable[0].data == 0) && (gSaveBlockTable[0].size == 0)) {
        return;
    }
loop:
    WriteSramWithRetries(0, block_address, 1);
    {
        s32 block_size;
        block_size = gSaveBlockTable[block_index].size;
        block_size += 0xC;
        block_address += block_size;
    }
    block_index = (u8)(block_index + 1);
    if (gSaveBlockTable[block_index].data != 0) {
        goto loop;
    }
    if (gSaveBlockTable[block_index].size != 0) {
        goto loop;
    }
}
