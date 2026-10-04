#include "m2c_prelude.h"
void BiosLz77ToWram(void) {
    asm("svc 0x11");
}
