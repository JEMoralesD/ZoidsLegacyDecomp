#include "m2c_prelude.h"
extern void RunMenuScript(int) asm("func_8098BB4");

void sub_080CCEAC(u8 arg0)
{
    switch (arg0) {
    case 0:
        RunMenuScript(0x08004174);
        return;
    case 1:
        RunMenuScript(0x08004177);
        return;
    case 2:
        RunMenuScript(0x0800417A);
        return;
    }
}
