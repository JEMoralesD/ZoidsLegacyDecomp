#include "m2c_prelude.h"
M2C_UNK BiosCpuSet(s32, s32, u32) asm("func_080ECD2C");
void CopyDeferredMemory(void *request) {
    register s32 source asm("r3") = M2C_FIELD(request, s32 *, 0);
    register s32 destination asm("r1") = M2C_FIELD(request, s32 *, 4);
    register u32 byte_count asm("r2") = M2C_FIELD(request, u32 *, 8);
    BiosCpuSet(source, destination, (u32)((byte_count + (byte_count >> 31)) << 10) >> 11);
}
