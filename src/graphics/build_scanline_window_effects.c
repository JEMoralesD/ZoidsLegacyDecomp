#include "m2c_prelude.h"
#include "screen_effects.h"
void BuildScaledScanlineWindows(void) asm("func_0809570C");
void BuildPolygonScanlineWindows(void) asm("func_08095864");
void BuildRotatingSplitScanlineWindows(void) asm("func_08095B84");
void BuildChevronScanlineWindow(void) asm("func_08095EE4");
void BuildCheckerboardScanlineWindows(void) asm("func_08095F9C");

void BuildScanlineWindowEffects(void) asm("func_08096278");

void BuildScanlineWindowEffects(void) {
    u8 window_effect_state = *(u8 *)SCANLINE_WINDOW_ADDRESS(flags);
    s32 window_effect_mode;

    if (window_effect_state != 0) {
        window_effect_mode = SCANLINE_WINDOW_MODE_MASK;
        window_effect_mode &= window_effect_state;
        switch (window_effect_mode) {
        case SCANLINE_WINDOW_FIXED:
            break;
        case SCANLINE_WINDOW_SCALED:
            BuildScaledScanlineWindows();
            break;
        case SCANLINE_WINDOW_POLYGONS:
            BuildPolygonScanlineWindows();
            break;
        case SCANLINE_WINDOW_ROTATING_SPLIT:
            BuildRotatingSplitScanlineWindows();
            break;
        case SCANLINE_WINDOW_CHEVRON:
            BuildChevronScanlineWindow();
            break;
        case SCANLINE_WINDOW_CHECKERBOARD:
            BuildCheckerboardScanlineWindows();
            break;
        }
    }
}
