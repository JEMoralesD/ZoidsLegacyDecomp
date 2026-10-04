#include "m2c_prelude.h"
#include "battle.h"

void ResetBattleEffectDisplayQueue(void) asm("func_080C5D14");

void ResetBattleEffectDisplayQueue(void) {
    s32 side;
    u8 unit_slot;
    u8 effect_slot;
    s32 unit_offset;
    s32 unit_offset_copy;
    s32 unit_queue_address;
    s32 effect_record_address;
    s32 next_unit_slot;
    register s32 side_queue_stride asm("r10");
    register s32 applied_kind_flags_offset asm("r9");
    register s32 removed_kind_flags_offset asm("r8");
    register s32 clear_value asm("r6");
    register s32 next_side asm("r12");
    register s32 side_queue_offset asm("r4");

    side = 0;
    {
        register s32 queue_layout_value asm("r1");

        queue_layout_value = (s32)sizeof(struct BattleUnitEffectDisplayQueue) * BATTLE_ACTIVE_UNIT_COUNT;
        side_queue_stride = queue_layout_value;
    }
    clear_value = 0;
    {
        register s32 queue_layout_value asm("r2");

        queue_layout_value = BATTLE_EFFECT_DISPLAY_QUEUE_OFFSET(units[0][0].applied_effects[0].kind_flags);
        asm volatile("" : "+r"(queue_layout_value));
        applied_kind_flags_offset = queue_layout_value;
    }
    {
        register s32 queue_layout_value asm("r5");

        queue_layout_value = BATTLE_EFFECT_DISPLAY_QUEUE_OFFSET(units[0][0].removed_effects[0].kind_flags);
        asm volatile("" : "+r"(queue_layout_value));
        removed_kind_flags_offset = queue_layout_value;
    }
    do {
        unit_slot = 0;
        {
            register s32 next_side_seed asm("r7");

            next_side_seed = side + 1;
            next_side = next_side_seed;
        }
        side_queue_offset = side_queue_stride;
        side_queue_offset *= side;
        do {
            unit_offset = unit_slot << 1;
            unit_offset += unit_slot;
            unit_offset <<= 6;
            unit_offset += unit_slot;
            unit_offset <<= 2;
            unit_queue_address = unit_offset + side_queue_offset;
            unit_queue_address += 0x02034B4C;
            *(u16 *)(unit_queue_address + BATTLE_EFFECT_DISPLAY_QUEUE_OFFSET(units[0][0].damage)) = clear_value;
            *(u16 *)(unit_queue_address + BATTLE_EFFECT_DISPLAY_QUEUE_OFFSET(units[0][0].flags)) = clear_value;
            effect_slot = 0;
            next_unit_slot = unit_slot + 1;
            unit_offset_copy = unit_offset;
            do {
                effect_record_address = effect_slot << 1;
                effect_record_address += effect_slot;
                effect_record_address <<= 2;
                effect_record_address += unit_offset_copy;
                effect_record_address += side_queue_offset;
                effect_record_address += 0x02034B4C;
                *(u16 *)(effect_record_address + applied_kind_flags_offset) = clear_value;
                *(u16 *)(effect_record_address + removed_kind_flags_offset) = clear_value;
                effect_slot += 1;
            } while ((u32)effect_slot <= BATTLE_EFFECT_SLOT_COUNT - 1);
            unit_slot = (u8)next_unit_slot;
        } while ((u32)unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1);
        {
            register s32 next_side_reload asm("r1");
            s32 next_side_shifted;

            next_side_reload = next_side;
            next_side_shifted = next_side_reload << 24;
            side = (u32)next_side_shifted >> 24;
        }
    } while ((u32)side <= BATTLE_SIDE_COUNT - 1);
}
