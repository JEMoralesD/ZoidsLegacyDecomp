#include "m2c_prelude.h"
#include "field_display.h"
extern u32 gFieldBg1ScrollDeltaXFixed8 asm("D_02032590");
extern u32 gFieldBg1ScrollDeltaYFixed8 asm("D_02032594");
extern u32 gFieldBg2ScrollDeltaYFixed8 asm("D_02032598");
extern u8 gFieldPaletteAnimationFrame asm("D_0203259C");
extern u8 gFieldPaletteAnimationTimer asm("D_0203259D");
void UpdateFieldDisplay(void) asm("func_0809DFFC");
void YieldTaskForUpdates(int) asm("func_080ED17C");

void RunFieldDisplayTask(void) asm("func_0809E1CC");

void RunFieldDisplayTask(void) {
    gFieldBg1ScrollDeltaXFixed8 = gFieldBg1ScrollDeltaYFixed8 = gFieldBg2ScrollDeltaYFixed8 = 0;
    gFieldPaletteAnimationFrame = gFieldPaletteAnimationTimer = 0;
    for (;;) {
        UpdateFieldDisplay();
        YieldTaskForUpdates(1);
    }
}
