#include "m2c_prelude.h"
#include "battle.h"
#include "battle_animation.h"


extern void LoadBattleAnimationResources(s32) asm("func_080D2340");


void StartBattleAnimation(struct BattleAnimationEntry *animation_entry, s32 side, s32 zoid_id, u16 item_id, s32 equipment_slot, s32 resource_variant) asm("func_080D0B60");

void StartBattleAnimation(struct BattleAnimationEntry *animation_entry, s32 side, s32 zoid_id, u16 item_id, s32 equipment_slot, s32 resource_variant)
{
    register struct BattleAnimationState *state asm("r4");
    register struct BattleAnimationState *initial_state asm("r3");
    u8 actual_mode = (u8)equipment_slot;
    u32 actual_column = (u8)resource_variant;
    s32 value;
    s16 *initial_cursor;
    u16 callback_kind;
    u8 i;

    __asm__("" : : : "memory");
    initial_state = (struct BattleAnimationState *)0x02033FD0;

    initial_state->status = BATTLE_ANIMATION_ACTIVE;
    initial_state->side = side;
    initial_state->zoid_id = zoid_id;
    initial_state->equipment_slot = actual_mode;
    actual_column = actual_column + (animation_entry->control & BATTLE_ANIMATION_VARIANT_MASK);
    initial_state->resource_variant = actual_column;
    actual_column <<= 24;
    actual_column >>= 24;
    if (actual_column > 2) {
        initial_state->resource_variant = 2;
    }
    {
        register u32 flags asm("r0");
        asm volatile(
            "ldrb r1, [%1, #4]\n\t"
            "mov r0, #60\n\t"
            "and r0, r1"
            : "=r"(flags)
            : "r"(animation_entry)
            : "r1", "cc", "memory");
        initial_state->anchor_mode = flags;
    }
    {
        register struct BattleAnimationState *value_state asm("r4") = initial_state;
        asm volatile("" : "+r"(value_state));
        {
            register s32 loaded_value asm("r3");
            loaded_value = animation_entry->x_offset;
            asm volatile("" : "+r"(loaded_value));
            value = loaded_value;
        }
        if (actual_mode == 2) {
            register s32 adjusted_value asm("r1") = value;
            asm volatile("" : "+r"(adjusted_value));
            if (item_id <= 100) {
                s16 *table = (s16 *)0x087AC90C;
                asm volatile("" : : "r"(item_id), "r"(value));
                asm volatile(
                    "lsl r1, r7, #1\n\t"
                    "add r1, r1, %0\n\t"
                    "mov r2, #0\n\t"
                    "ldrsh %0, [r1, r2]\n\t"
                    "sub %1, r3, %0"
                    : "+r"(table), "=r"(adjusted_value)
                    :
                    : "r2", "cc", "memory");
            }
            value = adjusted_value;
        }
        value_state->x_offset = value;
    }
    state = (struct BattleAnimationState *)0x02033FD0;
    state->y_offset = animation_entry->y_offset;
    state->spawn_count = 0;
    state->delay = 0;
    state->loop_count = 0;
    *(s32 *)0x02034050 = 0;
    initial_cursor = animation_entry->script;
    state->script_cursor = initial_cursor;
    {
        register s32 call_arg asm("r0") = (s32)initial_cursor;
        asm volatile(
            "mov r1, #0\n\t"
            "ldrsh r0, [r0, r1]\n\t"
            "ldrb r2, [%1, #4]\n\t"
            "add r0, r0, r2"
            : "+r"(call_arg)
            : "r"(state)
            : "r1", "r2", "cc", "memory");
        LoadBattleAnimationResources(call_arg);
    }
    {
    register u8 *entry_table asm("r6") = (u8 *)0x087D2338;
    register s32 lookup_index asm("r1");
    s16 *cursor;
    asm volatile("" : "+r"(entry_table));
    cursor = state->script_cursor;
    asm volatile(
        "mov r0, #0\n\t"
        "ldrsh r1, [%1, r0]\n\t"
        "ldrb r2, [%2, #4]\n\t"
        "add r1, r1, r2"
        : "=r"(lookup_index)
        : "r"(cursor), "r"(state)
        : "r0", "r2", "cc", "memory");
    callback_kind = *(u16 *)(entry_table + lookup_index * 0x1C);
    state->callback_kind = callback_kind;
    asm volatile(
        "lsl %0, %0, #16\n\t"
        "lsr %0, %0, #16"
        : "+r"(callback_kind)
        :
        : "cc");
    if (callback_kind == 0x59) {
        u8 *base;
        register u32 outer asm("r0");
        u32 outer_offset;
        u32 inner_offset;
        u8 *dst_base;
        s32 result;
        register u32 tail_offset asm("r1");
        base = (u8 *)0x02034B4C;
        tail_offset = 0x27A5;
        asm volatile("" : "+r"(tail_offset));
        asm volatile(
            "add r0, %1, %2\n\t"
            "ldrb r0, [r0]"
            : "=r"(outer)
            : "r"(base), "r"(tail_offset)
            : "cc", "memory");
        outer_offset = outer * 0x270;
        asm volatile("" : "+r"(outer_offset));
        tail_offset -= 1;
        asm volatile("" : "+r"(tail_offset));
        asm volatile(
            "add r0, %1, r1\n\t"
            "ldrb r1, [r0]"
            : "+r"(tail_offset)
            : "r"(base)
            : "r0", "cc", "memory");
        inner_offset = tail_offset * 0x1380;
        asm volatile("" : "+r"(inner_offset));
        outer_offset += inner_offset;
        asm volatile("" : "+r"(outer_offset));
        dst_base = base + outer_offset;
        asm volatile("" : "+r"(dst_base));
        result = *(u16 *)(entry_table + (*cursor + state->resource_variant) * 0x1C + 2) + 1;
        asm volatile("" : "+r"(result));
        *(u16 *)(dst_base + 0x264) = result;
    }
    }
    {
    s16 *next_cursor = state->script_cursor;
    next_cursor += 1;
    state->script_cursor = next_cursor;
    state->loop_start = next_cursor;
    }
    {
        register volatile u16 *display_a asm("r5");
        register volatile u16 *display_b asm("r6");
        register u32 *slots asm("r3");
        i = 0;
        display_a = (volatile u16 *)0x0300004E;
        asm volatile("" : "+r"(display_a));
        display_b = (volatile u16 *)0x03000050;
        asm volatile("" : "+r"(display_b));
        slots = state->groups;
        asm volatile("" : "+r"(slots));
        do {
            slots[i] = 0;
            i++;
        } while (i <= 15);
        asm volatile(
            "mov r2, #232\n\t"
            "lsl r2, r2, #3\n\t"
            "mov r0, r2\n\t"
            "strh r0, [%0]\n\t"
            "mov r1, #129\n\t"
            "lsl r1, r1, #4\n\t"
            "mov r0, r1\n\t"
            "strh r0, [%1]"
            :
            : "r"(display_a), "r"(display_b)
            : "r0", "r1", "r2", "cc", "memory");
    }
}
