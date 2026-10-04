#include "m2c_prelude.h"
#include "../event_script.h"
M2C_UNK SeekEventCommand(u8, s32, s32) asm("func_080A016C");
M2C_UNK HideBattleUnitSprites(u8, u8) asm("func_080BB05C");
extern s8 gBattleState;

s32 EventRemoveBattleUnit(u8 script_slot, void **script_cursor) asm("func_080A2E60");

s32 EventRemoveBattleUnit(u8 script_slot, void **script_cursor) {
    void *command;
    void *command_reloaded;
    s8 *battle_state;
    u8 unit_slot;
    s32 unit_offset;

    command = *script_cursor;
    HideBattleUnitSprites(M2C_FIELD(command, u8 *, 1), M2C_FIELD(command, u8 *, 2));
    battle_state = &gBattleState;
    command_reloaded = *script_cursor;
    unit_slot = M2C_FIELD(command_reloaded, u8 *, 2);
    unit_offset = unit_slot * 0x270;
    unit_offset += M2C_FIELD(command_reloaded, u8 *, 1) * 0x1380;
    battle_state[unit_offset] = 0;
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
