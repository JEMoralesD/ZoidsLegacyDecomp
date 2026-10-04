#include "m2c_prelude.h"
void BiosLz77ToVram(void) {
    asm("svc 0x12");
}
