#include "m2c_prelude.h"
#include "../event_script.h"
extern u8 gFieldEventActive asm("D_02030664");
extern void ShakeEventCamera(void) asm("func_0809FF54");
extern void StartTask() asm("func_08092D8C");
extern void SeekEventCommand() asm("func_80A016C");

int EventShakeFieldCamera(u8 script_slot) asm("func_080A2238");

int EventShakeFieldCamera(u8 script_slot) {
    gFieldEventActive = 1;
    StartTask(5, ShakeEventCamera);
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
