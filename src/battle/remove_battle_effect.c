#include "m2c_prelude.h"
#include "battle.h"


extern u8 gBattleState[];

u8 FindBattleEffect(u32, u32, s32) asm("func_080BF464");
s32 ScaleByPercent(s32, s32) asm("func_080E522C");

#define APPLY_ADJUST(slot_stride, side_stride, field_offset) do {       \
    register s32 current asm("r0") = destination->value;                \
    register u8 *stat_base asm("r3") = gBattleState;                      \
    register u32 stat_address asm("r2") = unit_slot * (slot_stride);         \
    register u32 side_offset asm("r1") = side * (side_stride);       \
    register s32 stat asm("r1");                                       \
    stat_address += side_offset;                                       \
    stat_address += (u32)stat_base;                                     \
    stat_address += (field_offset);                                     \
    stat = *(s16 *)stat_address;                                        \
    destination->value = ScaleByPercent(current, stat + 100);            \
} while (0)

void RemoveBattleEffect(u32 side_arg, u32 unit_slot_arg, u32 effect_slot_arg)
{
    register u32 side asm("r5") = (u8)side_arg;
    register u32 unit_slot asm("r4") = (u8)unit_slot_arg;
    register u32 effect_slot asm("r2") = (u8)effect_slot_arg;
    register struct BattleEffect *source asm("r8");
    register u32 removed_effect_slot asm("r6");
    struct BattleEffect *destination;
    register u8 *base asm("ip");
    register u8 *source_base asm("r3");
    register u32 display_unit_offset asm("r3");
    register u32 slot_twice asm("r2");
    register u32 offset asm("r0");
    register u32 source_kind asm("r1");

    {
        register u32 side_offset asm("r1") = side * 0x1380;
        register u32 slot_offset asm("r0") = unit_slot * 0x270;
        register u32 item_offset asm("r0");
        register struct BattleEffect *source_init asm("r1");

        source_base = gBattleState;
        slot_offset += (u32)source_base;
        side_offset += slot_offset;
        item_offset = effect_slot * 12 + 0xE4;
        side_offset += item_offset;
        source_init = (struct BattleEffect *)side_offset;
        source = source_init;
        source_kind = source_init->kind_flags;
    }
    {
        register u32 masked_kind asm("r0") = BATTLE_EFFECT_KIND_MASK;
        masked_kind &= source_kind;
        base = source_base;
        if (masked_kind == 0x1A) {
            goto done;
        }
    }
    {
        removed_effect_slot = 0;
        display_unit_offset = side * 0x1218;
        slot_twice = unit_slot << 1;
        {
            register u32 bank_slot asm("r0") = slot_twice + unit_slot;
            register u8 *bank_base asm("r1");

            bank_slot <<= 6;
            bank_slot += unit_slot;
            bank_slot <<= 2;
            bank_base = base + 0x7C28;
            bank_slot += (u32)bank_base;
            display_unit_offset += bank_slot;
        }
        offset = 0x184;
        goto scan_test;

scan_advance:
        {
            register u32 next asm("r0") = removed_effect_slot + 1;
            next <<= 24;
            removed_effect_slot = next >> 24;
        }
        if (removed_effect_slot > 31) {
            goto done;
        }
        offset = removed_effect_slot * 12 + 0x184;

scan_test:
        destination = (struct BattleEffect *)(display_unit_offset + offset);
        if (destination->kind_flags != 0) {
            goto scan_advance;
        }

        if (removed_effect_slot <= 31) {
            u16 *active;
            register u32 active_slot asm("r1") = slot_twice + unit_slot;
            register u32 active_side_offset asm("r0");
            register u32 active_bits asm("r2");
            register u32 marked asm("r0");

            active_slot <<= 6;
            active_slot += unit_slot;
            active_slot <<= 2;
            active_side_offset = side * 0x1218;
            active_slot += active_side_offset;
            active_slot += (u32)base;
            active = (u16 *)(active_slot + 0x7C28);
            active_bits = *active;
            marked = 1;
            marked |= active_bits;
            *active = marked;
            *destination = *source;

            if (FindBattleEffect(side, unit_slot, BATTLE_EFFECT_PILOT_INACTIVE) == BATTLE_EFFECT_NOT_FOUND) {
                switch (destination->kind_flags & BATTLE_EFFECT_KIND_MASK) {
                case 2:
                    APPLY_ADJUST(0x270, 0x1380, 0xA4);
                    break;
                case 8:
                    APPLY_ADJUST(0x270, 0x1380, 0xA6);
                    break;
                case 11:
                    APPLY_ADJUST(0x270, 0x1380, 0xA8);
                    break;
                case 16:
                    APPLY_ADJUST(0x270, 0x1380, 0xAA);
                    break;
                case 3:
                    APPLY_ADJUST(0x270, 0x1380, 0xAC);
                    break;
                }
            }

            if (FindBattleEffect(side, unit_slot, 0x17) != BATTLE_EFFECT_NOT_FOUND) {
                switch (destination->kind_flags & BATTLE_EFFECT_KIND_MASK) {
                case 15:
                    APPLY_ADJUST(0x270, 0x1380, 0xDC);
                    break;
                case 11:
                    APPLY_ADJUST(0x270, 0x1380, 0xDE);
                    break;
                case 7:
                    APPLY_ADJUST(0x270, 0x1380, 0xE0);
                    break;
                case 9:
                    APPLY_ADJUST(0x270, 0x1380, 0xE2);
                    break;
                }
            }
        }
    }

done:
    {
        register u16 result asm("r0") = 0;
        register struct BattleEffect *clear_source asm("r6") = source;

        asm volatile("" : : "r"(result));
        clear_source->kind_flags = result;
    }
}
