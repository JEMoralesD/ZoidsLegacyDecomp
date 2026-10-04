#include "m2c_prelude.h"
M2C_UNK BiosCpuSet(M2C_UNK *, s32, s32) asm("func_080ECD2C");

void FillDeferredMemory(void *request) {
    register u16 *fill_value_address asm("r2");
    register s32 destination asm("r1");
    u32 byte_count;
    s32 fill_value;

    fill_value_address = (u16 *)&fill_value;
    *fill_value_address = M2C_FIELD(request, u16 *, 0);
    destination = M2C_FIELD(request, s32 *, 4);
    byte_count = M2C_FIELD(request, u32 *, 8);
    BiosCpuSet(&fill_value, destination, ((u32)((byte_count + (byte_count >> 0x1F)) << 0xA) >> 0xB) | 0x01000000);
}
