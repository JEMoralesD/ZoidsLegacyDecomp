#include "m2c_prelude.h"
void BiosDiv(void) {
    asm("svc 0x6");
}
