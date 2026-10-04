#include "../../field/field_actor.h"
#include "../event_script.h"
#include "../../graphics/screen_effects.h"

void StartScreenTransition(s32, s32) asm("func_08096308");
s32 IsScreenTransitionComplete(void) asm("func_0809669C");
void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 EventConcealFieldWithRotatingSquare(s32 script_slot) asm("func_080A1AE8");

s32 EventConcealFieldWithRotatingSquare(s32 script_slot)
{
    u8 saved_script_slot;

    script_slot <<= 24;
    saved_script_slot = (u32)script_slot >> 24;
    *(u8 *)FIELD_EVENT_ACTIVE_RAM = 1;
    if ((IsScreenTransitionComplete() << 24) == 0) {
        goto wait_for_transition;
    }

    StartScreenTransition(SCREEN_TRANSITION_ROTATING_SQUARE_CONCEAL, 0);
    {
        register struct FieldActor *controlled_actor asm("r2") = *(struct FieldActor **)FIELD_CONTROLLED_ACTOR_POINTER_RAM;

        if (controlled_actor == 0) {
            goto scan_camera_follow_actor;
        }
        {
            register s16 *focus_coordinate asm("r4") = (s16 *)SCREEN_TRANSITION_ADDRESS(focus_x);
            register s32 *camera_scroll_fixed8 asm("r3") = (s32 *)FIELD_CAMERA_SCROLL_RAM;
            register s32 screen_position_fixed8 asm("r0");

            screen_position_fixed8 = controlled_actor->world_x_fixed8 - camera_scroll_fixed8[0];
            if (screen_position_fixed8 < 0) {
                screen_position_fixed8 += 0xFF;
            }
            *focus_coordinate = screen_position_fixed8 >> 8;
            focus_coordinate = (s16 *)SCREEN_TRANSITION_ADDRESS(focus_y);
            screen_position_fixed8 = controlled_actor->world_y_fixed8 - camera_scroll_fixed8[1];
            if (screen_position_fixed8 < 0) {
                screen_position_fixed8 += 0xFF;
            }
            *focus_coordinate = screen_position_fixed8 >> 8;
        }
        goto advance_script;
    }

scan_camera_follow_actor:
    {
        register u32 actor_slot asm("r2") = 0;
        register u8 *actor_bytes asm("r3") = (u8 *)FIELD_ACTORS_RAM;
        register s16 *focus_x asm("r4");
        register s16 *focus_y asm("r6");
        register u8 *actor_table_bytes asm("r5");
        register u32 active_flags asm("r1") = *(u32 *)actor_bytes;
        register u32 active_mask asm("r0") = FIELD_ACTOR_ACTIVE;

        active_flags &= active_mask;
        asm volatile("" : "+r"(active_flags));
        focus_x = (s16 *)SCREEN_TRANSITION_ADDRESS(focus_x);
        asm volatile("" : "+r"(focus_x));
        focus_y = (s16 *)SCREEN_TRANSITION_ADDRESS(focus_y);
        asm volatile("" : "+r"(focus_y));
        actor_table_bytes = actor_bytes;
        if (active_flags != 0) {
            goto check_actor_behavior;
        }
scan_next_actor:
        actor_slot++;
        if (actor_slot > FIELD_ACTOR_MAX_SLOT) {
            goto use_screen_center;
        }
        {
            register u32 actor_offset asm("r0") = actor_slot << 3;
            actor_offset += actor_slot;
            actor_offset <<= 3;
            actor_bytes = (u8 *)(actor_offset + (u32)actor_table_bytes);
        }
        if ((*(u32 *)actor_bytes & FIELD_ACTOR_ACTIVE) == 0) {
            goto scan_next_actor;
        }
check_actor_behavior:
        if (FIELD_ACTOR_FIELD(actor_bytes, u16 *, behavior) != FIELD_ACTOR_SCRIPTED_CAMERA_FOLLOW) {
            goto scan_next_actor;
        }
        if (actor_slot > FIELD_ACTOR_MAX_SLOT) {
            goto use_screen_center;
        }
        {
            register s32 *camera_scroll_fixed8 asm("r2") = (s32 *)FIELD_CAMERA_SCROLL_RAM;
            register s32 screen_position_fixed8 asm("r0");

            screen_position_fixed8 = FIELD_ACTOR_FIELD(actor_bytes, s32 *, world_x_fixed8) - camera_scroll_fixed8[0];
            if (screen_position_fixed8 < 0) {
                screen_position_fixed8 += 0xFF;
            }
            *focus_x = screen_position_fixed8 >> 8;
            screen_position_fixed8 = FIELD_ACTOR_FIELD(actor_bytes, s32 *, world_y_fixed8) - camera_scroll_fixed8[1];
            if (screen_position_fixed8 < 0) {
                screen_position_fixed8 += 0xFF;
            }
            screen_position_fixed8 >>= 8;
            *focus_y = screen_position_fixed8;
        }
        goto advance_script;

use_screen_center:
        *focus_x = SCREEN_WIDTH / 2;
        *focus_y = SCREEN_HEIGHT / 2;
    }

advance_script:
    SeekEventCommand(saved_script_slot, EVENT_SCAN_NEXT, 0);
    goto finish;

wait_for_transition:
    YieldTaskForUpdates(1);

finish:
    return EVENT_CONTINUE;
}
