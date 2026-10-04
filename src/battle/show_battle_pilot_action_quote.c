#include "m2c_prelude.h"
#include "battle_display.h"

extern void PrintWindowText(s32, s32, s32) asm("func_08098248");
extern void ClearWindow(s32) asm("func_080986B4");
extern void QueuePilotPortraitGraphics(u8, u8, s32, s32, s32, s32) asm("func_0809A9C8");

void ShowBattlePilotActionQuote(u8 side, u16 equipment_or_action_id, u8 pilot_id_arg, u32 quote_kind_arg, s32 quote_index_arg) asm("func_080CCBD4");

void ShowBattlePilotActionQuote(u8 side, u16 equipment_or_action_id, u8 pilot_id_arg, u32 quote_kind_arg, s32 quote_index_arg)
{
    u8 pilot_id = pilot_id_arg;
    u8 quote_kind = quote_kind_arg;
    register u32 quote_index asm("r4") = (u8)quote_index_arg;
    u8 *battle_story_setup = (u8 *)0x0203055C;
    u8 *pilot_quote_table = (u8 *)BATTLE_PILOT_QUOTE_TABLE_ROM;
    register struct BattlePilotQuoteEntry *quote_entry asm("r6");
    register u32 quote_index_times_eight asm("r5");
    register u32 quote_kind_times_two asm("sl");
    register u32 pilot_id_times_sixteen asm("r4");
    register u32 quote_kind_times_two_copy asm("r3");
    s32 quote_text;

    asm volatile(
        "ldrb r0, [r0, #5]\n\t"
        "cmp r0, #5\n\t"
        "bne 1f\n\t"
        "cmp r2, #52\n\t"
        "beq 2f\n\t"
        "cmp r2, #94\n\t"
        "bne 1f\n"
        "2:\n\t"
        "mov r0, r9\n\t"
        "cmp r0, #2\n\t"
        "bne 1f\n\t"
        "movs r1, #97\n\t"
        "mov r8, r1\n"
        "1:"
        : "+r"(battle_story_setup), "+r"(pilot_id)
        : "r"(quote_kind), "r"(pilot_id_arg)
        : "r1", "cc", "memory");

    {
        register u8 *initial_quote_table asm("r2") = (u8 *)BATTLE_PILOT_QUOTE_TABLE_ROM;
        asm volatile(
            "lsl r5, r4, #3\n\t"
            "mov r3, r9\n\t"
            "lsl r3, r3, #1\n\t"
            "mov sl, r3\n\t"
            "mov r1, sl\n\t"
            "add r1, r9\n\t"
            "lsl r1, r1, #3\n\t"
            "add r1, r5, r1\n\t"
            "mov r0, r8\n\t"
            "lsl r4, r0, #4\n\t"
            "sub r0, r4, r0\n\t"
            "lsl r0, r0, #3\n\t"
            "add r1, r1, r0\n\t"
            "add r6, r1, r2"
            : "=r"(quote_entry), "=r"(quote_index_times_eight), "=r"(quote_kind_times_two), "=r"(pilot_id_times_sixteen)
            : "r"(initial_quote_table), "r"(quote_index), "r"(quote_kind), "r"(pilot_id)
            : "r0", "r1", "r3", "cc", "memory");
    }
    QueuePilotPortraitGraphics(pilot_id, quote_entry->portrait_variant,
        0, BATTLE_PILOT_QUOTE_PORTRAIT_TILE_OFFSET, BATTLE_PILOT_QUOTE_PORTRAIT_PALETTE_BANK, BATTLE_PILOT_QUOTE_PORTRAIT_BUFFER_RAM);
    ClearWindow(1);
    quote_kind_times_two_copy = quote_kind_times_two;
    asm volatile("" : "+r"(quote_kind_times_two_copy));

    {
        register u32 pilot_id_test asm("r1") = pilot_id;
        asm volatile("" : "+r"(pilot_id_test));
        if (pilot_id_test == 0x64) {
            quote_text = BATTLE_PILOT_QUOTE_FIELD(quote_entry, s32, text);
            goto print_quote;
        }
    }

    if (quote_kind == BATTLE_PILOT_QUOTE_ATTACK) {
        if (equipment_or_action_id == 0x136) {
            quote_text = 0x08028063;
            goto print_quote;
        }
        if (equipment_or_action_id == 0x12B || equipment_or_action_id == 0x133 || equipment_or_action_id == 0x137 ||
            equipment_or_action_id == 0x13B || equipment_or_action_id == 0x13F || equipment_or_action_id == 0x163 ||
            equipment_or_action_id == 0x1C7 || equipment_or_action_id == 0x1CB || equipment_or_action_id == 0x1CF ||
            equipment_or_action_id == 0x1D3 || equipment_or_action_id == 0x1DB || equipment_or_action_id == 0x2A0 ||
            equipment_or_action_id == 0x2A4 || equipment_or_action_id == 0x2CF) {
            quote_text = 0x08028088;
            goto print_quote;
        }
        if (equipment_or_action_id == 0x1FA) {
            quote_text = 0x080280B1;
            goto print_quote;
        }
        if (equipment_or_action_id == 0x295) {
            quote_text = 0x080280D8;
            goto print_quote;
        }
        if (equipment_or_action_id == 0xEA) {
            quote_text = 0x080280FD;
            goto print_quote;
        }
    }
    if (quote_kind == BATTLE_PILOT_QUOTE_COMMAND) {
            if (equipment_or_action_id == 0x320 && pilot_id == 1) {
                quote_text = 0x0802811E;
                goto print_quote;
            }
            if (quote_kind == BATTLE_PILOT_QUOTE_COMMAND) {
                if (equipment_or_action_id == 0x320 && (pilot_id == 0xE || pilot_id == 0x16)) {
                    quote_text = 0x0802812B;
                    goto print_quote;
                }
                if (quote_kind == BATTLE_PILOT_QUOTE_COMMAND) {
                    if (equipment_or_action_id == 0x320 && pilot_id == 0x1C) {
                        quote_text = 0x08028136;
                        goto print_quote;
                    }
                    if (quote_kind == BATTLE_PILOT_QUOTE_COMMAND) {
                        if (equipment_or_action_id == 0x320 && pilot_id == 0x1D) {
                            quote_text = 0x08028145;
                            goto print_quote;
                        }
                        if (quote_kind == BATTLE_PILOT_QUOTE_COMMAND && (u16)(equipment_or_action_id - 0xA5) <= 2) {
                            quote_text = 0x08028158;
                            goto print_quote;
                        }
                    }
                }
            }
    }

    if (quote_kind == BATTLE_PILOT_QUOTE_ATTACK) {
        if (equipment_or_action_id == 0x2C7) {
            quote_text = 0x08028167;
            goto print_quote;
        }
        if (equipment_or_action_id == 0x2CA) {
            quote_text = 0x08028182;
            goto print_quote;
        }
    }

    if (pilot_id != 1 || quote_kind != BATTLE_PILOT_QUOTE_ATTACK) {
        register u8 *final_quote_table asm("r2") = pilot_quote_table;
        register s32 table_quote_text asm("r0");
        asm volatile(
            "mov r1, r9\n\t"
            "add r0, r3, r1\n\t"
            "lsl r0, r0, #3\n\t"
            "add r0, r5, r0\n\t"
            "mov r3, r8\n\t"
            "sub r1, r4, r3\n\t"
            "lsl r1, r1, #3\n\t"
            "add r0, r0, r1\n\t"
            "add r0, r0, r2\n\t"
            "ldr r0, [r0]"
            : "=r"(table_quote_text), "+r"(quote_kind_times_two_copy)
            : "r"(final_quote_table), "r"(quote_index_times_eight), "r"(pilot_id_times_sixteen),
              "r"(pilot_id), "r"(quote_kind)
            : "r1", "cc", "memory");
        quote_text = table_quote_text;
print_quote:
        PrintWindowText(quote_text, 0, 1);
        return;
    }

    PrintWindowText(side * BATTLE_PILOT_CUSTOM_QUOTE_BYTES + BATTLE_PILOT_CUSTOM_QUOTES_RAM, 0, 1);
}
