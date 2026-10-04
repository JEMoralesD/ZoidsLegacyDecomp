#include "m2c_prelude.h"
#include "battle_combination.h"

s32 IsBattleUnitActive(s32, s32) asm("func_080E9D88");                        /* extern */
M2C_UNK jtbl_080C0CAC();                            /* static */

s32 IsBattleCombinationFormationValid(s32 recipe_id, s32 side, s32 first_unit_slot) asm("func_080C0C54");

s32 IsBattleCombinationFormationValid(s32 recipe_id, s32 side, s32 first_unit_slot) {
    register s32 default_result asm("r0") = recipe_id;
    register u8 initial_zero asm("r2");
    u8 matched_recipe_models[6];
    volatile u32 first_unit_slot_byte;
    volatile s32 saved_group_stride;
    s32 consecutive_index;
    s32 pair_index;
    s32 rectangle_index;
    register s32 host_unbiased_index asm("r0");
    register s32 host_index asm("r1");
    s32 all_units_index;
    s32 consecutive_models_offset;
    s32 consecutive_scan_offset;
    s32 rectangle_scan_offset;
    register u32 pair_stride asm("r5");
    register u32 pair_stride_seed asm("r0");
    register u32 pair_zero asm("r4");
    register u32 rectangle_zero asm("r3");
    register u32 rectangle_unit_seed asm("r0");
    register u32 all_units_seed asm("r5");
    register u32 consecutive_base asm("r3");
    register u32 consecutive_table asm("r8");
    register u32 consecutive_data_base asm("r2");
    register u32 consecutive_scan_base asm("r4");
    register u32 consecutive_unit_offset asm("r1");
    register u32 consecutive_saved asm("r5");
    register u32 consecutive_arg8 asm("r5");
    register u32 rectangle_base asm("r2");
    register u32 rectangle_base_copy asm("r0");
    register u32 rectangle_scan_arg8 asm("r5");
    register u32 rectangle_table_address asm("r1");
    register u32 rectangle_value_address asm("r0");
    register u32 rectangle_table asm("r12");
    register u32 host_base asm("r4");
    register u32 host_work asm("r3");
    register u32 host_initial_table asm("r1");
    register u32 host_initial_offset asm("r2");
    register u32 host_unit_offset asm("r0");
    register u32 all_units_data_base asm("r2");
    register u32 all_units_unit_offset asm("r1");
    register u32 all_units_stride asm("r0");
    u8 *consecutive_model_match;
    u8 *rectangle_model_match;
    u8 *all_units_model_match;
    u8 next_pair_member;
    u8 next_rectangle_group;
    u8 next_all_units_group;
    register u32 consecutive_value asm("r1");
    register u32 rectangle_value asm("r0");
    register u32 host_value asm("r2");
    u8 all_units_value;
    register u8 formation_mode asm("r1");
    u8 recipe_id_byte;
    register u32 consecutive_unit_slot asm("r4");
    register u32 pair_unit_slot asm("r4");
    register u32 rectangle_unit_slot_or_offset asm("r4");
    register u32 all_units_slot asm("r4");
    u8 side_byte;
    u8 clear_model_index;
    u8 consecutive_model_index;
    u8 rectangle_model_index;
    u8 rear_model_index;
    u8 all_units_model_index;
    u32 component_index_or_recipe_table;
    register u32 group_index_or_battle_state asm("r8");

    recipe_id_byte = recipe_id;
    side_byte = side;
    first_unit_slot_byte = (u8) first_unit_slot;
    clear_model_index = 0;
    initial_zero = 0;
    do {
        *(matched_recipe_models + clear_model_index) = initial_zero;
        clear_model_index += 1;
    } while ((u32) clear_model_index <= 5U);
    formation_mode = M2C_FIELD((recipe_id_byte * sizeof(struct BattleCombinationFormationRule)), u8 *, BATTLE_COMBINATION_FORMATION_TABLE_ROM);
    switch ((u32) formation_mode) {                        /* irregular */
    case BATTLE_COMBINATION_CONSECUTIVE_COMPONENTS:
        component_index_or_recipe_table = 0;
        consecutive_index = (recipe_id_byte * sizeof(struct BattleCombinationFormationRule)) + 1;
        asm volatile("" : "+r"(consecutive_index));
        consecutive_base = BATTLE_COMBINATION_FORMATION_TABLE_ROM;
        asm volatile("" : "+r"(consecutive_base));
        if (M2C_FIELD(consecutive_index, volatile u8 *, consecutive_base) == 0) {
            goto accept_formation;
        }
        consecutive_table = consecutive_base;
loop_8:
        consecutive_saved = first_unit_slot_byte;
        asm volatile("" :: "r"(consecutive_saved));
        consecutive_unit_slot = component_index_or_recipe_table + consecutive_saved;
        if ((IsBattleUnitActive(side_byte, (u8) consecutive_unit_slot) << 0x18) != 0) {
            consecutive_model_index = 0;
            consecutive_arg8 = recipe_id_byte * 8;
            asm volatile("" : "+r"(consecutive_arg8));
            consecutive_index = (consecutive_arg8 - recipe_id_byte) + 1;
            asm volatile("" : "+r"(consecutive_index));
            if (M2C_FIELD(consecutive_index, u8 *, consecutive_table) != 0) {
                consecutive_data_base = 0x02034B4C;
                asm volatile("" :: "r"(consecutive_data_base));
                consecutive_unit_offset = consecutive_unit_slot * 0x270;
                asm volatile("" :: "r"(consecutive_unit_offset));
                consecutive_scan_base = BATTLE_COMBINATION_FORMATION_TABLE_ROM;
                asm volatile("" :: "r"(consecutive_scan_base));
                consecutive_unit_offset += side_byte * 0x1380;
                consecutive_value = M2C_FIELD(consecutive_unit_offset, u8 *, consecutive_data_base);
loop_11:
                {
                    register s32 consecutive_unbiased_index asm("r0");
                    register s32 consecutive_index_rhs asm("r2") = recipe_id_byte;
                    consecutive_unbiased_index = consecutive_arg8 - consecutive_index_rhs;
                    consecutive_scan_offset = consecutive_unbiased_index + 1;
                }
                asm volatile("" :: "r"(consecutive_scan_offset));
                if (consecutive_value == M2C_FIELD((consecutive_model_index + consecutive_scan_offset), u8 *, consecutive_scan_base)) {
                    consecutive_model_match = matched_recipe_models + consecutive_model_index;
                    if (*consecutive_model_match == 0) {
                        goto consecutive_store;
                    }
reject_formation:
                    return 0U;
                }
                consecutive_model_index += 1;
                if (((u32) consecutive_model_index > 5U) || (M2C_FIELD((consecutive_model_index + consecutive_scan_offset), u8 *, consecutive_scan_base) == 0)) {
                    goto block_17;
                }
                goto loop_11;
            }
block_17:
            if (consecutive_model_index != 6) {
                consecutive_models_offset = (recipe_id_byte * sizeof(struct BattleCombinationFormationRule)) + 1;
                if (({
                        register u32 consecutive_postscan_address asm("r0");
                        register u32 consecutive_postscan_table asm("r2");
                        u32 consecutive_postscan_value;
                        consecutive_postscan_address = consecutive_model_index + consecutive_models_offset;
                        consecutive_postscan_table = BATTLE_COMBINATION_FORMATION_TABLE_ROM;
                        consecutive_postscan_address += consecutive_postscan_table;
                        consecutive_postscan_value = M2C_FIELD(consecutive_postscan_address, u8 *, 0);
                        consecutive_postscan_value;
                    }) != 0) {
                    goto consecutive_increment;
                }
                goto reject_formation;
consecutive_store:
                *consecutive_model_match = 1;
                goto block_17;
consecutive_increment:
                component_index_or_recipe_table = (u8) (component_index_or_recipe_table + 1);
                if ((u32) component_index_or_recipe_table <= 2U) {
                    if (({
                            register u32 consecutive_increment_address asm("r0");
                            register u32 consecutive_increment_table asm("r3");
                            consecutive_increment_address = component_index_or_recipe_table + consecutive_models_offset;
                            consecutive_increment_table = BATTLE_COMBINATION_FORMATION_TABLE_ROM;
                            consecutive_increment_address += consecutive_increment_table;
                            M2C_FIELD(consecutive_increment_address, u8 *, 0);
                        }) != 0) {
                        goto loop_8;
                    }
                    goto accept_formation;
                }
                goto accept_formation;
            }
            goto reject_formation;
        } else {
            goto reject_formation;
        }
    case BATTLE_COMBINATION_FRONT_REAR_PAIR:
        pair_zero = 0;
        group_index_or_battle_state = pair_zero;
        component_index_or_recipe_table = BATTLE_COMBINATION_FORMATION_TABLE_ROM;
        pair_stride_seed = side_byte * 4;
        pair_stride_seed = ((pair_stride_seed + side_byte) * 8) - side_byte;
        pair_stride = pair_stride_seed * 0x80;
loop_27:
        pair_unit_slot = ({
            register u32 pair_lane_seed asm("r1");
            register u32 pair_lane_sum asm("r0");
            register u32 pair_saved_arg asm("r2");
            register u32 pair_lane_result asm("r4");
            pair_lane_seed = group_index_or_battle_state;
            pair_lane_sum = pair_lane_seed * 2;
            pair_lane_sum += group_index_or_battle_state;
            pair_saved_arg = first_unit_slot_byte;
            pair_lane_result = pair_saved_arg + pair_lane_sum;
            pair_lane_result;
        });
        if (((IsBattleUnitActive(side_byte, (u8) pair_unit_slot) << 0x18) != 0) && (M2C_FIELD(({
                register u32 pair_data_address asm("r0");
                register u32 pair_data_base asm("r3");
                pair_data_address = pair_unit_slot * 0x270;
                asm volatile("add %0, %0, %1"
                    : "+r"(pair_data_address)
                    : "r"(pair_stride));
                pair_data_base = 0x02034B4C;
                asm volatile("add %0, %0, %1"
                    : "+r"(pair_data_address)
                    : "r"(pair_data_base));
                pair_data_address;
            }), u8 *, 0) == M2C_FIELD(((pair_index = ({
                register s32 pair_index_lhs asm("r4");
                register s32 pair_index_rhs asm("r2");
                register s32 pair_index_result asm("r1");
                asm volatile(
                    "mov %0, sl\n"
                    "mov %1, r9\n"
                    "sub %2, %0, %1"
                    : "=r"(pair_index_lhs), "=r"(pair_index_rhs),
                      "=r"(pair_index_result));
                pair_index_result + 1;
            })) + group_index_or_battle_state), u8 *, component_index_or_recipe_table))) {
            next_pair_member = group_index_or_battle_state + 1;
            group_index_or_battle_state = next_pair_member;
            if ((u32) next_pair_member > 1U) {
                goto accept_formation;
            }
            goto loop_27;
        }
        goto reject_formation;
accept_formation:
        {
            register s32 success_result asm("r0");
            asm volatile("mov %0, #1" : "=r"(success_result));
            return success_result;
    }
    case BATTLE_COMBINATION_TWO_BY_TWO_COMPONENTS:
        asm volatile(
            "mov %0, #0\n"
            "mov %1, %0"
            : "=r"(rectangle_zero), "=r"(group_index_or_battle_state));
loop_32:
        component_index_or_recipe_table = 0;
        {
            register u32 rectangle_outer_copy asm("r4");
            asm volatile(
                "mov %0, r8\n"
                "lsl %0, %0, #1"
                : "=r"(rectangle_outer_copy));
            rectangle_unit_slot_or_offset = rectangle_outer_copy;
            saved_group_stride = rectangle_outer_copy;
        }
loop_33:
        rectangle_unit_slot_or_offset = ({
            register u32 rectangle_lane_sum asm("r0") = saved_group_stride;
            register u32 rectangle_saved_arg asm("r5");
            register u32 rectangle_lane_result asm("r4");
            rectangle_lane_sum += group_index_or_battle_state;
            rectangle_lane_sum = component_index_or_recipe_table + rectangle_lane_sum;
            rectangle_saved_arg = first_unit_slot_byte;
            rectangle_lane_result = rectangle_saved_arg + rectangle_lane_sum;
            rectangle_lane_result;
        });
        if ((IsBattleUnitActive(side_byte, (u8) rectangle_unit_slot_or_offset) << 0x18) != 0) {
            rectangle_model_index = 0;
            {
                register u32 rectangle_initial_seed asm("r5");
                register s32 rectangle_index_rhs asm("r1");
                register s32 rectangle_index_result asm("r0");
                rectangle_initial_seed = recipe_id_byte * 8;
                asm volatile("" : "+r"(rectangle_initial_seed));
                asm volatile(
                    "mov %0, r9\n"
                    "sub %1, r5, %0"
                    : "=r"(rectangle_index_rhs), "=r"(rectangle_index_result));
                rectangle_index = rectangle_index_result + 1;
                asm volatile("" :: "r"(rectangle_initial_seed), "r"(rectangle_index));
            }
            rectangle_base = BATTLE_COMBINATION_FORMATION_TABLE_ROM;
            asm volatile("" : "+r"(rectangle_base));
            if (M2C_FIELD(rectangle_index, u8 *, rectangle_base) != 0) {
                rectangle_base_copy = rectangle_base;
                asm volatile("" : "+r"(rectangle_base_copy));
                rectangle_table = rectangle_base_copy;
                rectangle_unit_seed = rectangle_unit_slot_or_offset * 4;
                asm volatile("" : "+r"(rectangle_unit_seed));
                rectangle_unit_seed = ((rectangle_unit_seed + rectangle_unit_slot_or_offset) * 8) - rectangle_unit_slot_or_offset;
                rectangle_unit_slot_or_offset = rectangle_unit_seed * 0x10;
                asm volatile("" : "=r"(rectangle_scan_arg8));
loop_36:
                asm volatile("" : "+r"(side_byte));
                {
                    register u32 rectangle_reserve2 asm("r2");
                    asm volatile("" : "=&r"(rectangle_reserve2));
                    rectangle_value_address = (rectangle_unit_slot_or_offset + (side_byte * 0x1380)) + 0x02034B4C;
                    asm volatile("" :: "r"(rectangle_reserve2));
                }
                rectangle_scan_offset = (rectangle_scan_arg8 - recipe_id_byte) + 1;
                asm volatile("" :: "r"(rectangle_scan_arg8), "r"(rectangle_scan_offset));
                rectangle_table_address = (rectangle_model_index + rectangle_scan_offset) + rectangle_table;
                asm volatile("" :: "r"(rectangle_value_address), "r"(rectangle_table_address));
                rectangle_value = M2C_FIELD(rectangle_value_address, volatile u8 *, 0);
                if (rectangle_value == M2C_FIELD(rectangle_table_address, u8 *, 0)) {
                    rectangle_model_match = matched_recipe_models + rectangle_model_index;
                    if (*rectangle_model_match == 0) {
                        goto rectangle_store;
                    }
                    goto reject_formation;
                }
                rectangle_model_index += 1;
                if (((u32) rectangle_model_index > 5U) || (M2C_FIELD((rectangle_model_index + rectangle_scan_offset), u8 *, rectangle_table) == 0)) {
                    goto block_42;
                }
                goto loop_36;
            }
block_42:
            if (rectangle_model_index == 6) {
                goto reject_formation;
            }
            rectangle_index = (recipe_id_byte * sizeof(struct BattleCombinationFormationRule)) + 1;
            asm volatile("" : "+r"(rectangle_index));
            if (M2C_FIELD((rectangle_model_index + rectangle_index), u8 *, BATTLE_COMBINATION_FORMATION_TABLE_ROM) != 0) {
                goto rectangle_increment;
            }
            goto reject_formation;
rectangle_store:
            *rectangle_model_match = 1;
            goto block_42;
rectangle_increment:
            component_index_or_recipe_table = (u8) (component_index_or_recipe_table + 1);
            if ((u32) component_index_or_recipe_table > 1U) {
                next_rectangle_group = group_index_or_battle_state + 1;
                group_index_or_battle_state = next_rectangle_group;
                if ((u32) next_rectangle_group <= 1U) {
                    goto loop_32;
                }
                goto accept_formation;
            }
            goto loop_33;
        }
        goto reject_formation;
    case BATTLE_COMBINATION_HOST_AND_REAR_COMPONENTS:
        host_work = first_unit_slot_byte;
        asm volatile("" :: "r"(host_work));
        if (host_work <= 2U) {
            goto reject_formation;
        }
        if ((IsBattleUnitActive(side_byte, 1U) << 0x18) == 0) {
            goto reject_formation;
        }
        host_base = 0x02034B4C;
        asm volatile("" : "+r"(host_base));
        if (M2C_FIELD((({
                register u32 host_group_seed asm("r3");
                register u32 host_group_stride asm("r0");
                host_group_seed = side_byte * 4;
                host_group_stride = host_group_seed + side_byte;
                host_group_stride *= 8;
                host_group_stride -= side_byte;
                host_group_stride *= 128;
                host_group_stride += host_base;
                host_group_stride;
            }) + 0x270), u8 *, 0) != ({
                host_initial_table = BATTLE_COMBINATION_FORMATION_TABLE_ROM;
                asm volatile("" : "+r"(host_initial_table));
                host_initial_offset = recipe_id_byte * sizeof(struct BattleCombinationFormationRule);
                asm volatile("" : "+r"(host_initial_offset));
                host_initial_table += 1;
                host_initial_offset += host_initial_table;
                M2C_FIELD(host_initial_offset, u8 *, 0);
            })) {
            goto reject_formation;
        }
        component_index_or_recipe_table = first_unit_slot_byte;
        group_index_or_battle_state = host_base;
loop_57:
        if ((IsBattleUnitActive(side_byte, component_index_or_recipe_table) << 0x18) != 0) {
            rear_model_index = 1;
            host_unit_offset = component_index_or_recipe_table * 0x270;
            asm volatile("" : "+r"(host_unit_offset));
            host_unit_offset += side_byte * 0x1380;
            host_value = M2C_FIELD(host_unit_offset, u8 *, group_index_or_battle_state);
            host_unbiased_index = recipe_id_byte * sizeof(struct BattleCombinationFormationRule);
            host_index = host_unbiased_index + 1;
            asm volatile("" :: "r"(host_unbiased_index), "r"(host_index));
loop_59:
            if (host_value != ({
                register u32 host_scan_table asm("r4");
                    host_unbiased_index = rear_model_index + host_index;
                    host_scan_table = BATTLE_COMBINATION_FORMATION_TABLE_ROM;
                    asm volatile("" : "+r"(host_scan_table));
                    host_unbiased_index += host_scan_table;
                    M2C_FIELD(host_unbiased_index, u8 *, 0);
                })) {
                rear_model_index += 1;
                if ((u32) rear_model_index <= 4U) {
                    goto loop_59;
                }
            } else {
                goto host_store;
            }
host_postscan:
            if (rear_model_index != 5) {
                goto block_68;
            }
            if (component_index_or_recipe_table != first_unit_slot_byte) {
                goto block_68;
            }
            goto reject_formation;
host_store:
            asm volatile(
                "mov r2, sp\n"
                "add r1, r2, %0\n"
                "mov r0, #1\n"
                "strb r0, [r1]"
                :: "r"(rear_model_index)
                : "memory");
            goto host_postscan;
        }
        host_work = first_unit_slot_byte;
        asm volatile("" :: "r"(host_work));
        if (component_index_or_recipe_table == host_work) {
            goto reject_formation;
        }
block_68:
        component_index_or_recipe_table = (u32) (u8) (component_index_or_recipe_table - 1);
        if (component_index_or_recipe_table <= 2U) {
            if ((M2C_FIELD(matched_recipe_models, u8 *, 1) != 0) && (M2C_FIELD(matched_recipe_models, u8 *, 2) != 0)) {
                goto accept_formation;
            }
            if (M2C_FIELD(matched_recipe_models, u8 *, 3) != 0) {
                goto accept_formation;
            }
            if (M2C_FIELD(matched_recipe_models, u8 *, 4) == 0) {
                goto reject_formation;
            }
            goto accept_formation;
        }
        goto loop_57;
    case BATTLE_COMBINATION_ALL_SIX_COMPONENTS:
        all_units_slot = 0;
        asm volatile("" :: "r"(all_units_slot));
        group_index_or_battle_state = all_units_slot;
loop_78:
        component_index_or_recipe_table = 0;
        all_units_seed = group_index_or_battle_state;
        asm volatile("" : "+r"(all_units_seed));
        all_units_seed *= 2;
        saved_group_stride = all_units_seed;
loop_79:
        all_units_stride = saved_group_stride;
        asm volatile("" : "+r"(all_units_stride));
        all_units_stride += group_index_or_battle_state;
        all_units_stride = component_index_or_recipe_table + all_units_stride;
        all_units_unit_offset = first_unit_slot_byte;
        asm volatile("" : "+r"(all_units_unit_offset));
        all_units_slot = all_units_unit_offset + all_units_stride;
        if ((IsBattleUnitActive(side_byte, (u8) all_units_slot) << 0x18) == 0) {
            goto reject_formation;
        }
        all_units_model_index = 0;
        all_units_seed = side_byte * 4;
        asm volatile("" :: "r"(all_units_seed));
        all_units_data_base = 0x02034B4C;
        asm volatile("" :: "r"(all_units_data_base));
        all_units_unit_offset = all_units_slot * 0x270;
        asm volatile("" :: "r"(all_units_unit_offset));
        all_units_stride = all_units_seed + side_byte;
        asm volatile("" : "+r"(all_units_stride));
        all_units_unit_offset += ((all_units_stride * 8) - side_byte) * 0x80;
        all_units_value = M2C_FIELD(all_units_unit_offset, u8 *, all_units_data_base);
loop_82:
        if (all_units_value == ({
                register s32 all_units_index_lhs asm("r2");
                register s32 all_units_index_rhs asm("r4");
                register u32 all_units_table_address asm("r0");
                register u32 all_units_table_base asm("r5");
                asm volatile(
                    "mov %0, sl\n"
                    "mov %1, r9\n"
                    "sub %2, %0, %1"
                    : "=r"(all_units_index_lhs), "=r"(all_units_index_rhs),
                      "=r"(all_units_table_address));
                all_units_table_address += 1;
                all_units_table_address = all_units_model_index + all_units_table_address;
                all_units_table_base = BATTLE_COMBINATION_FORMATION_TABLE_ROM;
                asm volatile("add %0, %0, %1"
                    : "+r"(all_units_table_address)
                    : "r"(all_units_table_base));
                M2C_FIELD(all_units_table_address, u8 *, 0);
            })) {
            {
                register u8 *all_units_slot_address asm("r1");
                asm volatile(
                    "mov r0, sp\n"
                    "add %0, r0, %1"
                    : "=r"(all_units_slot_address)
                    : "r"(all_units_model_index));
                all_units_model_match = all_units_slot_address;
            }
            if (*all_units_model_match == 0) {
                goto all_units_store;
            }
            goto reject_formation;
        }
        all_units_model_index += 1;
        if ((u32) all_units_model_index > 5U) {
block_87:
            if (all_units_model_index != 6) {
                goto all_units_increment;
            }
            goto reject_formation;
all_units_store:
            *all_units_model_match = 1;
            goto block_87;
all_units_increment:
            component_index_or_recipe_table = (u8) (component_index_or_recipe_table + 1);
            if ((u32) component_index_or_recipe_table > 2U) {
                next_all_units_group = group_index_or_battle_state + 1;
                group_index_or_battle_state = next_all_units_group;
                if ((u32) next_all_units_group > 1U) {
                    return 1U;
                }
                goto loop_78;
            }
            goto loop_79;
        }
        goto loop_82;
    default:
        return default_result;
    }
}
