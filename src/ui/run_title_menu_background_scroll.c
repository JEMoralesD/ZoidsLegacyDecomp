#include "m2c_prelude.h"
#include "title_menu.h"
extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");
extern void YieldTaskForUpdates(int) asm("func_080ED17C");

void RunTitleMenuBackgroundScroll(void) asm("func_0809B4FC");

void RunTitleMenuBackgroundScroll(void) {
    for (;;) {
        if (gFieldCameraScrollOffsets[1] <= 0x6FFF) {
            gFieldCameraScrollOffsets[1] += 0x80;
        }
        YieldTaskForUpdates(1);
    }
}
