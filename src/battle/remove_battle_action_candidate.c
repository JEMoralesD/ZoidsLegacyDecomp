#include "m2c_prelude.h"
#include "battle.h"

extern u8 gBattleActionCandidateCount asm("D_0203EFA8");
extern u8 gBattleActionCandidateChoiceCounts[] asm("D_0203EFA9");
extern u8 gBattleActionCandidateEquipmentSlots[] asm("D_0203EF70");
extern u8 gBattleActionCandidateTargetChoices[] asm("D_0203EF78");

u8 RemoveBattleActionCandidate(s32 candidate_index, s32 choice_index) asm("func_080CA570");

u8 RemoveBattleActionCandidate(s32 candidate_index, s32 choice_index)
{
    register u32 equipment_candidate_index asm("r5");
    candidate_index <<= 24;
    equipment_candidate_index = (u32)candidate_index >> 24;
    choice_index <<= 24;
    choice_index = (u32)choice_index >> 24;
    if (choice_index != BATTLE_REMOVE_EQUIPMENT_CANDIDATE) {
        register u8 *count_slot asm("r4");
        register u8 *count_base asm("r0");
        u32 count;

        count_base = gBattleActionCandidateChoiceCounts;
        count_slot = (u8 *)(equipment_candidate_index + (s32)count_base);
        count = *count_slot - 1;
        *count_slot = count;
        count = (u8)count;
        if (count != 0) {
            register u32 index asm("r3");

            index = choice_index;
            if (index < count) {
                register u8 *target_choices asm("r6");
                register u32 triple asm("r0");
                register u32 row_offset asm("r2");
                register u32 next_row_byte asm("r5");

                target_choices = gBattleActionCandidateTargetChoices;
                triple = equipment_candidate_index * 2;
                triple += equipment_candidate_index;
                row_offset = triple * 2;
                next_row_byte = row_offset + 1;
                do {
                    register u32 next asm("r0");
                    register u8 *destination asm("r1");
                    register u8 *source asm("r0");

                    destination = (u8 *)(index + row_offset);
                    destination += (s32)target_choices;
                    source = (u8 *)(index + next_row_byte);
                    source += (s32)target_choices;
                    *destination = *source;
                    next = index + 1;
                    index = (u8)next;
                } while (index < *count_slot);
            }
            return BATTLE_CANDIDATE_REMAINS;
        }
    }

    {
        register u8 *count_slot asm("r9");
        register u8 *count_ptr asm("r1");
        u32 count;
        register u32 index asm("r3");

        count_ptr = &gBattleActionCandidateCount;
        count = *count_ptr - 1;
        *count_ptr = count;
        index = equipment_candidate_index;
        count = (u8)count;
        count_slot = count_ptr;
        if (index < count) {
            register u8 *equipment_slots asm("r8");
            u8 *target_choices;
            register u8 *counts asm("ip");
            register u8 *outer_load asm("r1");
            register u8 *counts_load asm("r2");

            outer_load = gBattleActionCandidateEquipmentSlots;
            asm volatile("" : "+r"(outer_load));
            equipment_slots = outer_load;
            target_choices = gBattleActionCandidateTargetChoices;
            counts_load = gBattleActionCandidateChoiceCounts;
            asm volatile("" : "+r"(counts_load));
            counts = counts_load;
            do {
                register u32 next asm("r1");
                register u32 target_choice_index asm("r2");
                register u32 current_offset asm("r6");
                register u32 next_offset asm("r5");
                register u32 triple asm("r0");
                register u32 next_index asm("r4");

                {
                    register u8 *outer_view asm("r4");
                    register u8 *destination asm("r2");
                    register u8 *source asm("r0");

                    outer_view = equipment_slots;
                    destination = (u8 *)(index + (s32)outer_view);
                    next = index + 1;
                    source = (u8 *)(next + (s32)outer_view);
                    *destination = *source;
                }
                target_choice_index = 0;
                {
                    next_index = next;
                    asm volatile("" : "+r"(next_index));

                    triple = index * 2;
                    triple += index;
                    current_offset = triple * 2;
                    triple = next_index * 2;
                    triple += next_index;
                    next_offset = triple * 2;
                }
                do {
                    register u8 *destination asm("r1");
                    register u8 *source asm("r0");
                    register u32 next_inner asm("r0");

                    destination = (u8 *)(target_choice_index + current_offset);
                    destination += (s32)target_choices;
                    source = (u8 *)(target_choice_index + next_offset);
                    source += (s32)target_choices;
                    *destination = *source;
                    next_inner = target_choice_index + 1;
                    target_choice_index = (u8)next_inner;
                } while (target_choice_index <= 5);
                {
                    register u8 *base asm("r0") = counts;
                    register u8 *destination asm("r1") = (u8 *)(index + (s32)base);
                    register u8 *source asm("r0") = (u8 *)(next_index + (s32)base);
                    register u32 normalized asm("r0");

                    *destination = *source;
                    normalized = next_index << 24;
                    index = normalized >> 24;
                }
            } while (index < ({
                register u8 *check asm("r1") = count_slot;
                asm volatile("" : "+r"(check));
                *check;
            }));
        }
    }
    return BATTLE_CANDIDATE_REMOVED;
}
