#include "m2c_prelude.h"
#include "../event_script.h"
M2C_UNK DestroyEventSprite(u8) asm("func_0809FB78");
M2C_UNK InitializeEventSpritePool(void) asm("func_0809F850");
M2C_UNK SeekEventCommand(u8, s32, s32) asm("func_80A016C");

int EventResetSpritePool(u8 script_slot) asm("func_080A49FC");

int EventResetSpritePool(u8 script_slot) {
    u32 sprite_slot;
    for (sprite_slot = 0; sprite_slot < 16; sprite_slot++) {
        DestroyEventSprite(sprite_slot);
    }
    InitializeEventSpritePool();
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
