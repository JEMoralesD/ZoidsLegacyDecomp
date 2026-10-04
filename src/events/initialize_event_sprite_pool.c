#include "m2c_prelude.h"
#include "event_script.h"
extern u8 gEventSpriteResourceCount asm("D_020317DA");
extern u8 gEventSpriteTileBoundary asm("D_0203183C");
extern u8 gEventSpritePaletteBoundary asm("D_0203183E");
extern u8 gEventSpriteMovementStates asm("D_02031840");
extern u8 gEventSprites asm("D_02031940");
void InitializeEventSpritePool(void) asm("func_0809F850");

void InitializeEventSpritePool(void) {
    u8 sprite_slot;

    *(s8 *)((u32)&gEventSpriteResourceCount) = 0;
    *(s16 *)((u32)&gEventSpriteTileBoundary) = EVENT_SPRITE_INITIAL_TILE_BOUNDARY;
    *(s16 *)((u32)&gEventSpritePaletteBoundary) = EVENT_SPRITE_INITIAL_PALETTE_BOUNDARY;
    sprite_slot = 0;
    do {
        M2C_FIELD((sprite_slot * 4), s32 *, ((u32)&gEventSprites)) = 0;
        M2C_FIELD((sprite_slot * 0x10), s8 *, ((u32)&gEventSpriteMovementStates)) = 0;
        sprite_slot += 1;
    } while ((u32) sprite_slot <= EVENT_SPRITE_LAST_SLOT);
}
