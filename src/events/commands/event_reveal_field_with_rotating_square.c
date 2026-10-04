#include "../../field/field_actor.h"
#include "../event_script.h"
#include "../../graphics/screen_effects.h"

void StartScreenTransition(s32, s32) asm("func_08096308");
s32 IsScreenTransitionComplete(void) asm("func_0809669C");
void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 EventRevealFieldWithRotatingSquare(u8 script_slot) asm("func_080A19F4");

s32 EventRevealFieldWithRotatingSquare(u8 script_slot)
{
    *(s8 *)FIELD_EVENT_ACTIVE_RAM = 1;
    if ((IsScreenTransitionComplete() << 24) == 0) {
        goto wait_for_transition;
    }
    StartScreenTransition(SCREEN_TRANSITION_ROTATING_SQUARE_REVEAL, 0);
    YieldTaskForUpdates(1);
    {
        register struct FieldActor *controlled_actor asm("r2") =
            *(struct FieldActor **)FIELD_CONTROLLED_ACTOR_POINTER_RAM;

        if (controlled_actor == 0) {
            goto scan_camera_follow_actor;
        }
        {
            register volatile s16 *focus_coordinate asm("r4") = (s16 *)SCREEN_TRANSITION_ADDRESS(focus_x);
            register s32 *camera_scroll_fixed8 asm("r3") = (s32 *)FIELD_CAMERA_SCROLL_RAM;
            register s32 screen_position_fixed8 asm("r0") = controlled_actor->world_x_fixed8 - camera_scroll_fixed8[0];

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
        register struct FieldActor *camera_follow_actor asm("r3") =
            (struct FieldActor *)FIELD_ACTORS_RAM;
        register volatile s16 *focus_x asm("r4");
        register volatile s16 *focus_y asm("r6");
        register struct FieldActor *actor_table asm("r5");
        register s32 screen_position_fixed8_or_pixels asm("r0");
        register u32 active_flags asm("r1") = camera_follow_actor->flags;
        register u32 active_mask asm("r0") = FIELD_ACTOR_ACTIVE;

        active_flags &= active_mask;
        focus_x = (s16 *)SCREEN_TRANSITION_ADDRESS(focus_x);
        focus_y = (s16 *)SCREEN_TRANSITION_ADDRESS(focus_y);
        actor_table = camera_follow_actor;
        if (active_flags == 0) {
scan_next_actor:
            actor_slot++;
            if (actor_slot > FIELD_ACTOR_MAX_SLOT) {
                goto use_screen_center;
            }
            {
                register u32 actor_offset asm("r0") = actor_slot << 3;

                actor_offset += actor_slot;
                actor_offset <<= 3;
                camera_follow_actor = (struct FieldActor *)(actor_offset + (u32)actor_table);
            }
            if ((camera_follow_actor->flags & FIELD_ACTOR_ACTIVE) == 0) {
                goto scan_next_actor;
            }
        }
        if (camera_follow_actor->behavior != FIELD_ACTOR_SCRIPTED_CAMERA_FOLLOW) {
            goto scan_next_actor;
        }
        if (actor_slot > FIELD_ACTOR_MAX_SLOT) {
            goto use_screen_center;
        }
        {
            register s32 *camera_scroll_fixed8 asm("r2") = (s32 *)FIELD_CAMERA_SCROLL_RAM;

            screen_position_fixed8_or_pixels = camera_follow_actor->world_x_fixed8 - camera_scroll_fixed8[0];
            if (screen_position_fixed8_or_pixels < 0) {
                screen_position_fixed8_or_pixels += 0xFF;
            }
            *focus_x = screen_position_fixed8_or_pixels >> 8;
            screen_position_fixed8_or_pixels = camera_follow_actor->world_y_fixed8 - camera_scroll_fixed8[1];
            if (screen_position_fixed8_or_pixels < 0) {
                screen_position_fixed8_or_pixels += 0xFF;
            }
            screen_position_fixed8_or_pixels >>= 8;
            goto store_focus_y;
        }

use_screen_center:
        *focus_x = SCREEN_WIDTH / 2;
        screen_position_fixed8_or_pixels = SCREEN_HEIGHT / 2;
store_focus_y:
        *focus_y = screen_position_fixed8_or_pixels;
    }

advance_script:
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    goto finish;
wait_for_transition:
    YieldTaskForUpdates(1);
finish:
    return EVENT_CONTINUE;
}
