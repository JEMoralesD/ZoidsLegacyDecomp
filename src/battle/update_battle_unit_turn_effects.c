#include "m2c_prelude.h"
#include "battle.h"

M2C_UNK ApplyBattleRecoveryEffect(u8, u8, void *) asm("func_080BF4C0");
M2C_UNK RemoveBattleEffect(u8, u8, u8) asm("func_080BF514");
M2C_UNK RecalculateBattleUnitStats(u8, u8) asm("func_080E8B08");
s32 IsBattleUnitActive(u8, u8) asm("func_080E9D88");
extern u8 gBattleState;


void UpdateBattleUnitTurnEffects(s32 side_arg, s32 unit_slot_arg) {
    u16 kind_flags;
    u8 temp_r0;
    u8 unit_slot;
    u8 side;
    u8 effect_slot;
    struct BattleUnit *unit;
    struct BattleEffect *effect;
    register s32 side_times_four asm("r10");
    register s32 unit_times_four asm("r9");
    register u16 available_ep asm("r12");

    side_arg <<= 24;
    side = (u32)side_arg >> 24;
    unit_slot_arg <<= 24;
    unit_slot = (u32)unit_slot_arg >> 24;
    if ((IsBattleUnitActive(side, unit_slot) << 24) != 0) {
        effect_slot = 0;
        side_times_four = side << 2;
        {
            register s32 col_tmp asm("r3");

            col_tmp = unit_slot << 2;
            asm volatile("" : "+r"(col_tmp));
            unit_times_four = col_tmp;
        }
        do {
            unit = (struct BattleUnit *)((u8 *)&gBattleState
                + (((side_times_four + side) * 8 - side) << 7)
                + (((unit_times_four + unit_slot) * 8 - unit_slot) << 4));
            {
                register s32 index_tmp asm("r3");
                s32 index_offset;

                index_tmp = effect_slot;
                index_offset = ((index_tmp << 1) + effect_slot) << 2;
                index_offset += 0xE4;
                effect = (struct BattleEffect *)((u8 *)unit + index_offset);
                asm volatile("" :: "r"(index_tmp));
            }
            kind_flags = effect->kind_flags;
            if (kind_flags != 0) {
                if ((kind_flags & BATTLE_EFFECT_KIND_MASK) == BATTLE_EFFECT_TURN_MARKER) {
                    goto activate;
                }
                {
                    register s32 mask asm("r4");
                    register s32 bits asm("r0");

                    mask = BATTLE_EFFECT_LIFETIME_MASK;
                    asm volatile("" : "+r"(mask));
                    bits = mask;
                    bits &= kind_flags;
                    if (bits != BATTLE_EFFECT_TURN_EP_COST) {
                        goto after_effect;
                    }
                }
                {
                    register u16 parent_input asm("r3");

                    parent_input = *(u16 *)((u8 *)unit + 8);
                    asm volatile("" : "+r"(parent_input));
                    available_ep = parent_input;
                }
                if (*(s16 *)((u8 *)unit + 8) >=
                    (s16)*(u16 *)((u8 *)effect + 8)) {
                    goto subtract;
                }
activate:
                RemoveBattleEffect(side, unit_slot, effect_slot);
                goto after_effect;
subtract:
                *(u16 *)((u8 *)unit + 8) = available_ep -
                    *(u16 *)((u8 *)effect + 8);
after_effect:
                ApplyBattleRecoveryEffect(side, unit_slot, effect);
            }
            temp_r0 = effect_slot + 1;
            effect_slot = temp_r0;
        } while ((u32)temp_r0 <= (BATTLE_EFFECT_SLOT_COUNT - 1));
        RecalculateBattleUnitStats(side, unit_slot);
    }
}
