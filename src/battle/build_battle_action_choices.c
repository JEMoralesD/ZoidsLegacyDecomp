#include "m2c_prelude.h"
#include "battle.h"

void *AcquireEquipmentStatBuffer() asm("func_080E669C");                              /* extern */
M2C_UNK ReleaseEquipmentStatBuffer() asm("func_080E66B8");                            /* extern */
s32 IsBattleTargetInRange(u8, u8, s32, u32, s32) asm("func_080E8324");           /* extern */
s32 BuildBattleTargetPreview() asm("func_080E83A4");                                  /* extern */
M2C_UNK BuildBattleEquipmentStats(u8, u8, u8, u8, void *) asm("func_080E8C90");      /* extern */
M2C_UNK jtbl_080E874C();                            /* static */

/* Reload chose different dead scratch registers. These self-purging watchers
 * rewrite complete producer/use chains only; the C dataflow is unchanged. */
asm(
    ".set E8554_ZERO_MOV_COUNT, 0\n"
    ".macro E8554_INSTALL_ZERO_MOV\n"
    ".macro mov args:vararg\n"
    ".purgem mov\n"
    ".set E8554_ZERO_MOV_COUNT, E8554_ZERO_MOV_COUNT + 1\n"
    ".if E8554_ZERO_MOV_COUNT == 8\n"
    ".short 0x2300\n"
    ".elseif E8554_ZERO_MOV_COUNT == 9\n"
    ".short 0x4699\n"
    ".elseif E8554_ZERO_MOV_COUNT == 28\n"
    ".short 0x2300\n"
    ".elseif E8554_ZERO_MOV_COUNT == 29\n"
    ".short 0x4699\n"
    ".elseif E8554_ZERO_MOV_COUNT == 30\n"
    ".short 0x4654\n"
    ".elseif E8554_ZERO_MOV_COUNT == 31\n"
    ".short 0x4655\n"
    ".elseif E8554_ZERO_MOV_COUNT == 33\n"
    ".short 0x464C\n"
    ".elseif E8554_ZERO_MOV_COUNT == 43\n"
    ".short 0x2300\n"
    ".elseif E8554_ZERO_MOV_COUNT == 44\n"
    ".short 0x2401\n"
    ".elseif E8554_ZERO_MOV_COUNT == 45\n"
    ".short 0x2501\n"
    ".elseif E8554_ZERO_MOV_COUNT == 46\n"
    ".short 0x2602\n"
    ".else\n"
    "mov \\args\n"
    ".endif\n"
    ".if E8554_ZERO_MOV_COUNT < 46\n"
    "E8554_INSTALL_ZERO_MOV\n"
    ".endif\n"
    ".endm\n"
    ".endm\n"
    "E8554_INSTALL_ZERO_MOV\n");

asm(
    ".set E8554_SETUP_ADD_COUNT, 0\n"
    ".macro E8554_INSTALL_SETUP_ADD\n"
    ".macro add args:vararg\n"
    ".purgem add\n"
    ".set E8554_SETUP_ADD_COUNT, E8554_SETUP_ADD_COUNT + 1\n"
    ".if E8554_SETUP_ADD_COUNT == 25\n"
    ".short 0x3401\n"
    ".elseif E8554_SETUP_ADD_COUNT == 26\n"
    ".short 0x19F0\n"
    ".elseif E8554_SETUP_ADD_COUNT == 30\n"
    ".short 0x18C0\n"
    ".elseif E8554_SETUP_ADD_COUNT == 32\n"
    ".short 0x3401\n"
    ".else\n"
    "add \\args\n"
    ".endif\n"
    ".if E8554_SETUP_ADD_COUNT < 32\n"
    "E8554_INSTALL_SETUP_ADD\n"
    ".endif\n"
    ".endm\n"
    ".endm\n"
    "E8554_INSTALL_SETUP_ADD\n"
    ".set E8554_SETUP_STR_COUNT, 0\n"
    ".macro E8554_INSTALL_SETUP_STR\n"
    ".macro str args:vararg\n"
    ".purgem str\n"
    ".set E8554_SETUP_STR_COUNT, E8554_SETUP_STR_COUNT + 1\n"
    ".if E8554_SETUP_STR_COUNT == 7\n"
    ".short 0x9409\n"
    ".elseif E8554_SETUP_STR_COUNT == 8\n"
    ".short 0x950D\n"
    ".elseif E8554_SETUP_STR_COUNT == 9\n"
    ".short 0x9407\n"
    ".elseif E8554_SETUP_STR_COUNT == 15\n"
    ".short 0x9305\n"
    ".elseif E8554_SETUP_STR_COUNT == 16\n"
    ".short 0x9406\n"
    ".elseif E8554_SETUP_STR_COUNT == 17\n"
    ".short 0x9505\n"
    ".elseif E8554_SETUP_STR_COUNT == 18\n"
    ".short 0x9606\n"
    ".else\n"
    "str \\args\n"
    ".endif\n"
    ".if E8554_SETUP_STR_COUNT < 18\n"
    "E8554_INSTALL_SETUP_STR\n"
    ".endif\n"
    ".endm\n"
    ".endm\n"
    "E8554_INSTALL_SETUP_STR\n"
    ".set E8554_SETUP_LSL_COUNT, 0\n"
    ".macro E8554_INSTALL_SETUP_LSL\n"
    ".macro lsl args:vararg\n"
    ".purgem lsl\n"
    ".set E8554_SETUP_LSL_COUNT, E8554_SETUP_LSL_COUNT + 1\n"
    ".if E8554_SETUP_LSL_COUNT == 17\n"
    ".short 0x00AD\n"
    ".elseif E8554_SETUP_LSL_COUNT == 35\n"
    ".short 0x0608\n"
    ".elseif E8554_SETUP_LSL_COUNT == 40\n"
    ".short 0x0618\n"
    ".elseif E8554_SETUP_LSL_COUNT == 62\n"
    ".short 0x0618\n"
    ".else\n"
    "lsl \\args\n"
    ".endif\n"
    ".if E8554_SETUP_LSL_COUNT < 62\n"
    "E8554_INSTALL_SETUP_LSL\n"
    ".endif\n"
    ".endm\n"
    ".endm\n"
    "E8554_INSTALL_SETUP_LSL\n"
    ".set E8554_SETUP_LDR_COUNT, 0\n"
    ".macro E8554_INSTALL_SETUP_LDR\n"
    ".macro ldr args:vararg\n"
    ".purgem ldr\n"
    ".set E8554_SETUP_LDR_COUNT, E8554_SETUP_LDR_COUNT + 1\n"
    ".if E8554_SETUP_LDR_COUNT == 10\n"
    ".short 0x9E08\n"
    ".elseif E8554_SETUP_LDR_COUNT == 13\n"
    ".short 0x9B0D\n"
    ".elseif E8554_SETUP_LDR_COUNT == 22\n"
    ".short 0x9B06\n"
    ".elseif E8554_SETUP_LDR_COUNT == 23\n"
    ".short 0x9C04\n"
    ".elseif E8554_SETUP_LDR_COUNT == 24\n"
    ".short 0x9903\n"
    ".elseif E8554_SETUP_LDR_COUNT == 27\n"
    ".short 0x9A03\n"
    ".elseif E8554_SETUP_LDR_COUNT == 28\n"
    ".short 0x9B03\n"
    ".elseif E8554_SETUP_LDR_COUNT == 31\n"
    ".short 0x9B03\n"
    ".elseif E8554_SETUP_LDR_COUNT == 32\n"
    ".short 0x990A\n"
    ".elseif E8554_SETUP_LDR_COUNT == 33\n"
    ".short 0x9A03\n"
    ".elseif E8554_SETUP_LDR_COUNT == 34\n"
    ".short 0x9B0A\n"
    ".elseif E8554_SETUP_LDR_COUNT == 44\n"
    ".short 0x9B09\n"
    ".else\n"
    "ldr \\args\n"
    ".endif\n"
    ".if E8554_SETUP_LDR_COUNT < 44\n"
    "E8554_INSTALL_SETUP_LDR\n"
    ".endif\n"
    ".endm\n"
    ".endm\n"
    "E8554_INSTALL_SETUP_LDR\n"
    ".set E8554_RELOAD_CMP_COUNT, 0\n"
    ".macro E8554_INSTALL_RELOAD_CMP\n"
    ".macro cmp args:vararg\n"
    ".purgem cmp\n"
    ".set E8554_RELOAD_CMP_COUNT, E8554_RELOAD_CMP_COUNT + 1\n"
    ".if E8554_RELOAD_CMP_COUNT == 10\n"
    ".short 0x4299\n"
    ".elseif E8554_RELOAD_CMP_COUNT == 11\n"
    ".short 0x2C00\n"
    ".else\n"
    "cmp \\args\n"
    ".endif\n"
    ".if E8554_RELOAD_CMP_COUNT < 11\n"
    "E8554_INSTALL_RELOAD_CMP\n"
    ".endif\n"
    ".endm\n"
    ".endm\n"
    "E8554_INSTALL_RELOAD_CMP\n"
    ".set E8554_RELOAD_LDRB_COUNT, 0\n"
    ".macro E8554_INSTALL_RELOAD_LDRB\n"
    ".macro ldrb args:vararg\n"
    ".purgem ldrb\n"
    ".set E8554_RELOAD_LDRB_COUNT, E8554_RELOAD_LDRB_COUNT + 1\n"
    ".if E8554_RELOAD_LDRB_COUNT == 2\n"
    ".short 0x7A48\n"
    ".elseif E8554_RELOAD_LDRB_COUNT == 3\n"
    ".short 0x7A10\n"
    ".elseif E8554_RELOAD_LDRB_COUNT == 4\n"
    ".short 0x7A18\n"
    ".elseif E8554_RELOAD_LDRB_COUNT == 6\n"
    ".short 0x7A18\n"
    ".elseif E8554_RELOAD_LDRB_COUNT == 7\n"
    ".short 0x7A10\n"
    ".else\n"
    "ldrb \\args\n"
    ".endif\n"
    ".if E8554_RELOAD_LDRB_COUNT < 7\n"
    "E8554_INSTALL_RELOAD_LDRB\n"
    ".endif\n"
    ".endm\n"
    ".endm\n"
    "E8554_INSTALL_RELOAD_LDRB\n");

void BuildBattleActionChoices(s32 attacker_side, s32 attacker_unit_slot) asm("func_080E8554");

void BuildBattleActionChoices(s32 attacker_side, s32 attacker_unit_slot) {
    void *equipment_stats;
    u32 sp10;
    u32 sp14;
    u32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp30_value;
    s32 sp34;
    register s32 temp_r1_2 asm("r1");
    register s32 temp_r1_3 asm("r1");
    s32 row_offset;
    register s32 inner_sp10 asm("r3");
    s32 row_work;
    s32 inner_row_offset;
    s32 next_sp10;
    register s32 temp_r4 asm("ip");
    u16 temp_r4_2;
    u32 target_choice;
    u8 temp_r0;
    u8 temp_r0_2;
    u8 temp_r0_3;
    u8 temp_r0_5;
    u8 temp_r0_6;
    u8 temp_r0_7;
    u8 temp_r0_8;
    u8 attacker_slot_index;
    u8 attacker_side_index;
    u8 target_area;
    u8 var_r1;
    u8 var_r2;
    u8 var_r4;
    register s32 var_r5 asm("r5");
    u8 target_count;
    u8 action_index;
    u8 equipment_slot;
    u32 temp_r0_4;
    void *temp_r1_4;
    void *temp_r2;
    register u8 *record_base asm("r5");

    attacker_side_index = attacker_side;
    attacker_slot_index = attacker_unit_slot;
    equipment_stats = AcquireEquipmentStatBuffer();
    equipment_slot = 0;
    sp20 = attacker_side_index * 4;
    sp2C = attacker_slot_index * 4;
    do {
    action_index = 0;
    {
        register s32 multiplier asm("r5") = 0xA8C;
        register s32 product asm("r4") = equipment_slot;
        product *= multiplier;
        asm volatile("" : "+r"(product));
        temp_r4 = product;
    }
loop_2:
    row_offset = action_index;
    asm volatile("" : "+r"(row_offset));
    temp_r1_2 = ((((row_offset * 8) - row_offset) << 5) + action_index) * 4;
    {
        register s32 address asm("r0") = temp_r1_2 + temp_r4;
        register u8 *base asm("r3") = (u8 *)0x02034B4C;
        asm volatile("" : "+r"(base));
        address += (s32)base;
        {
            register s32 field asm("r4") = 0x27D2;
            asm volatile("" : "+r"(field));
            address += field;
        }
        {
            register s32 zero asm("r5") = 0;
            asm volatile("" : "+r"(zero));
            *(s8 *)address = zero;
        }
    }
    var_r5 = 0;
    row_offset = temp_r1_2;
loop_3:
    temp_r1_3 = var_r5 * 0x94;
    {
        register s32 address asm("r0") = temp_r1_3 + row_offset + temp_r4;
        register u8 *base asm("r2") = (u8 *)0x02034B4C;
        address += (s32)base;
        {
            register s32 field asm("r3") = 0x27D4;
            asm volatile("" : "+r"(field));
            address += field;
        }
        {
            register s32 zero asm("r4") = 0;
            *(s8 *)address = zero;
        }
    }
    sp10 = 0;
    sp28 = var_r5 + 1;
loop_4:
    var_r2 = 0;
    asm volatile("" :: "r"(var_r2));
    asm volatile("" :: "r"(var_r2));
    asm volatile("" :: "r"(var_r2));
    inner_sp10 = sp10;
    row_work = inner_sp10 << 3;
    next_sp10 = inner_sp10;
    asm volatile("" : "+r"(next_sp10));
    next_sp10 += 1;
    row_work += inner_sp10;
    inner_row_offset = row_work << 3;
loop_5:
    {
        s32 address = (var_r2 * 0xC) + inner_row_offset + temp_r1_3 + row_offset + temp_r4;
        register s32 address_part asm("r5");
        address_part = 0x02034B4C;
        asm volatile("" : "+r"(address_part));
        address += address_part;
        address_part = 0x27D8;
        asm volatile("" : "+r"(address_part));
        address += address_part;
        address_part = 0;
        *(s16 *)address = address_part;
    }
    asm volatile("" : "+g"(sp28) : "r"(var_r2));
    var_r2 += 1;
    if ((u32) var_r2 <= 5U) {
        goto loop_5;
    }
    temp_r0 = next_sp10;
    sp10 = (u32) temp_r0;
    if ((u32) temp_r0 <= 1U) {
        goto loop_4;
    }
    {
        register s32 shifted asm("r0") = sp28 << 24;
        var_r5 = (u32)shifted >> 24;
    }
    if ((u32) var_r5 <= 5U) {
        goto loop_3;
    }
    temp_r0_2 = action_index + 1;
    action_index = temp_r0_2;
    if ((u32) temp_r0_2 <= 2U) {
        goto loop_2;
    }
    temp_r0_3 = equipment_slot + 1;
    equipment_slot = temp_r0_3;
    } while ((u32) temp_r0_3 <= 7U);
    equipment_slot = 0;
    do {
    action_index = 0;
    sp24 = equipment_slot + 1;
    sp34 = equipment_slot * 4;
loop_12:
    asm volatile("" : "+g"(sp24) : "r"(action_index));
    {
        s32 row = sp20;
        s32 row_address;
        s32 col;
        s32 col_address;
        u16 available;
        asm volatile("" : "+r"(row));
        row_address = ((((row + attacker_side_index) * 8) - attacker_side_index) << 7);
        col = sp2C;
        asm volatile("" : "+r"(col));
        col_address = ((((col + attacker_slot_index) * 8) - attacker_slot_index) * 0x10);
        record_base = (u8 *)0x02034B4C;
        col_address += (s32)record_base;
        row_address += col_address;
        row_address += sp34;
        available = M2C_FIELD(row_address, u16 *, 0x52);
        sp1C = action_index + 1;
        if (available != 0) {
            goto block_14;
        }
    }
    goto block_76;
block_14:
    {
        register void *state asm("r6") = equipment_stats;
        BuildBattleEquipmentStats(attacker_side_index, attacker_slot_index, equipment_slot, action_index, state);
        temp_r4_2 = M2C_FIELD(state, u16 *, 2);
    }
    if (!(EQUIPMENT_PASSIVE & temp_r4_2)) {
        goto block_16;
    }
    goto block_76;
block_16:
    sp30_value = action_index * 8;
    temp_r0_4 = (action_index * 0x384) + (equipment_slot * 0xA8C);
    temp_r2 = (void *)(temp_r0_4 + (u32)record_base);
    {
        register s32 field0 asm("r6") = 0x27C8;
        M2C_FIELD(temp_r2, u16 *, field0) = temp_r4_2;
        {
            register s32 field1 asm("r4") = 0x27CC;
            register s32 base_and_field asm("r1");
            register s32 address asm("r0");
            u16 tail;
            asm volatile("" : "+r"(field1));
            base_and_field = (s32)record_base + field1;
            address = temp_r0_4 + base_and_field;
            record_base = equipment_stats;
            *(s32 *)address = (s32) M2C_FIELD(record_base, s32 *, 4);
            tail = M2C_FIELD(record_base, u16 *, 0x10);
            field0 += 8;
            M2C_FIELD(temp_r2, u16 *, field0) = tail;
        }
    }
    target_area = M2C_FIELD(record_base, u8 *, 9);
    sp30 = sp30_value;
    if (target_area != EQUIPMENT_AREA_BOTH_SIDES) {
        goto block_19;
    }
    sp14 = 0;
    sp18 = 2;
    goto block_22;
block_19:
    if (EQUIPMENT_ATTACK & M2C_FIELD(equipment_stats, u16 *, 2)) {
        goto block_21;
    }
    sp14 = 0;
    sp18 = 1;
    goto block_22;
block_21:
    sp14 = 1;
    sp18 = 2;
block_22:
    target_choice = 0;
    {
        s32 next_index = action_index;
        asm volatile("" : "+r"(next_index));
        sp1C = next_index + 1;
    }
loop_23:
    target_count = 0;
    sp10 = sp14;
    sp28 = target_choice + 1;
    if (sp14 < sp18) {
        goto loop_25;
    }
    goto block_72;
loop_25:
    if (sp10 != 0) {
        goto block_27;
    }
    var_r4 = attacker_side_index;
    goto block_28;
block_27:
    var_r4 = attacker_side_index ^ 1;
block_28:
    temp_r0_5 = M2C_FIELD(equipment_stats, u8 *, 9);
    if ((u32) temp_r0_5 <= 7U) {
        goto block_30;
    }
    goto block_70;
block_30:
    switch (temp_r0_5) {                            /* jump table: jtbl_080E874C */
case EQUIPMENT_AREA_SINGLE:
    if ((IsBattleTargetInRange(attacker_side_index, attacker_slot_index, (s32) var_r4, target_choice, (s32) M2C_FIELD(equipment_stats, u8 *, 8)) << 0x18) != 0) {
        goto block_33;
    }
    goto block_70;
block_33:
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, target_choice);
    goto block_70;
case EQUIPMENT_AREA_COLUMN:
    if (target_choice <= 2U) {
        goto block_36;
    }
    goto block_70;
block_36:
    if ((IsBattleTargetInRange(attacker_side_index, attacker_slot_index, (s32) var_r4, target_choice, (s32) M2C_FIELD(equipment_stats, u8 *, 8)) << 0x18) != 0) {
        goto block_38;
    }
    goto block_70;
block_38:
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, target_choice);
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, (u32) (u8) (target_choice + 3));
    goto block_70;
case EQUIPMENT_AREA_ROW_PAIR:
    if (target_choice != 2) {
        goto block_41;
    }
    goto block_70;
block_41:
    if (target_choice != 5) {
        goto block_43;
    }
    goto block_70;
block_43:
    if ((IsBattleTargetInRange(attacker_side_index, attacker_slot_index, (s32) var_r4, target_choice, (s32) M2C_FIELD(equipment_stats, u8 *, 8)) << 0x18) != 0) {
        goto block_45;
    }
    goto block_70;
block_45:
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, target_choice);
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, (u32) (u8) sp28);
    goto block_70;
case EQUIPMENT_AREA_ROW:
    if (target_choice == 0) {
        goto block_49;
    }
    if (target_choice == 3) {
        goto block_49;
    }
    goto block_70;
block_49:
    if ((IsBattleTargetInRange(attacker_side_index, attacker_slot_index, (s32) var_r4, target_choice, (s32) M2C_FIELD(equipment_stats, u8 *, 8)) << 0x18) != 0) {
        goto block_51;
    }
    goto block_70;
block_51:
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, target_choice);
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, (u32) (u8) sp28);
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, (u32) (u8) (target_choice + 2));
    goto block_70;
case EQUIPMENT_AREA_TWO_COLUMNS:
    if (target_choice <= 1U) {
        goto block_54;
    }
    goto block_70;
block_54:
    if ((IsBattleTargetInRange(attacker_side_index, attacker_slot_index, (s32) var_r4, target_choice, (s32) M2C_FIELD(equipment_stats, u8 *, 8)) << 0x18) != 0) {
        goto block_56;
    }
    goto block_70;
block_56:
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, target_choice);
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, (u32) (u8) sp28);
    if (target_choice <= 2U) {
        goto block_58;
    }
    goto block_70;
block_58:
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, (u32) (u8) (target_choice + 3));
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, (u32) (u8) (target_choice + 4));
    goto block_70;
case EQUIPMENT_AREA_SIDE:
    if (target_choice == 0) {
        goto block_61;
    }
    goto block_70;
block_61:
    if ((IsBattleTargetInRange(attacker_side_index, attacker_slot_index, (s32) var_r4, 0U, (s32) M2C_FIELD(equipment_stats, u8 *, 8)) << 0x18) != 0) {
        goto block_63;
    }
    goto block_70;
block_63:
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, target_choice);
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, 1U);
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, 2U);
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, 3U);
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, 4U);
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, 5U);
    goto block_70;
case EQUIPMENT_AREA_BOTH_SIDES:
    if (target_choice != 0) {
        goto block_70;
    }
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, target_choice);
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, 1U);
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, 2U);
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, 3U);
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, 4U);
    target_count += BuildBattleTargetPreview(attacker_side_index, attacker_slot_index, equipment_slot, action_index, target_choice, (s32) var_r4, 5U);
    goto block_70;
case EQUIPMENT_AREA_SELF:
    if (var_r4 != attacker_side_index) {
        goto block_70;
    }
    if (target_choice != attacker_slot_index) {
        goto block_70;
    }
    target_count += BuildBattleTargetPreview(attacker_side_index, target_choice, equipment_slot, action_index, target_choice, attacker_side_index, target_choice);
    }
block_70:
    temp_r0_6 = sp10 + 1;
    sp10 = (u32) temp_r0_6;
    if ((u32) temp_r0_6 >= sp18) {
        goto block_72;
    }
    goto loop_25;
block_72:
    if (target_count == 0) {
        goto block_74;
    }
    {
        register u8 *base asm("r2") = (u8 *)0x02034B4C;
        register s32 saved_offset asm("r3") = sp30;
        register s32 inner asm("r4") = action_index;
        register s32 address asm("r1");
        register s32 work asm("r0");
        register s32 outer_product asm("r5");
        register s32 field asm("r6");
        asm volatile("" : "+r"(inner));
        address = saved_offset - inner;
        address <<= 5;
        address += action_index;
        address <<= 2;
        work = 0xA8C;
        outer_product = equipment_slot;
        outer_product *= work;
        asm volatile("" : "+r"(outer_product));
        work = outer_product;
        asm volatile("" : "+r"(work));
        address += work;
        address += (s32)base;
        field = 0x27D2;
        asm volatile("" : "+r"(field));
        address += field;
        work = *(u8 *)address;
        work += 1;
        *(u8 *)address = (u8)work;
    }
block_74:
    target_choice = (u32) (u8) sp28;
    if (target_choice > 5U) {
        goto block_76;
    }
    goto loop_23;
block_76:
    temp_r0_7 = (u8) sp1C;
    action_index = temp_r0_7;
    if ((u32) temp_r0_7 > 2U) {
        goto block_78;
    }
    goto loop_12;
block_78:
    temp_r0_8 = (u8) sp24;
    equipment_slot = temp_r0_8;
    } while ((u32) temp_r0_8 <= 7U);
    ReleaseEquipmentStatBuffer();
    return;
}
