#include "m2c_prelude.h"
#include "battle.h"
#include "battle_animation.h"

extern s32 CreateBattleAnimationGroup(u8, s32, s32, s32, s32) asm("func_080E083C");
extern u8 func_080ECF78(u8, s32);
extern u8 gBattleSetup[];


struct BattleAnimationScriptState {
    u8 status;
    u8 side;
    u8 zoid_id;
    u8 equipment_slot;
    u8 resource_variant;
    u8 anchor_mode;
    u16 x_offset;
    u16 y_offset;
    u16 sprite_flags;
    u8 pad2[2];
    u8 spawn_count;
    u8 pad3;
    struct BattleAnimationCommand *script;
    struct BattleAnimationCommand *loop_start;
    u16 callback_kind;
    u16 loop_count;
    u16 delay;
    u16 pad4;
    void *groups[16];
};

void UpdateBattleAnimationScript(void) asm("func_080D1090");

void UpdateBattleAnimationScript(void)
{
    register u32 state_value asm("r4");
    register void **scan_slots;
    u8 index;

#define STATE ((struct BattleAnimationScriptState *)state_value)

    index = 0;
    state_value = 0x02033FD0;
    scan_slots = STATE->groups;
    {
    register u32 live_mask asm("r3") = 1;

    do {
        register void **slot_ptr asm("r0") =
            (void **)(((u32)index << 2) + (u32)scan_slots);
        register void *slot asm("r1") = *slot_ptr;

        if (slot != 0) {
            if ((*(u32 *)slot & live_mask) == 0) {
                *slot_ptr = 0;
            }
        }
        index++;
    } while (index <= 15);
    }

    if (STATE->delay == 0) {
        if (STATE->script->delay_or_command != BATTLE_ANIMATION_SCRIPT_END) {

    {
    register struct BattleAnimationScriptState *work_state asm("r6") = STATE;
    register void **work_slots asm("r8");

    asm volatile(
        "mov r3, #32\n\t"
        "add r3, r3, %1\n\t"
        "mov %0, r3"
        : "=r"(work_slots)
        : "r"(state_value)
        : "r3");

command_loop:
    {
                register struct BattleAnimationCommand *script asm("r2") = work_state->script;
                register s32 command asm("r1");

                asm volatile(
                    "mov r5, #0\n\t"
                    "ldrsh %0, [%1, r5]"
                    : "=r"(command)
                    : "r"(script)
                    : "r5");

                if (command == BATTLE_ANIMATION_SCRIPT_LOOP) {
                    register s32 control_x_offset asm("r7");
                    register s32 control_x asm("r0");
                    register u32 current_position asm("r1");

                    STATE->loop_count++;
                    control_x_offset = 2;
                    asm volatile("ldrsh %0, [%1, %2]"
                        : "=r"(control_x)
                        : "r"(script), "r"(control_x_offset));
                    asm volatile("ldrh %0, [%1, #26]"
                        : "=r"(current_position)
                        : "r"(state_value));
                    if (control_x == current_position) {
                        register struct BattleAnimationCommand *next_script asm("r0") =
                            (struct BattleAnimationCommand *)((u8 *)script + 4);
                        work_state->script = next_script;
                    } else {
                        work_state->script = work_state->loop_start;
                    }
                } else {
                    register s32 x asm("r2");
                    register s32 y asm("r3");
                    register u32 parameter asm("r1");
                    void *created;

                    state_value = STATE->callback_kind;
                    if (state_value == 1) {
                        register u32 mode_bit asm("r0") = work_state->spawn_count;

                        mode_bit &= state_value;
                        state_value = mode_bit + 1;
                    } else if (state_value == 0x5F && func_080ECF78(work_state->spawn_count, 3) != 0) {
                        volatile u8 *scene_base = gBattleSetup;
                        u8 scene = scene_base[2];

                        state_value = 0x61;
                        if (scene == 0xD) {
                            state_value = 0x60;
                        }
                    }

                    {
                    register struct BattleAnimationScriptState *call_state asm("r5") =
                        (struct BattleAnimationScriptState *)0x02033FD0;
                    register u32 object_type asm("r0") = call_state->zoid_id;
                    register struct BattleAnimationCommand *call_script asm("r1") =
                        call_state->script;
                    u32 offset;

                    asm volatile(
                        "ldrh %0, [%3, #2]\n\t"
                        "ldrh %1, [%4, #6]\n\t"
                        "add %0, %0, %1\n\t"
                        "lsl %0, %0, #16\n\t"
                        "asr %0, %0, #16\n\t"
                        "ldrh %1, [%3, #4]\n\t"
                        "ldrh %2, [%4, #8]\n\t"
                        "add %1, %1, %2\n\t"
                        "lsl %1, %1, #16\n\t"
                        "asr %1, %1, #16"
                        : "=r"(x), "=r"(y), "=&l"(offset)
                        : "r"(call_script), "r"(call_state)
                        );
                    asm volatile(
                        "ldrh %0, [%0, #6]\n\t"
                        "lsl %0, %0, #6\n\t"
                        "ldrh %1, [%2, #10]\n\t"
                        "add %0, %0, %1\n\t"
                        "lsl %0, %0, #16\n\t"
                        "lsr %0, %0, #16"
                        : "=r"(parameter), "=&l"(offset)
                        : "r"(call_state), "0"(call_script));
                    created = (void *)CreateBattleAnimationGroup(
                        object_type, state_value, x, y, parameter);

                    index = 0;
                    if (call_state->groups[0] == 0) {
                        call_state->groups[0] = created;
                        state_value = (u32)call_state;
                    } else {
                        while (1) {
                            index++;
                            state_value = 0x02033FD0;
                            if (index > 15) {
                                break;
                            }
                            {
                            void **created_slot =
                                (void **)(((u32)index << 2) + (u32)work_slots);

                            if (*created_slot == 0) {
                                *created_slot = created;
                                break;
                            }
                            }
                        }
                    }
                    }

                    {
                    register struct BattleAnimationCommand *advance_script asm("r1") =
                        work_state->script;

                    work_state->delay = advance_script->delay_or_command;
                    advance_script += 1;
                    work_state->script = advance_script;
                    }
                    work_state->spawn_count++;
                }
    }
    if (STATE->delay == 0) {
        register struct BattleAnimationCommand *loop_script asm("r0") = STATE->script;
        register s32 loop_command asm("r1");

        asm volatile(
            "mov r7, #0\n\t"
            "ldrsh %0, [%1, r7]"
            : "=r"(loop_command)
            : "r"(loop_script)
            : "r7");
        if (loop_command != BATTLE_ANIMATION_SCRIPT_END) {
            goto command_loop;
        }
    }
    }
        } else {
            goto terminal_scan;
        }
    }
    goto check_terminal;

check_terminal:
    {
    register struct BattleAnimationCommand *tail_script asm("r0") = STATE->script;
    register s32 tail_command asm("r1");

    asm volatile(
        "mov r2, #0\n\t"
        "ldrsh %0, [%1, r2]"
        : "=r"(tail_command)
        : "r"(tail_script)
        : "r2");
    if (tail_command != BATTLE_ANIMATION_SCRIPT_END) {
        goto decrement_delay;
    }
    }

terminal_scan:
    {
        index = 0;
        if (STATE->groups[0] == 0) {
            register void **final_slots asm("r1") = STATE->groups;

            do {
                index++;
            } while (index <= 15 && final_slots[index] == 0);
        }
        if (index == 16) {
            STATE->status = BATTLE_ANIMATION_SCRIPT_FINISHED;
        }
    }
    goto done;

decrement_delay:
    (*(volatile u16 *)&STATE->delay)--;

done:
    ;
#undef STATE
}
