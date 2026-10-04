#include "m2c_prelude.h"
void BiosCpuFastSet(void) {
    asm("svc 0xc");
}
