#include "m2c_prelude.h"
#include "event_script.h"

extern struct EventSpritePositionView *gEventSprites[] asm("D_02031940");
extern struct EventSpriteMovementState gEventSpriteMovementStates[] asm("D_02031840");

void YieldTaskForUpdates(s32) asm("func_080ED17C");

void RunEventSpriteMovementTask(void) asm("func_0809FA9C");

void RunEventSpriteMovementTask(void) {
    s32 movement_pending;
    u8 sprite_slot;
    struct EventSpriteMovementState *movement;
    struct EventSpritePositionView *sprite;
    s32 next_position;
    s32 duration_updates;
    s32 next_elapsed_updates;

    do {
        movement_pending = 0;
        sprite_slot = 0;
        do {
            if (gEventSprites[sprite_slot] != 0) {
                movement = &gEventSpriteMovementStates[sprite_slot];
                if (movement->active != 0) {
                    movement->x_accumulator = movement->x_accumulator + movement->x_distance;
                    if ((s16)movement->x_accumulator > (duration_updates = movement->duration_updates)) {
                        do {
                            if (movement->x_decreases == 0) {
                                sprite = gEventSprites[sprite_slot];
                                next_position = sprite->x + 1;
                            } else {
                                sprite = gEventSprites[sprite_slot];
                                next_position = sprite->x - 1;
                            }
                            sprite->x = next_position;
                            movement->x_accumulator = movement->x_accumulator - movement->duration_updates;
                        } while ((s16)movement->x_accumulator > (duration_updates = movement->duration_updates));
                    }
                    movement->y_accumulator = movement->y_accumulator + movement->y_distance;
                    if ((s16)movement->y_accumulator > duration_updates) {
                        do {
                            if (movement->y_decreases == 0) {
                                sprite = gEventSprites[sprite_slot];
                                next_position = sprite->y + 1;
                            } else {
                                sprite = gEventSprites[sprite_slot];
                                next_position = sprite->y - 1;
                            }
                            sprite->y = next_position;
                            movement->y_accumulator = movement->y_accumulator - movement->duration_updates;
                        } while ((s16)movement->y_accumulator > movement->duration_updates);
                    }
                    next_elapsed_updates = movement->elapsed_updates + 1;
                    movement->elapsed_updates = next_elapsed_updates;
                    if ((u32)(u8)next_elapsed_updates >= (u32)movement->duration_updates) {
                        movement->active = 0;
                    } else {
                        movement_pending = 1;
                    }
                }
            }
            sprite_slot = sprite_slot + 1;
        } while (sprite_slot <= 15);
        YieldTaskForUpdates(1);
    } while (movement_pending != 0);
}
