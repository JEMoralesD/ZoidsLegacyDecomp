#include "m2c_prelude.h"
#include "event_script.h"
extern u8 gEventSpriteMovementStates[] asm("D_02031840");
extern void *gEventSprites[] asm("D_02031940");

void BeginEventSpriteMovement(u8 sprite_slot, int target_x_bits, u16 target_y_bits, u8 duration_updates) asm("func_0809FA24");

void BeginEventSpriteMovement(u8 sprite_slot, int target_x_bits, u16 target_y_bits, u8 duration_updates) {
    u8 *movement_bytes = &gEventSpriteMovementStates[sprite_slot * 0x10];
    int x_distance, y_distance, start_x, start_y;
    s8 y_decreases;
    int zero_accumulator;
    movement_bytes[EVENT_SPRITE_MOVEMENT_OFFSET(active)] = 1;
    start_x = *(u16 *)((u8 *)gEventSprites[sprite_slot] + 4);
    x_distance = (s16)target_x_bits - start_x;
    *(s16 *)(movement_bytes + EVENT_SPRITE_MOVEMENT_OFFSET(x_distance)) = x_distance;
    if ((x_distance << 0x10) < 0) { *(s16 *)(movement_bytes + EVENT_SPRITE_MOVEMENT_OFFSET(x_distance)) = -x_distance; movement_bytes[EVENT_SPRITE_MOVEMENT_OFFSET(x_decreases)] = 1; } else { movement_bytes[EVENT_SPRITE_MOVEMENT_OFFSET(x_decreases)] = 0; }
    start_y = *(u16 *)((u8 *)gEventSprites[sprite_slot] + 6);
    y_distance = (s16)target_y_bits - start_y;
    *(s16 *)(movement_bytes + EVENT_SPRITE_MOVEMENT_OFFSET(y_distance)) = y_distance;
    if ((y_distance << 0x10) < 0) { *(s16 *)(movement_bytes + EVENT_SPRITE_MOVEMENT_OFFSET(y_distance)) = -y_distance; y_decreases = 1; } else { y_decreases = 0; }
    movement_bytes[EVENT_SPRITE_MOVEMENT_OFFSET(y_decreases)] = y_decreases;
    zero_accumulator = 0;
    movement_bytes[EVENT_SPRITE_MOVEMENT_OFFSET(duration_updates)] = duration_updates;
    *(s16 *)(movement_bytes + EVENT_SPRITE_MOVEMENT_OFFSET(y_accumulator)) = zero_accumulator;
    *(s16 *)(movement_bytes + EVENT_SPRITE_MOVEMENT_OFFSET(x_accumulator)) = zero_accumulator;
    movement_bytes[EVENT_SPRITE_MOVEMENT_OFFSET(elapsed_updates)] = 0;
}
