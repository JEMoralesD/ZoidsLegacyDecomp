#include "m2c_prelude.h"
#include "battle.h"

extern u8 HasBattleActionCandidates(void) asm("func_080CA560");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern void BuildBattleActionCandidates(void) asm("func_080CA238");
extern void KeepBattleNonAttackCandidates(void) asm("func_080CA860");
extern void AppendBattleActionSelection(s32, s32) asm("func_080CA1B4");

void AppendRandomBattleAction(void) asm("func_080CA650");

void AppendRandomBattleAction(void)
{
    register u8 *battle_state asm("r6");
    register u8 **active_unit_slot asm("r9");
    register u32 attempt asm("r8");

    attempt = 0;
    battle_state = (u8 *)0x02034B4C;
    active_unit_slot = (u8 **)(battle_state + 0x27A8);
    do {
        if (HasBattleActionCandidates()) {
            register u32 *rng asm("r10");
            register u32 random asm("r0");
            register u32 count asm("r1");
            register u32 candidate_index asm("r4");
            register u8 *candidate_equipment_slots asm("r0");
            register u8 *entry asm("r5");
            register s32 equipment_slot asm("r1");
            register s32 record_stride asm("r0");
            register s32 record_offset asm("r2");
            register u32 action_index asm("r1");
            register s32 row_offset asm("r0");
            register u8 *record_base asm("r1");
            register u8 *equipment_action asm("r2");
            register s32 unit_ep asm("r3");
            register s32 planned_ep_cost asm("r0");
            register s32 action_ep_cost asm("r1");
            u8 **slot_view;
            s32 off_rec;

            {
                register u32 *rng_lo asm("r3");
                rng_lo = (u32 *)0x03000010;
                asm volatile("" : "+r"(rng_lo));
                rng = rng_lo;
                random = CallFunctionR0(*rng_lo);
            }
            count = *(u8 *)0x0203EFA8;
            random *= count;
            random >>= 15;
            candidate_index = (u8)random;
            candidate_equipment_slots = (u8 *)0x0203EF70;
            asm volatile("" : "+r"(candidate_equipment_slots));
            entry = (u8 *)(candidate_index + (s32)candidate_equipment_slots);
            equipment_slot = *entry;
            asm volatile("" : "+r"(equipment_slot));
            record_stride = 0xA8C;
            record_offset = equipment_slot;
            record_offset *= record_stride;
            action_index = battle_state[0xA1AF];
            row_offset = action_index << 3;
            row_offset -= action_index;
            row_offset <<= 5;
            row_offset += action_index;
            row_offset <<= 2;
            off_rec = 0x27C8;
            asm volatile("" : "+r"(off_rec));
            record_base = battle_state + off_rec;
            row_offset += (s32)record_base;
            record_offset += row_offset;
            equipment_action = (u8 *)record_offset;
            slot_view = active_unit_slot;
            asm volatile("" : "+r"(slot_view) : "r"(row_offset), "r"(record_base), "r"(off_rec));
            asm volatile("" :: "r"(row_offset), "r"(record_base), "r"(off_rec));
            unit_ep = *(s16 *)(*slot_view + 8);
            planned_ep_cost = *(s16 *)0x0203EFB2;
            action_ep_cost = BATTLE_ACTION_FIELD(equipment_action, s16, ep_cost);
            planned_ep_cost += action_ep_cost;
            if (unit_ep >= planned_ep_cost) {
                register u32 random_target_choice asm("r2");
                register u32 target_choice_count asm("r1");
                register u32 selected_equipment_slot asm("r0");
                register u32 table_offset asm("r1");

                {
                    register u32 *rng_view asm("r1");
                    rng_view = rng;
                    asm volatile("" : "+r"(rng_view));
                    random = CallFunctionR0(*rng_view);
                }
                {
                    register u8 *count_table asm("r1");
                    count_table = (u8 *)0x0203EFA9;
                    asm volatile("" : "+r"(count_table));
                    target_choice_count = *(u8 *)(candidate_index + (s32)count_table);
                }
                random_target_choice = random;
                random_target_choice *= target_choice_count;
                random_target_choice >>= 15;
                random_target_choice = (u8)random_target_choice;
                selected_equipment_slot = *entry;
                {
                    register u8 *table asm("r3");
                    table = (u8 *)0x0203EF78;
                    asm volatile("" : "+r"(table));
                    table_offset = candidate_index << 1;
                    table_offset += candidate_index;
                    table_offset <<= 1;
                    random_target_choice += table_offset;
                    random_target_choice += (u32)table;
                }
                AppendBattleActionSelection(selected_equipment_slot, *(u8 *)random_target_choice);
                return;
            }
        }
        {
            register u32 attempt_view asm("r3");
            attempt_view = attempt;
            asm volatile("" : "+r"(attempt_view));
            if (attempt_view == 0) {
                BuildBattleActionCandidates();
                KeepBattleNonAttackCandidates();
                {
                    register u32 one asm("r7");
                    one = 1;
                    asm volatile("" : "+r"(one));
                    attempt = one;
                }
            } else {
                {
                    register s32 t asm("r0");
                    register s32 offa asm("r1");
                    register s32 offb asm("r3");
                    register u32 onev asm("r1");
                    offa = 0x27A4;
                    asm volatile("" : "+r"(offa));
                    t = (s32)(battle_state + offa);
                    t = *(u8 *)t;
                    t <<= 2;
                    t += (s32)battle_state;
                    offb = 0xA07C;
                    asm volatile("" : "+r"(offb));
                    t += offb;
                    onev = 1;
                    *(u8 *)t = onev;
                }
                return;
            }
        }
    } while (1);
}
