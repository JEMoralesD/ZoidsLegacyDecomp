#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../graphics/screen_effects.h"
extern u8 gFieldEventActive asm("D_02030664");
extern u16 gDisplayBlendControl asm("D_0300004E");
extern u16 gDisplayBrightness asm("D_03000052");
void YieldTaskForUpdates() asm("func_080ED17C");
void SeekEventCommand() asm("func_80A016C");

s32 EventFlashScreenWhite(u8 script_slot) asm("func_080A1CD8");

s32 EventFlashScreenWhite(u8 script_slot) {
    u16 saved_blend_control, saved_brightness;
    gFieldEventActive = 1;
    saved_blend_control = gDisplayBlendControl;
    saved_brightness = gDisplayBrightness;
    gDisplayBlendControl = SCREEN_BRIGHTEN_ALL_LAYERS;
    gDisplayBrightness = SCREEN_MAX_BRIGHTNESS;
    YieldTaskForUpdates(2);
    gDisplayBlendControl = saved_blend_control;
    gDisplayBrightness = saved_brightness;
    YieldTaskForUpdates(2);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
