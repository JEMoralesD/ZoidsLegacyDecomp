#include "m2c_prelude.h"
void BiosArcTan2(void) {
    asm("svc 0xa");
}
