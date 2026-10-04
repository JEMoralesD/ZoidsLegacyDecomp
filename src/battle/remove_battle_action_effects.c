#include "m2c_prelude.h"
#include "battle.h"

M2C_UNK RemoveBattleEffect(u8, u8, u8) asm("func_080BF514");
M2C_UNK RecalculateBattleUnitStats(u8, u8) asm("func_080E8B08");
s32 IsBattleUnitActive(u8, u8) asm("func_080E9D88");

void RemoveBattleActionEffects(u8 side, u8 unit_slot, u8 mask_bank) {
    u8 effect_slot;
    s32 row_offset;
    s32 col_offset;
    s32 slot_offset;
    s32 next_mask_bank;
    s32 mask_bank_offset;
    s32 unit_times_four;
    u8 *effect;
    u32 *removal_mask;
    register s32 side_times_four asm("r9");
    register s32 saved_mask_bank asm("r10");
    register u8 *base asm("r8");
    register s32 kind_flags asm("r1");

    saved_mask_bank = mask_bank;
    if ((IsBattleUnitActive(side, unit_slot) << 24) != 0) {
        effect_slot = 0;
        side_times_four = side << 2;
        unit_times_four = unit_slot << 2;
        {
            register u8 *base_input asm("r1");

            base_input = (u8 *)0x02034B4C;
            asm volatile("" : "+r"(base_input));
            base = base_input;
        }
        do {
            {
                register s32 row4 asm("r2");

                row4 = side_times_four;
                row_offset = row4 + side;
                asm volatile("" :: "r"(row4));
            }
            row_offset <<= 3;
            row_offset -= side;
            row_offset <<= 7;
            col_offset = unit_times_four + unit_slot;
            col_offset <<= 3;
            col_offset -= unit_slot;
            col_offset <<= 4;
            col_offset += (s32)base;
            row_offset += col_offset;
            slot_offset = effect_slot << 1;
            slot_offset += effect_slot;
            slot_offset <<= 2;
            slot_offset += 0xE4;
            effect = (u8 *)(row_offset + slot_offset);
            kind_flags = *(u16 *)(effect + 4);
            if (kind_flags != 0) {
                register s32 mask asm("r2");
                register s32 bits asm("r0");

                mask = BATTLE_EFFECT_LIFETIME_MASK;
                asm volatile("" : "+r"(mask));
                bits = mask;
                bits &= kind_flags;
                if (bits == BATTLE_EFFECT_ACTION_MASK) {
                    kind_flags = side << 1;
                    kind_flags += side;
                    kind_flags <<= 3;
                    kind_flags = unit_times_four + kind_flags;
                    next_mask_bank = saved_mask_bank + 1;
                    mask_bank_offset = next_mask_bank << 1;
                    mask_bank_offset += next_mask_bank;
                    mask_bank_offset <<= 4;
                    kind_flags += mask_bank_offset;
                    removal_mask = (u32 *)(kind_flags + 0x0203ED28);
                    asm volatile("" :: "r"(mask_bank_offset));
                    if (*removal_mask & (1 << effect_slot)) {
                        RemoveBattleEffect(side, unit_slot, effect_slot);
                    }
                }
            }
            effect_slot += 1;
        } while ((u32)effect_slot <= (BATTLE_EFFECT_SLOT_COUNT - 1));
        RecalculateBattleUnitStats(side, unit_slot);
    }
}
