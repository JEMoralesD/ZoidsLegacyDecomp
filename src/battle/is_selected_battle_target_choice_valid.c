#include "m2c_prelude.h"
#include "battle.h"

s32 IsSelectedBattleTargetChoiceValid(s32 side, s32 target_choice) asm("func_080CBE08");

s32 IsSelectedBattleTargetChoiceValid(s32 side, s32 target_choice) {
    u8 equipment_slot;
    s32 side_stride_work;
    s32 offset;
    s32 equipment_offset;
    s32 next_target_unit;
    register s32 side_offset asm("r12");
    s32 target_choice_offset;
    register s32 action_offset asm("r6");
    register u8 *base asm("r5");
    register s32 action_index asm("r4");
    register s32 action_stride_work asm("r0");
    register s32 target_unit_index asm("r3");

    side <<= 24;
    side = (u32)side >> 24;
    target_unit_index = 0;
    base = (u8 *)0x02034B4C;
    side_stride_work = side << 3;
    side_stride_work += side;
    side_offset = side_stride_work << 3;
    target_choice <<= 24;
    target_choice >>= 24;
    target_choice_offset = target_choice * 0x94;
    action_index = base[0xA1AF];
    action_stride_work = action_index << 3;
    action_stride_work -= action_index;
    action_stride_work <<= 5;
    action_stride_work += action_index;
    action_offset = action_stride_work << 2;
loop:
        offset = target_unit_index << 1;
        offset += target_unit_index;
        offset <<= 2;
        offset += side_offset;
        offset += target_choice_offset;
        offset += action_offset;
        {
            register u8 *secondary_ptr asm("r1");

            secondary_ptr = base + 0xA1B0;
            equipment_slot = *(u8 *)((s32)action_index + (s32)secondary_ptr);
        }
        equipment_offset = equipment_slot * 0xA8C;
        offset += equipment_offset;
        offset += (s32)base;
        offset += 0x27D8;
        if (*(u16 *)offset & 1) {
            goto done;
        }
        next_target_unit = target_unit_index + 1;
        next_target_unit <<= 24;
        asm volatile("" :: "r"(target_unit_index));
        target_unit_index = (u32)next_target_unit >> 24;
        if ((u32)target_unit_index <= 5) {
            goto loop;
        }
done:
    return (u32)target_unit_index <= 5;
}
