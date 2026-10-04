#include "m2c_prelude.h"
#include "battle.h"

u8 RemoveBattleActionCandidate(u32, u32) asm("func_080CA570");

void KeepBattleCandidatesWithMaxHitRateOrValue(void) asm("func_080CAB00");

void KeepBattleCandidatesWithMaxHitRateOrValue(void)
{
    register u32 choice_index asm("r8");
    register u32 candidate_index asm("r9");
    register s32 grid_offset asm("sl");
    register s32 second_index asm("ip");
    register u32 has_maximum asm("r6");
    register u32 target_side asm("r5");
    register u32 target_unit_index asm("r4");
    u32 next_target_side;
    volatile s32 maximum_value;
    volatile u32 candidate_count;
    volatile s32 record_product;
    volatile s32 row_eight;
    volatile s32 outer_six;
    u8 *volatile equipment_slot_ptr;
    volatile s32 signed_maximum_value;
    volatile u32 next_candidate_index;
    volatile u32 next_choice_index;
    volatile u32 sub_eight;
    u8 *target_preview;

    maximum_value = 0;
    candidate_index = 0;
    {
        register u32 initial_count_r1 asm("r1");
        register u32 outer_bound_r6 asm("r6");
        register u32 outer_load_r2 asm("r2");
        initial_count_r1 = ((struct BattleActionCandidates *)0x0203EF70)->equipment_count;
        if (candidate_index < initial_count_r1) {
            candidate_count = initial_count_r1;
            do {
                register u32 zero_r1 asm("r1");
                register u32 next_outer_r2 asm("r2");
                register u8 *count_addr_r0 asm("r0");
                zero_r1 = 0;
                asm volatile("" : "+r"(zero_r1));
                choice_index = zero_r1;
                count_addr_r0 = (u8 *)(0x0203EFA9 + candidate_index);
                next_outer_r2 = candidate_index + 1;
                next_candidate_index = next_outer_r2;
                if (choice_index < *count_addr_r0) {
                    {
                        register s32 work_r0 asm("r0");
                        register s32 calc_r1 asm("r1");
                        register u32 record_id_r2 asm("r2");
                        register u32 outer_copy_r6 asm("r6");
                        outer_copy_r6 = candidate_index;
                        asm volatile("" : "+r"(outer_copy_r6));
                        calc_r1 = outer_copy_r6 << 1;
                        work_r0 = 0x0203EF70 + candidate_index;
                        record_id_r2 = *(u8 *)work_r0;
                        work_r0 = 0xA8C;
                        calc_r1 += candidate_index;
                        calc_r1 <<= 1;
                        outer_six = calc_r1;
                        calc_r1 = record_id_r2;
                        calc_r1 *= work_r0;
                        record_product = calc_r1;
                    }
                    {
                        register u32 row_r2 asm("r2");
                        register u32 row asm("ip");
                        row_r2 = 0x0203ECFB;
                        asm volatile("" : "+r"(row_r2));
                        row_r2 = *(u8 *)row_r2;
                        row = row_r2;
                        row_r2 <<= 3;
                        row_eight = row_r2;
                    }
                    do {
                        target_side = 0;
                        {
                            register u32 mid_next_r6 asm("r6");
                            mid_next_r6 = choice_index + 1;
                            asm volatile("" : "+r"(mid_next_r6));
                            next_choice_index = mid_next_r6;
                        }
                        {
                            register s32 acc_r0 asm("r0");
                            register s32 row8_r1 asm("r1");
                            register u32 row_r2 asm("r2");
                            register u32 row asm("ip");
                            row8_r1 = row_eight;
                            row_r2 = row;
                            acc_r0 = row8_r1 - row_r2;
                            acc_r0 <<= 5;
                            acc_r0 += row;
                            acc_r0 <<= 2;
                            grid_offset = acc_r0;
                        }
                        do {
                            target_unit_index = 0;
                            {
                                register u32 sub_eight_r6 asm("r6");
                                sub_eight_r6 = target_side * 8;
                                asm volatile("" : "+r"(sub_eight_r6));
                                sub_eight = sub_eight_r6;
                            }
                            next_target_side = target_side + 1;
                            {
                                register u32 addr_r3 asm("r3");
                                register u32 base_r0 asm("r0");
                                register u32 record_r1 asm("r1");
                                base_r0 = 0x02037314;
                                base_r0 += grid_offset;
                                record_r1 = record_product;
                                addr_r3 = record_r1 + base_r0;
                            do {
                                register u32 base_r0 asm("r0");
                                register u32 record_r1 asm("r1");
                                register u32 cell_r2 asm("r2");
                                base_r0 = outer_six + choice_index;
                                cell_r2 = 0x0203EF78;
                                asm volatile("" : "+r"(cell_r2));
                                base_r0 += cell_r2;
                                record_r1 = *(u8 *)base_r0;
                                base_r0 = 0x94;
                                record_r1 *= base_r0;
                                record_r1 += 12;
                                record_r1 = addr_r3 + record_r1;
                                {
                                    register u32 sub8_r6 asm("r6");
                                    sub8_r6 = sub_eight;
                                    asm volatile("" : "+r"(sub8_r6));
                                    base_r0 = sub8_r6 + target_side;
                                }
                                base_r0 <<= 3;
                                base_r0 += 4;
                                record_r1 += base_r0;
                                base_r0 = target_unit_index << 1;
                                base_r0 += target_unit_index;
                                base_r0 <<= 2;
                                cell_r2 = record_r1 + base_r0;
                                target_preview = (u8 *)cell_r2;
                                if ((BATTLE_PREVIEW_FIELD(target_preview, u16, flags) & 1) != 0) {
                                    register s32 max_load_r0 asm("r0");
                                    register s32 max_r1 asm("r1");
                                    register s32 offset_r6 asm("r6");
                                    max_load_r0 = maximum_value;
                                    max_r1 = max_load_r0 << 16;
                                    max_r1 >>= 16;
                                    offset_r6 = (s32)&((struct BattleTargetPreview *)0)->hit_rate_or_value;
                                    max_load_r0 = *(s16 *)(target_preview + offset_r6);
                                    if (max_r1 < max_load_r0) {
                                        register u32 update_r2 asm("r2");
                                        update_r2 = BATTLE_PREVIEW_FIELD(target_preview, u16, hit_rate_or_value);
                                        maximum_value = update_r2;
                                    }
                                }
                                {
                                    register u32 next_inner_r0 asm("r0");
                                    next_inner_r0 = target_unit_index + 1;
                                    next_inner_r0 <<= 24;
                                    target_unit_index = next_inner_r0 >> 24;
                                }
                            } while (target_unit_index <= 5);
                            }
                            {
                                register u32 next_sub_r0 asm("r0");
                                next_sub_r0 = next_target_side << 24;
                                target_side = next_sub_r0 >> 24;
                            }
                        } while (target_side <= 1);
                        {
                            register u32 next_mid_r0 asm("r0");
                            register u32 mid_load_r1 asm("r1");
                            mid_load_r1 = next_choice_index;
                            next_mid_r0 = mid_load_r1 << 24;
                            choice_index = next_mid_r0 >> 24;
                        }
                    } while (choice_index < *(u8 *)(0x0203EFA9 + candidate_index));
                }
                {
                    register u32 next_outer_r0 asm("r0");
                    outer_load_r2 = next_candidate_index;
                    next_outer_r0 = outer_load_r2 << 24;
                    candidate_index = next_outer_r0 >> 24;
                }
                outer_bound_r6 = candidate_count;
                asm volatile("" : "+r"(outer_bound_r6));
            } while (candidate_index < outer_bound_r6);
        }
    }

    {
        register u8 *initial_count_addr_r1 asm("r1");
        register u32 initial_count_r1 asm("r1");
        candidate_index = 0;
        initial_count_addr_r1 = &((struct BattleActionCandidates *)0x0203EF70)->equipment_count;
        asm volatile("" : "+r"(initial_count_addr_r1));
        initial_count_r1 = *initial_count_addr_r1;
        if (candidate_index < initial_count_r1) {
            do {
                register u32 zero_r2 asm("r2");
                register u8 *mid_count_base_r0 asm("r0");
                register u32 outer_copy_r6 asm("r6");
                register u8 *mid_count_ptr_r1 asm("r1");
                register u32 mid_count_r1 asm("r1");
                zero_r2 = 0;
                asm volatile("" : "+r"(zero_r2));
                choice_index = zero_r2;
                mid_count_base_r0 = ((struct BattleActionCandidates *)0x0203EF70)->target_choice_counts;
                asm volatile("" : "+r"(mid_count_base_r0));
                outer_copy_r6 = candidate_index;
                asm volatile("" : "+r"(outer_copy_r6));
                mid_count_ptr_r1 = (u8 *)(outer_copy_r6 + (u32)mid_count_base_r0);
                asm volatile("" : "+r"(mid_count_ptr_r1));
                mid_count_r1 = *mid_count_ptr_r1;
                if (choice_index < mid_count_r1) {
                    {
                        register s32 max_r1 asm("r1");
                        register s32 signed_r0 asm("r0");
                        max_r1 = maximum_value;
                        signed_r0 = max_r1 << 16;
                        signed_r0 >>= 16;
                        signed_maximum_value = signed_r0;
                    }
second_mid:
                    has_maximum = 0;
                    target_side = 0;
                    {
                        register u32 outer_r2 asm("r2");
                        register u32 index_r0 asm("r0");
                        register u8 *record_r0 asm("r0");
                        outer_r2 = candidate_index;
                        asm volatile("" : "+r"(outer_r2));
                        index_r0 = outer_r2 << 1;
                        index_r0 += candidate_index;
                        index_r0 <<= 1;
                        index_r0 += choice_index;
                        second_index = index_r0;
                        record_r0 = ((struct BattleActionCandidates *)0x0203EF70)->equipment_slots;
                        record_r0 += candidate_index;
                        equipment_slot_ptr = record_r0;
                    }
second_sub:
                    target_unit_index = 0;
                    next_target_side = target_side + 1;
                    if (has_maximum != 0) {
                        goto second_sub_next;
                    }
                    {
                        register s32 product_r3 asm("r3");
                        {
                            register u8 *record_r2 asm("r2");
                            register u32 record_id_r1 asm("r1");
                            register u32 stride_r0 asm("r0");
                            record_r2 = equipment_slot_ptr;
                            record_id_r1 = *record_r2;
                            asm volatile("" : "+r"(record_id_r1));
                            stride_r0 = 0xA8C;
                            product_r3 = record_id_r1;
                            product_r3 *= stride_r0;
                        }
                        {
                            register s32 base_r2 asm("r2");
                            register u8 *grid_r0 asm("r0");
                            register u8 *row_addr_r0 asm("r0");
                            register u32 action_index_r1 asm("r1");
                            register s32 row_calc_r0 asm("r0");
                            base_r2 = 0x02037314;
                            grid_r0 = ((struct BattleActionCandidates *)0x0203EF70)->target_choices[0];
                            asm volatile("" : "+r"(grid_r0));
                            grid_offset = (s32)grid_r0;
                            row_addr_r0 = (u8 *)0x0203ECFB;
                            action_index_r1 = *row_addr_r0;
                            row_calc_r0 = action_index_r1 << 3;
                            row_calc_r0 -= action_index_r1;
                            row_calc_r0 <<= 5;
                            row_calc_r0 += action_index_r1;
                            row_calc_r0 <<= 2;
                            row_calc_r0 += base_r2;
                            product_r3 += row_calc_r0;
                        }
second_inner:
                        asm volatile("" : "+r"(target_side));
                        {
                            register u32 index_r0 asm("r0");
                            register u32 type_r1 asm("r1");
                            register u32 factor_r0 asm("r0");
                            register u32 address_r1 asm("r1");
                            register u32 cell_r2 asm("r2");
                            index_r0 = second_index;
                            index_r0 += grid_offset;
                            type_r1 = *(u8 *)index_r0;
                            factor_r0 = 0x94;
                            type_r1 *= factor_r0;
                            type_r1 += 12;
                            address_r1 = product_r3 + type_r1;
                            index_r0 = target_side << 3;
                            index_r0 += target_side;
                            index_r0 <<= 3;
                            index_r0 += 4;
                            address_r1 += index_r0;
                            index_r0 = target_unit_index << 1;
                            index_r0 += target_unit_index;
                            index_r0 <<= 2;
                            cell_r2 = address_r1 + index_r0;
                            target_preview = (u8 *)cell_r2;
                            if ((BATTLE_PREVIEW_FIELD(target_preview, u16, flags) & 1) != 0) {
                                register s32 offset_r1 asm("r1");
                                register s32 value_r0 asm("r0");
                                register s32 maximum_r2 asm("r2");
                                offset_r1 = (s32)&((struct BattleTargetPreview *)0)->hit_rate_or_value;
                                value_r0 = *(s16 *)(target_preview + offset_r1);
                                maximum_r2 = signed_maximum_value;
                                if (maximum_r2 == value_r0) {
                                    has_maximum = 1;
                                }
                            }
                        }
                        {
                            register u32 next_inner_r0 asm("r0");
                            next_inner_r0 = target_unit_index + 1;
                            asm volatile("" : "+r"(next_inner_r0));
                            next_inner_r0 <<= 24;
                            target_unit_index = next_inner_r0 >> 24;
                        }
                        if (target_unit_index <= 5 && has_maximum == 0) {
                            goto second_inner;
                        }
                    }
second_sub_next:
                    {
                        register u32 next_sub_r0 asm("r0");
                        next_sub_r0 = next_target_side << 24;
                        target_side = next_sub_r0 >> 24;
                    }
                    if (target_side <= 1 && has_maximum == 0) {
                        goto second_sub;
                    }
                    if (has_maximum == 0) {
                        if (RemoveBattleActionCandidate(candidate_index, choice_index) == 0) {
                            candidate_index = (u8)(candidate_index - 1);
                            goto second_outer_next;
                        } else {
                            choice_index = (u8)(choice_index - 1);
                        }
                    }
                    choice_index = (u8)(choice_index + 1);
                    if (choice_index < *(u8 *)(0x0203EFA9 + candidate_index)) {
                        goto second_mid;
                    }
                }
second_outer_next:
                candidate_index = (u8)(candidate_index + 1);
            } while (candidate_index < ((struct BattleActionCandidates *)0x0203EF70)->equipment_count);
        }
    }
}
