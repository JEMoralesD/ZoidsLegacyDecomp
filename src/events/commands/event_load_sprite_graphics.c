#include "m2c_prelude.h"
#include "../event_script.h"
void LoadEventSpriteGraphics(int) asm("func_0809F8A0");
void SeekEventCommand(int, int, int) asm("func_80A016C");

int EventLoadSpriteGraphics(u8 script_slot, u8 **script_cursor) asm("func_080A48E0");

int EventLoadSpriteGraphics(u8 script_slot, u8 **script_cursor) {
    LoadEventSpriteGraphics(EVENT_COMMAND_BYTE((*script_cursor), EventSpriteResourceCommand, resource_id));
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
