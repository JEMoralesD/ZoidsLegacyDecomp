#include "m2c_prelude.h"
#include "../event_script.h"
extern void CreateEventSprite() asm("func_0809F94C");
extern void SeekEventCommand() asm("func_80A016C");
extern void SetSpriteAnimation() asm("func_8094564");
extern s32 gEventSprites[] asm("D_02031940");

s32 EventCreateSprite(u8 script_slot, u8 **script_cursor) asm("func_080A4904");

s32 EventCreateSprite(u8 script_slot, u8 **script_cursor) {
    u8 *command;
    command = *script_cursor;
    CreateEventSprite(EVENT_COMMAND_BYTE(command, EventSpriteCreateCommand, sprite_slot), EVENT_COMMAND_BYTE(command, EventSpriteCreateCommand, resource_id), EVENT_COMMAND_BYTE(command, EventSpriteCreateCommand, animation_id),
                 (s16)((EVENT_COMMAND_BYTE(command, EventSpriteCreateCommand, x_high) << 8) + EVENT_COMMAND_BYTE(command, EventSpriteCreateCommand, x_low)),
                 (s32)(s16)((EVENT_COMMAND_BYTE(command, EventSpriteCreateCommand, y_high) << 8) + EVENT_COMMAND_BYTE(command, EventSpriteCreateCommand, y_low)),
                 (s32)EVENT_COMMAND_BYTE(command, EventSpriteCreateCommand, playback_mode));
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}

s32 EventSetSpriteAnimation(u8 script_slot, u8 **script_cursor) asm("func_080A4958");

s32 EventSetSpriteAnimation(u8 script_slot, u8 **script_cursor) {
    s32 *sprite_addresses;
    u8 *command;
    sprite_addresses = gEventSprites;
    command = *script_cursor;
    SetSpriteAnimation(sprite_addresses[EVENT_COMMAND_BYTE(command, EventSpriteAnimationCommand, sprite_slot)], EVENT_COMMAND_BYTE(command, EventSpriteAnimationCommand, animation_id));
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
