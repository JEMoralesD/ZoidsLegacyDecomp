#include "m2c_prelude.h"
extern volatile u16 gDisplayStatus asm("D_04000004");
extern u16 gDisplayControl asm("D_0300004C");
extern u16 gFieldBackdropColorBeforeBlank asm("D_02031988");

void RestoreFieldDisplayAtHBlank(void) asm("func_080A00D4");

void RestoreFieldDisplayAtHBlank(void) {
    while ((gDisplayStatus & 2) == 0)
        ;
    *(volatile u16 *)0x04000000 = gDisplayControl;
    *(volatile u16 *)0x05000000 = gFieldBackdropColorBeforeBlank;
}
