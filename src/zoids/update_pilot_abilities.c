#include "m2c_prelude.h"
#include "../game/player_state.h"

asm(".set sub_080E705C_entries, 0x087B79B0");
extern u8 gPilotAbilityScheduleBytes[] asm("sub_080E705C_entries");

#define PILOT_ABILITY_UPDATE_BYTES ((u8 *)0x02032E20)
#define PILOT_ABILITY_SCHEDULE ((struct PilotAbilityScheduleEntry *)gPilotAbilityScheduleBytes)

s32 UpdatePilotAbilities(struct PlayerPilotRecordView *pilot, u8 rebuild_all) asm("func_080E705C");

s32 UpdatePilotAbilities(struct PlayerPilotRecordView *pilot, u8 rebuild_all)
{
    u8 schedule_index;
    register u32 pilot_id asm("r2");
    s32 next_schedule_index;
    u8 ability_index;
    u8 free_ability_slot;
    s32 schedule_entry_offset;
    volatile s32 saved_rebuild_all;
    volatile s32 saved_schedule_index_times_two;
    s32 schedule_index_times_two;

    saved_rebuild_all = rebuild_all;
    if (rebuild_all != 0) {
        for (schedule_index = 0; schedule_index < PLAYER_PILOT_ABILITY_COUNT; schedule_index++) {
            pilot->ability_kinds[schedule_index] = 0;
        }
    }
    {
        register u32 initial_zero asm("r0") = 0;
        register u8 *initial_output asm("r5");

        asm volatile("" : "+r"(initial_zero));
        initial_output = PILOT_ABILITY_UPDATE_BYTES;
        *initial_output = initial_zero;
    }
    schedule_index = 0;
    do {
        {
        register s32 required_level asm("r0");
        register struct PilotAbilityScheduleEntry *schedule_entry asm("r4");
        register u32 schedule_table_address asm("r6");
        register s32 zero_offset asm("r5");

        schedule_index_times_two = schedule_index * 2;
        schedule_entry_offset = (schedule_index_times_two + schedule_index) * 2;
        asm volatile(
            "mov r5, %1\n"
            "ldrb %0, [r5]"
            : "=l"(pilot_id)
            : "h"(pilot)
            : "r5", "memory");
        schedule_entry_offset += pilot_id * sizeof(struct PilotAbilityScheduleRow);
        schedule_table_address = (u32)gPilotAbilityScheduleBytes;
        schedule_entry = (struct PilotAbilityScheduleEntry *)(schedule_entry_offset + schedule_table_address);
        zero_offset = 0;
        asm volatile("ldrsh %0, [%1, %2]"
                     : "=r"(required_level)
                     : "r"(schedule_entry), "r"(zero_offset));
        next_schedule_index = schedule_index + 1;
        saved_schedule_index_times_two = schedule_index_times_two;
        if (required_level == 0) goto next_schedule_entry;
        {
            register s32 scheduled_ability_kind asm("r0");

            asm volatile(
                "mov %0, %1\n"
                "add %0, #2\n"
                "add %0, %2, %0\n"
                "mov %1, #0\n"
                "ldrsh %0, [%0, %1]"
                : "=&r"(scheduled_ability_kind), "+r"(schedule_table_address)
                : "r"(schedule_entry_offset));
            if (scheduled_ability_kind == 0) goto next_schedule_entry;
        }
        if (saved_rebuild_all == 0) {
            /* The packed schedule still uses a native aligned halfword load. */
            if (M2C_FIELD(schedule_entry, s16 *, 0) == pilot->level) goto learn_scheduled_ability;
            goto next_schedule_entry;
        }
        {
            register s32 required_level_copy asm("r1");
            register s32 zero_offset_copy asm("r5");

            zero_offset_copy = 0;
            asm volatile("ldrsh %0, [%1, %2]"
                         : "=l"(required_level_copy)
                         : "l"(schedule_entry), "l"(zero_offset_copy)
                         : "memory");
            if (required_level_copy <= pilot->level) goto learn_scheduled_ability;
        }
        goto next_schedule_entry;
        }
learn_scheduled_ability:
        {
            register u8 *ability_kinds asm("r4");

            asm volatile(
                ".set e705c_dispatch_ldr_count, 0\n"
                ".macro e705c_install_dispatch_ldr\n"
                ".macro ldr dst, src:vararg\n"
                ".purgem ldr\n"
                ".if e705c_dispatch_ldr_count == 1\n"
                "ldr \\dst, .Le705c_pool\n"
                ".elseif e705c_dispatch_ldr_count == 2\n"
                "ldr \\dst, .Le705c_pool+4\n"
                ".else\n"
                "ldr \\dst, \\src\n"
                ".endif\n"
                ".set e705c_dispatch_ldr_count, e705c_dispatch_ldr_count + 1\n"
                ".if e705c_dispatch_ldr_count < 3\n"
                "e705c_install_dispatch_ldr\n"
                ".endif\n"
                ".endm\n"
                ".endm\n"
                "e705c_install_dispatch_ldr\n"
                ".macro mov dst, src\n"
                ".purgem mov\n"
                "mov r7, r9\n"
                "add r7, #1\n"
                "mov \\dst, \\src\n"
                ".align 2, 0\n"
                ".Le705c_pool:\n"
                ".word sub_080E705C_entries\n"
                ".word .Le705c_table\n"
                ".align 2, 0\n"
                ".align 2, 0\n"
                ".Le705c_table:\n"
                ".word .Le705c_case01\n"
                ".word .Le705c_case01\n"
                ".word .L30\n"
                ".word .L30\n"
                ".word .L30\n"
                ".word .L39\n"
                ".word .L39\n"
                ".word .L39\n"
                ".word .L48\n"
                ".word .L48\n"
                ".word .L48\n"
                ".word .L57\n"
                ".word .L57\n"
                ".word .L57\n"
                ".word .L65\n"
                ".word .L65\n"
                ".word .L125\n"
                ".word .L125\n"
                ".word .L125\n"
                ".word .L125\n"
                ".word .L73\n"
                ".word .L73\n"
                ".word .L125\n"
                ".word .L81\n"
                ".word .L13\n"
                ".word .L13\n"
                ".word .L90\n"
                ".word .L99\n"
                ".word .L108\n"
                ".word .L117\n"
                ".word .L125\n"
                ".word .L125\n"
                ".word .L81\n"
                ".word .L13\n"
                ".word .L13\n"
                ".word .L90\n"
                ".word .L99\n"
                ".word .L108\n"
                ".word .L117\n"
                ".pushsection .e705c_discard, \"\", %progbits\n"
                ".endm");
            switch ((s16)(({
                register u32 selector_row asm("r1");
                register u32 selector_work asm("r0");

                selector_row = saved_schedule_index_times_two + schedule_index;
                selector_row *= 2;
                selector_work = pilot_id * sizeof(struct PilotAbilityScheduleRow);
                selector_row += selector_work;
                selector_work = (u32)PILOT_ABILITY_SCHEDULE;
                selector_work += 2;
                selector_row += selector_work;
                selector_work = *(u16 *)selector_row;
                selector_work;
            }) - 1)) {
            case 0:
            case 1:
            {
                register u8 *ability_kinds_base asm("r2");
                register s32 zero asm("r3");
                u8 *ability_kind_address;

                asm volatile(".popsection\n.Le705c_case01:");
                ability_index = 0;
                ability_kinds = pilot->ability_kinds;
                asm volatile("mov %0, %1\nadd %0, #1"
                             : "=r"(next_schedule_index) : "r"(schedule_index));
                ability_kinds_base = ability_kinds;
                zero = 0;
                do {
                    ability_kind_address = &ability_kinds_base[ability_index];
                    if ((u8)(*ability_kind_address - 1) <= 1) *ability_kind_address = zero;
                    ability_index += 1;
                } while (ability_index < PLAYER_PILOT_ABILITY_COUNT);
                break;
            }
            case 2:
            case 3:
            case 4:
            {
                register s32 schedule_entry_row_offset asm("sl");
                register s16 *scheduled_ability_values asm("r8");
                u8 *ability_kind_address;

                ability_index = 0;
                ability_kinds = pilot->ability_kinds;
                asm volatile("mov %0, %1\nadd %0, #1"
                             : "=r"(next_schedule_index) : "r"(schedule_index));
                schedule_entry_row_offset = (saved_schedule_index_times_two + schedule_index) * 2;
                {
                    register s16 *schedule_values_seed asm("r6");

                    schedule_values_seed = (s16 *)0x087B79B4;
                    asm volatile("" : "+r"(schedule_values_seed));
                    scheduled_ability_values = schedule_values_seed;
                }
                do {
                    ability_kind_address = &ability_kinds[ability_index];
                    if ((u8)(*ability_kind_address - 3) <= 2 &&
                        (s16)pilot->ability_values[ability_index] ==
                            *(s16 *)((u8 *)scheduled_ability_values +
                                     (({
                                         register struct PlayerPilotRecordView *pilot_view
                                             asm("r0");

                                         pilot_view = pilot;
                                         asm volatile("" : "+r"(pilot_view));
                                         pilot_view->active_pilot_id;
                                     }) * sizeof(struct PilotAbilityScheduleRow) + schedule_entry_row_offset))) {
                        *ability_kind_address = 0;
                    }
                    ability_index += 1;
                } while (ability_index < PLAYER_PILOT_ABILITY_COUNT);
                break;
            }
            case 5:
            case 6:
            case 7:
            {
                register u8 *ability_kinds_base asm("r2");
                register s32 zero asm("r3");
                u8 *ability_kind_address;
                ability_index = 0;
                ability_kinds = pilot->ability_kinds;
                asm volatile("mov %0, %1\nadd %0, #1"
                             : "=r"(next_schedule_index) : "r"(schedule_index));
                ability_kinds_base = ability_kinds;
                zero = 0;
                do {
                    ability_kind_address = &ability_kinds_base[ability_index];
                    if ((u8)(*ability_kind_address - 6) <= 2) *ability_kind_address = zero;
                    ability_index += 1;
                } while (ability_index < PLAYER_PILOT_ABILITY_COUNT);
                break;
            }
            case 8:
            case 9:
            case 10:
            {
                register u8 *ability_kinds_base asm("r2");
                register s32 zero asm("r3");
                u8 *ability_kind_address;
                ability_index = 0;
                ability_kinds = pilot->ability_kinds;
                asm volatile("mov %0, %1\nadd %0, #1"
                             : "=r"(next_schedule_index) : "r"(schedule_index));
                ability_kinds_base = ability_kinds;
                zero = 0;
                do {
                    ability_kind_address = &ability_kinds_base[ability_index];
                    if ((u8)(*ability_kind_address - 9) <= 2) *ability_kind_address = zero;
                    ability_index += 1;
                } while (ability_index < PLAYER_PILOT_ABILITY_COUNT);
                break;
            }
            case 11:
            case 12:
            case 13:
            {
                register u8 *ability_kinds_base asm("r2");
                register s32 zero asm("r3");
                u8 *ability_kind_address;
                ability_index = 0;
                ability_kinds = pilot->ability_kinds;
                asm volatile("mov %0, %1\nadd %0, #1"
                             : "=r"(next_schedule_index) : "r"(schedule_index));
                ability_kinds_base = ability_kinds;
                zero = 0;
                do {
                    ability_kind_address = &ability_kinds_base[ability_index];
                    if ((u8)(*ability_kind_address - 12) <= 2) *ability_kind_address = zero;
                    ability_index += 1;
                } while (ability_index < PLAYER_PILOT_ABILITY_COUNT);
                break;
            }
            case 14:
            case 15:
            {
                register u8 *ability_kinds_base asm("r2");
                register s32 zero asm("r3");
                u8 *ability_kind_address;
                ability_index = 0;
                ability_kinds = pilot->ability_kinds;
                asm volatile("mov %0, %1\nadd %0, #1"
                             : "=r"(next_schedule_index) : "r"(schedule_index));
                ability_kinds_base = ability_kinds;
                zero = 0;
                do {
                    ability_kind_address = &ability_kinds_base[ability_index];
                    if ((u8)(*ability_kind_address - 15) <= 1) *ability_kind_address = zero;
                    ability_index += 1;
                } while (ability_index < PLAYER_PILOT_ABILITY_COUNT);
                break;
            }
            case 20:
            case 21:
            {
                register u8 *ability_kinds_base asm("r2");
                register s32 zero asm("r3");
                u8 *ability_kind_address;
                ability_index = 0;
                ability_kinds = pilot->ability_kinds;
                asm volatile("mov %0, %1\nadd %0, #1"
                             : "=r"(next_schedule_index) : "r"(schedule_index));
                ability_kinds_base = ability_kinds;
                zero = 0;
                do {
                    ability_kind_address = &ability_kinds_base[ability_index];
                    if ((u8)(*ability_kind_address - 21) <= 1) *ability_kind_address = zero;
                    ability_index += 1;
                } while (ability_index < PLAYER_PILOT_ABILITY_COUNT);
                break;
            }
            case 23:
            case 32:
            {
                register u8 *ability_kinds_base asm("r2");
                register s32 zero asm("r3");
                u8 *ability_kind_address;
                u8 existing_ability_kind;
                ability_index = 0;
                ability_kinds = pilot->ability_kinds;
                asm volatile("mov %0, %1\nadd %0, #1"
                             : "=r"(next_schedule_index) : "r"(schedule_index));
                ability_kinds_base = ability_kinds;
                zero = 0;
                do {
                    ability_kind_address = &ability_kinds_base[ability_index];
                    existing_ability_kind = *ability_kind_address;
                    if (existing_ability_kind == 24 || existing_ability_kind == 33) *ability_kind_address = zero;
                    ability_index += 1;
                } while (ability_index < PLAYER_PILOT_ABILITY_COUNT);
                break;
            }
            case 26:
            case 35:
            {
                register u8 *ability_kinds_base asm("r2");
                register s32 zero asm("r3");
                u8 *ability_kind_address;
                u8 existing_ability_kind;
                ability_index = 0;
                ability_kinds = pilot->ability_kinds;
                asm volatile("mov %0, %1\nadd %0, #1"
                             : "=r"(next_schedule_index) : "r"(schedule_index));
                ability_kinds_base = ability_kinds;
                zero = 0;
                do {
                    ability_kind_address = &ability_kinds_base[ability_index];
                    existing_ability_kind = *ability_kind_address;
                    if (existing_ability_kind == 27 || existing_ability_kind == 36) *ability_kind_address = zero;
                    ability_index += 1;
                } while (ability_index < PLAYER_PILOT_ABILITY_COUNT);
                break;
            }
            case 27:
            case 36:
            {
                register u8 *ability_kinds_base asm("r2");
                register s32 zero asm("r3");
                u8 *ability_kind_address;
                u8 existing_ability_kind;
                ability_index = 0;
                ability_kinds = pilot->ability_kinds;
                asm volatile("mov %0, %1\nadd %0, #1"
                             : "=r"(next_schedule_index) : "r"(schedule_index));
                ability_kinds_base = ability_kinds;
                zero = 0;
                do {
                    ability_kind_address = &ability_kinds_base[ability_index];
                    existing_ability_kind = *ability_kind_address;
                    if (existing_ability_kind == 28 || existing_ability_kind == 37) *ability_kind_address = zero;
                    ability_index += 1;
                } while (ability_index < PLAYER_PILOT_ABILITY_COUNT);
                break;
            }
            case 28:
            case 37:
            {
                register u8 *ability_kinds_base asm("r2");
                register s32 zero asm("r3");
                u8 *ability_kind_address;
                u8 existing_ability_kind;
                ability_index = 0;
                ability_kinds = pilot->ability_kinds;
                asm volatile("mov %0, %1\nadd %0, #1"
                             : "=r"(next_schedule_index) : "r"(schedule_index));
                ability_kinds_base = ability_kinds;
                zero = 0;
                do {
                    ability_kind_address = &ability_kinds_base[ability_index];
                    existing_ability_kind = *ability_kind_address;
                    if (existing_ability_kind == 29 || existing_ability_kind == 38) *ability_kind_address = zero;
                    ability_index += 1;
                } while (ability_index < PLAYER_PILOT_ABILITY_COUNT);
                break;
            }
            case 29:
            case 38:
            {
                register u8 *ability_kinds_base asm("r2");
                register s32 zero asm("r3");
                u8 *ability_kind_address;
                u8 existing_ability_kind;
                ability_index = 0;
                ability_kinds = pilot->ability_kinds;
                asm volatile("mov %0, %1\nadd %0, #1"
                             : "=r"(next_schedule_index) : "r"(schedule_index));
                ability_kinds_base = ability_kinds;
                zero = 0;
                do {
                    ability_kind_address = &ability_kinds_base[ability_index];
                    existing_ability_kind = *ability_kind_address;
                    if (existing_ability_kind == 30 || existing_ability_kind == 39) *ability_kind_address = zero;
                    ability_index += 1;
                } while (ability_index < PLAYER_PILOT_ABILITY_COUNT);
                break;
            }
            default:
            {
                register s32 schedule_entry_row_offset asm("r6");
                register u8 *ability_kinds_copy asm("r8");
                register s32 zero asm("sl");
                register s32 scheduled_kind_or_address asm("r0");
                register s32 schedule_kind_base_or_zero asm("r1");
                register u8 *ability_kind_address asm("r3");
                register s32 existing_ability_kind asm("r2");

                ability_index = 0;
                asm volatile("" : "+r"(ability_index));
                ability_kinds = pilot->ability_kinds;
                asm volatile("mov %0, %1\nadd %0, #1"
                             : "=r"(next_schedule_index) : "r"(schedule_index));
                ability_kinds_copy = ability_kinds;
                {
                    register s32 schedule_index_work asm("r0");
                    schedule_index_work = saved_schedule_index_times_two + schedule_index;
                    asm volatile("" : "+r"(schedule_index_work));
                    schedule_entry_row_offset = schedule_index_work * 2;
                }
                zero = ability_index;
                do {
                    ability_kind_address = &ability_kinds_copy[ability_index];
                    existing_ability_kind = *ability_kind_address;
                    scheduled_kind_or_address = pilot->active_pilot_id * sizeof(struct PilotAbilityScheduleRow);
                    scheduled_kind_or_address = schedule_entry_row_offset + scheduled_kind_or_address;
                    schedule_kind_base_or_zero = 0x087B79B2;
                    scheduled_kind_or_address += schedule_kind_base_or_zero;
                    schedule_kind_base_or_zero = 0;
                    asm volatile("ldrsh %0, [%0, %1]"
                                 : "+r"(scheduled_kind_or_address)
                                 : "r"(schedule_kind_base_or_zero));
                    if (existing_ability_kind == scheduled_kind_or_address) {
                        *ability_kind_address = zero;
                    }
                    ability_index += 1;
                } while (ability_index < PLAYER_PILOT_ABILITY_COUNT);
                break;
            }
            }
        free_ability_slot = 0;
        if (pilot->ability_kinds[0] != 0) {
            u8 *scan_ids;

            scan_ids = ability_kinds;
            do {
                free_ability_slot += 1;
                if (free_ability_slot > 9) goto next_schedule_entry;
            } while (scan_ids[free_ability_slot] != 0);
        }
        if (free_ability_slot < PLAYER_PILOT_ABILITY_COUNT) {
            s32 schedule_entry_row_offset;
            u16 learned_ability_kind;
            u16 learned_ability_value;
            register u8 *result_kind_address asm("r3");
            register u8 *update_results asm("r6");
            register u8 *result_kinds_base asm("r0");
            register u32 schedule_kind_address asm("r0");
            register u32 schedule_kinds_base asm("r1");
            register u32 ability_value_offset asm("r0");
            register u16 *ability_value_address asm("r4");
            register u8 *result_value_address asm("r3");
            register u8 *result_values_base asm("r0");
            u8 *free_ability_kind_address;

            free_ability_kind_address = &ability_kinds[free_ability_slot];
            result_kinds_base = &PILOT_ABILITY_UPDATE_BYTES[(s32)&((struct PilotAbilityUpdateResults *)0)->kinds];
            asm volatile("" : "+r"(result_kinds_base));
            update_results = PILOT_ABILITY_UPDATE_BYTES;
            result_kind_address = (u8 *)(u32)update_results[0];
            result_kind_address += (u32)result_kinds_base;
            schedule_entry_row_offset = (saved_schedule_index_times_two + schedule_index) * 2;
            schedule_kind_address = pilot->active_pilot_id * sizeof(struct PilotAbilityScheduleRow);
            schedule_kind_address = schedule_entry_row_offset + schedule_kind_address;
            asm volatile(
                ".macro ldr dst, src\n"
                ".purgem ldr\n"
                ".set e705c_entries_pool, \\src\n"
                "ldr \\dst, \\src\n"
                ".endm");
            schedule_kinds_base = (u32)PILOT_ABILITY_SCHEDULE;
            schedule_kinds_base += 2;
            schedule_kind_address += schedule_kinds_base;
            learned_ability_kind = *(u16 *)schedule_kind_address;
            *result_kind_address = (u8)learned_ability_kind;
            *free_ability_kind_address = (u8)learned_ability_kind;
            ability_value_offset = free_ability_slot << 1;
            ability_value_address = pilot->ability_values;
            ability_value_address = (u16 *)((u8 *)ability_value_address + ability_value_offset);
            result_values_base = &PILOT_ABILITY_UPDATE_BYTES[(s32)&((struct PilotAbilityUpdateResults *)0)->values];
            asm volatile("" : "+r"(result_values_base));
            result_value_address = (u8 *)(u32)update_results[0];
            result_value_address += (u32)result_values_base;
            asm volatile(
                "mov r5, %2\n"
                "ldrb r1, [r5]\n"
                "lsl %0, r1, #4\n"
                "sub %0, %0, r1\n"
                "lsl %0, %0, #2\n"
                "add %1, %1, %0\n"
                "ldr %0, e705c_entries_pool\n"
                "add %0, #4\n"
                "add %1, %1, %0\n"
                "ldrh %0, [%1]"
                : "=&l"(learned_ability_value), "+l"(schedule_entry_row_offset)
                : "h"(pilot)
                : "r1", "r5", "cc", "memory");
            *result_value_address = (u8)learned_ability_value;
            *ability_value_address = (u8)learned_ability_value;
            update_results[0] += 1;
        }
        }
next_schedule_entry:
        schedule_index = next_schedule_index;
    } while (schedule_index < PLAYER_PILOT_ABILITY_COUNT);
    {
        register u8 *update_results_copy asm("r6");

        update_results_copy = PILOT_ABILITY_UPDATE_BYTES;
        if (update_results_copy[0] != 0) {
            return 1;
        }
    }
    return 0;
}
