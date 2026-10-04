#include "m2c_prelude.h"
void BiosCpuSet(void) {
    asm("svc 0xb");
}
