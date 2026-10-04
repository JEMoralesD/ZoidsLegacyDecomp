#include "m2c_prelude.h"
#include "field_display.h"
extern u8 gFieldDecorationSpriteSlots[] asm("D_020324BB");
extern u8 gFieldDecorationSprites[] asm("D_020324E8");
void ResetFieldMapDecorationSprites(void) asm("func_0809D700");

void ResetFieldMapDecorationSprites(void) {
    u8 sprite_slot;
    for (sprite_slot = 0; sprite_slot < FIELD_DECORATION_SLOT_COUNT; sprite_slot++) {
        gFieldDecorationSpriteSlots[sprite_slot] |= 0xFF;
        *(s32 *)(gFieldDecorationSprites + sprite_slot * 4) = 0;
    }
}
