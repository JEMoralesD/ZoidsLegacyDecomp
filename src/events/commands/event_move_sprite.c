#include "m2c_prelude.h"
#include "../event_script.h"
extern void BeginEventSpriteMovement(u8, s16, s16, u8) asm("func_0809FA24");
extern void StartTask(s32, void (*)(void)) asm("func_08092D8C");
extern void SeekEventCommand(u8, s32, s32) asm("func_80A016C");
extern void RunEventSpriteMovementTask(void) asm("func_0809FA9C");

int EventMoveSprite(u8 script_slot, u8 **script_cursor) asm("func_080A498C");

int EventMoveSprite(u8 script_slot, u8 **script_cursor) {
    u8 *command = *script_cursor;
    BeginEventSpriteMovement(EVENT_COMMAND_BYTE(command, EventSpriteMoveCommand, sprite_slot), (s16)((EVENT_COMMAND_BYTE(command, EventSpriteMoveCommand, x_high) << 8) + EVENT_COMMAND_BYTE(command, EventSpriteMoveCommand, x_low)), (s16)((EVENT_COMMAND_BYTE(command, EventSpriteMoveCommand, y_high) << 8) + EVENT_COMMAND_BYTE(command, EventSpriteMoveCommand, y_low)), EVENT_COMMAND_BYTE(command, EventSpriteMoveCommand, duration_updates));
    StartTask(EVENT_SPRITE_MOVEMENT_TASK, RunEventSpriteMovementTask);
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
