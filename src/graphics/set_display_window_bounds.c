#include "m2c_prelude.h"
#include "screen_effects.h"
extern u16 D_03005EFC, D_03005EFE, D_03005F00, D_03005F02;
void SetDisplayWindowBounds(u16 window0_horizontal, u16 window0_vertical, u16 window1_horizontal, u16 window1_vertical) asm("func_0809544C");

void SetDisplayWindowBounds(u16 window0_horizontal, u16 window0_vertical, u16 window1_horizontal, u16 window1_vertical) {
    if ((window0_horizontal != 0) && (window0_vertical != 0)) {
        D_03005EFC = window0_horizontal;
        D_03005EFE = window0_vertical;
    }
    if ((window1_horizontal != 0) && (window1_vertical != 0)) {
        D_03005F00 = window1_horizontal;
        D_03005F02 = window1_vertical;
    }
}
