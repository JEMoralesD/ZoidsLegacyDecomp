#include "m2c_prelude.h"
void RequestWindowRefresh(void) {
    *(s8 *)0x02021674 = 1;
}
