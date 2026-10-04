#include "../../field/field_actor.h"
#include "../../game/game_state.h"
#include "../../game/player_state.h"
#include "../event_script.h"

M2C_UNK PlayOrContinueSong(u8) asm("func_08092E74");                          /* extern */
M2C_UNK PlaySong(s32) asm("func_08092E84");                         /* extern */
M2C_UNK StopSong(u8) asm("func_08092EA0");                          /* extern */
s32 CreateSprite(M2C_UNK, M2C_UNK, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484"); /* extern */
M2C_UNK DestroySprite() asm("func_08094554");                            /* extern */
M2C_UNK QueueCopy(s32, s32, s32) asm("func_08095208");               /* extern */
M2C_UNK PrintWindowTextAt(M2C_UNK, s32, s32, s32) asm("func_080981F0");      /* extern */
M2C_UNK PrintWindowNumberAt(u8, s32, s32, s32, s32, s32, s32) asm("func_0809844C"); /* extern */
M2C_UNK PrintWindowNumberAtWithStackArguments(s32, s32, s32, s32) asm("func_0809844C");
M2C_UNK RunMenuScript(M2C_UNK) asm("func_08098BB4");                     /* extern */
M2C_UNK QueuePilotPortraitGraphics(u16, s32, s32, s32, s32, s32) asm("func_0809A9C8"); /* extern */
M2C_UNK SeekEventCommand(s32, s32, s32) asm("func_080A016C");               /* extern */

s32 EventOfferPulseEmotionGrowth(s32 script_slot_word, s32 *command_cursor) asm("func_080A4A2C");

s32 EventOfferPulseEmotionGrowth(s32 script_slot_word, s32 *command_cursor) {
    volatile s32 saved_script_slot;
    /* All-zero resulting growth leaves the native selected-emotion stack slot untouched. */
    volatile s32 selected_emotion_index;
    u8 * volatile current_emotion_growth;
    register s32 next_growth_row asm("r4");
    s16 growth_row;
    register s32 next_emotion_index asm("r6");
    s32 branch_opcode;
    s32 portrait_palette_variant;
    register s32 growth_row_highlight asm("r8");
    register s32 seek_script_slot asm("r0");
    u16 portrait_pilot_id;
    register u32 largest_proposed_growth asm("r2");
    register u32 displayed_emotion_index asm("r5");
    u8 *current_growth_address;
    u8 previous_menu_state;
    u8 displayed_growth_value;
    u32 proposed_growth_clipped;
    register u32 emotion_index asm("r5");
    void *pulse_field_actor;

    register s32 *cursor_carrier asm("sl");
    asm volatile("mov %0, %2" : "=r"(cursor_carrier), "+r"(script_slot_word) : "r"(command_cursor));
    script_slot_word <<= 24;
    script_slot_word = (u32)script_slot_word >> 24;
    saved_script_slot = script_slot_word;
    *(s8 *)FIELD_EVENT_ACTIVE_RAM = 1;
    {
    register u8 *pulse_record_bytes asm("r1") = (u8 *)PULSE_PLAYER_RECORD_RAM;
    asm volatile("" : "+r"(pulse_record_bytes));
    if (*pulse_record_bytes == 0) {
        goto pulse_absent;
    }
    }
    {
        StopSong(*(u8 *)0x02030667);
        PlaySong(EVENT_PULSE_GROWTH_SOUND);
        if (*(s32 *)0x02031744 != 0) {
            DestroySprite();
            *(s32 *)0x02031744 = 0;
        }
        if ((*(s32 *)0x02021690 != GAME_MODE_BATTLE_SCENE) || (*(u8 *)0x02031748 != 0)) {
            register u8 *menu_state asm("r4") = (u8 *)0x02030666;
            asm volatile("" : "+r"(menu_state));
            previous_menu_state = *menu_state;
            if (previous_menu_state != 0) {
                goto close_previous_text_menu;
            }
            goto open_growth_popup_menu;
update_pulse_actor_palette:
            {
            register s32 *pulse_palette_pointer_table asm("r1") = (s32 *)PULSE_FIELD_PALETTE_POINTERS_ROM;
            register s32 selected_emotion_carrier asm("r3") = selected_emotion_index;
            register s32 pulse_actor_palette asm("r0");
            register u32 palette_pointer_offset asm("r0");
            asm volatile("" : "+r"(pulse_palette_pointer_table));
            asm volatile("" : "+r"(selected_emotion_carrier));
            palette_pointer_offset = (u32)selected_emotion_carrier << 2;
            asm volatile("" : "+r"(palette_pointer_offset));
            asm volatile(".syntax unified\n\tadds %0, %0, %1\n\t.syntax divided"
                         : "+r"(palette_pointer_offset) : "r"(pulse_palette_pointer_table) : "cc");
            pulse_actor_palette = *(s32 *)palette_pointer_offset;
            QueueCopy(pulse_actor_palette,
                           (M2C_FIELD(pulse_field_actor, u8 *, FIELD_ACTOR_OFFSET(actor_id)) << 5) + 0x05000200,
                           0x20);
            }
            goto store_selected_emotion;
close_previous_text_menu:
            if (previous_menu_state == 1) {
                RunMenuScript(0x080177F5);
open_growth_popup_menu:
                RunMenuScript(0x080177ED);
                *menu_state = 2;
            }
        }
        largest_proposed_growth = 0;
        emotion_index = 0;
        {
        register u8 *current_growth_base asm("r3") = (u8 *)PULSE_PLAYER_RECORD_RAM;
        asm volatile("" : "+r"(current_growth_base));
        current_growth_base += PLAYER_AUXILIARY_PILOT_OFFSET(emotion_growth);
        current_emotion_growth = current_growth_base;
        }
        {
        register u8 *field_display_flags asm("r8") = (u8 *)0x020324B0;
        register u16 *auxiliary_pilot_id_table asm("r9") =
            ({ register u16 *auxiliary_pilot_id_table_seed asm("r1") = (u16 *)PULSE_AUXILIARY_PILOT_IDS_ROM;
               asm volatile("" : "+r"(auxiliary_pilot_id_table_seed));
               auxiliary_pilot_id_table_seed; });
        register u8 *auxiliary_pilot_record asm("r3") = (u8 *)PULSE_PLAYER_RECORD_RAM;
        u32 auxiliary_pilot_id = *auxiliary_pilot_record;
        register u8 *portrait_graphics_buffer asm("ip") = (u8 *)0x02002880;
        register u8 *current_growth_values asm("r6") = current_emotion_growth;
        register s32 *command_cursor_carrier asm("r1") = cursor_carrier;
        register u8 *growth_command_bytes asm("r3") = (u8 *)*command_cursor_carrier;
        asm volatile("" : "+r"(field_display_flags), "+r"(auxiliary_pilot_id_table), "+r"(auxiliary_pilot_id));
        asm volatile("" : "+r"(portrait_graphics_buffer), "+r"(current_growth_values), "+r"(growth_command_bytes));
        do {
            register u8 *current_growth_slot asm("r1") = current_growth_values + emotion_index;
            register u8 *command_growth_row_base asm("r0");
            register u32 proposed_growth_amount asm("r0");
            register u32 old_growth_amount asm("r1");
            register u32 proposed_growth_sum asm("r0");
            register u32 proposed_growth_low_byte asm("r4");
            asm volatile("" : "+r"(current_growth_slot));
            asm volatile(".syntax unified\n\tadds %0, %1, %2\n\t.syntax divided"
                         : "=r"(command_growth_row_base) : "r"(emotion_index), "r"(growth_command_bytes) : "cc");
            proposed_growth_amount = command_growth_row_base[EVENT_COMMAND_OFFSET(EventPulseEmotionGrowthCommand, growth_amounts)];
            old_growth_amount = *current_growth_slot;
            asm volatile("" : "+r"(proposed_growth_amount), "+r"(old_growth_amount));
            proposed_growth_sum = proposed_growth_amount + old_growth_amount;
            asm volatile(".syntax unified\n\tlsls %0, %0, #24\n\tlsrs %1, %0, #24\n\t.syntax divided"
                         : "+r"(proposed_growth_sum), "=r"(proposed_growth_low_byte) : : "cc");
            proposed_growth_clipped = proposed_growth_low_byte;
            if ((u32) proposed_growth_clipped > PULSE_EMOTION_GROWTH_LIMIT) {
                proposed_growth_clipped = PULSE_EMOTION_GROWTH_LIMIT;
            }
            if ((u32) proposed_growth_clipped > largest_proposed_growth) {
                largest_proposed_growth = (u32) proposed_growth_clipped;
                {
                register u32 selected_emotion_carrier asm("r0");
                selected_emotion_carrier = emotion_index << 24;
                selected_emotion_carrier >>= 24;
                selected_emotion_index = (s32)selected_emotion_carrier;
                }
            }
            emotion_index += 1;
        } while ((u32) emotion_index <= PULSE_EMOTION_COUNT - 1);
        {
        register u8 *field_display_flag_address asm("r2") = field_display_flags;
        asm volatile("" : "+r"(field_display_flag_address));
        if (4 & *field_display_flag_address) {
            *(s8 *)0x020314A4 = 0xF;
        }
        }
        portrait_pilot_id = auxiliary_pilot_id_table[auxiliary_pilot_id];
        portrait_palette_variant = 0;
        if (portrait_pilot_id == PULSE_PILOT_ID) {
            portrait_palette_variant = selected_emotion_index;
        }
        QueuePilotPortraitGraphics(portrait_pilot_id, 0, portrait_palette_variant, 0x3C2, 0xE,
                      ({ register s32 gfx_arg asm("r3") = (s32)portrait_graphics_buffer;
                         asm volatile("" : "+r"(gfx_arg));
                         gfx_arg; }));
        }
        *(s32 *)0x02031744 = CreateSprite(0x08359850, 0x0835985C, 0, 0x18, 0x18, 0x3C2, 0xE, 8, 0);
        RunMenuScript(EVENT_PULSE_EMOTION_GROWTH_MENU);
        displayed_emotion_index = 0;
        asm volatile("" : "+r"(displayed_emotion_index));
draw_emotion_growth_row:
        {
        register s32 one asm("r0") = 1;
        asm volatile("" : "+r"(one));
        growth_row_highlight = one;
        asm volatile("" : "+r"(growth_row_highlight));
        }
        {
        register s32 selected_emotion_carrier asm("r1") = selected_emotion_index;
        asm volatile("" : "+r"(selected_emotion_carrier));
        if (displayed_emotion_index != selected_emotion_carrier) {
            register s32 zero asm("r2") = 0;
            asm volatile("" : "+r"(zero));
            growth_row_highlight = zero;
            asm volatile("" : "+r"(growth_row_highlight));
        }
        }
        switch (displayed_emotion_index) {
        case PULSE_EMOTION_WHITE: goto draw_white_growth_label;
        case PULSE_EMOTION_RED: goto draw_red_growth_label;
        case PULSE_EMOTION_BLUE: goto draw_blue_growth_label;
        case PULSE_EMOTION_BLACK: goto draw_black_growth_label;
        default:
            next_emotion_index = displayed_emotion_index + 1;
            goto print_growth_row_values;
        }
draw_white_growth_label:
        next_growth_row = displayed_emotion_index + 1;
        {
        register s32 stack_arg asm("r0") = (s32)(s16)next_growth_row;
        asm volatile("" : "+r"(stack_arg));
        asm volatile("str %0, [sp]" : : "r"(stack_arg) : "memory");
        PrintWindowTextAt(EVENT_PULSE_WHITE_GROWTH_TEXT, growth_row_highlight, 5, 0);
        }
        next_emotion_index = next_growth_row;
        goto print_growth_row_values;
draw_red_growth_label:
        {
        register s32 stack_arg asm("r0") = 2;
        asm volatile("" : "+r"(stack_arg));
        asm volatile("str %0, [sp]" : : "r"(stack_arg) : "memory");
        PrintWindowTextAt(EVENT_PULSE_RED_GROWTH_TEXT, growth_row_highlight, 5, 0);
        }
        next_emotion_index = 2;
        goto print_growth_row_values;
draw_blue_growth_label:
        {
        register s32 stack_arg asm("r0") = 3;
        asm volatile("" : "+r"(stack_arg));
        asm volatile("str %0, [sp]" : : "r"(stack_arg) : "memory");
        PrintWindowTextAt(EVENT_PULSE_BLUE_GROWTH_TEXT, growth_row_highlight, 5, 0);
        }
        next_emotion_index = 3;
        goto print_growth_row_values;
draw_black_growth_label:
        {
        register s32 stack_arg asm("r0") = 4;
        asm volatile("" : "+r"(stack_arg));
        asm volatile("str %0, [sp]" : : "r"(stack_arg) : "memory");
        PrintWindowTextAt(EVENT_PULSE_BLACK_GROWTH_TEXT, growth_row_highlight, 5, 0);
        }
        next_emotion_index = 4;
print_growth_row_values:
        {
        register u8 *current_growth_base asm("r3") = current_emotion_growth;
        asm volatile("" : "+r"(current_growth_base));
        current_growth_address = &current_growth_base[displayed_emotion_index];
        }
        growth_row = next_emotion_index;
        displayed_growth_value = *current_growth_address;
        {
        register s32 five_seed asm("r1") = 5;
        register s32 five_value asm("r9") = five_seed;
        register u8 *displayed_growth_command_row_base asm("r0");
        PrintWindowNumberAt(displayed_growth_value, 2, growth_row_highlight, 0xA, five_seed, 7, (s32) growth_row);
        asm volatile("mov r2, %1\n\tldr %0, [r2]\n\t.syntax unified\n\tadds %0, %2, %0\n\t.syntax divided"
                     : "=r"(displayed_growth_command_row_base) : "r"(cursor_carrier), "r"(displayed_emotion_index) : "r2", "cc");
        displayed_growth_value = displayed_growth_command_row_base[EVENT_COMMAND_OFFSET(EventPulseEmotionGrowthCommand, growth_amounts)];
        asm volatile("mov r3, %0\n\tstr r3, [sp]\n\tmovs r1, #10\n\tstr r1, [sp, #4]\n\tstr %1, [sp, #8]"
                     : : "r"(five_value), "r"((s32)growth_row) : "r1", "r3", "memory");
        PrintWindowNumberAtWithStackArguments(displayed_growth_value, 2, growth_row_highlight, 0xA);
        {
        register s32 *growth_cursor_carrier asm("r1") = cursor_carrier;
        register u8 *growth_command_row_base asm("r0");
        register u32 proposed_growth_amount asm("r0");
        register u32 current_growth_address_or_preview asm("r4") = (u32)current_growth_address;
        asm volatile("" : "+r"(growth_cursor_carrier));
        growth_command_row_base = (u8 *)*growth_cursor_carrier;
        asm volatile(".syntax unified\n\tadds %0, %1, %0\n\t.syntax divided"
                     : "+r"(growth_command_row_base) : "r"(displayed_emotion_index) : "cc");
        proposed_growth_amount = growth_command_row_base[EVENT_COMMAND_OFFSET(EventPulseEmotionGrowthCommand, growth_amounts)];
        asm volatile("ldrb %0, [%0]\n\t.syntax unified\n\tadds %1, %1, %0\n\tlsls %1, %1, #24\n\tlsrs %0, %1, #24\n\tcmp %0, #99\n\tbls 0f\n\tmovs %0, #99\n0:\n\t.syntax divided"
                     : "+r"(current_growth_address_or_preview), "+r"(proposed_growth_amount) : : "cc");
        asm volatile("mov r2, %0\n\tstr r2, [sp]\n\tmovs r0, #14\n\tstr r0, [sp, #4]\n\tstr %1, [sp, #8]"
                     : : "r"(five_value), "r"((s32)growth_row) : "r0", "r2", "memory");
        PrintWindowNumberAtWithStackArguments(current_growth_address_or_preview, 2, growth_row_highlight, 0xA);
        }
        }
        displayed_emotion_index = (u32) next_emotion_index;
        if (displayed_emotion_index <= PULSE_EMOTION_COUNT - 1) {
            goto draw_emotion_growth_row;
        }
        *(s32 *)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = EVENT_PULSE_CONFIRM_EMOTION_TEXT;
        RunMenuScript(0x080177D5);
        RunMenuScript(EVENT_PULSE_CONFIRM_EMOTION_MENU);
        DestroySprite(*(s32 *)0x02031744);
        *(s32 *)0x02031744 = 0;
        *(u8 *)0x020314A4 = 0x5F;
        RunMenuScript(EVENT_PULSE_CLOSE_EMOTION_MENU);
        StopSong(EVENT_PULSE_GROWTH_SOUND);
        PlayOrContinueSong(*(u8 *)0x02030667);
        if ((*(u8 *)0x0200A882 == 1) && (*(u8 *)0x0200A880 == 0)) {
            register u32 updated_emotion_index asm("r5");
            register u8 *current_growth_write_address asm("r4");
            register u32 maximum_emotion_growth asm("r1");
            updated_emotion_index = 0;
            asm volatile("" : "+r"(updated_emotion_index));
            maximum_emotion_growth = PULSE_EMOTION_GROWTH_LIMIT;
            asm volatile("" : "+r"(maximum_emotion_growth));
            current_growth_write_address = current_emotion_growth;
            asm volatile("" : "+r"(current_growth_write_address));
            do {
                register s32 *growth_cursor_carrier asm("r3") = cursor_carrier;
                register u8 *growth_command_bytes asm("r0");
                register u32 updated_growth asm("r0");
                register u32 previous_growth asm("r2");
                asm volatile("" : "+r"(growth_cursor_carrier));
                growth_command_bytes = (u8 *)*growth_cursor_carrier;
                asm volatile(".syntax unified\n\tadds %0, %1, %0\n\t.syntax divided"
                             : "+r"(growth_command_bytes) : "r"(updated_emotion_index) : "cc");
                updated_growth = growth_command_bytes[EVENT_COMMAND_OFFSET(EventPulseEmotionGrowthCommand, growth_amounts)];
                previous_growth = *current_growth_write_address;
                updated_growth += previous_growth;
                *current_growth_write_address = updated_growth;
                updated_growth <<= 24;
                updated_growth >>= 24;
                if (updated_growth > PULSE_EMOTION_GROWTH_LIMIT) {
                    *current_growth_write_address = maximum_emotion_growth;
                }
                current_growth_write_address += 1;
                updated_emotion_index += 1;
            } while (updated_emotion_index <= PULSE_EMOTION_COUNT - 1);
            {
            register u8 *pulse_record_bytes asm("r3") = (u8 *)PULSE_PLAYER_RECORD_RAM;
            register u32 previous_emotion_variant asm("r0");
            register s32 selected_emotion_carrier asm("r1");
            asm volatile("" : "+r"(pulse_record_bytes));
            previous_emotion_variant = pulse_record_bytes[PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)];
            selected_emotion_carrier = selected_emotion_index;
            asm volatile("" : "+r"(previous_emotion_variant), "+r"(selected_emotion_carrier));
            if (previous_emotion_variant != selected_emotion_carrier) {
                register u32 actor_slot asm("r5") = 0;
                asm volatile("" : "+r"(actor_slot));
                pulse_field_actor = (void *)FIELD_ACTORS_RAM;
find_pulse_field_actor:
                if (M2C_FIELD(pulse_field_actor, u8 *, FIELD_ACTOR_OFFSET(model_id)) == PULSE_FIELD_ACTOR_MODEL_ID) {
                    goto update_pulse_actor_palette;
                } else {
                    pulse_field_actor += (s32)sizeof(struct FieldActor);
                    actor_slot += 1;
                    if (actor_slot <= FIELD_ACTOR_MAX_SLOT) {
                        goto find_pulse_field_actor;
                    }
                }
            }
            }
store_selected_emotion:
            {
            register u32 selected_emotion_carrier asm("r3");
            register u8 *pulse_record_bytes asm("r2");
            asm volatile("mov r2, sp\n\tldrb %0, [r2, #24]"
                         : "=r"(selected_emotion_carrier) : : "r2");
            pulse_record_bytes = (u8 *)PULSE_PLAYER_RECORD_RAM;
            asm volatile("" : "+r"(pulse_record_bytes));
            pulse_record_bytes[PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)] = selected_emotion_carrier;
            }
            seek_script_slot = saved_script_slot;
            asm volatile("" : "+r"(seek_script_slot));
            branch_opcode = EVENT_PULSE_GROWTH_ACCEPTED;
        } else {
            seek_script_slot = saved_script_slot;
            asm volatile("" : "+r"(seek_script_slot));
            branch_opcode = EVENT_PULSE_GROWTH_DECLINED;
        }
    }
    goto seek_growth_branch;
pulse_absent:
    branch_opcode = EVENT_SCAN_NEXT;
    seek_script_slot = saved_script_slot;
    asm volatile("" : "+r"(seek_script_slot));
seek_growth_branch:
    SeekEventCommand(seek_script_slot, branch_opcode, 0);
    return EVENT_CONTINUE;
}
