#include "m2c_prelude.h"
#include "../game/player_state.h"

s32 AddPulseEffect(u8, s16) asm("func_080E7868");

s32 UpdatePulseEffectsFromEmotionGrowth(void) asm("func_080E79F4");

s32 UpdatePulseEffectsFromEmotionGrowth(void)
{
    u8 * volatile saved_learned_effect_count;
    register u8 *learned_effect_count asm("r3");
    register u32 emotion_index asm("r6");
    register u8 *growth_schedule_bytes asm("r9");

    {
        register u8 *initial_learned_effect_count asm("r2") = (u8 *)PULSE_EFFECT_UPDATE_RESULTS_RAM;
        register u32 zero asm("r1") = 0;

        *initial_learned_effect_count = zero;
        emotion_index = 0;
        growth_schedule_bytes = (u8 *)PULSE_EFFECT_GROWTH_TABLE_ROM;
        learned_effect_count = initial_learned_effect_count;
    }
    do {
        u8 schedule_entry_index = 0;
        register u32 emotion_index_times_16 asm("r8");
        register u32 next_emotion_index asm("sl");

        {
            register u32 emotion_scale_carrier asm("r1") = emotion_index << 4;
            emotion_index_times_16 = emotion_scale_carrier;
        }
        {
            register u32 next_emotion_carrier asm("r2") = emotion_index + 1;
            next_emotion_index = next_emotion_carrier;
        }
        do {
            register u32 schedule_entry_offset asm("r1");
            register u8 *growth_schedule_entry asm("r2");
            register s32 growth_threshold asm("r0");

            {
                register u32 entry_byte_offset asm("r0") = schedule_entry_index << 1;
                register u32 emotion_row_byte_offset asm("r1");
                register u32 emotion_index_scale_carrier asm("r4") = emotion_index_times_16;

                entry_byte_offset += schedule_entry_index;
                entry_byte_offset <<= 1;
                emotion_row_byte_offset = emotion_index_scale_carrier - emotion_index;
                emotion_row_byte_offset <<= 2;
                schedule_entry_offset = entry_byte_offset + emotion_row_byte_offset;
            }
            growth_schedule_entry = (u8 *)(schedule_entry_offset + (u32)growth_schedule_bytes);
            growth_threshold = *(s16 *)growth_schedule_entry;
            if (growth_threshold != 0) {
                register s32 growth_threshold_copy asm("r2") = growth_threshold;
                register u8 *emotion_growth_base asm("r4") = (u8 *)(PULSE_PLAYER_RECORD_RAM + PLAYER_AUXILIARY_PILOT_OFFSET(processed_emotion_growth));
                register u8 *emotion_growth_address asm("r0");

                asm volatile("" : "+r"(emotion_growth_base));
                emotion_growth_address = emotion_growth_base + emotion_index;
                if (growth_threshold_copy > *emotion_growth_address) {
                    emotion_growth_base = (u8 *)(PULSE_PLAYER_RECORD_RAM + PLAYER_AUXILIARY_PILOT_OFFSET(emotion_growth));
                    asm volatile("" : "+r"(emotion_growth_base));
                    emotion_growth_address = emotion_growth_base + emotion_index;
                    if (growth_threshold_copy <= *emotion_growth_address) {
                        register u8 *scheduled_kind_address asm("r4");
                        register u8 *scheduled_value_address asm("r5");
                        register u8 *scheduled_kind_base asm("r0") = growth_schedule_bytes + PULSE_SCHEDULE_ENTRY_OFFSET(kind);
                        register u8 *scheduled_value_base asm("r2");
                        register u32 scheduled_kind asm("r0");
                        register s32 scheduled_value asm("r1");
                        register s32 effect_added asm("r0");

                        asm volatile("" : "+r"(scheduled_kind_base));
                        scheduled_kind_address = (u8 *)(schedule_entry_offset + (u32)scheduled_kind_base);
                        scheduled_kind = *scheduled_kind_address;
                        scheduled_value_base = (u8 *)(PULSE_EFFECT_GROWTH_TABLE_ROM + PULSE_SCHEDULE_ENTRY_OFFSET(value));
                        scheduled_value_address = (u8 *)(schedule_entry_offset + (u32)scheduled_value_base);
                        scheduled_value = *(s16 *)scheduled_value_address;
                        saved_learned_effect_count = learned_effect_count;
                        effect_added = AddPulseEffect(scheduled_kind, scheduled_value);
                        effect_added <<= 24;
                        learned_effect_count = saved_learned_effect_count;
                        if (effect_added != 0) {
                            register u8 *learned_effect_output asm("r0");
                            register u32 learned_effect_address_or_value asm("r1");

                            learned_effect_address_or_value = (PULSE_EFFECT_UPDATE_RESULTS_RAM + PULSE_EFFECT_UPDATE_OFFSET(kinds));
                            asm volatile("" : "+r"(learned_effect_address_or_value));
                            learned_effect_output = (u8 *)(u32)*learned_effect_count;
                            learned_effect_output += learned_effect_address_or_value;
                            learned_effect_address_or_value = *(u16 *)scheduled_kind_address;
                            *learned_effect_output = learned_effect_address_or_value;
                            learned_effect_address_or_value = (PULSE_EFFECT_UPDATE_RESULTS_RAM + PULSE_EFFECT_UPDATE_OFFSET(values));
                            asm volatile("" : "+r"(learned_effect_address_or_value));
                            learned_effect_output = (u8 *)((u32)*learned_effect_count << 1);
                            learned_effect_output += learned_effect_address_or_value;
                            learned_effect_address_or_value = *(u16 *)scheduled_value_address;
                            *(u16 *)learned_effect_output = learned_effect_address_or_value;
                            (*learned_effect_count)++;
                        }
                    }
                }
            }
            {
                register u32 next_schedule_entry_index asm("r0") = schedule_entry_index + 1;
                schedule_entry_index = (u8)next_schedule_entry_index;
            }
        } while (schedule_entry_index <= PLAYER_PILOT_ABILITY_COUNT - 1);

        {
            register u8 *processed_growth_base asm("r4") = (u8 *)(PULSE_PLAYER_RECORD_RAM + PLAYER_AUXILIARY_PILOT_OFFSET(processed_emotion_growth));
            register u8 *processed_growth_address asm("r1");
            register u8 *current_growth_base asm("r2");
            register u8 *current_growth_address asm("r0");
            register u32 processed_growth_value asm("r0");

            asm volatile("" : "+r"(processed_growth_base));
            processed_growth_address = processed_growth_base;
            processed_growth_address += emotion_index;
            asm volatile("" : "+r"(processed_growth_address));
            current_growth_base = (u8 *)(PULSE_PLAYER_RECORD_RAM + PLAYER_AUXILIARY_PILOT_OFFSET(emotion_growth));
            asm volatile("" : "+r"(current_growth_base));
            current_growth_address = current_growth_base;
            current_growth_address += emotion_index;
            asm volatile("" : "+r"(current_growth_address));
            processed_growth_value = *current_growth_address;
            *processed_growth_address = processed_growth_value;
        }
        {
            register u32 next_emotion_carrier asm("r4") = next_emotion_index;
            register u32 next_emotion_index_shifted asm("r0") = next_emotion_carrier << 24;

            emotion_index = next_emotion_index_shifted >> 24;
        }
    } while (emotion_index <= PULSE_EMOTION_COUNT - 1);

    if (*(u8 *)PULSE_EFFECT_UPDATE_RESULTS_RAM != 0) {
        return 1;
    }
    return 0;
}
