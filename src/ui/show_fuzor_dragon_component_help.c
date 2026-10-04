#include "m2c_prelude.h"
s32 CreateSprite(M2C_UNK, M2C_UNK, s32, s32, s32, s32, s32, s32, s32) asm("func_8094484"); /* extern */
M2C_UNK DestroySprite(s32) asm("func_8094554");                             /* extern */
M2C_UNK RunMenuScript(M2C_UNK) asm("func_8098BB4");                         /* extern */
M2C_UNK LoadPilotPortraitGraphics(s32, s32, s32, s32, s32) asm("func_0809A94C");         /* extern */

void ShowFuzorDragonComponentHelp(void) asm("func_080B56CC");

void ShowFuzorDragonComponentHelp(void) {
    s32 portrait_sprite;

    LoadPilotPortraitGraphics(5, 0, 0, 0, 0);
    portrait_sprite = CreateSprite(0x08359850, 0x0835985C, 0, 8, 0x68, 0, 0, 8, 0);
    RunMenuScript(0x080035DE);
    DestroySprite(portrait_sprite);
}
