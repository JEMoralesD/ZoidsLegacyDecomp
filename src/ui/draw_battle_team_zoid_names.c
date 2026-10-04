#include "player_selection.h"

s32 PrintWindowTextAt(s32, s32, s32, s32, s32) asm("func_080981F0");
void ClearWindow(s32) asm("func_080986B4");

s32 DrawBattleTeamZoidNames(void) asm("func_080B460C");

s32 DrawBattleTeamZoidNames(void) {
    u32 index;
    u8 *battle_units;
    s32 *zoid_names;
    register s32 callback_result asm("r0");

    ClearWindow(3);
    index = 0;
    battle_units = (u8 *)0x02034B4C;
    zoid_names = (s32 *)0x087EDD54;
    do {
        register s32 team_units_offset asm("r1");
        register u8 *team_units asm("r0");
        register u8 *team_slot asm("r1");
        register s32 unit_index asm("r0");

        team_units_offset = 0xA084;
        team_units = (u8 *)((s32)battle_units + team_units_offset);
        team_slot = (u8 *)(index + (s32)team_units);
        unit_index = *team_slot;
        if (unit_index != 0xFF) {
            register s32 unit_index_copy asm("r1");
            register s32 unit_offset asm("r0");
            register struct BattleUnit *unit asm("r0");
            register s32 name_offset asm("r0");
            register s32 name_text_address asm("r0");

            unit_index_copy = unit_index;
            unit_offset = unit_index_copy << 2;
            unit_offset += unit_index_copy;
            unit_offset <<= 3;
            unit_offset -= unit_index_copy;
            unit_offset <<= 4;
            unit = (struct BattleUnit *)(unit_offset + (s32)battle_units);
            name_offset = unit->zoid_id << 2;
            name_text_address = *(s32 *)(name_offset + (s32)zoid_names);
            callback_result = PrintWindowTextAt(name_text_address, 0, 3, 0, index << 1);
        } else {
            callback_result = PrintWindowTextAt(PARTY_EMPTY_SLOT_TEXT_ROM, 0, 3, 0, index << 1);
        }
        {
            register u32 next_index asm("r1");

            next_index = index + 1;
            next_index <<= 24;
            index = next_index >> 24;
        }
    } while (index <= 5);
    return callback_result;
}
