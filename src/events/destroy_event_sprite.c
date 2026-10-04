#include "m2c_prelude.h"
#include "event_script.h"
extern u8 gEventSprites[] asm("D_02031940"); extern u8 gEventSpriteMovementStates[] asm("D_02031840");
void DestroySprite(void) asm("func_8094554");
void DestroyEventSprite(u8 sprite_slot) asm("func_0809FB78");

void DestroyEventSprite(u8 sprite_slot) {
    u8 slot_index = sprite_slot;
    s32 *sprite_addresses = (s32 *)gEventSprites;
    if (sprite_addresses[slot_index] != 0) {
        DestroySprite();
        sprite_addresses[slot_index] = 0;
        gEventSpriteMovementStates[sprite_slot * 0x10] = 0;
    }
}
