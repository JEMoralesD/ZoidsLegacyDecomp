#include "m2c_prelude.h"
void BiosSqrt(void) {
    asm("svc 0x8");
}
