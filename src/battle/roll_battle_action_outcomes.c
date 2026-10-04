#include "m2c_prelude.h"
#include "battle.h"

s32 RollBattleStatusResistance(s32, s32, s32) asm("func_080BE488");                   /* extern */
u8 FindBattleEffect(u8, u8, s32) asm("func_080BF464");                      /* extern */
s32 FindAbilityValue(s32, s32, s32) asm("func_080E74F0");                   /* extern */
s32 FindActiveBattleAuxiliaryPilotEffectValue(s32, s32, s32) asm("func_080E7AE0");                   /* extern */
u32 CallFunctionR0(s32) asm("func_080ECD5C");                             /* extern */
extern u8 gBattleState[];
extern u8 D_000027BE[];

void RollBattleActionOutcomes(void) asm("func_080E9998");

void RollBattleActionOutcomes(void) {
    volatile u32 action_index;
    u8 *sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 var_r0_2;
    s32 var_r2;
    s32 temp_r1;
    register s32 temp_r5 asm("r5");
    register s32 var_r0 asm("r0");
    register u32 or_value_r1 asm("r1");
    u32 temp_r0;
    u8 *temp_r4;
    u8 temp_r0_2;
    register u32 target_unit_slot asm("r8");
    register u32 target_side asm("sl");
    register void *temp_r2 asm("r9");
    register u8 *temp_r6 asm("r6");
    register u8 *type_ptr_r1 asm("r1");

    action_index = 0;
    asm volatile("" : "=m"(sp4), "=m"(sp8), "=m"(spC), "=m"(sp10),
        "=m"(sp14), "=m"(sp18), "=m"(sp1C), "=m"(sp20));
    {
        register u8 *base_r0 asm("r0") = gBattleState;
        register u32 offset_r2 asm("r2") = 0xA1AF;
        register u8 *limit_r1 asm("r1");
        register u32 outer_r3 asm("r3");
        register u32 limit_r4 asm("r4");

        limit_r1 = base_r0 + offset_r2;
        outer_r3 = action_index;
        limit_r4 = *limit_r1;
        asm volatile("" : "+r"(limit_r4));
        if (outer_r3 >= limit_r4) {
            return;
        }
    }
loop_2:
    {
        register u32 zero_r5 asm("r5") = 0;
        asm volatile("" : "+r"(zero_r5));
        target_side = zero_r5;
    }
    {
        register u32 outer_r6 asm("r6") = action_index;
        sp1C = outer_r6 * 8;
    }
    sp14 = action_index * 2;
    {
        register u32 outer_r1 asm("r1") = action_index;
        spC = outer_r1 + 1;
    }
    {
        register u32 address_r2 asm("r2") = action_index;
        register u32 base_r3 asm("r3") = 0x0203ECFC;
        address_r2 += base_r3;
        sp4 = (u8 *)address_r2;
    }
loop_3:
    {
        register u32 zero_r4 asm("r4") = 0;
        asm volatile("" : "+r"(zero_r4));
        target_unit_slot = zero_r4;
    }
    {
        register u32 inner_r5 asm("r5") = target_side;
        asm volatile("" : "+r"(inner_r5));
        sp20 = inner_r5 * 8;
    }
    {
        register u32 inner_r6 asm("r6") = target_side;
        asm volatile("" : "+r"(inner_r6));
        sp18 = inner_r6 * 2;
    }
    sp10 = target_side + 1;
    {
        register u32 inner_r1 asm("r1") = target_side;
        register u32 stride_r0 asm("r0");

        stride_r0 = inner_r1 << 2;
        stride_r0 += target_side;
        stride_r0 <<= 3;
        stride_r0 -= inner_r1;
        stride_r0 <<= 7;
        sp8 = stride_r0;
    }
loop_4:
    {
        register u8 *row_r2 asm("r2") = sp4;
        register u32 row_r1 asm("r1");

        row_r1 = *row_r2;
        temp_r5 = row_r1 * 0xA8C;
    }
    {
        register s32 phase_r3 asm("r3");
        s32 phase_r7;
        register u32 address_r2 asm("r2");
        register u32 work_r0 asm("r0");
        register u32 work_r1 asm("r1");
        register u8 *output_r4 asm("r4");

        {
            register u32 outer_r4 asm("r4");

            phase_r3 = sp1C;
            outer_r4 = action_index;
            work_r0 = phase_r3 - outer_r4;
            work_r0 <<= 5;
            work_r0 += outer_r4;
            phase_r3 = work_r0 << 2;
            temp_r6 = gBattleState;
            work_r0 = 0x27C8;
            address_r2 = (u32)temp_r6 + work_r0;
            address_r2 = phase_r3 + address_r2;
            address_r2 = temp_r5 + address_r2;

            work_r1 = 0xA1B3;
            work_r0 = (u32)temp_r6 + work_r1;
            work_r0 = outer_r4 - (0U - work_r0);
            work_r1 = *(u8 *)work_r0;
        }
        work_r0 = 0x94;
        phase_r7 = work_r1;
        phase_r7 *= work_r0;
        work_r0 = phase_r7;
        work_r0 += 0xC;
        address_r2 += work_r0;

        work_r0 = sp20 + target_side;
        work_r0 <<= 3;
        work_r0 += 4;
        address_r2 += work_r0;
        output_r4 = (u8 *)target_unit_slot;
        asm volatile("" : "+r"(output_r4));
        work_r0 = (u32)output_r4 << 1;
        work_r0 += target_unit_slot;
        work_r0 <<= 2;
        address_r2 += work_r0;
        temp_r2 = (void *)address_r2;

        temp_r6 = (u8 *)sp14;
        work_r0 = action_index;
        address_r2 = (u32)temp_r6 + work_r0;
        address_r2 <<= 2;
        work_r0 = sp18 + target_side;
        work_r0 <<= 1;
        output_r4 = gBattleState;
        temp_r6 = (u8 *)0xA1B6;
        work_r1 = (u32)output_r4 + (u32)temp_r6;
        work_r0 += work_r1;
        address_r2 += work_r0;
        work_r0 = target_unit_slot;
        output_r4 = (u8 *)(address_r2 + work_r0);
        temp_r4 = output_r4;

        work_r0 = phase_r3 + temp_r5;
        work_r1 = (u32)gBattleState;
        temp_r6 = (u8 *)(work_r0 + work_r1);
        if (M2C_FIELD(temp_r6, u8 *, 0x27D2) == 0) {
            goto block_zero;
        }
        work_r0 = phase_r7 + phase_r3;
        work_r0 += temp_r5;
        work_r0 += work_r1;
        if (M2C_FIELD(work_r0, u8 *, 0x27D4) == 0) {
            goto block_zero;
        }
    }
    {
        register u32 one_r3 asm("r3");

        {
            register u8 *flags_r5 asm("r5") = temp_r2;
            register u32 flags_r1 asm("r1");

            flags_r1 = *(u16 *)flags_r5;
            one_r3 = 1;
            if (!(BATTLE_TARGET_VALID & flags_r1)) {
                goto block_zero;
            }
        }
        if (1 & ({
                register u32 flag_r1 asm("r1") = 0x27C8;
                register u32 flag_address_r0 asm("r0");

                asm volatile("" : "+r"(flag_r1));
                flag_address_r0 = (u32)temp_r6 + flag_r1;
                flag_r1 = *(u16 *)flag_address_r0;
                flag_r1;
            })) {
            goto block_set3;
        }
        {
        register u8 *base_r2 asm("r2") = gBattleState;
        register u32 type_offset_r5 asm("r5") = 0x27BE;
        register u32 type_value_r0 asm("r0");

        asm volatile("" : "+r"(base_r2), "+r"(type_offset_r5));
        type_value_r0 = (u32)base_r2 + type_offset_r5;
        type_value_r0 += target_side;
        type_value_r0 = *(u8 *)type_value_r0;
        if ((type_value_r0 == 0x21) && (({
                register u32 index_r6 asm("r6") = target_unit_slot;
                register u32 address_r0 asm("r0");
                register u32 row_r1 asm("r1");

                address_r0 = index_r6 << 2;
                address_r0 += target_unit_slot;
                address_r0 <<= 3;
                address_r0 -= index_r6;
                address_r0 <<= 4;
                row_r1 = sp8;
                address_r0 += row_r1;
                address_r0 += (u32)base_r2;
                address_r0 += 0x70;
                address_r0 = *(u8 *)address_r0;
                address_r0;
            }) != 1)) {
            *temp_r4 = one_r3;
            goto block_random;
        }
        }
    }
    temp_r0 = CallFunctionR0(*(s32 *)0x03000010) * 0x64;
    temp_r0 *= 2;
    temp_r6 = (u8 *) (temp_r0 >> 0x10);
    temp_r1 = (s32) temp_r0 >> 0x10;
    if (temp_r1 == 0) {
        goto block_set1;
    }
    if (temp_r1 > (s32) ({
            register u8 *entry_r2 asm("r2") = temp_r2;
            register u32 offset_r3 asm("r3");
            register s32 value_r0 asm("r0");
            asm volatile(
                "mov %1, #2\n\t"
                "ldrsh %0, [%2, %1]"
                : "=r"(value_r0), "=r"(offset_r3)
                : "r"(entry_r2));
            value_r0;
        })) {
        goto block_set1;
    }
    if (((FindActiveBattleAuxiliaryPilotEffectValue(target_side, target_unit_slot, 0x15) << 0x10) != 0) &&
        ((s32) ({
            register u32 index_r1 asm("r1") = target_unit_slot;
            register u32 address_r0 asm("r0");
            register u32 row_r2 asm("r2");
            register u8 *base_r3 asm("r3");

            address_r0 = index_r1 << 2;
            address_r0 += target_unit_slot;
            address_r0 <<= 3;
            address_r0 -= index_r1;
            address_r0 <<= 4;
            row_r2 = sp8;
            address_r0 += row_r2;
            base_r3 = gBattleState;
            address_r0 += (u32)base_r3;
            index_r1 = 8;
            *(s16 *)(address_r0 + index_r1);
        }) > 1) && ((CallFunctionR0(*(s32 *)0x03000010) >> 0xE) == 0)) {
        goto block_set2;
    }
    {
        register u8 *base_r2 asm("r2") = gBattleState;
        {
            register u32 type_offset_r3 asm("r3") = 0x27BE;
            register u8 *type_base_r0 asm("r0");
            register u32 type_index_r5 asm("r5");
            type_base_r0 = base_r2 + type_offset_r3;
            type_index_r5 = target_side;
            asm volatile("add %0, %1, %2"
                : "=r"(type_ptr_r1)
                : "r"(type_index_r5), "r"(type_base_r0));
            if ((*type_ptr_r1 == 0x20) &&
                (({
                    register u32 address_r0 asm("r0") = 0x94;
                    register u32 index_r3 asm("r3") = target_side;
                    register u32 offset_r5 asm("r5");

                    index_r3 *= address_r0;
                    address_r0 = index_r3;
                    address_r0 += (u32)base_r2;
                    offset_r5 = 0xA084;
                    asm volatile("" : "+r"(offset_r5));
                    address_r0 += offset_r5;
                    address_r0 = *(u8 *)address_r0;
                    address_r0;
                }) == target_unit_slot)) {
                goto block_set5;
            }
        }
        {
            register s32 compare_r1 asm("r1");
            register u32 offset_r3 asm("r3");
            register s32 entry_value_r0 asm("r0");

            compare_r1 = (u32)temp_r6 << 16;
            compare_r1 >>= 15;
            temp_r6 = (u8 *)temp_r2;
            asm volatile(
                "mov %1, #6\n\t"
                "ldrsh %0, [%2, %1]"
                : "=r"(entry_value_r0), "=r"(offset_r3)
                : "r"(temp_r6));
            if (compare_r1 > entry_value_r0) {
                *temp_r4 = BATTLE_OUTCOME_HIT;
                {
                    register u32 index_r5 asm("r5") = target_unit_slot;
                    register u32 address_r0 asm("r0");
                    register u32 row_r6 asm("r6");
                    register u32 offset_r1 asm("r1");
                    register s32 field_r2 asm("r2");

                    address_r0 = index_r5 << 2;
                    address_r0 += target_unit_slot;
                    address_r0 <<= 3;
                    address_r0 -= index_r5;
                    address_r0 <<= 4;
                    row_r6 = sp8;
                    address_r0 += row_r6;
                    address_r0 += (u32)base_r2;
                    asm volatile(
                        "mov %1, #60\n\t"
                        "ldrsh %0, [%2, %1]"
                        : "=r"(field_r2), "=r"(offset_r1)
                        : "r"(address_r0));
                    var_r2 = field_r2;
                }
                {
                    register u8 *entry_r3 asm("r3") = temp_r2;
                    register u32 offset_r5 asm("r5");
                    register s32 field_r0 asm("r0");

                    asm volatile(
                        "mov %1, #4\n\t"
                        "ldrsh %0, [%2, %1]"
                        : "=r"(field_r0), "=r"(offset_r5)
                        : "r"(entry_r3));
                    var_r0_2 = field_r0;
                }
            } else {
                *temp_r4 = BATTLE_OUTCOME_DIRECT_HIT;
                {
                    register u32 address_r0 asm("r0");
                    register s32 field_r2 asm("r2");

                    temp_r6 = (u8 *)target_unit_slot;
                    address_r0 = (u32)temp_r6 << 2;
                    address_r0 += target_unit_slot;
                    address_r0 <<= 3;
                    address_r0 -= (u32)temp_r6;
                    address_r0 <<= 4;
                    compare_r1 = sp8;
                    address_r0 += compare_r1;
                    address_r0 += (u32)base_r2;
                    asm volatile(
                        "mov %1, #60\n\t"
                        "ldrsh %0, [%2, %1]"
                        : "=r"(field_r2), "=r"(offset_r3)
                        : "r"(address_r0));
                    var_r2 = field_r2;
                }
                {
                    register u8 *entry_r5 asm("r5") = temp_r2;
                    register s32 field_r0 asm("r0");

                    asm volatile(
                        "mov %1, #8\n\t"
                        "ldrsh %0, [%2, %1]"
                        : "=r"(field_r0), "=r"(temp_r6)
                        : "r"(entry_r5));
                    var_r0_2 = field_r0;
                }
            }
        }
        if (RollBattleStatusResistance(target_side, target_unit_slot, var_r2 - var_r0_2) != 0) {
            register u32 value_r1 asm("r1") = *temp_r4;
            register u32 mask_r0 asm("r0") = BATTLE_OUTCOME_FREEZE_RESISTED;

            value_r1 |= mask_r0;
            *temp_r4 = value_r1;
        }
    }
    if (((FindAbilityValue(({
            register u32 call_address_r0 asm("r0");
            register u8 *call_base_r1 asm("r1") = gBattleState;
            register u32 call_offset_r2 asm("r2") =
                (s32)D_000027BE - 0x12;

            call_address_r0 = (u32)call_base_r1 + call_offset_r2;
            call_address_r0 = *(s32 *)call_address_r0;
            call_address_r0;
        }), 0x14, ({
            register u32 call_address_r1 asm("r1");

            {
                register u32 phase_r3 asm("r3") = sp1C;
                register u32 outer_r5 asm("r5") = action_index;

                call_address_r1 = phase_r3 - outer_r5;
                call_address_r1 <<= 5;
                call_address_r1 += outer_r5;
                call_address_r1 <<= 2;
            }
            {
                register u8 *row_r6 asm("r6") = sp4;
                register u32 row_r3 asm("r3") = *row_r6;
                register u32 stride_r2 asm("r2") = 0xA8C;

                stride_r2 *= row_r3;
                call_address_r1 += stride_r2;
            }
            {
                register u32 table_r2 asm("r2") = 0x02037318;
                register s32 call_value_r2 asm("r2");

                asm volatile("" : "+r"(table_r2));
                call_address_r1 += table_r2;
                call_value_r2 = *(s32 *)call_address_r1;
                call_value_r2;
            }
        })) << 0x10) != 0) &&
        ((s32) ((u32) (CallFunctionR0(*(s32 *)0x03000010) * 0xA) >> 0xF) <= 2)) {
        register u32 value_r1 asm("r1") = *temp_r4;
        register u32 mask_r0 asm("r0") = 0x80;

        value_r1 |= mask_r0;
        *temp_r4 = value_r1;
    }
    if ((({
            register u32 call_base_r1 asm("r1") = (u32)gBattleState;
            register u32 first_offset_r3 asm("r3") =
                (s32)D_000027BE - 0x1A;
            register u32 first_value_r0 asm("r0");
            register u32 second_offset_r5 asm("r5");

            first_value_r0 = call_base_r1 + first_offset_r3;
            first_value_r0 = *(u8 *)first_value_r0;
            second_offset_r5 = (s32)D_000027BE - 0x19;
            asm volatile("" : "+r"(second_offset_r5));
            call_base_r1 += second_offset_r5;
            call_base_r1 = *(u8 *)call_base_r1;
            FindBattleEffect(first_value_r0, call_base_r1, BATTLE_EFFECT_MELEE_DEFENSE_DAMAGE_CHANCE);
        }) != 0xFF) && ({
            register u32 flag_address_r0 asm("r0");

            {
                register u32 phase_r6 asm("r6") = sp1C;
                register u32 outer_r1 asm("r1") = action_index;

                flag_address_r0 = phase_r6 - outer_r1;
                flag_address_r0 <<= 5;
                flag_address_r0 += outer_r1;
                flag_address_r0 <<= 2;
            }
            {
                register u8 *row_r3 asm("r3") = sp4;
                register u32 row_r2 asm("r2") = *row_r3;
                register u32 stride_r1 asm("r1") = 0xA8C;

                stride_r1 *= row_r2;
                flag_address_r0 += stride_r1;
            }
            {
                register u32 table_r5 asm("r5") = 0x02037318;
                register u32 flag_r1 asm("r1");
                register u32 mask_r0 asm("r0");

                asm volatile("" : "+r"(table_r5));
                flag_address_r0 += table_r5;
                flag_r1 = *(u32 *)flag_address_r0;
                mask_r0 = 0x10;
                flag_r1 &= mask_r0;
                flag_r1;
            }
        }) && ((CallFunctionR0(*(s32 *)0x03000010) >> 0xE) != 0)) {
        or_value_r1 = *temp_r4;
        var_r0 = 0x20;
        goto block_or;
    }
    goto block_random;

block_set5:
    *temp_r4 = 5;
    *type_ptr_r1 = 0U;
    goto block_random;
block_set2:
    *temp_r4 = BATTLE_OUTCOME_SHIELD;
    goto block_random;
block_set1:
    *temp_r4 = BATTLE_OUTCOME_MISS;
    goto block_random;
block_set3:
    *temp_r4 = BATTLE_OUTCOME_HIT;
    if (RollBattleStatusResistance(target_side, target_unit_slot, 0) != 0) {
        or_value_r1 = *temp_r4;
        var_r0 = BATTLE_OUTCOME_FREEZE_RESISTED;
        goto block_or;
    }
    goto block_random;
block_or:
    or_value_r1 |= var_r0;
    *temp_r4 = or_value_r1;
block_random:
    *temp_r4 |= ((u32) (CallFunctionR0(*(s32 *)0x03000010) * 3) >> 0xF) * 8;
    goto block_increment;
block_zero:
    *temp_r4 = BATTLE_OUTCOME_NONE;
block_increment:
    temp_r0_2 = target_unit_slot + 1;
    target_unit_slot = temp_r0_2;
    if ((u32) temp_r0_2 <= 5U) {
        goto loop_4;
    }
    {
        register u32 successor_r6 asm("r6") = sp10;
        register u32 narrow_r0 asm("r0");

        narrow_r0 = successor_r6 << 24;
        narrow_r0 >>= 24;
        target_side = narrow_r0;
        if (narrow_r0 <= 1U) {
            goto loop_3;
        }
    }
    {
        register u32 successor_r1 asm("r1") = spC;
        register u32 narrow_r0 asm("r0");
        register u32 limit_r2 asm("r2");

        narrow_r0 = successor_r1 << 24;
        narrow_r0 >>= 24;
        action_index = narrow_r0;
        limit_r2 = 0x0203ECFB;
        limit_r2 = *(u8 *)limit_r2;
        if (narrow_r0 < limit_r2) {
            goto loop_2;
        }
    }
}
