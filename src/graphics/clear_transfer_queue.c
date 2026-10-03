#include "m2c_prelude.h"
void ClearTransferQueue(void) {
    u8 request_index;

    request_index = 0;
    do {
        M2C_FIELD((request_index * 0x10), s32 *, 0x03005DE8) = 0;
        request_index += 1;
    } while ((u32) request_index <= 0xFU);
}
