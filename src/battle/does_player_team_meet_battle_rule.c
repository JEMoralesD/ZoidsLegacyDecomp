#include "m2c_prelude.h"
#include "../game/player_state.h"

#define NULL ((void *)0)

struct ZoidLigerModelListView {
    u8 pad[0x1A];
    u8 first_model_id;
};


s32 DoesPlayerTeamMeetBattleRule(s32 rule_id_arg) asm("func_080E60B0");

s32 DoesPlayerTeamMeetBattleRule(s32 rule_id_arg) {
    register s32 normalized_rule_id asm("r0") = rule_id_arg;
    register u32 rule_case_index asm("r0");
    u32 team_rule_case_index;
    u8 zoid_model_id;
    register u32 rule_id asm("r4");
    u8 team_zoid_count;
    u8 liger_model_index;
    u8 tiger_model_index;
    u8 wolf_model_index;
    void *species_model_index;
    void *count_team_slot_index;
    void *tiger_team_slot_index;
    void *wolf_team_slot_index;
    void *species_team_slot_index;
    void *size_s_team_slot_index;
    void *max_m_team_slot_index;
    void *size_m_team_slot_index;
    void *max_l_team_slot_index;
    void *required_size_team_slot_index;
    void *no_flying_team_slot_index;
    void *flying_only_team_slot_index;
    void *liger_team_slot_index;
    register u8 *xl_required_player_state_bytes asm("r8");
    register s32 xl_required_team_slots_address asm("r2");
    register u32 xl_required_zoid_slot_or_work asm("r0");

    asm volatile(
        "lsl %0, %0, #24\n\t"
        "lsr %1, %0, #24"
        : "+r"(normalized_rule_id), "=r"(rule_id)
        :
        : "cc");
    rule_case_index = rule_id - 1;
    switch (rule_case_index) {
    case BATTLE_RULE_UP_TO_ONE_ZOID - 1:
    case BATTLE_RULE_UP_TO_TWO_ZOIDS - 1:
    case BATTLE_RULE_UP_TO_THREE_ZOIDS - 1:
    case BATTLE_RULE_UP_TO_FOUR_ZOIDS - 1:
    case BATTLE_RULE_EXACTLY_TWO_ZOIDS - 1: {
        register u8 *count_player_state_bytes asm("r8");
        register s32 count_team_slots_address asm("r4");

        team_zoid_count = 0;
        {
            register u32 count_team_slot_seed asm("r3") = 0;

            asm volatile("" : "+r"(count_team_slot_seed));
            count_team_slot_index = (void *)count_team_slot_seed;
        }
        team_rule_case_index = rule_id - 1;
        count_player_state_bytes = (u8 *)0x020218E4;
        count_team_slots_address = PLAYER_STATE_OFFSET(team_zoid_slots);
        asm volatile("" : "+r"(count_player_state_bytes), "+r"(count_team_slots_address));
        count_team_slots_address += (s32)count_player_state_bytes;
        do {
            if (*(u8 *)((u32)count_team_slot_index + (u32)count_team_slots_address) != 0) {
                team_zoid_count += 1;
            }
            count_team_slot_index = (void *) (u8) (count_team_slot_index + 1);
        } while ((u32) count_team_slot_index < PLAYER_TEAM_SLOT_COUNT);
        switch (team_rule_case_index) {
        case BATTLE_RULE_UP_TO_ONE_ZOID - 1:
            if ((u32) team_zoid_count <= 1U) {
                goto rule_accepted;
            }
rule_rejected:
            return 0;
        case BATTLE_RULE_UP_TO_TWO_ZOIDS - 1:
            if ((u32) team_zoid_count <= 2U) {
                goto rule_accepted;
            }
            goto rule_rejected;
        case BATTLE_RULE_UP_TO_THREE_ZOIDS - 1:
            if ((u32) team_zoid_count <= 3U) {
                goto rule_accepted;
            }
            goto rule_rejected;
        case BATTLE_RULE_UP_TO_FOUR_ZOIDS - 1:
            if ((u32) team_zoid_count <= 4U) {
                goto rule_accepted;
            }
            goto rule_rejected;
        case BATTLE_RULE_EXACTLY_TWO_ZOIDS - 1:
            if (team_zoid_count == 2) {
                goto rule_accepted;
            }
            goto rule_rejected;
        default:
            goto rule_accepted;
        }
        break;
    }
    case BATTLE_RULE_SIZE_S_ONLY - 1: {
        register u8 *size_s_player_state_bytes asm("r8");
        register u8 *size_s_player_state_seed asm("r1");
        register s32 size_s_team_slots_address asm("r2");
        register u32 size_s_zoid_slot_or_work asm("r0");

        size_s_team_slot_index = NULL;
        size_s_player_state_seed = (u8 *)0x020218E4;
        asm volatile("mov %0, %1"
                     : "=r"(size_s_player_state_bytes)
                     : "r"(size_s_player_state_seed));
        size_s_team_slots_address = PLAYER_STATE_OFFSET(team_zoid_slots);
        asm volatile("" : "+r"(size_s_player_state_bytes), "+r"(size_s_team_slots_address));
        size_s_team_slots_address += (s32)size_s_player_state_bytes;
        do {
            asm volatile(
                "add r1, %1, %2\n\t"
                "ldrb %0, [r1]"
                : "=r"(size_s_zoid_slot_or_work)
                : "r"(size_s_team_slot_index), "r"(size_s_team_slots_address)
                : "r1", "cc", "memory");
            if ((size_s_zoid_slot_or_work != 0) && (size_s_player_state_bytes[(size_s_zoid_slot_or_work * 0x70) + PLAYER_STATE_OFFSET(zoids[0].size_class)] != 0)) {
                goto rule_rejected;
            }
            size_s_team_slot_index = (void *) (u8) (size_s_team_slot_index + 1);
        } while ((u32) size_s_team_slot_index < PLAYER_TEAM_SLOT_COUNT);
        goto rule_accepted;
    }
    case BATTLE_RULE_UP_TO_SIZE_M - 1: {
        register u8 *max_m_player_state_bytes asm("r8");
        register s32 max_m_team_slots_address asm("r2");
        register u32 max_m_zoid_slot_or_work asm("r0");

        max_m_team_slot_index = NULL;
        max_m_player_state_bytes = (u8 *)0x020218E4;
        max_m_team_slots_address = PLAYER_STATE_OFFSET(team_zoid_slots);
        asm volatile("" : "+r"(max_m_player_state_bytes), "+r"(max_m_team_slots_address));
        max_m_team_slots_address += (s32)max_m_player_state_bytes;
        do {
            asm volatile(
                "add r1, %1, %2\n\t"
                "ldrb %0, [r1]"
                : "=r"(max_m_zoid_slot_or_work)
                : "r"(max_m_team_slot_index), "r"(max_m_team_slots_address)
                : "r1", "cc", "memory");
            if ((max_m_zoid_slot_or_work != 0) && ((u32)max_m_player_state_bytes[(max_m_zoid_slot_or_work * 0x70) + PLAYER_STATE_OFFSET(zoids[0].size_class)] > 1U)) {
                goto rule_rejected;
            }
            max_m_team_slot_index = (void *) (u8) (max_m_team_slot_index + 1);
        } while ((u32) max_m_team_slot_index < PLAYER_TEAM_SLOT_COUNT);
        goto rule_accepted;
    }
    case BATTLE_RULE_SIZE_M_ONLY - 1: {
        register u8 *size_m_player_state_bytes asm("r8");
        register u8 *size_m_player_state_seed asm("r1");
        register s32 size_m_team_slots_address asm("r2");
        register u32 size_m_zoid_slot_or_work asm("r0");

        size_m_team_slot_index = NULL;
        size_m_player_state_seed = (u8 *)0x020218E4;
        asm volatile("mov %0, %1"
                     : "=r"(size_m_player_state_bytes)
                     : "r"(size_m_player_state_seed));
        size_m_team_slots_address = PLAYER_STATE_OFFSET(team_zoid_slots);
        asm volatile("" : "+r"(size_m_player_state_bytes), "+r"(size_m_team_slots_address));
        size_m_team_slots_address += (s32)size_m_player_state_bytes;
        do {
            asm volatile(
                "add r1, %1, %2\n\t"
                "ldrb %0, [r1]"
                : "=r"(size_m_zoid_slot_or_work)
                : "r"(size_m_team_slot_index), "r"(size_m_team_slots_address)
                : "r1", "cc", "memory");
            if ((size_m_zoid_slot_or_work != 0) && (size_m_player_state_bytes[(size_m_zoid_slot_or_work * 0x70) + PLAYER_STATE_OFFSET(zoids[0].size_class)] != 1)) {
                goto rule_rejected;
            }
            size_m_team_slot_index = (void *) (u8) (size_m_team_slot_index + 1);
        } while ((u32) size_m_team_slot_index < PLAYER_TEAM_SLOT_COUNT);
        goto rule_accepted;
    }
    case BATTLE_RULE_UP_TO_SIZE_L - 1: {
        register u8 *max_l_player_state_bytes asm("r8");
        register s32 max_l_team_slots_address asm("r2");
        register u32 max_l_zoid_slot_or_work asm("r0");

        max_l_team_slot_index = NULL;
        max_l_player_state_bytes = (u8 *)0x020218E4;
        max_l_team_slots_address = PLAYER_STATE_OFFSET(team_zoid_slots);
        asm volatile("" : "+r"(max_l_player_state_bytes), "+r"(max_l_team_slots_address));
        max_l_team_slots_address += (s32)max_l_player_state_bytes;
        do {
            asm volatile(
                "add r1, %1, %2\n\t"
                "ldrb %0, [r1]"
                : "=r"(max_l_zoid_slot_or_work)
                : "r"(max_l_team_slot_index), "r"(max_l_team_slots_address)
                : "r1", "cc", "memory");
            if ((max_l_zoid_slot_or_work != 0) && ((u32)max_l_player_state_bytes[(max_l_zoid_slot_or_work * 0x70) + PLAYER_STATE_OFFSET(zoids[0].size_class)] > 2U)) {
                goto rule_rejected;
            }
            max_l_team_slot_index = (void *) (u8) (max_l_team_slot_index + 1);
        } while ((u32) max_l_team_slot_index < PLAYER_TEAM_SLOT_COUNT);
        goto rule_accepted;
    }
    case BATTLE_RULE_AT_LEAST_ONE_SIZE_L_OR_LARGER - 1: {
        register u8 *large_required_player_state_bytes asm("r8");
        register u8 *large_required_player_state_seed asm("r1");
        register s32 large_required_team_slots_address asm("r2");
        register u32 large_required_zoid_slot_or_work asm("r0");

        required_size_team_slot_index = NULL;
        large_required_player_state_seed = (u8 *)0x020218E4;
        asm volatile("mov %0, %1"
                     : "=r"(large_required_player_state_bytes)
                     : "r"(large_required_player_state_seed));
        large_required_team_slots_address = PLAYER_STATE_OFFSET(team_zoid_slots);
        asm volatile("" : "+r"(large_required_player_state_bytes), "+r"(large_required_team_slots_address));
        large_required_team_slots_address += (s32)large_required_player_state_bytes;
find_large_zoid:
        asm volatile(
            "add r1, %1, %2\n\t"
            "ldrb %0, [r1]"
            : "=r"(large_required_zoid_slot_or_work)
            : "r"(required_size_team_slot_index), "r"(large_required_team_slots_address)
            : "r1", "cc", "memory");
        if ((large_required_zoid_slot_or_work == 0) || ((u32)large_required_player_state_bytes[(large_required_zoid_slot_or_work * 0x70) + PLAYER_STATE_OFFSET(zoids[0].size_class)] <= 1U)) {
            required_size_team_slot_index = (void *) (u8) (required_size_team_slot_index + 1);
            if ((u32) required_size_team_slot_index >= PLAYER_TEAM_SLOT_COUNT) {

            } else {
                goto find_large_zoid;
            }
        }
        goto test_required_size_found;
    }
    case BATTLE_RULE_AT_LEAST_ONE_SIZE_XL_OR_LARGER - 1:
        required_size_team_slot_index = NULL;
        xl_required_player_state_bytes = (u8 *)0x020218E4;
        xl_required_team_slots_address = PLAYER_STATE_OFFSET(team_zoid_slots);
        asm volatile("" : "+r"(xl_required_player_state_bytes), "+r"(xl_required_team_slots_address));
        xl_required_team_slots_address += (s32)xl_required_player_state_bytes;
find_xl_zoid:
        asm volatile(
            "add r1, %1, %2\n\t"
            "ldrb %0, [r1]"
            : "=r"(xl_required_zoid_slot_or_work)
            : "r"(required_size_team_slot_index), "r"(xl_required_team_slots_address)
            : "r1", "cc", "memory");
        if ((xl_required_zoid_slot_or_work == 0) || ((u32)xl_required_player_state_bytes[(xl_required_zoid_slot_or_work * 0x70) + PLAYER_STATE_OFFSET(zoids[0].size_class)] <= 2U)) {
            required_size_team_slot_index = (void *) (u8) (required_size_team_slot_index + 1);
            if ((u32) required_size_team_slot_index < PLAYER_TEAM_SLOT_COUNT) {
                goto find_xl_zoid;
            }
        }
test_required_size_found:
        if (required_size_team_slot_index != (void *)PLAYER_TEAM_SLOT_COUNT) {
            goto rule_accepted;
        }
        goto rule_rejected;
    case BATTLE_RULE_NO_FLYING_ZOIDS - 1: {
        register u8 *no_flying_player_state_bytes asm("r8");
        register u8 *no_flying_player_state_seed asm("r1");
        register s32 no_flying_team_slots_address asm("r2");
        register u32 no_flying_flying_mask asm("r4");
        register u32 no_flying_zoid_slot_or_work asm("r0");
        register u32 no_flying_zoid_slot_or_flags asm("r1");

        no_flying_team_slot_index = NULL;
        no_flying_player_state_seed = (u8 *)0x020218E4;
        asm volatile("mov %0, %1"
                     : "=r"(no_flying_player_state_bytes)
                     : "r"(no_flying_player_state_seed));
        no_flying_team_slots_address = PLAYER_STATE_OFFSET(team_zoid_slots);
        asm volatile("" : "+r"(no_flying_player_state_bytes), "+r"(no_flying_team_slots_address));
        no_flying_team_slots_address += (s32)no_flying_player_state_bytes;
        no_flying_flying_mask = ZOID_MOVEMENT_FLYING;
        do {
            asm volatile(
                "add r1, %1, %2\n\t"
                "ldrb %0, [r1]"
                : "=r"(no_flying_zoid_slot_or_work)
                : "r"(no_flying_team_slot_index), "r"(no_flying_team_slots_address)
                : "r1", "cc", "memory");
            if (no_flying_zoid_slot_or_work != 0) {
                no_flying_zoid_slot_or_flags = no_flying_zoid_slot_or_work;
                no_flying_zoid_slot_or_work = no_flying_zoid_slot_or_flags << 3;
                no_flying_zoid_slot_or_work -= no_flying_zoid_slot_or_flags;
                no_flying_zoid_slot_or_work <<= 4;
                no_flying_zoid_slot_or_work += (u32)no_flying_player_state_bytes;
                no_flying_zoid_slot_or_work += 0x3A;
                no_flying_zoid_slot_or_flags = *(u8 *)no_flying_zoid_slot_or_work;
                no_flying_zoid_slot_or_work = no_flying_flying_mask;
                no_flying_zoid_slot_or_work &= no_flying_zoid_slot_or_flags;
                if (no_flying_zoid_slot_or_work != 0) {
                    goto rule_rejected;
                }
            }
            no_flying_team_slot_index = (void *) (u8) (no_flying_team_slot_index + 1);
        } while ((u32) no_flying_team_slot_index < PLAYER_TEAM_SLOT_COUNT);
        goto rule_accepted;
    }
    case BATTLE_RULE_FLYING_ZOIDS_ONLY - 1: {
        register u8 *flying_only_player_state_bytes asm("r8");
        register s32 flying_only_team_slots_address asm("r2");
        register u32 flying_only_flying_mask asm("r4");
        register u32 flying_only_zoid_slot_or_work asm("r0");
        register u32 flying_only_zoid_slot_or_flags asm("r1");

        flying_only_team_slot_index = NULL;
        flying_only_player_state_bytes = (u8 *)0x020218E4;
        flying_only_team_slots_address = PLAYER_STATE_OFFSET(team_zoid_slots);
        asm volatile("" : "+r"(flying_only_player_state_bytes), "+r"(flying_only_team_slots_address));
        flying_only_team_slots_address += (s32)flying_only_player_state_bytes;
        flying_only_flying_mask = ZOID_MOVEMENT_FLYING;
        do {
            asm volatile(
                "add r1, %1, %2\n\t"
                "ldrb %0, [r1]"
                : "=r"(flying_only_zoid_slot_or_work)
                : "r"(flying_only_team_slot_index), "r"(flying_only_team_slots_address)
                : "r1", "cc", "memory");
            if (flying_only_zoid_slot_or_work != 0) {
                flying_only_zoid_slot_or_flags = flying_only_zoid_slot_or_work;
                flying_only_zoid_slot_or_work = flying_only_zoid_slot_or_flags << 3;
                flying_only_zoid_slot_or_work -= flying_only_zoid_slot_or_flags;
                flying_only_zoid_slot_or_work <<= 4;
                flying_only_zoid_slot_or_work += (u32)flying_only_player_state_bytes;
                flying_only_zoid_slot_or_work += 0x3A;
                flying_only_zoid_slot_or_flags = *(u8 *)flying_only_zoid_slot_or_work;
                flying_only_zoid_slot_or_work = flying_only_flying_mask;
                flying_only_zoid_slot_or_work &= flying_only_zoid_slot_or_flags;
                if (flying_only_zoid_slot_or_work == 0) {
                    goto rule_rejected;
                }
            }
            flying_only_team_slot_index = (void *) (u8) (flying_only_team_slot_index + 1);
        } while ((u32) flying_only_team_slot_index < PLAYER_TEAM_SLOT_COUNT);
        goto rule_accepted;
    }
    case BATTLE_RULE_LIGER_MODELS_ONLY - 1: {
        register u8 *liger_player_state_bytes asm("r8");
        register u8 *liger_player_state_seed asm("r1");
        register s32 liger_team_slots_offset asm("r0");
        register u8 *liger_team_slots_address asm("ip");
        register struct ZoidLigerModelListView *liger_model_list asm("r7");
        register u32 liger_zoid_slot_or_work asm("r0");
        register u32 liger_model_id_or_table_value asm("r1");

        liger_team_slot_index = NULL;
        liger_player_state_seed = (u8 *)0x020218E4;
        asm volatile("mov %0, %1"
                     : "=r"(liger_player_state_bytes)
                     : "r"(liger_player_state_seed));
        liger_team_slots_offset = PLAYER_STATE_OFFSET(team_zoid_slots);
        asm volatile("" : "+r"(liger_player_state_bytes), "+r"(liger_team_slots_offset));
        liger_team_slots_address = (u8 *)((s32)liger_player_state_bytes + liger_team_slots_offset);
        liger_model_list = (struct ZoidLigerModelListView *)0x087A5810;
        do {
            asm volatile(
                "mov r0, %1\n\t"
                "add r1, %2, r0\n\t"
                "ldrb %0, [r1]"
                : "=&r"(liger_zoid_slot_or_work)
                : "r"(liger_team_slots_address), "r"(liger_team_slot_index)
                : "r1", "cc", "memory");
            if (liger_zoid_slot_or_work != 0) {
            register u8 *liger_model_list_base asm("r6");

            liger_model_index = 0;
            liger_model_id_or_table_value = liger_zoid_slot_or_work << 3;
            liger_model_id_or_table_value -= liger_zoid_slot_or_work;
            liger_model_id_or_table_value <<= 4;
            liger_model_id_or_table_value += (u32)liger_player_state_bytes;
            liger_zoid_slot_or_work = *(u8 *)(liger_model_id_or_table_value + 4);
            liger_model_list_base = (u8 *)0x087A5810;
            asm volatile("" : "+r"(liger_model_list_base));
            asm volatile("ldrb %0, [%1, #26]"
                         : "=r"(liger_model_id_or_table_value)
                         : "r"(liger_model_list)
                         : "memory");
            if (liger_zoid_slot_or_work != liger_model_id_or_table_value) {
                register u8 *liger_player_state_copy asm("r5");
                register s32 liger_team_slots_offset_copy asm("r1");
                register u8 *liger_team_slot_address asm("r4");

                liger_player_state_copy = (u8 *)0x020218E4;
                liger_team_slots_offset_copy = PLAYER_STATE_OFFSET(team_zoid_slots);
                asm volatile("" : "+r"(liger_player_state_copy), "+r"(liger_team_slots_offset_copy));
                asm volatile(
                    "add r0, %1, %2\n\t"
                    "add %0, %3, r0"
                    : "=r"(liger_team_slot_address)
                    : "r"(liger_player_state_copy), "r"(liger_team_slots_offset_copy), "r"(liger_team_slot_index)
                    : "r0", "cc");
                liger_model_list_base += 0x1A;
find_liger_model:
                liger_model_index += 1;
                if ((u32) liger_model_index <= 0x19U) {
                    if (liger_player_state_copy[(*liger_team_slot_address * 0x70) + PLAYER_STATE_OFFSET(zoids[0].model_id)] != *(u8 *)((u32)liger_model_index + (u32)liger_model_list_base)) {
                        goto find_liger_model;
                    }
                }
            }
            if (liger_model_index == 0x1A) {
                goto rule_rejected;
            }
            }
            liger_team_slot_index = (void *) (u8) (liger_team_slot_index + 1);
        } while ((u32) liger_team_slot_index < PLAYER_TEAM_SLOT_COUNT);
        goto rule_accepted;
    }
    case BATTLE_RULE_TIGER_MODELS_ONLY - 1: {
        register u8 *tiger_player_state_bytes asm("r8");
        register s32 tiger_team_slots_offset asm("r1");
        register u8 *tiger_team_slots_address asm("ip");
        register u8 *tiger_model_list asm("r7");
        register u32 tiger_zoid_slot_or_work asm("r0");
        register u32 tiger_model_id_or_table_value asm("r1");

        tiger_team_slot_index = NULL;
        tiger_player_state_bytes = (u8 *)0x020218E4;
        tiger_team_slots_offset = PLAYER_STATE_OFFSET(team_zoid_slots);
        asm volatile("" : "+r"(tiger_player_state_bytes), "+r"(tiger_team_slots_offset));
        tiger_team_slots_address = (u8 *)((s32)tiger_player_state_bytes + tiger_team_slots_offset);
        tiger_model_list = (u8 *)0x087A58AC;
        do {
            asm volatile(
                "mov r0, %1\n\t"
                "add r1, %2, r0\n\t"
                "ldrb %0, [r1]"
                : "=&r"(tiger_zoid_slot_or_work)
                : "r"(tiger_team_slots_address), "r"(tiger_team_slot_index)
                : "r1", "cc", "memory");
            if (tiger_zoid_slot_or_work != 0) {
            register u8 *tiger_model_list_base asm("r6");

            tiger_model_index = 0;
            tiger_model_id_or_table_value = tiger_zoid_slot_or_work << 3;
            tiger_model_id_or_table_value -= tiger_zoid_slot_or_work;
            tiger_model_id_or_table_value <<= 4;
            tiger_model_id_or_table_value += (u32)tiger_player_state_bytes;
            tiger_zoid_slot_or_work = *(u8 *)(tiger_model_id_or_table_value + 4);
            tiger_model_list_base = (u8 *)0x087A5810;
            asm volatile("" : "+r"(tiger_model_list_base));
            asm volatile("ldrb %0, [%1, #0]"
                         : "=r"(tiger_model_id_or_table_value)
                         : "r"(tiger_model_list)
                         : "memory");
            if (tiger_zoid_slot_or_work != tiger_model_id_or_table_value) {
                register u8 *tiger_player_state_copy asm("r5");
                register s32 tiger_team_slots_offset_copy asm("r1");
                register u8 *tiger_team_slot_address asm("r4");

                tiger_player_state_copy = (u8 *)0x020218E4;
                tiger_team_slots_offset_copy = PLAYER_STATE_OFFSET(team_zoid_slots);
                asm volatile("" : "+r"(tiger_player_state_copy), "+r"(tiger_team_slots_offset_copy));
                asm volatile(
                    "add r0, %1, %2\n\t"
                    "add %0, %3, r0"
                    : "=r"(tiger_team_slot_address)
                    : "r"(tiger_player_state_copy), "r"(tiger_team_slots_offset_copy), "r"(tiger_team_slot_index)
                    : "r0", "cc");
                tiger_model_list_base += 0x9C;
find_tiger_model:
                tiger_model_index += 1;
                if ((u32) tiger_model_index <= 0x19U) {
                    if (tiger_player_state_copy[(*tiger_team_slot_address * 0x70) + PLAYER_STATE_OFFSET(zoids[0].model_id)] != *(u8 *)((u32)tiger_model_index + (u32)tiger_model_list_base)) {
                        goto find_tiger_model;
                    }
                }
            }
            if (tiger_model_index == 0x1A) {
                goto rule_rejected;
            }
            }
            tiger_team_slot_index = (void *) (u8) (tiger_team_slot_index + 1);
        } while ((u32) tiger_team_slot_index < PLAYER_TEAM_SLOT_COUNT);
        goto rule_accepted;
    }
    case BATTLE_RULE_WOLF_MODELS_ONLY - 1: {
        register u8 *wolf_player_state_bytes asm("r8");
        register u8 *wolf_player_state_seed asm("r0");
        register s32 wolf_team_slots_offset asm("r1");
        register u8 *wolf_team_slots_address asm("ip");
        register u8 *wolf_model_list asm("r7");
        register u32 wolf_zoid_slot_or_work asm("r0");
        register u32 wolf_model_id_or_table_value asm("r1");

        wolf_team_slot_index = NULL;
        wolf_player_state_seed = (u8 *)0x020218E4;
        asm volatile("mov %0, %1"
                     : "=r"(wolf_player_state_bytes)
                     : "r"(wolf_player_state_seed));
        wolf_team_slots_offset = PLAYER_STATE_OFFSET(team_zoid_slots);
        asm volatile("" : "+r"(wolf_player_state_bytes), "+r"(wolf_team_slots_offset));
        wolf_team_slots_address = (u8 *)((s32)wolf_player_state_bytes + wolf_team_slots_offset);
        wolf_model_list = (u8 *)0x087A58C6;
        do {
            asm volatile(
                "mov r0, %1\n\t"
                "add r1, %2, r0\n\t"
                "ldrb %0, [r1]"
                : "=&r"(wolf_zoid_slot_or_work)
                : "r"(wolf_team_slots_address), "r"(wolf_team_slot_index)
                : "r1", "cc", "memory");
            if (wolf_zoid_slot_or_work != 0) {
            register u8 *wolf_model_list_base asm("r6");

            wolf_model_index = 0;
            wolf_model_id_or_table_value = wolf_zoid_slot_or_work << 3;
            wolf_model_id_or_table_value -= wolf_zoid_slot_or_work;
            wolf_model_id_or_table_value <<= 4;
            wolf_model_id_or_table_value += (u32)wolf_player_state_bytes;
            wolf_zoid_slot_or_work = *(u8 *)(wolf_model_id_or_table_value + 4);
            wolf_model_list_base = (u8 *)0x087A5810;
            asm volatile("" : "+r"(wolf_model_list_base));
            asm volatile("ldrb %0, [%1, #0]"
                         : "=r"(wolf_model_id_or_table_value)
                         : "r"(wolf_model_list)
                         : "memory");
            if (wolf_zoid_slot_or_work != wolf_model_id_or_table_value) {
                register u8 *wolf_player_state_copy asm("r5");
                register s32 wolf_team_slots_offset_copy asm("r1");
                register u8 *wolf_team_slot_address asm("r4");

                wolf_player_state_copy = (u8 *)0x020218E4;
                wolf_team_slots_offset_copy = PLAYER_STATE_OFFSET(team_zoid_slots);
                asm volatile("" : "+r"(wolf_player_state_copy), "+r"(wolf_team_slots_offset_copy));
                asm volatile(
                    "add r0, %1, %2\n\t"
                    "add %0, %3, r0"
                    : "=r"(wolf_team_slot_address)
                    : "r"(wolf_player_state_copy), "r"(wolf_team_slots_offset_copy), "r"(wolf_team_slot_index)
                    : "r0", "cc");
                wolf_model_list_base += 0xB6;
find_wolf_model:
                wolf_model_index += 1;
                if ((u32) wolf_model_index <= 0x19U) {
                    if (wolf_player_state_copy[(*wolf_team_slot_address * 0x70) + PLAYER_STATE_OFFSET(zoids[0].model_id)] != *(u8 *)((u32)wolf_model_index + (u32)wolf_model_list_base)) {
                        goto find_wolf_model;
                    }
                }
            }
            if (wolf_model_index == 0x1A) {
                goto rule_rejected;
            }
            }
            wolf_team_slot_index = (void *) (u8) (wolf_team_slot_index + 1);
        } while ((u32) wolf_team_slot_index < PLAYER_TEAM_SLOT_COUNT);
        goto rule_accepted;
    }
    case BATTLE_RULE_LIGER_TIGER_OR_WOLF_MODELS_ONLY - 1: {
        register u8 *species_player_state_bytes asm("r8");
        register u8 *species_player_state_seed asm("r0");
        register s32 species_team_slots_offset asm("r1");
        register u8 *species_team_slots_address asm("ip");
        u8 *species_player_state_copy;

        species_team_slot_index = NULL;
        species_player_state_seed = (u8 *)0x020218E4;
        asm volatile("mov %0, %1"
                     : "=r"(species_player_state_bytes)
                     : "r"(species_player_state_seed));
        species_team_slots_offset = PLAYER_STATE_OFFSET(team_zoid_slots);
        asm volatile("" : "+r"(species_player_state_bytes), "+r"(species_team_slots_offset));
        species_team_slots_address = (u8 *)((s32)species_player_state_bytes + species_team_slots_offset);
        species_player_state_copy = species_player_state_bytes;
next_species_team_slot:
        {
        register u8 *species_team_slots_low_register asm("r1") = species_team_slots_address;
        register u8 *species_team_slot_address asm("r0");

        asm volatile("add %0, %1, %2"
                     : "=r"(species_team_slot_address)
                     : "r"(species_team_slot_index), "r"(species_team_slots_low_register)
                     : "cc");
        if (*species_team_slot_address != 0) {
            register u8 *species_model_list asm("r6");
            register u8 *species_liger_list_base asm("r5");
            register u8 *species_wolf_list_base asm("r4");
            register u8 *species_model_id_address asm("r0");
            register u8 *species_team_map_address asm("r0");
            register u32 species_stored_zoid_slot asm("r1");

            species_model_index = NULL;
            species_model_list = (u8 *)0x087A5810;
            asm volatile("" : "+r"(species_model_list));
            species_team_map_address = (u8 *)0x020281F0;
            asm volatile(
                "add %1, %2, %1\n\t"
                "ldrb %0, [%1]"
                : "=r"(species_stored_zoid_slot), "+r"(species_team_map_address)
                : "r"(species_team_slot_index)
                : "cc", "memory");
            zoid_model_id = species_player_state_copy[(species_stored_zoid_slot * 0x70) + PLAYER_STATE_OFFSET(zoids[0].model_id)];
            species_liger_list_base = species_model_list;
            species_wolf_list_base = species_model_list;
            species_wolf_list_base += 0xB6;
find_species_model:
            species_model_id_address = species_liger_list_base;
            species_model_id_address += 0x1A;
            asm volatile("add %0, %1, %0"
                         : "+r"(species_model_id_address)
                         : "r"(species_model_index)
                         : "cc");
            if (zoid_model_id != *species_model_id_address) {
                species_model_id_address = species_model_list;
                species_model_id_address += 0x9C;
                asm volatile("add %0, %1, %0"
                             : "+r"(species_model_id_address)
                             : "r"(species_model_index)
                             : "cc");
                if (zoid_model_id != *species_model_id_address) {
                    asm volatile("add %0, %1, %2"
                                 : "=r"(species_model_id_address)
                                 : "r"(species_model_index), "r"(species_wolf_list_base)
                                 : "cc");
                    if (zoid_model_id != *species_model_id_address) {
                        species_model_index = (void *) (u8) (species_model_index + 1);
                        if ((u32) species_model_index <= 0x19U) {
                            goto find_species_model;
                        }
                    }
                }
            }
            if (species_model_index == (void *)0x1A) {
                goto rule_rejected;
            }
            goto advance_species_team_slot;
        }
        }
advance_species_team_slot:
        species_team_slot_index = (void *) (u8) (species_team_slot_index + 1);
        if ((u32) species_team_slot_index >= PLAYER_TEAM_SLOT_COUNT) {
            goto rule_accepted;
        }
        goto next_species_team_slot;
    }
    }
rule_accepted:
    return 1;
}
