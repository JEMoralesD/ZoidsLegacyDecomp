#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../graphics/screen_effects.h"
#include "../../game/game_state.h"

extern u8 gFieldEventActive asm("D_02030664");
extern u16 gFieldActorCommandQueues[] asm("D_02030668");
extern u32 gGameMode;
extern u8 gEventSpriteMovementStates[][16] asm("D_02031840");
extern u32 *gEventSprites[] asm("D_02031940");
extern u32 gBattleUnitSprites[][6] asm("D_02032E8C");
extern u8 gBattleUnitSpriteMotionStates[][6] asm("D_02032EEC");
extern u8 gFieldCameraMoveActive asm("D_02031749");

u32 IsScreenTransitionComplete(void) asm("func_0809669C");
u32 IsBattleCameraTransitionComplete(void) asm("func_080BB654");
u32 IsBattleAnimationCameraReady(void) asm("func_080D18CC");
u32 DivideUnsigned32(u32, u32) asm("func_080ECF00");
u32 ModuloUnsigned32(u32, u32) asm("func_080ECF78");
void UpdateFieldDisplay(void) asm("func_0809DFFC");
void DisableDisplayWindows(void) asm("func_0809534C");
void StopBattleAnimation(void) asm("func_080D120C");
void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 EventWaitForSceneActions(u8 script_slot) asm("func_080A09C8");

s32 EventWaitForSceneActions(u8 script_slot)
{
    register u32 saved_script_slot asm("sl") = script_slot;
    u32 finished_sprite_count;
    u32 finished_actor_or_unit_count;
    u32 game_mode;

    gFieldEventActive = 1;

retry:
    game_mode = gGameMode;
    if (game_mode == GAME_MODE_FIELD) {
        u16 *actor_command_queue;

        finished_actor_or_unit_count = 0;
        actor_command_queue = gFieldActorCommandQueues;
        if (*actor_command_queue == 0) {
            do {
                actor_command_queue = (u16 *)((u8 *)actor_command_queue + 0x100);
                finished_actor_or_unit_count++;
                if (finished_actor_or_unit_count > 13)
                    break;
            } while (*actor_command_queue == 0);
        }

        finished_sprite_count = 0;
        {
            register u32 mask asm("r2") = 1;
            register u32 **sprite_pointer_slot asm("r1") = gEventSprites;

            do {
                u32 *sprite;

                sprite = *sprite_pointer_slot;
                if (sprite != 0 && ((*sprite & mask) == 0))
                    *sprite_pointer_slot = 0;
                sprite_pointer_slot++;
                finished_sprite_count++;
            } while (finished_sprite_count <= 15);
        }

        finished_sprite_count = 0;
        {
            register u32 mask asm("r3") = 4;
            register u8 *sprite_movement_state asm("r2") = &gEventSpriteMovementStates[0][0];
            register u32 **sprite_pointer_slot asm("r1") = gEventSprites;

            do {
                u32 *sprite;

                sprite = *sprite_pointer_slot;
                if (sprite != 0) {
                    if ((*sprite & mask) == 0)
                        goto scan_done;
                    if (*sprite_movement_state != 0)
                        goto scan_done;
                }
                sprite_movement_state += 16;
                sprite_pointer_slot++;
                finished_sprite_count++;
            } while (finished_sprite_count <= 15);
        }
    } else if (game_mode == GAME_MODE_BATTLE) {
        finished_actor_or_unit_count = 0;
        {
            register u32 *unit_sprite_slots asm("r9") = &gBattleUnitSprites[0][0];
            register u8 *unit_motion_states asm("r8") = &gBattleUnitSpriteMotionStates[0][0];
mode9_loop:
            {
                register u32 unit_slot asm("r5");
                register u32 unit_sprite_offset asm("r4");
                register u32 side_index_carrier asm("r1");
                register u32 side asm("r0");

                unit_slot = ModuloUnsigned32(finished_actor_or_unit_count, 6);
                unit_sprite_offset = unit_slot << 2;
                side = DivideUnsigned32(finished_actor_or_unit_count, 6);
                side_index_carrier = side << 1;
                side_index_carrier += side;
                side = side_index_carrier << 3;
                unit_sprite_offset += side;
                unit_sprite_offset += (u32)unit_sprite_slots;
                if (*(u32 *)unit_sprite_offset == 0)
                    goto mode9_next;
                side = side_index_carrier << 1;
                side = unit_slot + side;
                side += (u32)unit_motion_states;
                if (*(u8 *)side != 0)
                    goto scan_done;
            }
mode9_next:
            finished_actor_or_unit_count++;
            if (finished_actor_or_unit_count <= 11)
                goto mode9_loop;
        }
    }

scan_done:
    asm volatile("" : : "r"(finished_actor_or_unit_count));
    if ((IsScreenTransitionComplete() << 24) != 0) {
        register u32 *game_mode_address asm("r0") = &gGameMode;
        register u32 ready_game_mode asm("r1") = *game_mode_address;
        register u32 *saved_game_mode_address asm("r2") = game_mode_address;

        if (ready_game_mode == GAME_MODE_FIELD && finished_actor_or_unit_count == 14 && finished_sprite_count == 16 &&
            gFieldCameraMoveActive == 0)
            goto ready;
        if (*saved_game_mode_address == GAME_MODE_BATTLE && finished_actor_or_unit_count == 12 &&
            (IsBattleCameraTransitionComplete() << 24) != 0)
            goto ready;
        if (gGameMode == GAME_MODE_BATTLE_SCENE) {
            if ((IsBattleAnimationCameraReady() << 24) == 0)
                goto not_ready;
            goto ready_fallthrough;
        }
    }

    goto not_ready;

ready:
ready_fallthrough:
    SeekEventCommand(saved_script_slot, -1, 0);
    *(u8 *)0x020316F6 = 0;
    goto done;

not_ready:
    game_mode = gGameMode;
    if (game_mode == GAME_MODE_FIELD) {
        u8 *flag;

        flag = (u8 *)0x020316F6;
        if (*flag == 0) {
            UpdateFieldDisplay();
            *flag = 1;
            goto retry;
        }
    } else if (game_mode == GAME_MODE_BATTLE_SCENE) {
        u8 status;

        status = *(u8 *)0x03005F70;
        if ((status == 2 || status == 4 || status == 6 || status == 8 ||
             status == 11 || status == 12 || status == 14 || status == 18) &&
            *(u8 *)0x03005F72 == *(u8 *)0x03005F71) {
            DisableDisplayWindows();
            StopBattleAnimation();
        }
    }

    *(u8 *)0x020316F6 = 1;
    YieldTaskForUpdates(1);

done:
    return 0;
}
