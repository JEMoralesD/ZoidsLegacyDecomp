#include "m2c_prelude.h"
#include "../event_script.h"
extern u32 gEventPortraitSprite asm("D_02031744");
extern void DestroySprite(u32) asm("func_8094554");
extern int SeekEventCommand(u8, int, int) asm("func_80A016C");

int EventClosePortrait(u8 script_slot) asm("func_080A16B4");

int EventClosePortrait(u8 script_slot) {
    if (gEventPortraitSprite != 0) {
        DestroySprite(gEventPortraitSprite);
        gEventPortraitSprite = 0;
    }
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
