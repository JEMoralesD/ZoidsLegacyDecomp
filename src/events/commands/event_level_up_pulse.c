#include "m2c_prelude.h"
#include "../../game/game_state.h"
#include "../../game/player_state.h"
#include "../event_script.h"

void PlayOrContinueSong(u8) asm("func_08092E74");
void PlaySong(s32) asm("func_08092E84");
void StopSong(u8) asm("func_08092EA0");
s32 CreateSprite(s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484");
void DestroySprite() asm("func_08094554");
void RequestWindowRefresh(void) asm("func_080972C8");
void PrintWindowTextAt(s32, s32, s32, s32, s32) asm("func_080981F0");
void FormatNumberText(u8, s32, s32, s32) asm("func_08098284");
void PrintWindowNumberAt(s32, s32, s32, s32, s32, s32, s32) asm("func_0809844C");
M2C_UNK PrintWindowNumberAtWithStackArguments(s32, s32, s32, s32) asm("func_0809844C");
asm(
    ".macro A4D8C_REWARD_STACK three, five, eight\n"
    "str \\five, [sp]\n"
    "str \\eight, [sp, #4]\n"
    "movs \\three, #3\n"
    "str \\three, [sp, #8]\n"
    ".endm\n");
asm(
    ".macro A4D8C_ADD_AND_FIX_REWARD_LOAD dst, base\n"
    "add \\dst, \\base\n"
    ".macro mov args:vararg\n"
    ".short 0x2200\n"
    ".purgem mov\n"
    ".endm\n"
    ".macro ldrsh args:vararg\n"
    ".short 0x5E80\n"
    ".purgem ldrsh\n"
    ".endm\n"
    ".endm\n");
asm(
    ".macro A4D8C_SHIFT_AND_FIX_THIRD_UPDATE dst\n"
    "lsl \\dst, \\dst, #1\n"
    ".macro ldr out, addr\n"
    ".purgem ldr\n"
    "ldr r1, \\addr\n"
    ".endm\n"
    ".macro add out, lhs, rhs\n"
    ".purgem add\n"
    "add r0, r0, r1\n"
    ".endm\n"
    ".endm\n");
asm(
    ".macro A4D8C_INIT_AND_FIX_ITEM_LOOP index\n"
    "mov \\index, #0\n"
    ".set a4d8c_item_ldr_count, 0\n"
    ".macro ldr args:vararg\n"
    ".if a4d8c_item_ldr_count == 0\n"
    ".short 0x483C\n"
    ".elseif a4d8c_item_ldr_count == 1\n"
    ".short 0x483A\n"
    ".macro mov args:vararg\n"
    ".short 0x4680\n"
    ".purgem mov\n"
    ".endm\n"
    ".elseif a4d8c_item_ldr_count == 2\n"
    ".short 0x4C35\n"
    ".else\n"
    ".short 0x482E\n"
    ".purgem ldr\n"
    ".macro add args:vararg\n"
    ".short 0x1828\n"
    ".purgem add\n"
    ".endm\n"
    ".endif\n"
    ".set a4d8c_item_ldr_count, a4d8c_item_ldr_count + 1\n"
    ".endm\n"
    ".endm\n");
void ClearWindow(s32) asm("func_080986B4");
u8 CountEncodedTextGlyphs(s32) asm("func_08098B58");
void RunMenuScript(s32) asm("func_08098BB4");
void QueuePilotPortraitGraphics(u16, s32, u8, s32, s32, s32) asm("func_0809A9C8");
void SeekEventCommand(s32, s32, s32) asm("func_080A016C");
s32 UpdatePulseEffectsFromEmotionGrowth(void) asm("func_080E79F4");
s32 ModuloUnsigned32(s32, s32) asm("func_080ECF78");
void CopyBytes(void *, s32, s32) asm("func_080ED038");
void CopyString(void *, s32) asm("func_080ED128");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 EventLevelUpPulse(u8 script_slot) asm("func_080A4D8C");

s32 EventLevelUpPulse(u8 script_slot)
{
    s32 saved_script_slot;
    u8 *pulse_record_bytes;

    saved_script_slot = script_slot;
    *(u8 *)FIELD_EVENT_ACTIVE_RAM = 1;
    pulse_record_bytes = (u8 *)PULSE_PLAYER_RECORD_RAM;
    asm volatile("" : "+r"(pulse_record_bytes));
    if (pulse_record_bytes[0] != 0 && ({
            register u32 pulse_level_bonus asm("r1") = pulse_record_bytes[PLAYER_AUXILIARY_PILOT_OFFSET(level_bonus)];
            pulse_level_bonus;
        }) <= PLAYER_PILOT_LEVEL_LIMIT - 1) {
        register u32 four asm("r9");
        register u32 growth_window_id asm("r5");
        register u32 eight asm("r6");
        register s16 *hp_recovery_growth_table asm("r4");
        register s16 *speed_growth_table asm("sl");
        register s16 *defense_growth_table asm("r8");
        u8 previous_menu_state;
        u32 field_display_flags;
        u16 portrait_pilot_id;
        u8 portrait_palette_variant;
        register u16 *level_up_message asm("r6");
        register u32 level_number_glyph_count asm("r5");

        StopSong(*(u8 *)0x02030667);
        PlaySong(EVENT_PULSE_LEVEL_UP_SOUND);
        if (*(s32 *)0x02031744 != 0) {
            DestroySprite();
            *(s32 *)0x02031744 = 0;
        }
        if (*(s32 *)0x02021690 != GAME_MODE_BATTLE_SCENE || *(u8 *)0x02031748 != 0) {
            previous_menu_state = *(u8 *)0x02030666;
            if (previous_menu_state == 0) {
                goto open_menu;
            }
            if (previous_menu_state == 1) {
                RunMenuScript(0x080177F5);
open_menu:
                RunMenuScript(0x080177ED);
                *(u8 *)0x02030666 = 2;
            }
        }

        field_display_flags = *(u8 *)0x020324B0;
        {
            register u32 four_seed asm("r0") = 4;
            four = four_seed;
            four_seed &= field_display_flags;
            if (four_seed != 0) {
                *(u8 *)0x020314A4 = 0xF;
            }
        }
        {
            register u16 *auxiliary_pilot_id_table asm("r1") = (u16 *)PULSE_AUXILIARY_PILOT_IDS_ROM;
            asm volatile("" : "+r"(auxiliary_pilot_id_table));
            portrait_pilot_id = auxiliary_pilot_id_table[pulse_record_bytes[0]];
        }
        portrait_palette_variant = 0;
        if (portrait_pilot_id == PULSE_PILOT_ID) {
            portrait_palette_variant = pulse_record_bytes[PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)];
        }
        QueuePilotPortraitGraphics(portrait_pilot_id, 0, portrait_palette_variant, 0x3C2, 0xE, 0x02002880);

        *(s32 *)0x02031744 = CreateSprite(0x08359850, 0x0835985C,
            0, 0x18, 0x18, 0x3C2, 0xE,
            ({ eight = 8; eight; }), 0);
        RunMenuScript(EVENT_PULSE_STAT_GROWTH_MENU);

        PrintWindowNumberAt(M2C_FIELD(pulse_record_bytes, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(hp_recovery_percent)), 3, 0, 0xA,
            ({ growth_window_id = 5; growth_window_id; }), four, 0);
        PrintWindowNumberAt(M2C_FIELD(pulse_record_bytes, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(weapon_power_bonus_percent)), 3, 0, 0xA, growth_window_id, four, 1);
        PrintWindowNumberAt(M2C_FIELD(pulse_record_bytes, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(sensor_accuracy_bonus_percent)), 3, 0, 0xA, growth_window_id, four, 2);
        PrintWindowNumberAt(M2C_FIELD(pulse_record_bytes, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(speed_bonus_percent)), 3, 0, 0xA, growth_window_id, four, 3);
        PrintWindowNumberAt(M2C_FIELD(pulse_record_bytes, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(defense_bonus_percent)), 3, 0, 0xA, growth_window_id, four, four);

        hp_recovery_growth_table = (s16 *)PULSE_STAT_GROWTH_TABLE_ROM;
        PrintWindowNumberAt(M2C_FIELD((pulse_record_bytes[PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)] * (s32)sizeof(struct PulseStatGrowthBonuses)), s16 *, (u32)hp_recovery_growth_table),
            3, 0, 0xA, growth_window_id, eight, 0);
        PrintWindowNumberAt(M2C_FIELD((pulse_record_bytes[PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)] * (s32)sizeof(struct PulseStatGrowthBonuses)), s16 *, (PULSE_STAT_GROWTH_TABLE_ROM + PULSE_STAT_GROWTH_OFFSET(weapon_power_bonus_percent))),
            3, 0, 0xA, growth_window_id, eight, 1);
        PrintWindowNumberAt(M2C_FIELD((pulse_record_bytes[PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)] * (s32)sizeof(struct PulseStatGrowthBonuses)), s16 *, (PULSE_STAT_GROWTH_TABLE_ROM + PULSE_STAT_GROWTH_OFFSET(sensor_accuracy_bonus_percent))),
            3, 0, 0xA, growth_window_id, eight, 2);
        {
            register s32 speed_growth_bonus asm("r0");
            register s32 digit_width asm("r1");

            speed_growth_bonus = ({
                register u32 speed_growth_address asm("r0");
                {
                    register u32 speed_growth_variant asm("r1") = pulse_record_bytes[PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)];
                    speed_growth_address = speed_growth_variant << 2;
                    speed_growth_address += speed_growth_variant;
                    speed_growth_address <<= 1;
                }
                {
                    register u32 speed_growth_table_base asm("r1") = (u32)hp_recovery_growth_table + PULSE_STAT_GROWTH_OFFSET(speed_bonus_percent);
                    speed_growth_table = (s16 *)speed_growth_table_base;
                }
                asm volatile("A4D8C_ADD_AND_FIX_REWARD_LOAD %0, %1"
                             : "+r"(speed_growth_address)
                             : "r"(speed_growth_table)
                             : "cc");
                *(s16 *)speed_growth_address;
            });
            asm volatile("A4D8C_REWARD_STACK %0, %1, %2"
                         : "=r"(digit_width) : "r"(growth_window_id), "r"(eight) : "memory");
            PrintWindowNumberAtWithStackArguments(speed_growth_bonus, digit_width, 0, 0xA);
        }
        PrintWindowNumberAt(({
            register u32 defense_growth_address asm("r0");
            {
                register u32 defense_growth_variant asm("r1") = pulse_record_bytes[PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)];
                defense_growth_address = defense_growth_variant << 2;
                defense_growth_address += defense_growth_variant;
                defense_growth_address <<= 1;
            }
            {
                register u32 defense_growth_table_base asm("r2") = PULSE_STAT_GROWTH_OFFSET(defense_bonus_percent);
                defense_growth_table_base += (u32)hp_recovery_growth_table;
                defense_growth_table = (s16 *)defense_growth_table_base;
            }
            defense_growth_address += (u32)defense_growth_table;
            *(s16 *)defense_growth_address;
        }),
            3, 0, 0xA, growth_window_id, eight, four);

        {
            u8 *pulse_level_bonus_address = (u8 *)(PULSE_PLAYER_RECORD_RAM + PLAYER_AUXILIARY_PILOT_OFFSET(level_bonus));
            asm volatile("" : "+r"(pulse_level_bonus_address));
            *pulse_level_bonus_address += 1;
        }
        {
            register u32 growth_variant_or_table_or_value asm("r1") = pulse_record_bytes[PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)];
            register u32 growth_address_or_total asm("r0");
            register u32 current_bonus_value asm("r2");

            growth_address_or_total = growth_variant_or_table_or_value << 2;
            growth_address_or_total += growth_variant_or_table_or_value;
            growth_address_or_total <<= 1;
            growth_address_or_total += (u32)hp_recovery_growth_table;
            growth_address_or_total = *(u16 *)growth_address_or_total;
            current_bonus_value = M2C_FIELD(pulse_record_bytes, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(hp_recovery_percent));
            growth_address_or_total += current_bonus_value;
            M2C_FIELD(pulse_record_bytes, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(hp_recovery_percent)) = (u16)growth_address_or_total;

            growth_address_or_total = growth_variant_or_table_or_value << 2;
            growth_address_or_total += growth_variant_or_table_or_value;
            growth_address_or_total <<= 1;
            growth_variant_or_table_or_value = (PULSE_STAT_GROWTH_TABLE_ROM + PULSE_STAT_GROWTH_OFFSET(weapon_power_bonus_percent));
            growth_address_or_total += growth_variant_or_table_or_value;
            growth_address_or_total = *(u16 *)growth_address_or_total;
            current_bonus_value = M2C_FIELD(pulse_record_bytes, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(weapon_power_bonus_percent));
            growth_address_or_total += current_bonus_value;
            M2C_FIELD(pulse_record_bytes, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(weapon_power_bonus_percent)) = (u16)growth_address_or_total;
        }
        {
            register u32 growth_variant_or_table_or_value asm("r1") = pulse_record_bytes[PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)];
            register u32 growth_address_or_total asm("r0");
            register u32 current_bonus_value asm("r2");

            growth_address_or_total = growth_variant_or_table_or_value << 2;
            growth_address_or_total += growth_variant_or_table_or_value;
            asm volatile("A4D8C_SHIFT_AND_FIX_THIRD_UPDATE %0"
                         : "+r"(growth_address_or_total));
            growth_variant_or_table_or_value = (PULSE_STAT_GROWTH_TABLE_ROM + PULSE_STAT_GROWTH_OFFSET(sensor_accuracy_bonus_percent));
            growth_address_or_total += growth_variant_or_table_or_value;
            growth_address_or_total = *(u16 *)growth_address_or_total;
            current_bonus_value = M2C_FIELD(pulse_record_bytes, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(sensor_accuracy_bonus_percent));
            growth_address_or_total += current_bonus_value;
            M2C_FIELD(pulse_record_bytes, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(sensor_accuracy_bonus_percent)) = (u16)growth_address_or_total;

            growth_variant_or_table_or_value = *(volatile u8 *)(pulse_record_bytes + PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant));
            growth_address_or_total = growth_variant_or_table_or_value << 2;
            asm volatile("add %0, %0, %1"
                         : "+r"(growth_address_or_total)
                         : "r"(growth_variant_or_table_or_value));
            growth_address_or_total <<= 1;
            growth_address_or_total += (u32)speed_growth_table;
            growth_address_or_total = *(u16 *)growth_address_or_total;
            growth_variant_or_table_or_value = M2C_FIELD(pulse_record_bytes, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(speed_bonus_percent));
            growth_address_or_total += growth_variant_or_table_or_value;
            M2C_FIELD(pulse_record_bytes, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(speed_bonus_percent)) = (u16)growth_address_or_total;
        }
        {
            register u32 growth_variant_or_table_or_value asm("r1") = pulse_record_bytes[PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)];
            register u32 growth_address_or_total asm("r0");
            register u32 current_bonus_value asm("r2");

            growth_address_or_total = growth_variant_or_table_or_value << 2;
            growth_address_or_total += growth_variant_or_table_or_value;
            growth_address_or_total <<= 1;
            growth_address_or_total += (u32)defense_growth_table;
            growth_address_or_total = *(u16 *)growth_address_or_total;
            current_bonus_value = M2C_FIELD(pulse_record_bytes, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(defense_bonus_percent));
            growth_address_or_total += current_bonus_value;
            M2C_FIELD(pulse_record_bytes, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(defense_bonus_percent)) = (u16)growth_address_or_total;
        }

        PrintWindowNumberAt(M2C_FIELD(pulse_record_bytes, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(hp_recovery_percent)), 3, 0, 0xA, growth_window_id, 0xC, 0);
        PrintWindowNumberAt(M2C_FIELD(pulse_record_bytes, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(weapon_power_bonus_percent)), 3, 0, 0xA, growth_window_id, 0xC, 1);
        PrintWindowNumberAt(M2C_FIELD(pulse_record_bytes, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(sensor_accuracy_bonus_percent)), 3, 0, 0xA, growth_window_id, 0xC, 2);
        PrintWindowNumberAt(M2C_FIELD(pulse_record_bytes, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(speed_bonus_percent)), 3, 0, 0xA, growth_window_id, 0xC, 3);
        PrintWindowNumberAt(M2C_FIELD(pulse_record_bytes, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(defense_bonus_percent)), 3, 0, 0xA, growth_window_id, 0xC, four);

        level_up_message = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        {
            register u32 text_header asm("r0") = 0x201;
            level_up_message[0] = text_header;
        }
        CopyBytes(level_up_message + 1, 0x08103D98, 0x1D);
        level_up_message[15] = 1;
        CopyBytes(level_up_message + 16, 0x08103DB8, 0x11);
        CopyBytes(level_up_message + 24, 0x08103DCC, 7);
        FormatNumberText(({
                register u8 *pulse_level_bonus_read asm("r1") = (u8 *)(PULSE_PLAYER_RECORD_RAM + PLAYER_AUXILIARY_PILOT_OFFSET(level_bonus));
                *pulse_level_bonus_read;
            }), 2, 0, 0x02030564);
        {
            register u32 text_footer asm("r0") = 0x101;
            level_up_message[27] = text_footer;
        }
        CopyString(level_up_message + 28, 0x02030564);
        level_number_glyph_count = CountEncodedTextGlyphs(0x02030564);
        {
            register u32 mark_addr asm("r0") = level_number_glyph_count;
            mark_addr += 28;
            mark_addr <<= 1;
            mark_addr += (u32)level_up_message;
            *(u16 *)mark_addr = ({
                register u32 mark_value asm("r2") = 1;
                mark_value;
            });
        }
        {
            register u32 suffix_addr asm("r0") = level_number_glyph_count << 1;
            register u32 suffix_base asm("r1") = (u32)level_up_message;
            suffix_base += 58;
            suffix_addr += suffix_base;
            CopyBytes((void *)suffix_addr, 0x08103DD4, 3);
        }
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = level_up_message;
        RunMenuScript(0x080177D5);

        if ((UpdatePulseEffectsFromEmotionGrowth() << 24) != 0) {
            register u32 learned_effect_index asm("r5");

            RunMenuScript(EVENT_PULSE_LEARNED_EFFECTS_MENU);
            asm volatile("A4D8C_INIT_AND_FIX_ITEM_LOOP %0" : "=r"(learned_effect_index));
            if (learned_effect_index < *(u8 *)PULSE_EFFECT_UPDATE_RESULTS_RAM) {
                register s32 *auxiliary_effect_name_table asm("r8") = (s32 *)EVENT_PULSE_EFFECT_NAME_POINTERS_ROM;
                do {
                    u32 next_learned_effect_index;
                    s32 learned_effect_name;

                    next_learned_effect_index = learned_effect_index + 1;
                    if (learned_effect_index != 0 && ModuloUnsigned32(learned_effect_index, 3) == 0) {
                        RequestWindowRefresh();
                        do {
                            YieldTaskForUpdates(1);
                        } while (!(3 & *(volatile u16 *)0x0300000E));
                        PlaySong(EVENT_PULSE_CONFIRM_SOUND);
                        ClearWindow(5);
                    }
                    learned_effect_name = auxiliary_effect_name_table[*(u8 *)((PULSE_EFFECT_UPDATE_RESULTS_RAM + PULSE_EFFECT_UPDATE_OFFSET(kinds)) + learned_effect_index)];
                    PrintWindowTextAt(learned_effect_name, 0, 5, 0,
                        (s32)(s16)(ModuloUnsigned32(learned_effect_index, 3) * 2));
                    learned_effect_index = next_learned_effect_index;
                } while (learned_effect_index < *(u8 *)PULSE_EFFECT_UPDATE_RESULTS_RAM);
            }
            RequestWindowRefresh();
            do {
                YieldTaskForUpdates(1);
            } while (!(3 & *(volatile u16 *)0x0300000E));
            PlaySong(EVENT_PULSE_CONFIRM_SOUND);
        }

        DestroySprite(*(s32 *)0x02031744);
        *(s32 *)0x02031744 = 0;
        *(u8 *)0x020314A4 = 0x5F;
        RunMenuScript(EVENT_PULSE_CLOSE_LEVEL_UP_MENU);
        StopSong(EVENT_PULSE_LEVEL_UP_SOUND);
        PlayOrContinueSong(*(u8 *)0x02030667);
    }
    SeekEventCommand(saved_script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
