#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
#include "../../graphics/screen_effects.h"

extern void DestroySprite(void *) asm("func_08094554");
extern void ClearWindow(s32) asm("func_080986B4");
extern void RunMenuScript(s32) asm("func_08098BB4");
extern void RequestWindowRefresh(void) asm("func_080972C8");
extern void StartScreenTransition(s32, s32) asm("func_08096308");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");
extern u8 IsScreenTransitionComplete(void) asm("func_0809669C");
extern void RunPlayerTeamFormationMenu(s32) asm("func_080B35D4");
extern void SeekEventCommand(s32, s32, s32) asm("func_080A016C");

s32 EventOpenTeamSetupIfEmpty(u8 script_slot) asm("func_080A66C8");

s32 EventOpenTeamSetupIfEmpty(u8 script_slot)
{
    *(u8 *)0x02030664 = 1;
    {
        register u32 team_slot_index asm("r1") = 0;
        register u8 *player_state asm("r0") = (u8 *)0x020218E4;
        register u32 team_slots_offset asm("r3") = 0x690C;
        register u8 *team_slots asm("r2");

        asm volatile("" : "+r"(player_state));
        asm volatile("" : "+r"(team_slots_offset));
        team_slots = player_state + team_slots_offset;
        if (team_slots[0] == 0) {
scan_team_slots:
            {
                register u32 next_team_slot asm("r0");
                next_team_slot = team_slot_index + 1;
                team_slot_index = (u8)next_team_slot;
            }
            if (team_slot_index <= 5) {
                register u8 *team_slot_address asm("r0") = (u8 *)team_slot_index;

                team_slot_address += (u32)team_slots;
                if (*team_slot_address == 0) {
                    goto scan_team_slots;
                }
            }
        }
        if (team_slot_index != 6) {
            goto advance_event;
        }
    }

    {
        void **portrait_sprite_address = (void **)0x02031744;
        if (*portrait_sprite_address != 0) {
            DestroySprite(*portrait_sprite_address);
            *portrait_sprite_address = 0;
        }
    }
    {
        u8 *message_window_state = (u8 *)0x02030666;
        if (*message_window_state == 0) {
            RunMenuScript(0x080177ED);
            *message_window_state = 2;
        } else if (*message_window_state == 1) {
            RunMenuScript(0x080177F5);
            RunMenuScript(0x080177ED);
            *message_window_state = 2;
        } else {
            ClearWindow(2);
        }
        RunMenuScript(0x0801798D);
        RunMenuScript(0x080177FA);
        RequestWindowRefresh();
        StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_CONCEAL, 0);
        goto wait_for_conceal;
yield_for_conceal:
        YieldTaskForUpdates(1);
wait_for_conceal:
        if ((IsScreenTransitionComplete() << 24) == 0) {
            goto yield_for_conceal;
        }
        {
            register s32 *game_mode_address asm("r4") = (s32 *)0x02021690;

            *game_mode_address = -1;
            YieldTaskForUpdates(1);
            RunPlayerTeamFormationMenu(1);
            *game_mode_address = 3;
            YieldTaskForUpdates(1);
            StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_REVEAL, 0);
        }
        goto wait_for_reveal;
yield_for_reveal:
        YieldTaskForUpdates(1);
wait_for_reveal:
        if ((IsScreenTransitionComplete() << 24) == 0) {
            goto yield_for_reveal;
        }
    }

advance_event:
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
