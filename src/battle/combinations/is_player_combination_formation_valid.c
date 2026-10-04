#include "m2c_prelude.h"
#include "battle_combination.h"

M2C_UNK jtbl_080C10C4();                            /* static */

s32 IsPlayerCombinationFormationValid(s32 recipe_id, s32 first_unit_slot) asm("func_080C1070");

s32 IsPlayerCombinationFormationValid(s32 recipe_id, s32 first_unit_slot) {
    register s32 default_result asm("r0") = recipe_id;
    u8 matched_recipe_models[6];
    volatile s32 saved_recipe_stride;
    u8 * volatile saved_recipe_models;
    volatile s32 saved_group_stride;
    register s32 recipe_models_offset asm("r10");
    s32 consecutive_models_offset;
    s32 consecutive_models_end_offset;
    s32 rectangle_models_offset;
    register s32 host_models_offset asm("r2");
    s32 consecutive_index;
    register u32 consecutive_pre_arg asm("r1");
    register u32 consecutive_pre_index asm("r0");
    register u32 recipe_id_times_eight asm("r9");
    register u32 recipe_id_seed asm("r5");
    register u32 recipe_stride_work asm("r2");
    register u32 consecutive_table asm("r10");
    register u32 consecutive_units asm("r4");
    register u32 consecutive_loop_arg asm("r3");
    register u32 consecutive_unit_offset asm("r0");
    register u8 *consecutive_unit_address asm("r1");
    register u32 consecutive_stride asm("r4");
    register u32 consecutive_arg asm("r5");
    register u32 consecutive_records asm("r2");
    register u32 consecutive_scan_table asm("r5");
    register u32 consecutive_value asm("r2");
    register u32 rectangle_initial_table asm("r5");
    register u32 rectangle_outer3 asm("r9");
    register u32 rectangle_units asm("r2");
    register u32 rectangle_outer_address asm("r0");
    register u8 *rectangle_unit_address asm("r1");
    register u32 rectangle_stride asm("r4");
    register u32 rectangle_records asm("r2");
    register u32 rectangle_table asm("r5");
    register u32 rectangle_value asm("r2");
    register u8 *rectangle_tail_address asm("r0");
    register u8 *rectangle_spc_reload asm("r5");
    u8 *consecutive_model_match;
    u8 *rectangle_model_match;
    u8 *all_units_model_match;
    register u32 first_unit_slot_byte asm("r8");
    u8 formation_mode;
    u8 host_zoid_record_slot;
    u8 consecutive_unit;
    u8 rectangle_unit;
    u32 counter_update;
    register u32 recipe_id_byte asm("r12");
    u8 consecutive_model_index;
    u8 rectangle_model_index;
    u8 rear_model_index;
    u8 all_units_model_index;
    u8 clear_model_index;
    register u32 clear_zero asm("r2");
    register u32 component_index asm("r6");
    u32 group_index_or_recipe_table;

    recipe_id_byte = (u8) recipe_id;
    first_unit_slot_byte = (u8) first_unit_slot;
    clear_model_index = 0;
    group_index_or_recipe_table = BATTLE_COMBINATION_FORMATION_TABLE_ROM;
    clear_zero = 0;
    do {
        *(matched_recipe_models + clear_model_index) = clear_zero;
        clear_model_index += 1;
    } while ((u32) clear_model_index <= 5U);
    recipe_id_seed = recipe_id_byte;
    recipe_stride_work = recipe_id_seed * 8;
    formation_mode = M2C_FIELD((recipe_stride_work - recipe_id_seed), u8 *, group_index_or_recipe_table);
    asm volatile("" : "+r"(recipe_id_seed) : "r"(formation_mode));
    asm volatile("mov %0, %1"
        : "=r"(recipe_id_times_eight)
        : "r"(recipe_stride_work));
    switch ((u32) formation_mode) {                      /* irregular */
    case BATTLE_COMBINATION_CONSECUTIVE_COMPONENTS:
        component_index = 0;
        asm volatile("mov %0, r9" : "=r"(recipe_stride_work));
        consecutive_pre_arg = recipe_id_byte;
        asm volatile(
            "sub %0, r2, r1\n"
            "add %0, #1"
            : "=r"(consecutive_pre_index)
            : "r"(recipe_stride_work), "r"(consecutive_pre_arg));
        if (M2C_FIELD(consecutive_pre_index, u8 *, group_index_or_recipe_table) != 0) {
            consecutive_table = group_index_or_recipe_table;
loop_7:
            consecutive_loop_arg = first_unit_slot_byte;
            asm volatile("" : "+r"(consecutive_loop_arg));
            consecutive_unit_offset = component_index + consecutive_loop_arg;
            consecutive_units = 0x020281F0;
            asm volatile("" : "+r"(consecutive_units));
            consecutive_unit_address = (u8 *)(consecutive_unit_offset + consecutive_units);
            if (*consecutive_unit_address != 0) {
                consecutive_model_index = 0;
                consecutive_stride = recipe_stride_work;
                consecutive_arg = recipe_id_byte;
                consecutive_index = (consecutive_stride - consecutive_arg) + 1;
                if (M2C_FIELD(consecutive_index, u8 *, consecutive_table) != 0) {
                    consecutive_records = 0x020218E4;
                    asm volatile("" :: "r"(consecutive_records));
                    consecutive_unit = *consecutive_unit_address;
                    consecutive_scan_table = BATTLE_COMBINATION_FORMATION_TABLE_ROM;
                    consecutive_value = M2C_FIELD((consecutive_unit * sizeof(struct PlayerZoidRecordView)), u8 *, consecutive_records + PLAYER_STATE_OFFSET(zoids[0].model_id));
loop_10:
                    consecutive_models_offset = (consecutive_stride - recipe_id_byte) + 1;
                    asm volatile("" : "+r"(consecutive_stride) : "r"(consecutive_models_offset));
                    if (consecutive_value == M2C_FIELD((consecutive_model_index + consecutive_models_offset), u8 *, consecutive_scan_table)) {
                        consecutive_model_match = matched_recipe_models + consecutive_model_index;
                        if (*consecutive_model_match == 0) {
                            goto consecutive_store;
                        }
reject_formation:
                        return 0U;
                    }
                    consecutive_model_index += 1;
                    if (((u32) consecutive_model_index > 5U) || (M2C_FIELD((consecutive_model_index + consecutive_models_offset), u8 *, consecutive_scan_table) == 0)) {
                        goto block_16;
                    }
                    goto loop_10;
                }
block_16:
                if (consecutive_model_index != 6) {
                    register u32 consecutive_post_stride asm("r4");
                    register u32 consecutive_post_arg asm("r5");
                    register u32 consecutive_post_index asm("r1");
                    asm volatile(
                        "mov %0, r9\n"
                        "mov %1, ip\n"
                        "sub r0, %0, %1\n"
                        "add %2, r0, #1"
                        : "=r"(consecutive_post_stride), "=r"(consecutive_post_arg),
                          "=r"(consecutive_post_index));
                    consecutive_models_end_offset = consecutive_post_index;
                    if (M2C_FIELD((consecutive_model_index + consecutive_models_end_offset), u8 *, group_index_or_recipe_table) != 0) {
                        goto consecutive_increment;
                    }
                    goto reject_formation;
consecutive_store:
                    *consecutive_model_match = 1;
                    goto block_16;
consecutive_increment:
                    asm volatile(
                        "add %0, %1, #1\n"
                        "lsl %0, %0, #24\n"
                        "lsr %1, %0, #24"
                        : "=&r"(counter_update), "+r"(component_index));
                    if ((u32) component_index <= 2U) {
                        asm volatile("mov %0, r9" : "=r"(recipe_stride_work));
                        if (M2C_FIELD((component_index + consecutive_models_end_offset), u8 *, group_index_or_recipe_table) != 0) {
                            goto loop_7;
                        }
                        goto accept_formation;
                    }
                    goto accept_formation;
                }
                goto reject_formation;
            }
            goto reject_formation;
        }
        goto accept_formation;
    case BATTLE_COMBINATION_FRONT_REAR_PAIR: {
        register u8 *pair_records asm("r5");
        register u8 *pair_units asm("r4");
        register u32 pair_units_offset asm("r0");
        register u8 *pair_table asm("r3");
        register s32 pair_row asm("r2");
        register u32 pair_counter3 asm("r0");
        register u32 pair_unit asm("r0");
        register u32 pair_record_unit asm("r1");
        group_index_or_recipe_table = 0;
        pair_records = (u8 *)0x020218E4;
        pair_units_offset = PLAYER_STATE_OFFSET(team_zoid_slots);
        asm volatile("" : "+r"(pair_units_offset));
        pair_units = pair_records + pair_units_offset;
        pair_table = (u8 *)BATTLE_COMBINATION_FORMATION_TABLE_ROM;
        asm volatile(
            "mov r1, r9\n"
            "mov %0, ip\n"
            "sub r0, r1, %0\n"
            "add %0, r0, #1"
            : "=r"(pair_row)
            :
            : "r0", "r1");
loop_24:
        asm volatile(
            "lsl %0, r7, #1\n"
            "add %0, %0, r7"
            : "=r"(pair_counter3));
        asm volatile(
            "add %0, r8\n"
            "add r1, %0, r4\n"
            "ldrb %0, [r1]"
            : "=r"(pair_unit)
            : "0"(pair_counter3), "r"(pair_units)
            : "r1");
        if (pair_unit == 0) {
            goto pair_next;
        }
        asm volatile("add %0, %1, #0"
            : "=r"(pair_record_unit)
            : "r"(pair_unit));
        if (pair_records[(pair_record_unit * sizeof(struct PlayerZoidRecordView)) + PLAYER_STATE_OFFSET(zoids[0].model_id)] ==
                *(u8 *)((group_index_or_recipe_table + pair_row) + (u32)pair_table)) {
pair_next:
            group_index_or_recipe_table = (u8) (group_index_or_recipe_table + 1);
            if ((u32) group_index_or_recipe_table > 1U) {
                goto accept_formation;
            }
            goto loop_24;
        }
        goto reject_formation;
    }
accept_formation:
        {
            register s32 success_result asm("r0");
            asm volatile("mov %0, #1" : "=r"(success_result));
            return success_result;
        }
    case BATTLE_COMBINATION_TWO_BY_TWO_COMPONENTS:
        group_index_or_recipe_table = 0;
        saved_recipe_stride = recipe_id_times_eight;
        recipe_models_offset = (recipe_id_times_eight - recipe_id_byte) + 1;
        rectangle_initial_table = BATTLE_COMBINATION_FORMATION_TABLE_ROM;
        rectangle_initial_table += recipe_models_offset;
        saved_recipe_models = (u8 *) rectangle_initial_table;
loop_30:
        component_index = 0;
        asm volatile(
            "lsl r0, r7, #1\n"
            "add r0, r0, r7\n"
            "mov %0, r0"
            : "=r"(rectangle_outer3));
loop_31:
        asm volatile(
            "mov r1, r9\n"
            "add %0, r6, r1\n"
            "add %0, r8"
            : "=r"(rectangle_outer_address)
            : "r"(rectangle_outer3), "r"(first_unit_slot_byte), "r"(component_index)
            : "r1");
        rectangle_units = 0x020281F0;
        asm volatile("add %0, %1, r2"
            : "=r"(rectangle_unit_address)
            : "r"(rectangle_outer_address), "r"(rectangle_units));
        if (*rectangle_unit_address != 0) {
            rectangle_model_index = 0;
            rectangle_stride = saved_recipe_stride;
            rectangle_spc_reload = saved_recipe_models;
            if (*rectangle_spc_reload != 0) {
                rectangle_records = 0x020218E4;
                rectangle_table = BATTLE_COMBINATION_FORMATION_TABLE_ROM;
                asm volatile("" :: "r"(rectangle_table));
                rectangle_unit = *rectangle_unit_address;
                rectangle_value = M2C_FIELD((rectangle_unit * sizeof(struct PlayerZoidRecordView)), u8 *, rectangle_records + PLAYER_STATE_OFFSET(zoids[0].model_id));
loop_34:
                rectangle_models_offset = (rectangle_stride - recipe_id_byte) + 1;
                asm volatile("" : "+r"(rectangle_stride) : "r"(rectangle_models_offset));
                if (rectangle_value == M2C_FIELD((rectangle_model_index + rectangle_models_offset), u8 *, rectangle_table)) {
                    rectangle_model_match = matched_recipe_models + rectangle_model_index;
                    if (*rectangle_model_match == 0) {
                        goto rectangle_store;
                    }
                    goto reject_formation;
                }
                rectangle_model_index += 1;
                if ((u32) rectangle_model_index > 5U) {
                    goto block_39;
                }
                rectangle_tail_address = (u8 *)(rectangle_model_index + rectangle_models_offset);
                asm volatile("add %0, %0, r5"
                    : "+r"(rectangle_tail_address));
                if (*rectangle_tail_address == 0) {
                    goto block_39;
                }
                goto loop_34;
            }
block_39:
            if (rectangle_model_index == 6) {
                goto reject_formation;
            }
            if (M2C_FIELD((rectangle_model_index + recipe_models_offset), u8 *, BATTLE_COMBINATION_FORMATION_TABLE_ROM) != 0) {
                goto rectangle_increment;
            }
            goto reject_formation;
rectangle_store:
            *rectangle_model_match = 1;
            goto block_39;
rectangle_increment:
            asm volatile(
                "add %0, %1, #1\n"
                "lsl %0, %0, #24\n"
                "lsr %1, %0, #24"
                : "=&r"(counter_update), "+r"(component_index));
            if ((u32) component_index > 1U) {
                group_index_or_recipe_table = (u8) (group_index_or_recipe_table + 1);
                if ((u32) group_index_or_recipe_table <= 1U) {
                    goto loop_30;
                }
                goto accept_formation;
            }
            goto loop_31;
        }
        goto reject_formation;
    case BATTLE_COMBINATION_HOST_AND_REAR_COMPONENTS: {
        register u32 host_initial_record_address asm("r0");
        register u32 host_initial_table_address asm("r1");
        register u8 *host_initial_records asm("r0");
        register u32 host_initial_units_offset asm("r1");
        register u8 *host_initial_unit_address asm("r2");
        register u8 *host_records asm("r5");
        register u8 *host_units asm("r9");
        register u8 *host_table asm("r4");
        register u32 host_record_value asm("r1");
        register u32 host_table_value asm("r0");
        register u32 host_unit_table asm("r0");
        if ((u32) first_unit_slot_byte <= 2U) {
            goto reject_formation;
        }
        host_initial_records = (u8 *)0x020218E4;
        host_initial_units_offset = PLAYER_STATE_OFFSET(team_zoid_slots[1]);
        host_initial_unit_address = host_initial_records + host_initial_units_offset;
        host_zoid_record_slot = *host_initial_unit_address;
        asm volatile("add %0, %1, #0"
            : "=r"(host_records)
            : "r"(host_initial_records));
        if (host_zoid_record_slot == 0) {
            goto reject_formation;
        }
        host_initial_record_address = host_zoid_record_slot * sizeof(struct PlayerZoidRecordView);
        asm volatile("add %0, %0, %1"
            : "+r"(host_initial_record_address)
            : "r"(host_records));
        host_models_offset = recipe_id_times_eight - recipe_id_byte;
        asm volatile(
            "add %0, r7, #1\n"
            "add %0, r2, %0"
            : "=r"(host_initial_table_address));
        if (M2C_FIELD(host_initial_record_address, u8 *, 4) != M2C_FIELD(host_initial_table_address, u8 *, 0)) {
            goto reject_formation;
        }
        component_index = first_unit_slot_byte;
        host_units = host_records + PLAYER_STATE_OFFSET(team_zoid_slots);
        host_table = (u8 *) group_index_or_recipe_table;
        host_models_offset += 1;
loop_55:
        if (*(u8 *)((u32)component_index + (u32)host_units) != 0) {
            rear_model_index = 1;
            host_unit_table = 0x020281F0;
            asm volatile("" : "+r"(host_unit_table));
            host_record_value = host_records[(M2C_FIELD(component_index, u8 *, host_unit_table) * sizeof(struct PlayerZoidRecordView)) + 4];
loop_57:
            host_table_value = *(u8 *)((rear_model_index + host_models_offset) + (u32)host_table);
            if (host_record_value != host_table_value) {
                rear_model_index += 1;
                if ((u32) rear_model_index <= 4U) {
                    goto loop_57;
                }
            } else {
                goto host_store;
            }
host_postscan:
            if (rear_model_index != 5) {
                goto block_66;
            }
            if (component_index != first_unit_slot_byte) {
                goto block_66;
            }
            goto reject_formation;
host_store:
            *(matched_recipe_models + rear_model_index) = 1;
            goto host_postscan;
        }
        if (component_index == first_unit_slot_byte) {
            goto reject_formation;
        }
block_66:
        asm volatile(
            "sub %0, %1, #1\n"
            "lsl %0, %0, #24\n"
            "lsr %1, %0, #24"
            : "=&r"(counter_update), "+r"(component_index));
        if ((u32) component_index <= 2U) {
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
        goto loop_55;
    }
    case BATTLE_COMBINATION_ALL_SIX_COMPONENTS: {
        register u32 all_units_records asm("r2");
        register u32 all_units_units_offset asm("r5");
        register u32 all_units_sp_outer asm("r5");
        register u32 all_units_units_base asm("r1");
        register u32 all_units_unit_address asm("r0");
        register u32 all_units_record_address asm("r0");
        register u32 all_units_value asm("r2");
        register s32 all_units_index asm("r1");
        register u32 all_units_outer3 asm("r4");
        u8 all_units_unit;
        group_index_or_recipe_table = 0;
        consecutive_table = BATTLE_COMBINATION_FORMATION_TABLE_ROM;
loop_76:
        component_index = 0;
        asm volatile(
            "lsl r0, r7, #1\n"
            "add %0, r0, r7"
            : "=r"(all_units_outer3)
            :
            : "r0");
        saved_group_stride = all_units_outer3;
loop_77:
        if (M2C_FIELD((component_index + all_units_outer3 + first_unit_slot_byte), u8 *, 0x020281F0) == 0) {
            goto reject_formation;
        }
        all_units_model_index = 0;
        all_units_records = 0x020218E4;
        asm volatile("" :: "r"(all_units_records));
        all_units_sp_outer = saved_group_stride;
        all_units_unit_address = component_index + all_units_sp_outer;
        all_units_unit_address += first_unit_slot_byte;
        all_units_units_offset = PLAYER_STATE_OFFSET(team_zoid_slots);
        all_units_units_base = all_units_records + all_units_units_offset;
        all_units_unit_address += all_units_units_base;
        all_units_unit = M2C_FIELD(all_units_unit_address, u8 *, 0);
        all_units_record_address = all_units_unit * sizeof(struct PlayerZoidRecordView);
        asm volatile("add %0, %0, %1"
            : "+r"(all_units_record_address)
            : "r"(all_units_records));
        asm volatile(
            "mov r2, r9\n"
            "mov r5, ip\n"
            "sub %0, r2, r5\n"
            "add %0, #1"
            : "=r"(all_units_index));
        all_units_value = M2C_FIELD(all_units_record_address, u8 *, 4);
loop_80:
        if (all_units_value == M2C_FIELD((all_units_model_index + all_units_index), u8 *, consecutive_table)) {
            all_units_model_match = matched_recipe_models + all_units_model_index;
            if (*all_units_model_match == 0) {
                goto all_units_store;
            }
            goto reject_formation;
        }
        all_units_model_index += 1;
        if ((u32) all_units_model_index > 5U) {
block_85:
            if (all_units_model_index != 6) {
                goto all_units_increment;
            }
            goto reject_formation;
all_units_store:
            *all_units_model_match = 1;
            goto block_85;
all_units_increment:
            asm volatile(
                "add %0, %1, #1\n"
                "lsl %0, %0, #24\n"
                "lsr %1, %0, #24"
                : "=&r"(counter_update), "+r"(component_index));
            if ((u32) component_index > 2U) {
                group_index_or_recipe_table = (u8) (group_index_or_recipe_table + 1);
                if ((u32) group_index_or_recipe_table > 1U) {
                    return 1U;
                }
                goto loop_76;
            }
            goto loop_77;
        }
        goto loop_80;
    }
    default:
        return default_result;
    }
}
