#include "player_selection.h"

s32 PrintWindowTextAt(s32, s32, s32, s32, s32) asm("func_080981F0");
void ClearWindow(s32) asm("func_080986B4");

s32 DrawPlayerTeamZoidNames(void) asm("func_080B339C");

s32 DrawPlayerTeamZoidNames(void) {
    u32 index;
    u8 *player_state;
    s32 *zoid_names;
    register s32 callback_result asm("r0");

    ClearWindow(3);
    index = 0;
    player_state = (u8 *)0x020218E4;
    zoid_names = (s32 *)0x087EDD54;
    do {
        register s32 team_slots_offset asm("r1");
        register u8 *team_slots asm("r0");
        register u8 *team_slot asm("r1");
        register s32 zoid_slot asm("r0");

        team_slots_offset = PLAYER_STATE_OFFSET(team_zoid_slots);
        team_slots = (u8 *)((s32)player_state + team_slots_offset);
        team_slot = (u8 *)(index + (s32)team_slots);
        zoid_slot = *team_slot;
        if (zoid_slot != 0) {
            register s32 zoid_slot_copy asm("r1");
            register s32 zoid_offset asm("r0");
            register struct PlayerTeamZoidModelView *zoid_slot_view asm("r0");
            register s32 name_offset asm("r0");
            register s32 name_text_address asm("r0");

            zoid_slot_copy = zoid_slot;
            zoid_offset = zoid_slot_copy << 3;
            zoid_offset -= zoid_slot_copy;
            zoid_offset <<= 4;
            zoid_slot_view = (struct PlayerTeamZoidModelView *)(zoid_offset + (s32)player_state);
            name_offset = zoid_slot_view->model_id << 2;
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
