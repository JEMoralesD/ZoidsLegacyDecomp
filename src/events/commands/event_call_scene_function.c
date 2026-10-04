#include "m2c_prelude.h"
#include "../event_script.h"
extern u8 gFieldEventActive asm("D_02030664");
extern u32 gEventSceneFunctions[] asm("D_087A1B6C");
extern void CallFunctionR0(u32) asm("func_80ECD5C");
extern void SeekEventCommand(u8, s32, s32) asm("func_80A016C");

s32 EventCallSceneFunction(u8 script_slot, u8 **script_cursor) asm("func_080A53B0");

s32 EventCallSceneFunction(u8 script_slot, u8 **script_cursor) {
    gFieldEventActive = 1;
    CallFunctionR0(gEventSceneFunctions[EVENT_COMMAND_BYTE((*script_cursor), EventSceneFunctionCommand, function_index)]);
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
