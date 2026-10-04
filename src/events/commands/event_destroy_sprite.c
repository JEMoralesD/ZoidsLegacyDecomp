#include "m2c_prelude.h"
#include "../event_script.h"
extern int DestroyEventSprite(int) asm("func_0809FB78");
extern int SeekEventCommand(int, int, int) asm("func_80A016C");

int EventDestroySprite(u8 script_slot, u8 **script_cursor) asm("func_080A49D8");

int EventDestroySprite(u8 script_slot, u8 **script_cursor) {
    DestroyEventSprite(EVENT_COMMAND_BYTE((*script_cursor), EventSpriteSlotCommand, sprite_slot));
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
