#include "m2c_prelude.h"
#include "../game/game_state.h"
#include "event_script.h"

extern void *gEventCommandHandlers[];

extern void DestroySprite(void *) asm("func_08094554");
extern void RunMenuScript(s32) asm("func_08098BB4");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");
extern s32 CallFunctionR2(s32, void *, void *) asm("func_080ECD64");

void RunEventScripts(void)
{
    *(u8 *)0x020316F7 = 0;

    {
        register u32 script_slot asm("r6");

again:
        script_slot = 0;
loop:
        {
            register void **cursor asm("r5");
            register u32 shifted_slot asm("r8");
            register void **script_cursors asm("r1") = (void **)0x020314C4;
            register u32 offset asm("r0");
            register void **command_handlers asm("r2");
            void *handler;
            u8 handler_result;
            u8 opcode;

            offset = script_slot << 2;
            cursor = (void **)((u8 *)script_cursors + offset);
            if (*cursor != 0) {
                shifted_slot = script_slot << 24;
retry:
                command_handlers = gEventCommandHandlers;
                asm volatile("" : "+r"(command_handlers));
                opcode = *(u8 *)*cursor;
                handler = command_handlers[opcode];
                if (opcode == EVENT_CHANGE_MAP) {
                    CallFunctionR2(shifted_slot >> 24, cursor, handler);
                    return;
                }
                handler_result = CallFunctionR2(shifted_slot >> 24, cursor, handler);
                opcode = *(u8 *)*cursor;
                if (opcode == EVENT_END || opcode == EVENT_RESTART ||
                    (opcode == 0xC && *(u8 *)0x020316F6 != 0) ||
                    opcode == EVENT_WAIT_FRAMES || opcode == 0x35 || opcode == EVENT_ITEM_SHOP ||
                    opcode == EVENT_WEAPON_SHOP || opcode == EVENT_ARMOR_SHOP || opcode == 0x49 ||
                    opcode == 0x85 || opcode == 0x8E || opcode == 0x90 ||
                    opcode == 0x84) {
                    if (*(s32 *)0x02021690 != GAME_MODE_BATTLE_SCENE) {
                        register void **object asm("r4") = (void **)0x02031744;
                        if (*object != 0) {
                            DestroySprite(*object);
                            *object = 0;
                        }
                        {
                            u8 *state = (u8 *)0x02030666;
                            if (*state == 0) {
                                goto skip_message;
                            }
                            if (*state == 1) {
                                RunMenuScript(0x080177F5);
                            } else {
                                RunMenuScript(0x080177FA);
                            }
                            RunMenuScript(0x080177FD);
                            *(u8 *)0x02030666 = 0;
skip_message:
                            ;
                        }
                    }
                }
                if (handler_result == EVENT_CONTINUE) {
                    goto retry;
                }
            }
        }
        script_slot++;
        if (script_slot < EVENT_SCRIPT_SLOT_COUNT) {
            goto loop;
        }
    }
    YieldTaskForUpdates(1);
    goto again;
}
