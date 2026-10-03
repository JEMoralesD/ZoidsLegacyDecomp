#include "m2c_prelude.h"
#include "battle.h"

typedef struct {
    u32 condition_flags;
    u16 kind_flags;
    u16 defense_bonus;
    u32 unk8;
} ConditionalDefenseEffect;

extern struct EquipmentRecord *AcquireEquipmentStatBuffer(u32, u32, u32, u32) asm("func_080E669C");
extern void ReleaseEquipmentStatBuffer(void) asm("func_080E66B8");
extern void BuildBattleEquipmentStats(u32, u32, u32, u32, struct EquipmentRecord *) asm("func_080E8C90");
extern u8 FindBattleEffect(u8, u8, s32) asm("func_080BF464");
extern u8 gBattleSetup[];
extern u8 gBattleState[];

s16 CalculateBattleDamage(u32, u32, u32, u32, u32, u32, u32) asm("func_080E813C");

s16 CalculateBattleDamage(u32 attacker_side_arg, u32 attacker_unit_slot_arg, u32 equipment_slot_arg, u32 action_index_arg, u32 target_side_arg,
                 u32 target_unit_slot_arg, u32 direct_hit_arg)
{
    register u32 attacker_side asm("r4") = attacker_side_arg;
    register u32 attacker_unit_slot asm("r5") = attacker_unit_slot_arg;
    register u32 equipment_slot asm("r6") = equipment_slot_arg;
    register u32 action_index asm("r3") = action_index_arg;
    register u32 target_side_input asm("r0") = target_side_arg;
    register u32 target_unit_slot_input asm("r1") = target_unit_slot_arg;
    register u32 direct_hit_input asm("r2") = direct_hit_arg;
    register u32 target_side asm("r9");
    register u32 target_unit_slot asm("sl");
    register u32 damage asm("r8");
    volatile u32 outgoing;
    volatile u32 direct_hit;
    u8 *volatile target_unit;
    u32 action_index_saved;
    struct EquipmentRecord *equipment_stats;
    register u32 weapon_attributes asm("r2");
    u16 defense_total;

    attacker_side = (u8)attacker_side;
    attacker_unit_slot = (u8)attacker_unit_slot;
    equipment_slot = (u8)equipment_slot;
    action_index = (u8)action_index;
    target_side_input = (u8)target_side_input;
    target_side = target_side_input;
    target_unit_slot_input = (u8)target_unit_slot_input;
    target_unit_slot = target_unit_slot_input;
    direct_hit_input = (u8)direct_hit_input;
    direct_hit = direct_hit_input;
    action_index_saved = action_index;
    equipment_stats = AcquireEquipmentStatBuffer(target_side_input, target_unit_slot_input, direct_hit_input, action_index);
    target_unit = gBattleState + target_side * 0x1380 + target_unit_slot * 0x270;
    asm volatile(
        "str r7, [sp]\n"
        "add r0, r4, #0\n"
        "add r1, r5, #0\n"
        "add r2, r6, #0\n"
        "ldr r3, [sp, #12]\n"
        "bl func_080E8C90"
        : "=m"(outgoing)
        : "r"(attacker_side), "r"(attacker_unit_slot), "r"(equipment_slot), "r"(equipment_stats),
          "m"(action_index_saved)
        : "r0", "r1", "r2", "r3", "lr", "cc", "memory");

    if (FindBattleEffect(attacker_side, attacker_unit_slot, 30) != 0xFF &&
        (equipment_stats->attributes & WEAPON_MELEE) != 0 &&
        (target_unit[0x36] & ZOID_MOVEMENT_FLYING) != 0) {
        equipment_stats->power_or_value += 20;
    }

    {
        register u32 attack_power asm("r3") = equipment_stats->power_or_value;
        register u32 loaded_flags asm("r1");

        damage = attack_power;
        loaded_flags = equipment_stats->attributes;
        {
            register u32 flag_test asm("r0") = WEAPON_WATER_COMPATIBLE;

            flag_test &= loaded_flags;
            weapon_attributes = loaded_flags;
            if (flag_test == 0 && gBattleSetup[1] == 11) {
            register u32 shifted asm("r0") = attack_power << 16;
            register s32 half asm("r1") = (s32)shifted >> 16;

            shifted >>= 31;
            half += shifted;
            half <<= 15;
            half = (u32)half >> 16;
            damage = half;
            }
        }
    }

    defense_total = 0;
    if ((weapon_attributes & WEAPON_IGNORE_DEFENSE) == 0) {
        register u32 direct_hit_check asm("r4") = direct_hit;

        if (direct_hit_check == 0) {
            register u32 row_offset asm("ip");
            register u32 mask asm("r7");
            register u32 flags_view asm("r6");
            register u32 expected asm("r4");
            register u32 index asm("r3");

            defense_total = *(u16 *)(target_unit + 0x46);
            index = 0;
            {
                register u32 row_calc asm("r0");
                register u32 member_calc asm("r1");

                {
                    register u32 unit_view asm("r1") = target_side;

                    asm volatile("" : "+r"(unit_view));
                    row_calc = unit_view << 2;
                }
                {
                    register u32 member_view asm("r4") = target_unit_slot;

                    member_calc = member_view << 2;
                }
                row_calc += target_side;
                row_calc <<= 3;
                {
                    register u32 unit_again asm("r4") = target_side;

                    row_calc -= unit_again;
                }
                row_calc <<= 7;
                row_offset = row_calc;
                mask = WEAPON_TYPE_MASK;
                member_calc += target_unit_slot;
                member_calc <<= 3;
                {
                    register u32 member_again asm("r0") = target_unit_slot;

                    member_calc -= member_again;
                }
                member_calc <<= 4;
                target_unit_slot = member_calc;
                flags_view = weapon_attributes;
                expected = flags_view;
                expected &= mask;
            }

            do {
                register ConditionalDefenseEffect *entry asm("r2");
                register u32 scratch_r1 asm("r1");

                {
                    register u32 entry_offset asm("r0");

                    asm volatile("ldr %0, [pc, #120]"
                                 : "=r"(scratch_r1));
                    scratch_r1 += target_unit_slot;
                    scratch_r1 += row_offset;
                    entry_offset = index << 1;
                    entry_offset += index;
                    entry_offset <<= 2;
                    entry_offset += 0xE4;
                    entry = (ConditionalDefenseEffect *)(scratch_r1 + entry_offset);
                }
                {
                    scratch_r1 = entry->kind_flags;

                    if (scratch_r1 != 0) {
                        register u32 masked_kind asm("r0") = BATTLE_EFFECT_KIND_MASK;

                        masked_kind &= scratch_r1;
                        if (masked_kind == BATTLE_EFFECT_DEFENSE &&
                            ((((entry->condition_flags & BATTLE_CONDITION_WEAPON_TYPE_MASK) >> 24) & mask &
                              flags_view) == expected)) {
                            register s32 sum asm("r0") = (s16)defense_total;
                            register u32 addend asm("r2") = entry->defense_bonus;

                            sum += addend;
                            defense_total = (u16)sum;
                        }
                    }
                }
                {
                    register u32 next asm("r0") = index + 1;

                    next <<= 24;
                    index = next >> 24;
                }
            } while (index <= 31);
        }
    }

    {
        register u8 *base asm("r1") = gBattleState;
        register u32 offset asm("r2") = 0x27BE;
        register u8 *kind_flags asm("r0");

        kind_flags = base + offset;
        kind_flags += target_side;
        if (*kind_flags == 49) {
            register u32 stride asm("r0") = 148;
            register u32 entry_offset asm("r1") = target_side;
            register u8 *entry asm("r1");

            entry_offset *= stride;
            {
                register u8 *table_base asm("r3") = gBattleState;
                register u32 table_offset asm("r4") = 0xA084;
                register u8 *table asm("r0");

                table = table_base + table_offset;
                entry = entry_offset + (u32)table;
            }
            {
                register u32 sum asm("r0") = defense_total << 16;
                register u32 addend asm("r1");

                sum = (s32)sum >> 16;
                addend = *(u16 *)entry;
                sum += addend;
                defense_total = (u16)sum;
            }
        }
    }

    {
        s16 signed_accumulator = defense_total;

        if (signed_accumulator < 0) {
            defense_total = 0;
        } else if (signed_accumulator > 9999) {
            defense_total = 9999;
        }
    }

    {
        register s32 difference asm("r1");
        register s32 signed_result asm("r0");

        {
            register u32 result_view asm("r0") = damage;

            difference = result_view << 16;
            difference >>= 16;
        }
        {
            register s32 subtrahend asm("r0") = defense_total << 16;

            subtrahend >>= 16;
            difference -= subtrahend;
        }
        difference <<= 16;
        {
            register u32 normalized asm("r2") = (u32)difference >> 16;

            damage = normalized;
        }
        signed_result = difference >> 16;
        if (signed_result <= 0) {
            register u32 one asm("r3") = 1;

            asm volatile("" : "+r"(one));
            damage = one;
        } else if (signed_result > 9999) {
            damage = 9999;
        }
    }

    ReleaseEquipmentStatBuffer();
    {
        register u32 result_view asm("r4") = damage;

        return (s16)result_view;
    }
}
