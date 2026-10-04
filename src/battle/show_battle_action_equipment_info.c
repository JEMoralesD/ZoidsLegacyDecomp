#include "m2c_prelude.h"
#include "battle.h"
M2C_UNK RequestWindowRefresh() asm("func_080972C8");                            /* extern */
M2C_UNK PrintWindowTextAt(s32, s32, s32, s32, s32) asm("func_080981F0");     /* extern */
M2C_UNK PrintWindowText(M2C_UNK, s32, s32) asm("func_08098248");           /* extern */
M2C_UNK PrintWindowNumberAt(s32, s32, s32, s32, s32, s32, s32) asm("func_0809844C"); /* extern */
M2C_UNK RunMenuScript(M2C_UNK) asm("func_08098BB4");                     /* extern */
void *AcquireEquipmentStatBuffer() asm("func_080E669C");                              /* extern */
M2C_UNK ReleaseEquipmentStatBuffer() asm("func_080E66B8");                            /* extern */
u16 BuildBattleEquipmentStats(u32, u32, u32, u32, void *) asm("func_080E8C90");      /* extern */
M2C_UNK jtbl_080CB538();                            /* static */
M2C_UNK jtbl_080CB968();                            /* static */
M2C_UNK jtbl_080CB9E8();                            /* static */
extern u8 gBattleState[];
extern s32 gEquipmentNameTable[] asm("D_087EE170");

void ShowBattleActionEquipmentInfo(void) asm("func_080CB490");

void ShowBattleActionEquipmentInfo(void) {
    u16 item_id;
    register u32 action_index asm("r3");
    u32 side;
    u32 unit_index;
    register u8 *selection_base asm("r2");
    register u32 selection_offset asm("r4");
    u8 range_kind;
    u8 area_type;
    void *equipment_stats;

    equipment_stats = AcquireEquipmentStatBuffer();
    side = *(u8 *)0x02033F36;
    unit_index = *(u8 *)0x02033F37;
    selection_base = gBattleState;
    selection_offset = 0xA1AF;
    action_index = selection_base[selection_offset];
    selection_offset++;
    selection_base += selection_offset;
    selection_base = (u8 *)(action_index + (u32)selection_base);
    item_id = BuildBattleEquipmentStats(side, unit_index, *selection_base,
                            action_index, equipment_stats);
    if (!(EQUIPMENT_COMMAND & EQUIPMENT_RECORD_FIELD(equipment_stats, u16, flags))) {
        RunMenuScript(0x0800408F);
        PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xA, 2, 6, 2);
        PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, accuracy_or_secondary_value), 3, 0, 0xA, 2, 6, 3);
    } else {
        RunMenuScript(0x080040B9);
        switch (EQUIPMENT_RECORD_FIELD(equipment_stats, u8, attributes) - 1) {  /* switch 1; irregular */
        case 0:                                     /* switch 1 */
            PrintWindowTextAt(0x08108B5C, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            break;
        case 1:                                     /* switch 1 */
            PrintWindowTextAt(0x08108B5C, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            PrintWindowTextAt(0x08108B60, 0, 2, 0, 3);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, accuracy_or_secondary_value), 4, 0, 0xE, 2, 5, 2);
            break;
        case 2:                                     /* switch 1 */
            PrintWindowTextAt(0x08108B64, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            break;
        case 3:                                     /* switch 1 */
            PrintWindowTextAt(0x08108B68, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            break;
        case 4:                                     /* switch 1 */
            PrintWindowTextAt(0x08108B64, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            PrintWindowTextAt(0x08108B68, 0, 2, 0, 3);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, accuracy_or_secondary_value), 4, 0, 0xE, 2, 5, 3);
            break;
        case 5:                                     /* switch 1 */
            PrintWindowTextAt(0x08108B6C, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            break;
        case 6:                                     /* switch 1 */
            PrintWindowTextAt(0x08108B70, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            break;
        case 7:                                     /* switch 1 */
            PrintWindowTextAt(0x08108B6C, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            PrintWindowTextAt(0x08108B70, 0, 2, 0, 3);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, accuracy_or_secondary_value), 4, 0, 0xE, 2, 5, 3);
            break;
        case 8:                                     /* switch 1 */
            PrintWindowTextAt(0x08108B70, 0, 2, 0, 2);
            PrintWindowNumberAt(-EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            break;
        case 9:                                     /* switch 1 */
            PrintWindowTextAt(0x08108B74, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            break;
        case 10:                                    /* switch 1 */
            PrintWindowTextAt(0x08108B7C, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            break;
        case 11:                                    /* switch 1 */
            PrintWindowTextAt(0x08108B80, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            break;
        case 12:                                    /* switch 1 */
            PrintWindowTextAt(0x08108B88, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            break;
        case 13:                                    /* switch 1 */
            PrintWindowTextAt(0x08108B90, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            break;
        case 14:                                    /* switch 1 */
            PrintWindowTextAt(0x08108B88, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            PrintWindowTextAt(0x08108B90, 0, 2, 0, 3);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, accuracy_or_secondary_value), 4, 0, 0xE, 2, 5, 3);
            break;
        case 15:                                    /* switch 1 */
            PrintWindowTextAt(0x08108B98, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            break;
        case 16:                                    /* switch 1 */
            PrintWindowTextAt(0x08108B9C, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            break;
        case 17:                                    /* switch 1 */
            PrintWindowTextAt(0x08108B98, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xE, 2, 5, 2);
            PrintWindowTextAt(0x08108B9C, 0, 2, 0, 3);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, accuracy_or_secondary_value), 4, 0, 0xE, 2, 5, 3);
            break;
        case 18:                                    /* switch 1 */
            PrintWindowTextAt(0x08108BA0, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xA, 2, 6, 2);
            break;
        case 19:                                    /* switch 1 */
            PrintWindowTextAt(0x08108BA4, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xA, 2, 6, 2);
            break;
        case 20:                                    /* switch 1 */
            PrintWindowTextAt(0x08108BAC, 0, 2, 0, 2);
            PrintWindowNumberAt(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 3, 0, 0xA, 2, 7, 2);
            break;
        case 21:                                    /* switch 1 */
            PrintWindowTextAt(0x08108BB4, 0, 2, 0, 2);
            break;
        }
        if (EQUIPMENT_RECORD_FIELD(equipment_stats, u32, attributes) & EQUIPMENT_EFFECT_ANTI_AIR) {
            PrintWindowTextAt(0x08108BC0, 0, 2, 2, 3);
        }
    }
    if (EQUIPMENT_ATTACK & EQUIPMENT_RECORD_FIELD(equipment_stats, u16, flags)) {
        range_kind = EQUIPMENT_RECORD_FIELD(equipment_stats, u8, range_kind);
        switch (range_kind) {                        /* switch 2 */
        case 0:                                     /* switch 2 */
            PrintWindowTextAt(0x08108BCC, 0, 2, 6, 4);
            break;
        case 1:                                     /* switch 2 */
            PrintWindowTextAt(0x08108BD0, 0, 2, 6, 4);
            break;
        case 2:                                     /* switch 2 */
            PrintWindowTextAt(0x08108BD4, 0, 2, 6, 4);
            break;
        case 3:                                     /* switch 2 */
            PrintWindowTextAt(0x08108BD8, 0, 2, 6, 4);
            break;
        case 4:                                     /* switch 2 */
            PrintWindowTextAt(0x08108BDC, 0, 2, 6, 4);
            break;
        case 5:                                     /* switch 2 */
            PrintWindowTextAt(0x08108BE0, 0, 2, 6, 4);
            break;
        }
    } else {
        PrintWindowTextAt(0x08108BE4, 0, 2, 4, 4);
    }
    area_type = EQUIPMENT_RECORD_FIELD(equipment_stats, u8, area_type);
    switch (area_type) {                            /* switch 3 */
    case 0:                                         /* switch 3 */
        PrintWindowText(0x08108BEC, 0, 2);
        break;
    case 1:                                         /* switch 3 */
        PrintWindowText(0x08108BF0, 0, 2);
        break;
    case 2:                                         /* switch 3 */
        PrintWindowText(0x08108BF4, 0, 2);
        break;
    case 3:                                         /* switch 3 */
        PrintWindowText(0x08108BF8, 0, 2);
        break;
    case 4:                                         /* switch 3 */
        PrintWindowText(0x08108BFC, 0, 2);
        break;
    case 5:                                         /* switch 3 */
        PrintWindowText(0x08108C00, 0, 2);
        break;
    case 7:                                         /* switch 3 */
        PrintWindowTextAt(0x08108C04, 0, 2, 3, 4);
        break;
    case 6:                                         /* switch 3 */
        PrintWindowTextAt(0x08108C0C, 0, 2, 3, 4);
        break;
    }
    {
        register u32 ep_cost_offset asm("r4") = (s32)&((struct EquipmentRecord *)0)->ep_cost;
        register s32 ep_cost asm("r0");

        asm volatile("ldrsh %0, [%1, %2]"
                     : "=l"(ep_cost)
                     : "l"(equipment_stats), "l"(ep_cost_offset)
                     : "memory");
        PrintWindowNumberAt(ep_cost, 3, 0, 0xA, 2, 7, 5);
    }
    PrintWindowTextAt(gEquipmentNameTable[item_id], 0, 2, 0, 0);
    RequestWindowRefresh();
    ReleaseEquipmentStatBuffer();
}
