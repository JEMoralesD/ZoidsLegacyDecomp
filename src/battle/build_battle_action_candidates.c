#include "m2c_prelude.h"
#include "battle.h"

u8 FindBattleEffect(u8, u8, s32) asm("func_080BF464");
s32 TestBattleRuleFlag(s32) asm("func_080E6664");

void BuildBattleActionCandidates(void) asm("func_080CA238");

void BuildBattleActionCandidates(void) {
    register u32 equipment_slot asm("r8");
    u8 *equipment_action;
    register u32 next_equipment_slot asm("r6");
    register s32 reserve_r4 asm("r4");
    register s32 reserve_r5 asm("r5");
    register u32 effect_slot asm("r2");

#define SET_NEXT() \
    asm volatile("mov r6, r8\n\tadd r6, #1" : "=r"(next_equipment_slot) : "r"(equipment_slot))

    asm volatile("" : "=r"(reserve_r4), "=r"(reserve_r5) : : "r7");
    *(u8 *)0x0203EFA8 = 0;
    equipment_slot = 0;

loop:
    {
    register u32 stride asm("r0") = 0xA8C;
    register u32 record_offset asm("r2") = equipment_slot;

    asm volatile("" : "+r"(stride), "+r"(record_offset));
    asm volatile("mul r2, r0" : "+r"(record_offset) : "r"(stride) : "cc");
    {
    register u32 selection_state_base asm("r1") = 0x02034B4C;
    register u32 action_count_offset asm("r3") = 0xA1AF;
    register u32 action_count_address asm("r0");
    register u32 action_index asm("r1");
    register u32 action_offset asm("r0");
    register u8 *record_base asm("r4");
    register u32 record_base_offset asm("r5");
    register u8 *action_cache_base asm("r1");
    register u8 *count_base asm("r1");
    register u8 *count_ptr asm("r0");
    register u32 equipment_flags asm("r1");
    register u32 command_bit asm("r0");
    register u8 *battle_state_base asm("r5");
    asm volatile("" : "+r"(selection_state_base), "+r"(action_count_offset));
    action_count_address = selection_state_base + action_count_offset;
    action_index = *(u8 *)action_count_address;
    action_offset = action_index << 3;
    action_offset -= action_index;
    action_offset <<= 5;
    action_offset += action_index;
    action_offset <<= 2;
    record_base = (u8 *)0x02034B4C;
    record_base_offset = 0x27C8;
    asm volatile("" : "+r"(record_base), "+r"(record_base_offset));
    action_cache_base = record_base + record_base_offset;
    action_offset += (u32)action_cache_base;
    equipment_action = (u8 *)(record_offset + action_offset);

    count_base = (u8 *)0x0203EFA9;
    count_ptr = (u8 *)0x0203EFA8;
    asm volatile(
        "ldrb r0, [r0]\n\t"
        "add r0, r0, r1\n\t"
        "mov r1, #0\n\t"
        "strb r1, [r0]"
        : "+r"(count_ptr), "+r"(count_base) : : "cc", "memory");
    equipment_flags = BATTLE_ACTION_FIELD(equipment_action, u16, flags);
    command_bit = 1;
    command_bit &= equipment_flags;
    battle_state_base = record_base;

    if (command_bit == 0) {
        {
        register u8 *other asm("r2");
        if (battle_state_base[0x27BE] == 0x10) {
            goto type_10;
        }
        other = (u8 *)0x0203730B;
        asm volatile("" : "+r"(other));
        if (*other != 0x10) {
            goto after_type_10;
        }
type_10:
        {
            register u32 masked asm("r1") = BATTLE_ACTION_FIELD(equipment_action, u32, attributes);
            register u32 mask asm("r0") = WEAPON_MELEE;
            masked &= mask;
            SET_NEXT();
            if (masked != 0) {
                goto next;
            }
        }
after_type_10:
        ;
        }
        {
        register u8 *other asm("r4");
        register u32 filter_offset asm("r3") = 0x27BE;
        register u32 filter_value asm("r0");
        asm volatile("" : "+r"(filter_offset));
        filter_value = (u32)battle_state_base + filter_offset;
        filter_value = *(u8 *)filter_value;
        if (filter_value == 0x11) {
            goto type_11;
        }
        other = (u8 *)0x0203730B;
        asm volatile("" : "+r"(other));
        if (*other != 0x11) {
            goto after_type_11;
        }
type_11:
        {
            register u32 masked asm("r1") = BATTLE_ACTION_FIELD(equipment_action, u32, attributes);
            register u32 mask asm("r0") = WEAPON_MELEE;
            masked &= mask;
            SET_NEXT();
            if (masked == 0) {
                goto next;
            }
        }
after_type_11:
        ;
        }
        {
        register u8 *other asm("r2");
        register u32 filter_offset asm("r1") = 0x27BE;
        register u32 filter_value asm("r0");
        asm volatile("" : "+r"(filter_offset));
        filter_value = (u32)battle_state_base + filter_offset;
        filter_value = *(u8 *)filter_value;
        if (filter_value == 0x17) {
            goto type_17;
        }
        other = (u8 *)0x0203730B;
        asm volatile("" : "+r"(other));
        if (*other != 0x17) {
            goto after_type_17;
        }
type_17:
        {
            register u32 masked asm("r1") = BATTLE_ACTION_FIELD(equipment_action, u32, attributes);
            register u32 mask asm("r0") = WEAPON_LASER | WEAPON_PARTICLE;
            masked &= mask;
            SET_NEXT();
            if (masked != 0) {
                goto next;
            }
        }
after_type_17:
        asm volatile("" : "+r"(battle_state_base));
        }
        if ((TestBattleRuleFlag(BATTLE_RULE_MELEE_ONLY) << 24) != 0) {
            register u32 masked asm("r1") = BATTLE_ACTION_FIELD(equipment_action, u32, attributes);
            register u32 mask asm("r0") = WEAPON_MELEE;
            masked &= mask;
            SET_NEXT();
            if (masked == 0) {
                goto next;
            }
        }
        if ((TestBattleRuleFlag(BATTLE_RULE_NO_MELEE) << 24) != 0) {
            register u32 masked asm("r1") = BATTLE_ACTION_FIELD(equipment_action, u32, attributes);
            register u32 mask asm("r0") = WEAPON_MELEE;
            masked &= mask;
            SET_NEXT();
            if (masked != 0) {
                goto next;
            }
        }
    } else {
        if ((TestBattleRuleFlag(BATTLE_RULE_NO_RECOVERY) << 24) != 0) {
            register u32 type asm("r0") = equipment_action[4];
            type -= 0x10;
            SET_NEXT();
            if (type <= 2) {
                goto next;
            }
        }
        if ((TestBattleRuleFlag(0x16) << 24) != 0) {
            register u32 type asm("r0") = equipment_action[4];
            SET_NEXT();
            if (type == 0x15) {
                goto next;
            }
        }
    }
    }
    }

    if ((TestBattleRuleFlag(BATTLE_RULE_EQUIPMENT_SLOTS_4_TO_7_ONLY) << 24) != 0) {
        register u32 compare_index asm("r3");
        SET_NEXT();
        compare_index = equipment_slot;
        asm volatile("" : "+r"(compare_index));
        if (compare_index <= 3) {
            goto next;
        }
    }

    {
    register u32 equipment_flags asm("r1") = BATTLE_ACTION_FIELD(equipment_action, u16, flags);
    register u32 masked asm("r0") = 1;
    masked &= equipment_flags;
    SET_NEXT();
    if (masked == 0) {
        goto collect;
    }
    }

    {
    u32 flags = BATTLE_ACTION_FIELD(equipment_action, u32, attributes);
    u8 type = BATTLE_ACTION_FIELD(equipment_action, u8, attributes);
    if (type == 0x14) {
        register u8 *base asm("r4") = (u8 *)0x02034B4C;
        register u32 offset asm("r5") = 0x27A4;
        register u32 arg0 asm("r0");
        asm volatile("" : "+r"(base), "+r"(offset));
        arg0 = (u32)base + offset;
        arg0 = *(u8 *)arg0;
        {
        register u8 *arg1_ptr asm("r2") = (u8 *)0x020372F1;
        register u32 arg1 asm("r1");
        register u32 mode asm("r2");
        asm volatile("" : "+r"(arg1_ptr));
        arg1 = *arg1_ptr;
        mode = BATTLE_EFFECT_ENERGY_SHIELD;
        if (FindBattleEffect(arg0, arg1, mode) != 0xFF) {
            goto next;
        }
        }
        goto collect;
    }
    if (type == 0x15) {
        register u8 *base asm("r3") = (u8 *)0x02034B4C;
        register u32 offset asm("r4") = 0x27A4;
        register u32 arg0 asm("r0");
        asm volatile("" : "+r"(base), "+r"(offset));
        arg0 = (u32)base + offset;
        arg0 = *(u8 *)arg0;
        {
        register u8 *arg1_ptr asm("r5") = (u8 *)0x020372F1;
        register u32 arg1 asm("r1");
        asm volatile("" : "+r"(arg1_ptr));
        arg1 = *arg1_ptr;
        if (FindBattleEffect(arg0, arg1, 0x1C) != 0xFF) {
            goto next;
        }
        }
        goto collect;
    }

    if (flags & 0x40000000) {
        register u8 *search_base asm("r5");
        register u8 *retained_base asm("r9");
        register u32 member_offset asm("sl");
        register u32 member_pointer_offset asm("r1");
        register u32 member_value asm("r1");
        register u32 team_offset asm("r3");
        register u32 team_pointer_offset asm("r3");
        register u32 team_value asm("r1");
        register u32 team_work asm("r0");
        asm volatile("mov r2, #0" : "=r"(effect_slot));
        search_base = (u8 *)0x02034B4C;
        retained_base = search_base;
        member_pointer_offset = 0x27A5;
        asm volatile("" : "+r"(member_pointer_offset));
        team_work = (u32)search_base + member_pointer_offset;
        member_value = *(u8 *)team_work;
        team_work = (member_value << 2) + member_value;
        team_work <<= 3;
        team_work -= member_value;
        member_offset = team_work << 4;
        team_pointer_offset = 0x27A4;
        asm volatile("" : "+r"(team_pointer_offset));
        team_work = (u32)search_base + team_pointer_offset;
        team_value = *(u8 *)team_work;
        team_work = (team_value << 2) + team_value;
        team_work <<= 3;
        team_work -= team_value;
        team_offset = team_work << 7;
search_flagged:
        {
            register u32 entry_offset asm("r0") = effect_slot * 0xC;
            register u32 address asm("r1");
            register u8 *base_copy asm("r4");
            entry_offset += member_offset;
            address = entry_offset + team_offset;
            base_copy = retained_base;
            {
                register u8 *entry asm("r0") = (u8 *)((u32)address - (0 - (u32)base_copy));
                if (*(u16 *)(entry + 0xE8) != 0) {
                    register u8 *flag_address asm("r0") = search_base;
                    register u32 flag_value asm("r0");
                    flag_address += 0xE4;
                    flag_address = (u8 *)((u32)address - (0 - (u32)flag_address));
                    flag_value = *(u32 *)flag_address;
                    {
                    register u32 high_mask asm("r1") = 0x40000000;
                    register u32 masked_flag asm("r0") = flag_value;
                    asm volatile("" : "+r"(high_mask), "+r"(masked_flag));
                    masked_flag &= high_mask;
                    if (masked_flag != 0) {
                        goto search_done;
                    }
                    }
                }
            }
        }
        {
            register u32 next_slot asm("r0") = effect_slot + 1;
            next_slot <<= 24;
            effect_slot = next_slot >> 24;
        }
        if (effect_slot <= 0x1F) {
            goto search_flagged;
        }
    } else {
        register u8 *search_base asm("r4");
        register u32 member_offset asm("r9");
        register u32 member_pointer_offset asm("r5");
        register u32 member_value asm("r1");
        register u32 team_offset asm("r3");
        register u32 team_value asm("r1");
        register u32 team_work asm("r0");
        asm volatile("mov r2, #0" : "=r"(effect_slot));
        search_base = (u8 *)0x02034B4C;
        member_pointer_offset = 0x27A5;
        asm volatile("" : "+r"(member_pointer_offset));
        team_work = (u32)search_base + member_pointer_offset;
        member_value = *(u8 *)team_work;
        team_work = (member_value << 2) + member_value;
        team_work <<= 3;
        team_work -= member_value;
        member_offset = team_work << 4;
        team_value = search_base[0x27A4];
        team_work = (team_value << 2) + team_value;
        team_work <<= 3;
        team_work -= team_value;
        team_offset = team_work << 7;
search_index:
        {
            register u32 entry_offset asm("r0") = effect_slot * 0xC;
            register u16 value asm("r1");
            entry_offset += member_offset;
            entry_offset += team_offset;
            entry_offset += (u32)search_base;
            value = *(u16 *)(entry_offset + 0xE8);
            if (value != 0) {
                register u32 mask asm("r5") = 0xF00;
                register u32 extracted asm("r0") = mask;
                asm volatile("" : "+r"(mask));
                extracted = mask;
                extracted &= value;
                extracted >>= 8;
                if (extracted == equipment_slot) {
                    goto search_done;
                }
            }
        }
        {
            register u32 next_slot asm("r0") = effect_slot + 1;
            next_slot <<= 24;
            effect_slot = next_slot >> 24;
        }
        if (effect_slot <= 0x1F) {
            goto search_index;
        }
    }

search_done:
    if (effect_slot <= 0x1F) {
        goto next;
    }
    }

collect:
    {
    register u32 target_choice_count asm("r0") = BATTLE_ACTION_FIELD(equipment_action, u8, target_choice_count);
    register u8 *out_source asm("r1");
    register u8 *out_count_ptr asm("ip");
    register u8 *counts_source asm("r2");
    register u8 *counts_base asm("r9");
    out_source = (u8 *)0x0203EFA8;
    asm volatile("" : "+r"(out_source));
    out_count_ptr = out_source;
    asm volatile("" : "+r"(out_count_ptr));
    counts_source = (u8 *)0x0203EFA9;
    asm volatile("" : "+r"(counts_source));
    counts_base = counts_source;
    asm volatile("" : "+r"(counts_base));
    if (target_choice_count != 0) {
        register u32 target_choice asm("r3") = 0;
        register u8 *slots_source asm("r4") = (u8 *)0x0203EF78;
        register u8 *slots_base asm("sl");
        register u8 *counts_copy asm("r5");
        register u8 *out_count_copy asm("r4");
        asm volatile("" : "+r"(slots_source));
        slots_base = slots_source;
        asm volatile("" : "+r"(slots_base));
        counts_copy = counts_base;
        out_count_copy = out_count_ptr;
        do {
            register u32 target_choice_offset asm("r0") = target_choice;
            target_choice_offset *= 0x94;
            target_choice_offset = (u32)equipment_action - (0 - target_choice_offset);
            if (*(u8 *)(target_choice_offset + 0xC) != 0) {
                register u32 candidate_index asm("r1") = *out_count_copy;
                register u8 *count_ptr asm("r2") = (u8 *)((u32)candidate_index - (0 - (u32)counts_copy));
                register u32 slot_offset asm("r0") = candidate_index * 6;
                register u32 count asm("r2");
                asm volatile("" : "+r"(slot_offset));
                count = *count_ptr;
                slot_offset += count;
                slot_offset += (u32)slots_base;
                *(u8 *)slot_offset = target_choice;
                candidate_index = *out_count_copy;
                candidate_index = (u32)candidate_index - (0 - (u32)counts_copy);
                {
                    register u32 next_count asm("r0") = *(u8 *)candidate_index;
                    next_count++;
                    *(u8 *)candidate_index = next_count;
                }
            }
            {
                register u32 next_target_choice asm("r0") = target_choice + 1;
                next_target_choice <<= 24;
                target_choice = next_target_choice >> 24;
            }
        } while (target_choice <= 5);
    }

    {
        register u8 *out_count_copy asm("r5") = out_count_ptr;
        register u32 candidate_index asm("r1") = *out_count_copy;
        register u8 *counts_copy asm("r2") = counts_base;
        register u8 *count_ptr asm("r0") = (u8 *)((u32)candidate_index - (0 - (u32)counts_copy));
        if (*count_ptr != 0) {
            count_ptr = (u8 *)0x0203EF70;
            asm volatile("" : "+r"(count_ptr));
            count_ptr = (u8 *)((u32)candidate_index - (0 - (u32)count_ptr));
            {
                register u32 candidate_equipment_slot asm("r3") = equipment_slot;
                *count_ptr = candidate_equipment_slot;
            }
            {
                register u32 next_out asm("r0") = *out_count_copy;
                next_out++;
                *out_count_copy = next_out;
            }
        }
    }
    }

next:
    {
    register u32 normalized_index asm("r0") = next_equipment_slot;
    normalized_index <<= 24;
    normalized_index >>= 24;
    equipment_slot = normalized_index;
    if (normalized_index <= 7) {
        goto loop;
    }
    }
    asm volatile("" : : : "r9", "sl");
#undef SET_NEXT
}
