#include "m2c_prelude.h"
#include "battle.h"

extern u8 RemoveBattleActionCandidate(u32, u32) asm("func_080CA570");

void KeepBattleCandidatesWithMaxTargetCount(void) asm("func_080CB030");

void KeepBattleCandidatesWithMaxTargetCount(void)
{
    register s32 maximum_target_count asm("r8");
    register u32 candidate_index asm("r5");

    maximum_target_count = 0;
    candidate_index = 0;
    {
        register u8 *count_load asm("r0");
        register u32 count asm("r1");

        count_load = &((struct BattleActionCandidates *)0x0203EF70)->equipment_count;
        count = *count_load;
        if (candidate_index < count) {
            u8 *counts;
            register u8 *row_slot asm("ip");
            register u8 *candidate_target_choices asm("r9");
            register u32 saved_count asm("sl");

            counts = ((struct BattleActionCandidates *)0x0203EF70)->target_choice_counts;
            row_slot = (u8 *)0x0203ECFB;
            candidate_target_choices = ((struct BattleActionCandidates *)0x0203EF70)->target_choices[0];
            saved_count = count;
            do {
                register u32 choice_index asm("r4");
                register u32 next_outer asm("r6");
                register u8 *cslot asm("r0");
                register u32 climit asm("r0");

                choice_index = 0;
                cslot = (u8 *)(candidate_index + (s32)counts);
                next_outer = candidate_index + 1;
                climit = *cslot;
                if (choice_index < climit) {
                    register u8 *indices asm("r1");
                    register u8 *islot asm("r0");
                    register s32 record_index asm("r1");
                    register s32 record_stride asm("r0");
                    register s32 record_offset asm("r2");
                    register u32 action_index asm("r1");
                    register s32 row_offset asm("r0");
                    register u8 *row_base asm("r1");
                    register u8 *record asm("r3");

                    indices = ((struct BattleActionCandidates *)0x0203EF70)->equipment_slots;
                    asm volatile("" : "+r"(indices));
                    islot = (u8 *)(candidate_index + (s32)indices);
                    record_index = *islot;
                    asm volatile("" : "+r"(record_index));
                    record_stride = 0xA8C;
                    record_offset = record_index;
                    record_offset *= record_stride;
                    action_index = *row_slot;
                    row_offset = action_index << 3;
                    row_offset -= action_index;
                    row_offset <<= 5;
                    row_offset += action_index;
                    row_offset <<= 2;
                    row_base = (u8 *)0x02037314;
                    row_offset += (s32)row_base;
                    record = (u8 *)(record_offset + row_offset);
inner_loop1:
                    {
                        register u32 entry_offset asm("r0");
                        register u32 entry_index asm("r1");
                        register s32 field_offset asm("r0");
                        register u8 *field asm("r2");
                        register s32 target_count asm("r2");
                        register u32 next asm("r0");

                        asm volatile("" : "+r"(candidate_index));
                        entry_offset = candidate_index << 1;
                        entry_offset += candidate_index;
                        entry_offset <<= 1;
                        entry_offset = choice_index + entry_offset;
                        entry_offset += (u32)candidate_target_choices;
                        entry_index = *(u8 *)entry_offset;
                        field_offset = 0x94;
                        field_offset *= entry_index;
                        field_offset += 12;
                        field = record + field_offset;
                        target_count = *field;
                        if (maximum_target_count < target_count) {
                            maximum_target_count = target_count;
                        }
                        asm volatile("" : "+r"(counts));
                        next = choice_index + 1;
                        choice_index = (u8)next;
                        if (choice_index < *(u8 *)(candidate_index + (s32)counts)) {
                            goto inner_loop1;
                        }
                    }
                }
                {
                    register u32 t asm("r0");
                    t = next_outer << 24;
                    candidate_index = t >> 24;
                }
            } while (candidate_index < saved_count);
        }
    }

    candidate_index = 0;
    goto outer_test;
outer_loop:
    {
        register u32 choice_index asm("r4");
        register u8 *counts asm("r3");
        register u8 *cload asm("r0");
        register u8 *cslot asm("r1");
        register u32 climit asm("r1");

        choice_index = 0;
        cload = ((struct BattleActionCandidates *)0x0203EF70)->target_choice_counts;
        asm volatile("" : "+r"(cload));
        cslot = (u8 *)(candidate_index + (s32)cload);
        counts = cload;
        climit = *cslot;
        if (choice_index < climit) {
            register u8 *base asm("r6");
            register u8 *record_base asm("r9");
            u8 *candidate_target_choices;

            base = (u8 *)0x02034B4C;
            {
                register s32 rb asm("r1");
                rb = 0x27C8;
                asm volatile("" : "+r"(rb));
                rb += (s32)base;
                record_base = (u8 *)rb;
            }
            candidate_target_choices = ((struct BattleActionCandidates *)0x0203EF70)->target_choices[0];
inner_loop:
            {
                register u8 *index_table asm("r0");
                register s32 record_index asm("r1");
                register s32 record_stride asm("r0");
                register s32 record_offset asm("r2");
                register s32 row_off asm("r1");
                register u8 *row_slot2 asm("r0");
                register u32 action_index asm("r1");
                register s32 row_offset asm("r0");
                register u32 entry_offset asm("r0");
                register u32 entry_index asm("r1");
                register s32 field_offset asm("r0");
                register u32 target_count asm("r0");

                asm volatile("" : "+r"(candidate_index));
                index_table = ((struct BattleActionCandidates *)0x0203EF70)->equipment_slots;
                asm volatile("" : "+r"(index_table));
                index_table = (u8 *)(candidate_index + (s32)index_table);
                record_index = *index_table;
                asm volatile("" : "+r"(record_index));
                record_stride = 0xA8C;
                record_offset = record_index;
                record_offset *= record_stride;
                row_off = 0xA1AF;
                asm volatile("" : "+r"(row_off));
                row_slot2 = base + row_off;
                action_index = *row_slot2;
                row_offset = action_index << 3;
                row_offset -= action_index;
                row_offset <<= 5;
                row_offset += action_index;
                row_offset <<= 2;
                row_offset += (s32)record_base;
                record_offset += row_offset;
                entry_offset = candidate_index << 1;
                entry_offset += candidate_index;
                entry_offset <<= 1;
                entry_offset = choice_index + entry_offset;
                entry_offset += (u32)candidate_target_choices;
                entry_index = *(u8 *)entry_offset;
                field_offset = 0x94;
                field_offset *= entry_index;
                field_offset += 12;
                record_offset += field_offset;
                target_count = *(u8 *)record_offset;
                if (target_count != maximum_target_count) {
                    if (RemoveBattleActionCandidate(candidate_index, choice_index) == 0) {
                        register u32 t asm("r0");
                        t = candidate_index - 1;
                        t <<= 24;
                        candidate_index = t >> 24;
                        goto outer_next;
                    }
                    {
                        register u32 t2 asm("r0");
                        t2 = choice_index - 1;
                        t2 <<= 24;
                        choice_index = t2 >> 24;
                    }
                    counts = ((struct BattleActionCandidates *)0x0203EF70)->target_choice_counts;
                }
                {
                    register u32 t3 asm("r0");
                    t3 = choice_index + 1;
                    t3 <<= 24;
                    choice_index = t3 >> 24;
                }
                if (choice_index < *(u8 *)(candidate_index + (s32)counts)) {
                    goto inner_loop;
                }
            }
        }
    }
outer_next:
    {
        register u32 t4 asm("r0");
        t4 = candidate_index + 1;
        t4 <<= 24;
        candidate_index = t4 >> 24;
    }
outer_test:
    if (candidate_index < ((struct BattleActionCandidates *)0x0203EF70)->equipment_count) {
        goto outer_loop;
    }
}
