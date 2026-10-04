#include "m2c_prelude.h"
#include "../popups/battle_popup.h"
#include "deck_commands.h"
#include "../combinations/battle_combination.h"

#define SECOND_REG(value)                                                \
    ({                                                                  \
        register s32 second_reg asm("r1");                               \
        asm volatile("" : "=r"(second_reg) : "r"(value));                \
        second_reg;                                                      \
    })
#define ADD_BATTLE_EFFECT_WITH_STACK_ARGUMENTS(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9)       \
    do {                                                                \
        register volatile s32 *be65c_outgoing asm("sp");                 \
        be65c_outgoing[0] = (a4);                                       \
        be65c_outgoing[1] = (a5);                                       \
        be65c_outgoing[2] = (a6);                                       \
        be65c_outgoing[3] = (a7);                                       \
        be65c_outgoing[4] = (a8);                                       \
        be65c_outgoing[5] = (a9);                                       \
        {                                                               \
            register s32 be65c_arg0 asm("r0") = (a0);                    \
            register s32 be65c_arg1 asm("r1") = (a1);                    \
            register s32 be65c_arg2 asm("r2") = (a2);                    \
            register s32 be65c_arg3 asm("r3") = (a3);                    \
            AddBattleEffect(be65c_arg0, be65c_arg1,                        \
                          be65c_arg2, be65c_arg3);                        \
        }                                                               \
    } while (0)
#define QUEUE_BATTLE_EFFECT_DISPLAY_WITH_STACK_ARGUMENTS(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9)       \
    do {                                                                \
        register volatile s32 *be9d8_outgoing asm("sp");                 \
        be9d8_outgoing[0] = (a4);                                       \
        be9d8_outgoing[1] = (a5);                                       \
        be9d8_outgoing[2] = (a6);                                       \
        be9d8_outgoing[3] = (a7);                                       \
        be9d8_outgoing[4] = (a8);                                       \
        be9d8_outgoing[5] = (a9);                                       \
        {                                                               \
            register s32 be9d8_arg0 asm("r0") = (a0);                    \
            register s32 be9d8_arg1 asm("r1") = (a1);                    \
            register s32 be9d8_arg2 asm("r2") = (a2);                    \
            register s32 be9d8_arg3 asm("r3") = (a3);                    \
            QueueBattleEffectDisplay(be9d8_arg0, be9d8_arg1,                        \
                          be9d8_arg2, be9d8_arg3);                        \
        }                                                               \
    } while (0)

M2C_UNK LoadZoidIconGraphics(u8, u8, u16, u8) asm("func_0809A4CC");             /* extern */
M2C_UNK ConfigureBattleUnitSprites(s32, s32, s32, s32, s32, s32, s32) asm("func_080BAF2C"); /* extern */
M2C_UNK ConfigureBattleUnitSpritesWithStackArguments(s32, s32, s32, s32) asm("func_080BAF2C");
M2C_UNK StartBattleCameraTransition(s32, s32, s32, s32) asm("func_080BB224");          /* extern */
s32 IsBattleCameraTransitionComplete() asm("func_080BB654");                                /* extern */
M2C_UNK ClearBattleUnitEffects(s32, u8) asm("func_080BE560");                     /* extern */
M2C_UNK ClearBattlePassiveEquipmentEffects(s32, u8) asm("func_080BE5A8");                     /* extern */
M2C_UNK AddBattleEffect() asm("func_080BE65C");                            /* extern */
M2C_UNK QueueBattleEffectDisplay() asm("func_080BE9D8");                            /* extern */
M2C_UNK SetBattleTurnOrderEntry(u8, s32, u8, s16) asm("func_080C007C");            /* extern */
M2C_UNK SetBattleTurnOrderEntryWide(s32, s32, s32, s32) asm("func_080C007C");
M2C_UNK InsertBattleUnitTurnOrderEntries(s32, u8) asm("func_080C00B0");                     /* extern */
M2C_UNK RemoveBattleUnitFromTurnOrder(s32, u8) asm("func_080C02B4");                     /* extern */
M2C_UNK RemoveBattleUnitFromTurnOrderWide(s32, s32) asm("func_080C02B4");
M2C_UNK BuildBattleTurnOrder(s32) asm("func_080C030C");                         /* extern */
M2C_UNK MarkBattleUnitDestroyed(s32, u8) asm("func_080C04DC");                     /* extern */
M2C_UNK MarkBattleUnitDestroyedWide(s32, s32) asm("func_080C04DC");
M2C_UNK ReviveBattleUnit(s32, u8) asm("func_080C052C");                     /* extern */
s32 IsBattleCombinationFormationValid(s32, s32, u8) asm("func_080C0C54");                    /* extern */
s32 IsBattleCombinationFormationValidWide(s32, s32, s32) asm("func_080C0C54");
M2C_UNK ResetBattleCombinationPresentation() asm("func_080C2DB0");                            /* extern */
M2C_UNK AbsorbBattleCombinationUnit(s32, u8, u8) asm("func_080C3440");                 /* extern */
M2C_UNK AbsorbBattleCombinationUnitWide(s32, s32, s32) asm("func_080C3440");
M2C_UNK SetBattleUnitCombinedModel(s32, u8, s32) asm("func_080C34A4");                /* extern */
M2C_UNK SetBattleUnitCombinedModelWide(s32, s32, s32) asm("func_080C34A4");
M2C_UNK LoadBattlePopupGraphics(s32, s32) asm("func_080C9F00");                    /* extern */
s32 AreBattleUnitPopupsFinished() asm("func_080CA140");                                /* extern */
M2C_UNK RecalculateBattleUnitStats(s32, u8) asm("func_080E8B08");                     /* extern */
M2C_UNK RecalculateBattleUnitStatsWide(s32, s32) asm("func_080E8B08");
M2C_UNK ApplyBattlePassiveEquipmentEffects(s32, u8) asm("func_080E90AC");                     /* extern */
s32 IsBattleUnitActive(u32, u32) asm("func_080E9D88");                        /* extern */
u16 DivideSigned32(s32, s16) asm("func_080ECD98");                        /* extern */
u16 DivideSigned32FullArguments(s32, s32) asm("func_080ECD98");
u8 ModuloUnsigned32(u8, u8) asm("func_080ECF78");                           /* extern */
u8 ModuloUnsigned32FullArguments(s32, s32) asm("func_080ECF78");
M2C_UNK CopyBytes(void *, void *, s32) asm("func_080ED038");         /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */
M2C_UNK jtbl_080C3674();                            /* static */

void ExecuteBattleDeckCommand(u8 side) asm("func_080C35C4");

void ExecuteBattleDeckCommand(u8 side) {
    volatile s32 command_side;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp2C;
    s32 sp30;
    s32 frame_home34;
    s32 frame_home38;
    s32 frame_home3C;
    s32 frame_home40;
    s32 frame_home44;
    s32 frame_home48;
    s32 frame_home4C;
    s32 frame_home50;
    s32 frame_home54;
    s32 frame_home58;
    s32 sp5C;
    s32 sp60;
    s32 sp64;
    s32 sp68;
    register volatile s32 *physical_stack asm("sp");
#define command_side_times_four physical_stack[27]
#define sp28 physical_stack[10]
#define sp34 physical_stack[13]
#define sp38 physical_stack[14]
#define sp3C physical_stack[15]
#define sp40 physical_stack[16]
#define sp44 physical_stack[17]
#define sp48 physical_stack[18]
#define sp4C physical_stack[19]
#define sp50 physical_stack[20]
#define sp54 physical_stack[21]
#define sp58 physical_stack[22]
    s32 sp70;
    s32 sp74;
    s32 sp78;
    s32 sp7C;
    s16 temp_r0_2;
    s16 temp_r0_31;
    s32 temp_r0_4;
    register s32 temp_r1_2 asm("r1");
    register s32 reward_command_effect asm("r5");
    register s32 reward_command_arg1 asm("r1");
    register s32 reward_command_neg asm("r2");
    register s32 friendship_zero asm("r4");
    u16 friendship_max;
    s32 friendship_offset;
    register s32 the_brave_row2 asm("r4");
    register s32 the_brave_record_offset asm("r0");
    register u8 *the_brave_scan_base asm("r5");
    register s32 the_brave_r6_guard asm("r6");
    register s32 the_brave_zero asm("r5");
    s32 redistribution_side_sum;
    s32 redistribution_list_offset;
    s32 redistribution_copy_list_offset;
    u8 *redistribution_copy_dest;
    s32 gravity_storm_list_offset;
    s32 gravity_storm_first_list_offset;
    s32 temp_r0_40;
    register s32 temp_r0_53 asm("r0");
    s32 temp_r0_55;
    s32 temp_r0_57;
    s32 temp_r0_62;
    register s32 temp_r0_64 asm("r0");
    s32 temp_r0_73;
    s32 temp_r0_79;
    s32 temp_r1_10;
    register s32 temp_r1_11 asm("r1");
    s32 temp_r1_13;
    s32 temp_r1_16;
    register s32 temp_r1_17 asm("r1");
    register s32 temp_r1_18 asm("r1");
    s32 temp_r1_3;
    s32 temp_r1_5;
    s32 temp_r1_8;
    register s32 temp_r2_10 asm("r2");
    register s32 temp_r2_11 asm("r2");
    register s32 temp_r2_12 asm("r2");
    register s32 temp_r2_13 asm("r2");
    register s32 temp_r2_6 asm("r2");
    register s32 temp_r2_7 asm("r2");
    register s32 temp_r2_8 asm("r2");
    register s32 temp_r2_9 asm("r2");
    register s32 temp_r3_2 asm("r3");
    register s32 temp_r3_3 asm("r3");
    register s32 temp_r4_21 asm("r4");
    register s32 temp_r4_22 asm("r4");
    register s32 temp_r4_2 asm("r4");
    register s32 temp_r4_5 asm("r4");
    register s32 temp_r4_6 asm("r4");
    register s32 redistribution_second_index4 asm("r5");
    register s32 temp_r4_8 asm("r4");
    register s32 temp_r4_9 asm("r4");
    register s32 temp_r6_5 asm("r6");
    s32 temp_ret;
    register s32 next_battle_phase asm("r0");
    register s32 *final_state asm("r1");
    register s32 parts_or_decoy_display_kind asm("r0");
    register s32 parts_or_decoy_equipment_slot asm("r2");
    register s32 parts_or_decoy_zero asm("r3");
    register u8 *gravity_storm_wait_flag_address asm("r3");
    register s32 griffin_battle_state_address asm("r3");
    register u32 gojulox_gattai_copy asm("r6");
    s32 opening_unit_or_gravity_storm_side;
    register s32 opening_side asm("r8");
    register s32 muddy_ground_side asm("r8");
    register s32 hazard_display_side asm("r8");
    register s32 off_ground_mines_side asm("r8");
    register s32 water_mines_side asm("r8");
    register s32 obstacles_side asm("r8");
    register s32 coercion_side asm("r8");
    register s32 false_nego_side asm("r8");
    register s32 t_s_warp_side asm("r8");
    register s32 confusion_display_side asm("r8");
    s32 temp_r0_10;
    register u16 temp_r0_11 asm("r0");
    register s32 temp_r0_3 asm("r0");
    u16 temp_r0_7;
    register u16 two_arm_lizard_initial_hp asm("r9");
    register u16 two_arm_lizard_hp_total asm("r9");
    register u16 killer_dome_initial_hp asm("r9");
    register u16 killer_dome_hp_total asm("r9");
    register u16 giga_cannon_initial_hp asm("r9");
    register u16 giga_cannon_hp_total asm("r9");
    register u16 lord_gale_initial_hp asm("r9");
    register u16 lord_gale_hp_total asm("r9");
    register u16 fuzor_dragon_hp_total asm("r9");
    register u16 chimera_dragon_hp_total asm("r9");
    register u16 gojulox_hp_total asm("r9");
    register u32 griffin_hp_total asm("r9");
    register u8 *redistribution_players asm("r9");
    register u8 *redistribution_slots asm("r6");
    register u8 *gravity_storm_slots asm("r9");
    register u8 *gravity_storm_lists asm("r8");
    register s32 *gravity_storm_first_dest asm("r2");
    u32 command_dispatch_index;
    register u8 *persistent_command_source asm("r0");
    u8 *var_r0_5;
    register u8 *persistent_command_destination asm("r1");
    u8 temp_r0;
    u8 temp_r0_12;
    u8 temp_r0_13;
    u8 temp_r0_14;
    u8 temp_r0_15;
    u8 temp_r0_16;
    u8 temp_r0_17;
    u8 temp_r0_18;
    u8 temp_r0_19;
    u8 temp_r0_20;
    u8 temp_r0_21;
    u8 temp_r0_22;
    u8 temp_r0_23;
    u8 temp_r0_24;
    u8 temp_r0_25;
    u8 temp_r0_26;
    u8 temp_r0_27;
    u8 temp_r0_28;
    u8 temp_r0_29;
    u8 temp_r0_33;
    register u32 temp_r0_34 asm("r9");
    u8 temp_r0_35;
    u8 temp_r0_36;
    u8 temp_r0_37;
    u8 temp_r0_38;
    u8 temp_r0_39;
    register u32 temp_r0_41 asm("r9");
    u8 temp_r0_42;
    u8 temp_r0_43;
    u8 temp_r0_44;
    u8 temp_r0_45;
    u8 temp_r0_46;
    u8 temp_r0_47;
    u8 temp_r0_48;
    u8 temp_r0_49;
    u8 temp_r0_50;
    u8 temp_r0_51;
    u8 temp_r0_52;
    u8 temp_r0_54;
    u8 temp_r0_5;
    u8 temp_r0_61;
    u8 temp_r0_68;
    u8 temp_r0_6;
    u8 temp_r0_75;
    u8 temp_r0_76;
    u8 temp_r0_77;
    u8 temp_r0_78;
    u8 temp_r0_80;
    u8 temp_r0_8;
    register u32 temp_r4_10 asm("r4");
    register u32 temp_r4_23 asm("r4");
    register u32 temp_r5_2 asm("r5");
    u32 temp_r6_2;
    u8 temp_r6_3;
    u8 temp_r6_4;
    u8 temp_r6_6;
    u8 temp_r6_7;
    u32 temp_r6_8;
    u8 temp_r7;
    u8 var_r1;
    u8 parts_or_decoy_unit_slot;
    register s32 var_r4 asm("r4");
    u8 var_r6_2;
    u8 gravity_storm_turn_entry_index;
    register u32 protagonist_slot_carrier asm("r7");
    u8 redistribution_turn_entry_slot;
    u8 var_r7_12;
    u8 t_s_warp_display_slot;
    u8 confusion_display_unit_slot;
    register u32 two_arm_lizard_rank_base asm("r7");
    u8 var_r7_17;
    u8 var_r7_18;
    u8 gojulox_component_slot;
    u32 var_r7_20;
    register u32 lord_gale_rank_base asm("r7");
    u8 the_brave_revive_slot;
    u8 muddy_ground_slot;
    u8 hazard_display_slot;
    u8 off_ground_mines_slot;
    u8 water_mines_slot;
    u8 obstacles_slot;
    u8 coercion_slot;
    u8 false_nego_slot;
    register u32 logistics_rear_slot asm("r8");
    register u32 logistics_front_slot asm("r8");
    register u32 reward_display_slot asm("r8");
    register u32 covering_fire_slot asm("r8");
    register u32 strategy_meet_slot asm("r8");
    register u32 redistribution_saved_slot asm("r8");
    register u32 redistribution_destination_slot asm("r8");
    register u32 redistribution_wait_slot asm("r8");
    register u32 parts_removal_equipment_slot asm("r8");
    register u32 gods_territory_slot asm("r8");
    register u32 gravity_storm_side asm("r8");
    register u32 friendship_slot asm("r8");
    register u32 confusion_saved_entry_index asm("r8");
    register u32 confusion_destination_entry_index asm("r8");
    register u32 fionas_prayer_pilot_slot asm("r8");
    register u32 junos_prayer_pilot_slot asm("r8");
    register u32 two_arm_lizard_direction asm("r8");
    register u32 fuzor_dragon_direction asm("r8");
    register u32 chimera_dragon_direction asm("r8");
    register u32 griffin_component_slot asm("r8");
    register s32 killer_dome_front_slot asm("r8");
    register u32 conservation_slot asm("r8");
    register s32 giga_cannon_front_slot asm("r8");
    register u32 lord_gale_direction asm("r8");
    register u32 charge_energy_slot asm("r8");
    register u32 kings_way_heal_slot asm("r8");
    register u32 kings_way_pilot_slot asm("r8");
    register u32 the_brave_pilot_slot asm("r8");
    register u32 the_brave_icon_slot asm("r8");
    register u32 no_return_slot asm("r8");
    register s32 var_sl asm("sl");
    register u32 gravity_storm_destination_slot asm("sl");
    register u32 gravity_storm_wait_slot asm("sl");
    register void *temp_r0_30 asm("r0");
    void *temp_r0_32;
    void *temp_r0_56;
    void *temp_r0_58;
    void *temp_r0_59;
    void *temp_r0_60;
    void *temp_r0_63;
    void *temp_r0_65;
    void *temp_r0_66;
    void *temp_r0_67;
    void *temp_r0_69;
    void *temp_r0_70;
    void *temp_r0_71;
    void *temp_r0_72;
    void *temp_r0_74;
    void *temp_r0_9;
    void *temp_r1_12;
    void *temp_r1_14;
    register void *temp_r1_15 asm("r1");
    void *temp_r1_19;
    void *killer_dome_component_record;
    void *temp_r1_21;
    void *temp_r1_22;
    register void *temp_r1_23 asm("r1");
    void *temp_r1_4;
    s32 temp_r1_6;
    register s32 temp_r1_7 asm("r1");
    register void *temp_r1_9 asm("r1");
    register void *friendship_unit_record asm("r2");
    register void *conservation_unit_record asm("r2");
    register void *charge_energy_unit_record asm("r2");
    register void *kings_way_unit_record asm("r2");
    register void *the_brave_destroyed_unit_record asm("r2");
    register void *temp_r3 asm("r3");
    register void *temp_r4 asm("r4");
    register void *two_arm_lizard_component_record asm("r4");
    register void *fuzor_dragon_host_record asm("r4");
    register void *fuzor_dragon_combined_record asm("r4");
    register void *chimera_dragon_host_record asm("r4");
    register void *temp_r4_15 asm("r4");
    register void *gojulox_host_record asm("r4");
    register void *temp_r4_17 asm("r4");
    register void *griffin_component_record asm("r4");
    register void *griffin_combined_record asm("r4");
    register void *temp_r4_20 asm("r4");
    register void *lord_gale_pilot_component_record asm("r4");
    register void *link_support_payload asm("r4");
    register void *switch_unit_record asm("r4");
    register void *temp_r4_7 asm("r4");
    register void *temp_r5 asm("r5");
    register void *lord_gale_host_record asm("r5");
    register void *temp_r5_3 asm("r5");
    register void *two_arm_lizard_host_record asm("r5");
    register void *fuzor_dragon_pilot_component_record asm("r5");
    register void *chimera_dragon_pilot_component_record asm("r5");
    register void *temp_r5_7 asm("r5");
    register void *killer_dome_host_record asm("r5");
    register void *giga_cannon_host_record asm("r5");
    register u8 *redistribution_lists asm("r5");
    register s32 *redistribution_first_dest asm("r2");
    register u8 *redistribution_wait_records asm("r5");
    register u8 *redistribution_wait_flags asm("r4");
    register s32 **redistribution_wait_record0 asm("r6");
    u8 *var_r7_10;
    register u8 *unused_r8_pointer_carrier asm("r8");
    volatile u8 frame_padding[36];

    asm volatile("" : "=m"(frame_home34), "=m"(frame_home38),
                  "=m"(frame_home3C), "=m"(frame_home40),
                  "=m"(frame_home44), "=m"(frame_home48),
                  "=m"(frame_home4C), "=m"(frame_home50),
                  "=m"(frame_home54), "=m"(frame_home58));
    command_side = (s32) side;
    opening_side = 0;
    {
        register s32 opening_sp6c asm("r1") = command_side;
        opening_sp6c *= 4;
        command_side_times_four = opening_sp6c;
    }
    {
    register volatile s32 *opening_stack asm("sp");
    register u32 opening_successor asm("r2");
    register u32 opening_outer asm("r3");
    register u32 opening_triple asm("r0");
    register u32 opening_row asm("r4");
    register u32 opening_slots asm("r5") = 0x02032EBC;
    register u32 opening_slot_address asm("r0");
loop_1:
    opening_unit_or_gravity_storm_side = 0;
    opening_successor = opening_side;
    opening_successor += 1;
    opening_stack[24] = opening_successor;
    opening_outer = opening_side;
    asm volatile("" : "+r"(opening_outer));
    opening_triple = opening_outer * 2;
    opening_triple += opening_side;
    opening_row = opening_triple * 8;
loop_2:
    if ((IsBattleUnitActive(opening_side, opening_unit_or_gravity_storm_side) << 0x18) == 0) {
        goto block_4;
    }
    opening_slot_address = opening_unit_or_gravity_storm_side * 4;
    opening_slot_address += opening_row;
    opening_slot_address += opening_slots;
    *(s32 *)(*(u32 *)opening_slot_address + BATTLE_SPRITE_OFFSET(user_data.words[1])) = 0x20000;
block_4:
    {
        register u32 opening_next asm("r0");
        opening_next = opening_unit_or_gravity_storm_side + 1;
        opening_next <<= 24;
        opening_unit_or_gravity_storm_side = opening_next >> 24;
    }
    if ((u32)opening_unit_or_gravity_storm_side <= 5U) {
        goto loop_2;
    }
    {
        register u32 opening_outer_reload asm("r4") = opening_stack[24];
        register u32 opening_outer_normalized asm("r0");

        asm volatile(
            "lsl %0, %1, #24\n\t"
            "lsr %0, %0, #24"
            : "=r"(opening_outer_normalized)
            : "r"(opening_outer_reload));
        opening_side = opening_outer_normalized;
        if (opening_outer_normalized <= 1U) {
            goto loop_1;
        }
    }
    }
    StartBattleCameraTransition(2, command_side, 0, 0);
    goto loop_9;
block_8:
    YieldTaskForUpdates(1);
loop_9:
    if ((IsBattleCameraTransitionComplete() << 0x18) == 0) {
        goto block_8;
    }
    {
        register u32 opening_exit_base asm("r0") = BATTLE_STATE_RAM;
        register s32 opening_exit_state asm("r5");
        register u8 *opening_exit_address asm("r1");
        register u32 opening_exit_offset asm("r2");

        asm volatile("" : "+r"(opening_exit_base));
        opening_exit_state = command_side_times_four;
        opening_exit_address =
            (u8 *)(opening_exit_state + opening_exit_base);
        opening_exit_offset = BATTLE_DECK_PREPARATION_OFFSET(actions[0].command_id);
        opening_exit_address += opening_exit_offset;
        {
            register u32 opening_exit_value asm("r1") =
                *opening_exit_address;
            opening_exit_value -= 1;
            asm volatile("" : "+r"(opening_exit_value));
            command_dispatch_index = opening_exit_value;
        }
    }
    if (command_dispatch_index > 0x32U) {
        goto command_done;
    }
    switch (command_dispatch_index) {                              /* jump table: jtbl_080C3674 */
case DECK_COMMAND_FRIENDSHIP - DECK_COMMAND_FRIENDSHIP:
    {
        register s32 friendship_state_seed asm("r3") = 0;
        asm volatile("" : "+r"(friendship_state_seed));
        friendship_slot = friendship_state_seed;
    }
loop_13:
    if ((IsBattleUnitActive(command_side, friendship_slot) << 0x18) == 0) {
        goto block_17;
    }
    {
        register s32 friendship_row_base asm("r4") = command_side_times_four;
        register s32 friendship_side asm("r5") = command_side;
        asm volatile("" : "+r"(friendship_row_base), "+r"(friendship_side));
        friendship_unit_record =
            (void *)((((friendship_row_base + friendship_side) * 8)
                      - friendship_side) << 7);
    }
    asm volatile(
        "mov r7, r8\n\t"
        "lsl %0, r7, #2\n\t"
        "add %0, r8\n\t"
        "lsl %0, %0, #3\n\t"
        "sub %0, %0, r7\n\t"
        "lsl %0, %0, #4"
        : "=l"(friendship_offset)
        :
        : "cc");
    {
        register u32 friendship_record_base asm("r1") = BATTLE_STATE_RAM;
        asm volatile("" : "+r"(friendship_record_base));
        friendship_offset += friendship_record_base;
    }
    friendship_unit_record += friendship_offset;
    {
        register s32 friendship_sum asm("r0");
        asm volatile(
            "mov r1, #58\n\t"
            "ldrsh %0, [%1, r1]\n\t"
            "lsr r1, %0, #31\n\t"
            "add %0, %0, r1\n\t"
            "asr %0, %0, #1\n\t"
            "ldrh r3, [%1, #6]\n\t"
            "add %0, %0, r3"
            : "=r"(friendship_sum)
            : "l"(friendship_unit_record)
            : "cc");
        temp_r0_3 = friendship_sum;
    }
    friendship_zero = 0;
    BATTLE_UNIT_FIELD(friendship_unit_record, u16, hp) = temp_r0_3;
    temp_r0_3 = (s16) temp_r0_3;
    friendship_max = BATTLE_UNIT_FIELD(friendship_unit_record, u16, max_hp);
    if (temp_r0_3 <= (s32) BATTLE_UNIT_FIELD(friendship_unit_record, s16, max_hp)) {
        goto block_16;
    }
    BATTLE_UNIT_FIELD(friendship_unit_record, u16, hp) = friendship_max;
block_16:
    {
        register volatile s32 *friendship_outgoing asm("sp");
        friendship_outgoing[0] = friendship_zero;
        friendship_outgoing[1] = friendship_zero;
        friendship_outgoing[2] = 1;
        {
            register s32 friendship_half_value asm("r0");
            register s32 friendship_field_offset asm("r7");
            asm volatile(
                "movs %1, #58\n\t"
                "ldrsh %0, [%2, %1]"
                : "=r"(friendship_half_value), "=r"(friendship_field_offset)
                : "r"(friendship_unit_record)
                : "memory");
            friendship_outgoing[3] =
                (s32)(friendship_half_value +
                      ((u32)friendship_half_value >> 0x1F)) >> 1;
        }
        friendship_outgoing[4] = friendship_zero;
        friendship_outgoing[5] = friendship_zero;
    }
    {
        register s32 friendship_arg0 asm("r0") = command_side;
        register s32 friendship_arg1 asm("r1") = friendship_slot;
        register s32 friendship_arg2 asm("r2") = -1;
        register s32 friendship_arg3 asm("r3") = 0;
        asm volatile("" : "+r"(friendship_arg0), "+r"(friendship_arg1),
                     "+r"(friendship_arg2), "+r"(friendship_arg3));
        QueueBattleEffectDisplay(friendship_arg0, friendship_arg1, friendship_arg2, friendship_arg3);
    }
block_17:
    temp_r0_5 = friendship_slot + 1;
    friendship_slot = temp_r0_5;
    if ((u32) temp_r0_5 <= 5U) {
        goto loop_13;
    }
    return;
case DECK_COMMAND_CONSERVATION - DECK_COMMAND_FRIENDSHIP: {
    register s32 conservation_zero asm("r4");
    register s32 conservation_base asm("r3");
    register s32 conservation_unit asm("r5");
    {
        register s32 conservation_initial_zero asm("r0") = 0;
        conservation_slot = conservation_initial_zero;
    }
    conservation_zero = 0;
loop_21:
    if ((IsBattleUnitActive(command_side, conservation_slot) << 0x18) == 0) {
        goto block_23;
    }
    RemoveBattleUnitFromTurnOrderWide(command_side, conservation_slot);
    {
        register volatile s32 *conservation_outgoing asm("sp");
        conservation_outgoing[0] = conservation_zero;
        conservation_outgoing[1] = conservation_zero;
        conservation_outgoing[2] = BATTLE_EFFECT_TURN_MARKER;
        conservation_outgoing[3] = conservation_zero;
        conservation_outgoing[4] = 1;
        conservation_outgoing[5] = conservation_zero;
        {
            register s32 conservation_arg0 asm("r0") = command_side;
            register s32 conservation_arg1 asm("r1") = conservation_slot;
            register s32 conservation_arg2 asm("r2") = -1;
            register s32 conservation_arg3 asm("r3") = 0;
            AddBattleEffect(conservation_arg0, conservation_arg1,
                          conservation_arg2, conservation_arg3);
        }
    }
block_23:
    temp_r0_6 = conservation_slot + 1;
    conservation_slot = temp_r0_6;
    if ((u32) temp_r0_6 <= 5U) {
        goto loop_21;
    }
    {
        register s32 conservation_row_base asm("r1") = command_side_times_four;
        register s32 conservation_row_side asm("r3") = command_side;
        register s32 conservation_row asm("r2");
        asm volatile("" : "+r"(conservation_row_base),
                     "+r"(conservation_row_side));
        asm volatile("add %0, %1, %2"
                     : "=r"(conservation_row)
                     : "r"(conservation_row_base), "r"(conservation_row_side)
                     : "cc");
        conservation_row *= 8;
        conservation_row -= conservation_row_side;
        conservation_unit_record = conservation_row << 7;
    }
    conservation_base = BATTLE_STATE_RAM;
    {
        register u32 conservation_scale asm("r0") = 0x94;
        conservation_zero = command_side;
        conservation_unit = conservation_zero;
        conservation_unit *= conservation_scale;
    }
    conservation_unit += conservation_base;
    {
        register u32 conservation_field asm("r7") = BATTLE_DECK_PREPARATION_OFFSET(payloads);
        conservation_unit += conservation_field;
    }
    {
        register s32 conservation_effect asm("r3");
        register u32 conservation_current asm("r0");
        register volatile s32 *reward_command_outgoing asm("sp");

        conservation_unit_record += (M2C_FIELD(conservation_unit, u8 *, 0) * 0x270) + conservation_base;
        conservation_effect = BATTLE_UNIT_FIELD(conservation_unit_record, u16, max_ep);
        conservation_current = BATTLE_UNIT_FIELD(conservation_unit_record, u16, ep);
        asm volatile("" : "+r"(conservation_effect), "+r"(conservation_current));
        conservation_effect -= conservation_current;
        conservation_effect = (s16)conservation_effect;
        asm volatile("add %0, %1, %0"
                     : "+r"(conservation_current)
                     : "r"(conservation_effect)
                     : "cc");
        conservation_zero = 0;
        BATTLE_UNIT_FIELD(conservation_unit_record, u16, ep) = (u16)conservation_current;
        var_r1 = M2C_FIELD(conservation_unit, u8 *, 0);
        reward_command_arg1 = var_r1;
        reward_command_neg = -1;
        reward_command_outgoing[0] = conservation_zero;
        reward_command_outgoing[1] = conservation_zero;
        reward_command_outgoing[2] = 4;
        reward_command_outgoing[3] = conservation_effect;
        reward_command_outgoing[4] = conservation_zero;
        reward_command_outgoing[5] = conservation_zero;
    }
    goto block_33;
}
case DECK_COMMAND_CHARGE_ENERGY - DECK_COMMAND_FRIENDSHIP: {
    u8 *charge_energy_base;
    register u32 charge_energy_cap asm("r4");
    register u8 *charge_energy_unit asm("r6");
    register s32 charge_energy_field asm("r7");
    s32 charge_energy_post_base;
    register s32 charge_energy_row asm("r5");
    register s32 charge_energy_post_side asm("r5");
    register s32 charge_energy_zero asm("r3");
    {
        register s32 charge_energy_initial_scale asm("r0") = 0x94;
        register s32 charge_energy_initial_player asm("r1");
        register s32 charge_energy_initial_base asm("r2");
        register s32 charge_energy_initial_field asm("r3");

        charge_energy_initial_player = command_side;
        var_r4 = charge_energy_initial_player;
        var_r4 *= charge_energy_initial_scale;
        charge_energy_initial_base = BATTLE_STATE_RAM;
        asm volatile("" : "+r"(charge_energy_initial_base));
        var_r4 += charge_energy_initial_base;
        charge_energy_initial_field = BATTLE_DECK_PREPARATION_OFFSET(payloads);
        asm volatile("" : "+r"(charge_energy_initial_field));
        var_r4 += charge_energy_initial_field;
    }
    RemoveBattleUnitFromTurnOrder(command_side, *(u8 *)var_r4);
    AddBattleEffect(command_side, *(u8 *)var_r4, -1, 0,
                  0, 0, BATTLE_EFFECT_TURN_MARKER, 0, 1, 0);
    var_r4 = 0;
    charge_energy_slot = var_r4;
    charge_energy_base = (u8 *)BATTLE_STATE_RAM;
    {
        register s32 charge_energy_row_side asm("r7");
        register s32 charge_energy_row_work asm("r0");
        charge_energy_row = command_side_times_four;
        charge_energy_row_side = command_side;
        asm volatile("add %0, %1, %2"
                     : "=r"(charge_energy_row_work)
                     : "r"(charge_energy_row), "r"(charge_energy_row_side)
                     : "cc");
        charge_energy_row_work *= 8;
        charge_energy_row_work -= charge_energy_row_side;
        charge_energy_row = charge_energy_row_work << 7;
    }
loop_27:
    if ((IsBattleUnitActive(command_side, charge_energy_slot) << 0x18) == 0) {
        goto block_29;
    }
    {
        register u32 charge_energy_delta asm("r0");
        register s32 charge_energy_accumulator asm("r1");
        asm volatile(
            "mov r1, r8\n\t"
            "lsl %0, r1, #2\n\t"
            "add %0, r8\n\t"
            "lsl %0, %0, #3\n\t"
            "sub %0, %0, r1\n\t"
            "lsl %0, %0, #4"
            : "=r"(charge_energy_delta)
            :
            : "cc");
        charge_energy_delta += charge_energy_row;
        charge_energy_delta -= 0U - (u32)charge_energy_base;
        charge_energy_delta += 0x40;
        charge_energy_accumulator = (s16)var_r4;
        charge_energy_delta = *(u16 *)charge_energy_delta;
        charge_energy_accumulator += charge_energy_delta;
        var_r4 = (u16)charge_energy_accumulator;
    }
block_29:
    temp_r0_8 = charge_energy_slot + 1;
    charge_energy_slot = temp_r0_8;
    if ((u32) temp_r0_8 <= 5U) {
        goto loop_27;
    }
    {
        register s32 charge_energy_row_base asm("r3") = command_side_times_four;
        register s32 charge_energy_post_row asm("r2");
        charge_energy_post_side = command_side;
        asm volatile("" : "+r"(charge_energy_row_base),
                     "+r"(charge_energy_post_side));
        asm volatile("add %0, %1, %2"
                     : "=r"(charge_energy_post_row)
                     : "r"(charge_energy_row_base), "r"(charge_energy_post_side)
                     : "cc");
        charge_energy_post_row *= 8;
        charge_energy_post_row -= charge_energy_post_side;
        charge_energy_unit_record = charge_energy_post_row << 7;
    }
    charge_energy_post_base = BATTLE_STATE_RAM;
    {
        register s32 charge_energy_unit_base asm("r0") =
            (0x94 * charge_energy_post_side) + charge_energy_post_base;
        charge_energy_field = BATTLE_DECK_PREPARATION_OFFSET(payloads);
        asm volatile("add %0, %1, %2"
                     : "=r"(charge_energy_unit)
                     : "r"(charge_energy_unit_base), "r"(charge_energy_field)
                     : "cc");
    }
    charge_energy_unit_record += (*charge_energy_unit * 0x270) + charge_energy_post_base;
    temp_r0_10 = BATTLE_UNIT_FIELD(charge_energy_unit_record, u16, ep);
    {
        register s32 charge_energy_effect_shift asm("r1") = var_r4 << 16;
        asm volatile("" : "+r"(charge_energy_effect_shift));
        reward_command_effect = charge_energy_effect_shift >> 16;
    }
    asm volatile("add %0, %1, %0"
                 : "+r"(temp_r0_10)
                 : "r"(reward_command_effect)
                 : "cc");
    charge_energy_zero = 0;
    BATTLE_UNIT_FIELD(charge_energy_unit_record, u16, ep) = temp_r0_10;
    {
        register s32 charge_energy_current_signed asm("r0") = (s16)temp_r0_10;
        register u8 *charge_energy_compare_base asm("r2") = (u8 *)charge_energy_unit_record;
        register s32 charge_energy_signed_cap asm("r1");

        charge_energy_cap = BATTLE_UNIT_FIELD(charge_energy_unit_record, u16, max_ep);
        asm volatile("" : "+r"(charge_energy_cap)
                     : "r"(charge_energy_current_signed));
        asm volatile(
            ".syntax unified\n\t"
            "movs %1, #62\n\t"
            "ldrsh %0, [%2, %1]\n\t"
            ".syntax divided"
            : "=l"(charge_energy_signed_cap), "=l"(charge_energy_field)
            : "l"(charge_energy_compare_base));
        if (charge_energy_current_signed <= charge_energy_signed_cap) {
            goto block_32;
        }
    }
    BATTLE_UNIT_FIELD(charge_energy_unit_record, u16, ep) = (u16)charge_energy_cap;
block_32:
    var_r1 = *charge_energy_unit;
    reward_command_arg1 = var_r1;
    reward_command_neg = -1;
    {
        register volatile s32 *reward_command_outgoing asm("sp");
        reward_command_outgoing[0] = charge_energy_zero;
        reward_command_outgoing[1] = charge_energy_zero;
        reward_command_outgoing[2] = 4;
        reward_command_outgoing[3] = reward_command_effect;
        reward_command_outgoing[4] = charge_energy_zero;
        reward_command_outgoing[5] = charge_energy_zero;
    }
block_33:
    QueueBattleEffectDisplay(command_side, reward_command_arg1, reward_command_neg, 0);
    return;
}
case DECK_COMMAND_KINGS_WAY - DECK_COMMAND_FRIENDSHIP: {
    void *kings_way_base2;
    register s32 kings_way_offset asm("r0");
    s32 kings_way_row1;
    s32 kings_way_row2;
    register s32 kings_way_zero asm("r4");
    {
        register s32 kings_way_initial_zero asm("r0") = 0;
        asm volatile("" : "+r"(kings_way_initial_zero));
        kings_way_heal_slot = kings_way_initial_zero;
    }
    {
        register s32 kings_way_row1_base asm("r1") = command_side_times_four;
        register s32 kings_way_row1_side asm("r2") = command_side;
        register s32 kings_way_row1_work asm("r0");
        asm volatile("" : "+r"(kings_way_row1_base), "+r"(kings_way_row1_side));
        asm volatile("add %0, %1, %2"
                     : "=r"(kings_way_row1_work)
                     : "r"(kings_way_row1_base), "r"(kings_way_row1_side)
                     : "cc");
        kings_way_row1_work *= 8;
        kings_way_row1_work -= kings_way_row1_side;
        kings_way_row1 = kings_way_row1_work << 7;
    }
    kings_way_zero = 0;
loop_36:
    if ((IsBattleUnitActive(command_side, kings_way_heal_slot) << 0x18) == 0) {
        goto block_38;
    }
    {
        register s32 kings_way_index_view asm("r3");
        register void *kings_way_record_base asm("r1");

        asm volatile(
            "mov %1, r8\n\t"
            "lsl %0, %1, #2\n\t"
            "add %0, r8\n\t"
            "lsl %0, %0, #3\n\t"
            "sub %0, %0, %1\n\t"
            "lsl %0, %0, #4"
            : "=r"(kings_way_offset), "=r"(kings_way_index_view)
            : "r"(kings_way_heal_slot)
            : "cc");
        kings_way_record_base = (void *)BATTLE_STATE_RAM;
        asm volatile(
            "add %0, %0, %2\n\t"
            "add %1, %3, %0"
            : "+r"(kings_way_offset), "=r"(kings_way_unit_record)
            : "r"(kings_way_record_base), "r"(kings_way_row1)
            : "cc");
    }
    temp_r1_2 = BATTLE_UNIT_FIELD(kings_way_unit_record, u16, max_hp);
    temp_r0_11 = BATTLE_UNIT_FIELD(kings_way_unit_record, u16, hp);
    temp_r1_2 = (s16)(temp_r1_2 - temp_r0_11);
    BATTLE_UNIT_FIELD(kings_way_unit_record, u16, hp) = (u16) (temp_r1_2 + temp_r0_11);
    {
        register volatile s32 *kings_way_outgoing asm("sp");
        kings_way_outgoing[0] = kings_way_zero;
        kings_way_outgoing[1] = kings_way_zero;
        kings_way_outgoing[2] = 1;
        kings_way_outgoing[3] = (s32) temp_r1_2;
        kings_way_outgoing[4] = kings_way_zero;
        kings_way_outgoing[5] = kings_way_zero;
        {
            register s32 kings_way_arg0 asm("r0") = command_side;
            register s32 kings_way_arg1 asm("r1") = kings_way_heal_slot;
            register s32 kings_way_arg2 asm("r2") = -1;
            register s32 kings_way_arg3 asm("r3") = 0;
            asm volatile("" : "+r"(kings_way_arg0), "+r"(kings_way_arg1),
                         "+r"(kings_way_arg2), "+r"(kings_way_arg3));
            QueueBattleEffectDisplay(kings_way_arg0, kings_way_arg1, kings_way_arg2, kings_way_arg3);
        }
    }
block_38:
    temp_r0_12 = kings_way_heal_slot + 1;
    kings_way_heal_slot = temp_r0_12;
    if ((u32) temp_r0_12 <= 5U) {
        goto loop_36;
    }
    {
        register s32 kings_way_second_state_seed asm("r4") = 0;
        asm volatile("" : "+r"(kings_way_second_state_seed));
        kings_way_pilot_slot = kings_way_second_state_seed;
    }
    kings_way_base2 = (void *)BATTLE_STATE_RAM;
    {
        register s32 kings_way_row2_base asm("r7") = command_side_times_four;
        register s32 kings_way_row2_side asm("r1") = command_side;
        asm volatile("" : "+r"(kings_way_row2_base), "+r"(kings_way_row2_side));
        kings_way_row2 =
            (((kings_way_row2_base + kings_way_row2_side) * 8)
             - kings_way_row2_side) << 7;
    }
loop_40:
    if ((IsBattleUnitActive(command_side, kings_way_pilot_slot) << 0x18) == 0) {
        goto block_42;
    }
    {
        register void *kings_way_second_record asm("r0");
        register u32 kings_way_second_index_view asm("r2");
        asm volatile(
            "mov %1, r8\n\t"
            "lsl %0, %1, #2\n\t"
            "add %0, r8\n\t"
            "lsl %0, %0, #3\n\t"
            "sub %0, %0, %1\n\t"
            "lsl %0, %0, #4\n\t"
            "add %0, %0, r4\n\t"
            "add %0, %0, r5"
            : "=r"(kings_way_second_record),
              "=r"(kings_way_second_index_view)
            : "r"(kings_way_pilot_slot), "r"(kings_way_row2),
              "r"(kings_way_base2)
            : "cc");
        if (BATTLE_UNIT_FIELD(kings_way_second_record, u8, pilot_id) == 1) {
            goto block_43;
        }
    }
block_42:
    temp_r0_13 = kings_way_pilot_slot + 1;
    kings_way_pilot_slot = temp_r0_13;
    if ((u32) temp_r0_13 <= 5U) {
        goto loop_40;
    }
block_43:
    RemoveBattleUnitFromTurnOrderWide(command_side, kings_way_pilot_slot);
    {
        register s32 kings_way_final_neg asm("r2") = -1;
        register s32 kings_way_final_zero asm("r1");

        kings_way_final_zero = 0;
        AddBattleEffect(command_side, kings_way_pilot_slot, kings_way_final_neg, 0,
                      kings_way_final_zero, kings_way_final_zero, BATTLE_EFFECT_TURN_MARKER,
                      kings_way_final_zero, 1, kings_way_final_zero);
    }
    return;
}
case DECK_COMMAND_THE_BRAVE - DECK_COMMAND_FRIENDSHIP:
    {
        register s32 the_brave_state_seed asm("r3") = 0;
        asm volatile("" : "+r"(the_brave_state_seed));
        the_brave_pilot_slot = the_brave_state_seed;
    }
    the_brave_scan_base = (u8 *)BATTLE_STATE_RAM;
    {
        register s32 the_brave_scan_row asm("r4");
        register u8 *the_brave_scan_record asm("r0");

        asm volatile(
            "ldr r4, [sp, #108]\n\t"
            "ldr r7, [sp, #24]\n\t"
            "add r0, r4, r7\n\t"
            "lsl r0, r0, #3\n\t"
            "sub r0, r0, r7\n\t"
            "lsl %0, r0, #7"
            : "=&r"(the_brave_scan_row)
            :
            : "cc");
loop_45:
    if ((IsBattleUnitActive(command_side, the_brave_pilot_slot) << 0x18) == 0) {
        goto block_47;
    }
    asm volatile(
        "mov r1, r8\n\t"
        "lsl %0, r1, #2\n\t"
        "add %0, r8\n\t"
        "lsl %0, %0, #3\n\t"
        "sub %0, %0, r1\n\t"
        "lsl %0, %0, #4\n\t"
        "add %0, %0, %1\n\t"
        "add %0, %0, %2"
        : "=&r"(the_brave_scan_record)
        : "r"(the_brave_scan_row), "r"(the_brave_scan_base)
        : "cc");
    if (BATTLE_UNIT_FIELD(the_brave_scan_record, u8, pilot_id) == 1) {
        goto block_48;
    }
block_47:
    temp_r0_14 = the_brave_pilot_slot + 1;
    the_brave_pilot_slot = temp_r0_14;
    if ((u32) temp_r0_14 <= 5U) {
        goto loop_45;
    }
block_48:
    }
    LoadBattlePopupGraphics(command_side, BATTLE_POPUP_GRAPHICS_DESTROYED_UNIT);
    {
        register s32 the_brave_store_base asm("r2") = BATTLE_STATE_RAM;
        register s32 the_brave_store_index asm("r3") = the_brave_pilot_slot;
        register u8 *the_brave_store_record asm("r1");
        register s32 the_brave_store_row_base asm("r4");
        register s32 the_brave_store_side asm("r5");
        register s32 the_brave_store_row asm("r0");
        asm volatile("" : "+r"(the_brave_store_base), "+r"(the_brave_store_index));
        the_brave_store_record = (u8 *)(the_brave_store_index * 4);
        asm volatile("add %0, r8"
                     : "+l"(the_brave_store_record));
        the_brave_store_record =
            (u8 *)((((u32)the_brave_store_record * 8) - the_brave_store_index) * 16);
        the_brave_store_row_base = command_side_times_four;
        the_brave_store_side = command_side;
        asm volatile("" : "+r"(the_brave_store_row_base), "+r"(the_brave_store_side));
        the_brave_store_row = the_brave_store_row_base + the_brave_store_side;
        the_brave_store_row = (the_brave_store_row * 8) - the_brave_store_side;
        the_brave_store_row <<= 7;
        the_brave_store_record += the_brave_store_row;
        the_brave_store_record += the_brave_store_base;
        BATTLE_UNIT_FIELD(the_brave_store_record, s16, hp) = 0;
    }
    MarkBattleUnitDestroyedWide(command_side, the_brave_pilot_slot);
    goto loop_51;
block_50:
    YieldTaskForUpdates(1);
loop_51:
    temp_ret = AreBattleUnitPopupsFinished();
    if (temp_ret == 0) {
        goto block_50;
    }
    asm volatile("" : "=r"(the_brave_r6_guard));
    the_brave_revive_slot = 0;
    the_brave_zero = 0;
    {
        asm volatile(
            "ldr r1, [sp, #108]\n\t"
            "ldr r2, [sp, #24]\n\t"
            "add r0, r1, r2\n\t"
            "lsl r0, r0, #3\n\t"
            "sub r0, r0, r2\n\t"
            "lsl r4, r0, #7"
            : "=r"(the_brave_row2)
            :
            : "r0", "r1", "r2", "cc", "memory");
    }
loop_53:
    if (the_brave_revive_slot == the_brave_pilot_slot) {
        goto block_57;
    }
    asm volatile(
        "lsl r0, %1, #2\n\t"
        "add r0, r0, %1\n\t"
        "lsl r0, r0, #3\n\t"
        "sub r0, r0, %1\n\t"
        "lsl r0, r0, #4"
        : "=r"(the_brave_record_offset)
        : "r"(the_brave_revive_slot)
        : "cc");
    {
        register u32 the_brave_record_base asm("r1") = BATTLE_STATE_RAM;
        asm volatile(
            "add r0, r0, r1\n\t"
            "add r2, r4, r0"
            : "+r"(the_brave_record_offset), "=r"(the_brave_destroyed_unit_record)
            : "r"(the_brave_record_base), "r"(the_brave_row2)
            : "cc");
    }
    if (BATTLE_UNIT_FIELD(the_brave_destroyed_unit_record, u8, zoid_id) == 0) {
        goto block_57;
    }
    if (!(8 & BATTLE_UNIT_FIELD(the_brave_destroyed_unit_record, u16, flags))) {
        goto block_57;
    }
    ReviveBattleUnit(command_side, the_brave_revive_slot);
    InsertBattleUnitTurnOrderEntries(command_side, the_brave_revive_slot);
    QUEUE_BATTLE_EFFECT_DISPLAY_WITH_STACK_ARGUMENTS(command_side, the_brave_revive_slot, -1, 0,
                      the_brave_zero, the_brave_zero, BATTLE_EFFECT_PRESENTATION_GREEN_STREAK,
                      the_brave_zero, the_brave_zero, the_brave_zero);
block_57:
    the_brave_revive_slot += 1;
    if ((u32) the_brave_revive_slot <= 5U) {
        goto loop_53;
    }
    asm volatile("" :: "r"(the_brave_r6_guard));
    {
        register s32 the_brave_inner_state_seed asm("r3") = 0;
        asm volatile("" : "+r"(the_brave_inner_state_seed));
        the_brave_icon_slot = the_brave_inner_state_seed;
    }
    {
        s32 the_brave_one = 1;
        temp_r4_2 = command_side;
        temp_r4_2 ^= the_brave_one;
    }
    {
        u32 the_brave_data_base = BATTLE_STATE_RAM;
        u32 the_brave_ptr_base = 0x02032E8C;
loop_59:
        if ((IsBattleUnitActive(temp_r4_2, the_brave_icon_slot) << 0x18) == 0) {
            goto block_61;
        }
        {
            register u32 the_brave_index asm("r7") = the_brave_icon_slot;
            register u32 the_brave_index4 asm("r3");
            register u32 the_brave_arg0 asm("r0");
            register u32 the_brave_arg1 asm("r1");

            asm volatile("lsl %0, %1, #2"
                         : "=r"(the_brave_index4)
                         : "r"(the_brave_index));
            temp_r1_4 = (((the_brave_index4 + the_brave_index) * 8) - the_brave_index) * 16;
            temp_r1_4 += temp_r4_2 * 0x1380;
            the_brave_arg0 = M2C_FIELD(temp_r1_4, u8 *, the_brave_data_base);
            the_brave_arg1 = M2C_FIELD((temp_r1_4 + the_brave_data_base), u8 *, 1);
            asm volatile("" : : "r"(the_brave_arg0), "r"(the_brave_arg1));
            {
                register u32 the_brave_player24 asm("r2") = temp_r4_2 * 0x18;
                the_brave_index4 += the_brave_player24;
                the_brave_index4 += the_brave_ptr_base;
                temp_r3 = *(void **)the_brave_index4;
            }
            LoadZoidIconGraphics(the_brave_arg0, the_brave_arg1, M2C_FIELD(temp_r3, u16 *, 0xE), M2C_FIELD(temp_r3, u8 *, 0x10));
        }
block_61:
        temp_r0_15 = the_brave_icon_slot + 1;
        the_brave_icon_slot = temp_r0_15;
        if ((u32) temp_r0_15 <= 5U) {
            goto loop_59;
        }
        return;
    }
case DECK_COMMAND_NO_RETURN - DECK_COMMAND_FRIENDSHIP: {
    s32 no_return_neg;
    s32 no_return_zero;
    s32 no_return_one;
    {
        register s32 no_return_state_seed asm("r0") = 0;
        asm volatile("" : "+r"(no_return_state_seed));
        no_return_slot = no_return_state_seed;
    }
    no_return_neg = -1;
    no_return_zero = 0;
    no_return_one = 1;
    asm volatile("" : "+r"(no_return_neg), "+r"(no_return_zero), "+r"(no_return_one));
loop_64:
    if ((IsBattleUnitActive(command_side, no_return_slot) << 0x18) == 0) {
        goto block_66;
    }
    AddBattleEffect(command_side, no_return_slot, no_return_neg, 0,
                  no_return_zero, no_return_zero, BATTLE_EFFECT_DOUBLE_ATTACK_POWER,
                  no_return_zero, no_return_one, no_return_zero);
    AddBattleEffect(command_side, no_return_slot, no_return_neg, 0,
                  no_return_zero, no_return_zero, BATTLE_EFFECT_HALF_EVASION_RATE,
                  no_return_zero, no_return_one, no_return_zero);
    RecalculateBattleUnitStatsWide(command_side, no_return_slot);
block_66:
    temp_r0_16 = no_return_slot + 1;
    no_return_slot = temp_r0_16;
    if ((u32) temp_r0_16 <= 5U) {
        goto loop_64;
    }
    return;
}
case DECK_COMMAND_MUDDY_GROUND - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *muddy_ground_stack asm("sp");
    s32 muddy_ground_zero;
    {
        register s32 muddy_ground_state_seed asm("r1") = 0;
        asm volatile("" : "+r"(muddy_ground_state_seed));
        muddy_ground_side = muddy_ground_state_seed;
    }
    muddy_ground_zero = 0;
    asm volatile("" : "+r"(muddy_ground_zero));
loop_69:
    {
        register s32 muddy_ground_guard_r5 asm("r5");
        register s32 muddy_ground_guard_r6 asm("r6");
        asm volatile("" : "=r"(muddy_ground_guard_r5), "=r"(muddy_ground_guard_r6));
        muddy_ground_slot = 0;
        asm volatile("" :: "r"(muddy_ground_guard_r5), "r"(muddy_ground_guard_r6));
    }
    {
        register u32 muddy_ground_successor asm("r2") = muddy_ground_side;

        muddy_ground_successor += 1;
        muddy_ground_stack[24] = muddy_ground_successor;
    }
loop_70:
    if ((IsBattleUnitActive(muddy_ground_side, muddy_ground_slot) << 0x18) == 0) {
        goto block_72;
    }
    ADD_BATTLE_EFFECT_WITH_STACK_ARGUMENTS(muddy_ground_side, muddy_ground_slot, -1, 0,
                      muddy_ground_zero, muddy_ground_zero, BATTLE_EFFECT_HALF_EVASION_RATE,
                      muddy_ground_zero, 1, muddy_ground_zero);
    RecalculateBattleUnitStats(muddy_ground_side, muddy_ground_slot);
block_72:
    muddy_ground_slot += 1;
    if ((u32) muddy_ground_slot <= 5U) {
        goto loop_70;
    }
    {
        register u32 muddy_ground_reload asm("r3") = muddy_ground_stack[24];
        register u32 muddy_ground_normalized asm("r0");

        asm volatile(
            "lsl %0, %1, #24\n\t"
            "lsr %0, %0, #24"
            : "=r"(muddy_ground_normalized)
            : "r"(muddy_ground_reload));
        muddy_ground_side = muddy_ground_normalized;
        if (muddy_ground_normalized <= 1U) {
            goto loop_69;
        }
    }
    return;
}
case DECK_COMMAND_LOGISTICS_SUPPORT - DECK_COMMAND_FRIENDSHIP: {
    s32 logistics_support_zero;
    s32 logistics_support_second_zero;
    {
        register s32 logistics_support_state_seed asm("r4") = 3;
        asm volatile("" : "+r"(logistics_support_state_seed));
        logistics_rear_slot = logistics_support_state_seed;
    }
    logistics_support_zero = 0;
loop_76:
    if ((IsBattleUnitActive(command_side, logistics_rear_slot) << 0x18) == 0) {
        goto block_78;
    }
    RemoveBattleUnitFromTurnOrderWide(command_side, logistics_rear_slot);
    ADD_BATTLE_EFFECT_WITH_STACK_ARGUMENTS(command_side, logistics_rear_slot, -1, 0,
                      logistics_support_zero, logistics_support_zero, BATTLE_EFFECT_TURN_MARKER,
                      logistics_support_zero, 1, logistics_support_zero);
block_78:
    temp_r0_18 = logistics_rear_slot + 1;
    logistics_rear_slot = temp_r0_18;
    if ((u32) temp_r0_18 <= 5U) {
        goto loop_76;
    }
    {
        register s32 logistics_support_second_state_seed asm("r5") = 0;
        asm volatile("" : "+r"(logistics_support_second_state_seed));
        logistics_front_slot = logistics_support_second_state_seed;
    }
    logistics_support_second_zero = 0;
loop_80:
    if ((IsBattleUnitActive(command_side, logistics_front_slot) << 0x18) == 0) {
        goto block_82;
    }
    ADD_BATTLE_EFFECT_WITH_STACK_ARGUMENTS(command_side, logistics_front_slot, -1, 0,
                      logistics_support_second_zero, logistics_support_second_zero, BATTLE_EFFECT_DOUBLE_ATTACK_POWER,
                      logistics_support_second_zero, 1, logistics_support_second_zero);
block_82:
    temp_r0_19 = logistics_front_slot + 1;
    logistics_front_slot = temp_r0_19;
    if ((u32) temp_r0_19 <= 2U) {
        goto loop_80;
    }
    return;
}
case DECK_COMMAND_DATA_GATHER_1 - DECK_COMMAND_FRIENDSHIP:
case DECK_COMMAND_DATA_GATHER_2 - DECK_COMMAND_FRIENDSHIP:
case DECK_COMMAND_CORE_SECURITY_1 - DECK_COMMAND_FRIENDSHIP:
case DECK_COMMAND_CORE_SECURITY_2 - DECK_COMMAND_FRIENDSHIP:
case DECK_COMMAND_JUNK_PARTS - DECK_COMMAND_FRIENDSHIP:
case DECK_COMMAND_SUPPLIER - DECK_COMMAND_FRIENDSHIP:
case DECK_COMMAND_PROVEN_HERO - DECK_COMMAND_FRIENDSHIP:
case DECK_COMMAND_AEGIS_PHALANX - DECK_COMMAND_FRIENDSHIP: {
    s32 data_gather_1_zero;
    {
        register u32 data_gather_1_base asm("r7") = BATTLE_STATE_RAM;
        register u32 data_gather_1_carrier asm("r1") = BATTLE_DECK_REWARD_OFFSET(persistent_command_ids);
        register u8 *data_gather_1_dest asm("r0");
        register s32 data_gather_1_dest_index asm("r2");
        register s32 data_gather_1_source_index asm("r3");
        register u32 data_gather_1_source_offset asm("r4");

        asm volatile("" : "+r"(data_gather_1_base), "+r"(data_gather_1_carrier));
        data_gather_1_dest = (u8 *)(data_gather_1_base + data_gather_1_carrier);
        data_gather_1_dest_index = command_side;
        data_gather_1_dest = (u8 *)(
            (u32)data_gather_1_dest_index - (0U - (u32)data_gather_1_dest));
        data_gather_1_source_index = command_side_times_four;
        data_gather_1_carrier = data_gather_1_source_index + data_gather_1_base;
        data_gather_1_source_offset = BATTLE_DECK_PREPARATION_OFFSET(actions[0].command_id);
        asm volatile("" : "+r"(data_gather_1_source_offset));
        data_gather_1_carrier += data_gather_1_source_offset;
        *data_gather_1_dest = *(u8 *)data_gather_1_carrier;
    }
    {
        register s32 data_gather_1_state_seed asm("r5") = 0;
        asm volatile("" : "+r"(data_gather_1_state_seed));
        reward_display_slot = data_gather_1_state_seed;
    }
    data_gather_1_zero = 0;
loop_85:
    QUEUE_BATTLE_EFFECT_DISPLAY_WITH_STACK_ARGUMENTS(command_side, reward_display_slot, -1, 0,
                      data_gather_1_zero, data_gather_1_zero, BATTLE_EFFECT_PRESENTATION_GREEN_STREAK,
                      data_gather_1_zero, data_gather_1_zero, data_gather_1_zero);
    temp_r0_20 = reward_display_slot + 1;
    reward_display_slot = temp_r0_20;
    if ((u32) temp_r0_20 <= 5U) {
        goto loop_85;
    }
    return;
}
case DECK_COMMAND_MINES - DECK_COMMAND_FRIENDSHIP:
case DECK_COMMAND_SANDSTORM - DECK_COMMAND_FRIENDSHIP:
case DECK_COMMAND_BEAM_SCREEN - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *hazard_command_stack asm("sp");
    register s32 hazard_command_next asm("r0");
    s32 hazard_command_zero;
    {
        register u32 hazard_command_base asm("r7") = BATTLE_STATE_RAM;
        register u32 hazard_command_carrier asm("r1") = BATTLE_DECK_REWARD_OFFSET(persistent_command_ids);
        register u8 *hazard_command_dest asm("r0");
        register s32 hazard_command_dest_index asm("r2");
        register s32 hazard_command_source_index asm("r3");
        register u32 hazard_command_source_offset asm("r4");

        asm volatile("" : "+r"(hazard_command_base), "+r"(hazard_command_carrier));
        hazard_command_dest = (u8 *)(hazard_command_base + hazard_command_carrier);
        hazard_command_dest_index = command_side;
        hazard_command_dest = (u8 *)(
            (u32)hazard_command_dest_index - (0U - (u32)hazard_command_dest));
        hazard_command_source_index = command_side_times_four;
        hazard_command_carrier = hazard_command_source_index + hazard_command_base;
        hazard_command_source_offset = BATTLE_DECK_PREPARATION_OFFSET(actions[0].command_id);
        asm volatile("" : "+r"(hazard_command_source_offset));
        hazard_command_carrier += hazard_command_source_offset;
        *hazard_command_dest = *(u8 *)hazard_command_carrier;
    }
    {
        register s32 hazard_command_state_zero asm("r5") = 0;
        asm volatile("" : "+r"(hazard_command_state_zero));
        hazard_display_side = hazard_command_state_zero;
    }
    hazard_command_zero = 0;
loop_89:
    {
        register s32 hazard_command_guard_r5 asm("r5");
        register s32 hazard_command_guard_r6 asm("r6");
        asm volatile("" : "=r"(hazard_command_guard_r5), "=r"(hazard_command_guard_r6));
        hazard_display_slot = 0;
        asm volatile("" :: "r"(hazard_command_guard_r5), "r"(hazard_command_guard_r6));
    }
    hazard_command_stack[24] = hazard_display_side + 1;
loop_90:
    QUEUE_BATTLE_EFFECT_DISPLAY_WITH_STACK_ARGUMENTS(hazard_display_side, hazard_display_slot, -1, 0,
                      hazard_command_zero, hazard_command_zero, BATTLE_EFFECT_PRESENTATION_PURPLE_RING,
                      hazard_command_zero, hazard_command_zero, hazard_command_zero);
    hazard_display_slot += 1;
    if ((u32) hazard_display_slot <= 5U) {
        goto loop_90;
    }
    {
        register s32 hazard_command_reload asm("r1");
        hazard_command_reload = hazard_command_stack[24];
        hazard_command_next = (u8) hazard_command_reload;
    }
    hazard_display_side = hazard_command_next;
    if ((u32) hazard_command_next <= 1U) {
        goto loop_89;
    }
    return;
}
case DECK_COMMAND_OFF_GROUND_MINES - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *off_ground_mines_stack asm("sp");
    register u8 *off_ground_mines_base asm("r6");
    register s32 off_ground_mines_row asm("r5");
    s32 off_ground_mines_zero;
    {
        register s32 off_ground_mines_state_seed asm("r2") = 0;
        asm volatile("" : "+r"(off_ground_mines_state_seed));
        off_ground_mines_side = off_ground_mines_state_seed;
    }
    off_ground_mines_base = (u8 *)BATTLE_STATE_RAM;
    off_ground_mines_zero = 0;
    asm volatile("" : "+r"(off_ground_mines_zero));
loop_95:
    off_ground_mines_slot = 0;
    {
        register s32 off_ground_mines_next asm("r3") = off_ground_mines_side;
        off_ground_mines_next += 1;
        off_ground_mines_stack[24] = off_ground_mines_next;
    }
    asm volatile(
        "mov %0, %1\n\t"
        "lsl r0, %0, #2\n\t"
        "add r0, r8\n\t"
        "lsl r0, r0, #3\n\t"
        "sub r0, r0, %0\n\t"
        "lsl %0, r0, #7"
        : "=&r"(off_ground_mines_row)
        : "r"(off_ground_mines_side)
        : "r0", "cc");
loop_96:
    if ((IsBattleUnitActive(off_ground_mines_side, off_ground_mines_slot) << 0x18) == 0) {
        goto block_99;
    }
    {
        register u8 *off_ground_mines_record asm("r0");

        asm volatile(
            "lsl %0, %1, #2\n\t"
            "add %0, %0, %1\n\t"
            "lsl %0, %0, #3\n\t"
            "sub %0, %0, %1\n\t"
            "lsl %0, %0, #4\n\t"
            "add %0, %0, %2\n\t"
            "add %0, %0, %3"
            : "=&r"(off_ground_mines_record)
            : "r"(off_ground_mines_slot), "r"(off_ground_mines_row), "r"(off_ground_mines_base)
            : "cc");
        if (!(0x40 & BATTLE_UNIT_FIELD(off_ground_mines_record, u8, movement_flags))) {
            goto block_99;
        }
    }
    RemoveBattleUnitFromTurnOrder(off_ground_mines_side, off_ground_mines_slot);
    ADD_BATTLE_EFFECT_WITH_STACK_ARGUMENTS(off_ground_mines_side, off_ground_mines_slot, -1, 0,
                      off_ground_mines_zero, off_ground_mines_zero, BATTLE_EFFECT_TURN_MARKER,
                      off_ground_mines_zero, 1, off_ground_mines_zero);
block_99:
    off_ground_mines_slot += 1;
    if ((u32) off_ground_mines_slot <= 5U) {
        goto loop_96;
    }
    {
        register u32 off_ground_mines_reload asm("r7") = off_ground_mines_stack[24];
        register u32 off_ground_mines_normalized asm("r0");

        asm volatile(
            "lsl %0, %1, #24\n\t"
            "lsr %0, %0, #24"
            : "=r"(off_ground_mines_normalized)
            : "r"(off_ground_mines_reload));
        off_ground_mines_side = off_ground_mines_normalized;
        if (off_ground_mines_normalized <= 1U) {
            goto loop_95;
        }
    }
    return;
}
case DECK_COMMAND_WATER_MINES - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *water_mines_stack asm("sp");
    register s32 water_mines_outer_next asm("r0");
    register u8 *water_mines_base asm("r6");
    register s32 water_mines_row asm("r5");
    s32 water_mines_zero;
    {
        register s32 water_mines_outer_zero asm("r0") = 0;
        asm volatile("" : "+r"(water_mines_outer_zero));
        water_mines_side = water_mines_outer_zero;
    }
    water_mines_base = (u8 *)BATTLE_STATE_RAM;
    water_mines_zero = 0;
    asm volatile("" : "+r"(water_mines_zero));
loop_103:
    water_mines_slot = 0;
    asm volatile(
        "mov r1, r8\n\t"
        "add r1, #1\n\t"
        "str r1, [sp, #96]"
        :
        : "r"(water_mines_side)
        : "memory");
    asm volatile(
        "mov r2, %1\n\t"
        "lsl r0, r2, #2\n\t"
        "add r0, r8\n\t"
        "lsl r0, r0, #3\n\t"
        "sub r0, r0, r2\n\t"
        "lsl %0, r0, #7"
        : "=&r"(water_mines_row)
        : "r"(water_mines_side)
        : "cc");
loop_104:
    if ((IsBattleUnitActive(water_mines_side, water_mines_slot) << 0x18) == 0) {
        goto block_107;
    }
    {
        register u8 *water_mines_record asm("r0");

        asm volatile(
            "lsl %0, %1, #2\n\t"
            "add %0, %0, %1\n\t"
            "lsl %0, %0, #3\n\t"
            "sub %0, %0, %1\n\t"
            "lsl %0, %0, #4\n\t"
            "add %0, %0, %2\n\t"
            "add %0, %0, %3"
            : "=&r"(water_mines_record)
            : "r"(water_mines_slot), "r"(water_mines_row), "r"(water_mines_base)
            : "cc");
        if (!(0x80 & BATTLE_UNIT_FIELD(water_mines_record, u8, movement_flags))) {
            goto block_107;
        }
    }
    RemoveBattleUnitFromTurnOrder(water_mines_side, water_mines_slot);
    ADD_BATTLE_EFFECT_WITH_STACK_ARGUMENTS(water_mines_side, water_mines_slot, -1, 0,
                      water_mines_zero, water_mines_zero, BATTLE_EFFECT_TURN_MARKER,
                      water_mines_zero, 1, water_mines_zero);
block_107:
    water_mines_slot += 1;
    if ((u32) water_mines_slot <= 5U) {
        goto loop_104;
    }
    {
        register s32 water_mines_reload asm("r3");
        water_mines_reload = water_mines_stack[24];
        water_mines_outer_next = (u8) water_mines_reload;
    }
    water_mines_side = water_mines_outer_next;
    if ((u32) water_mines_outer_next <= 1U) {
        goto loop_103;
    }
    return;
}
case DECK_COMMAND_OBSTACLES - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *obstacles_stack asm("sp");
    register u8 *obstacles_base asm("r6");
    register s32 obstacles_row asm("r5");
    register s32 obstacles_outer_next asm("r0");
    s32 obstacles_zero = 0;
    asm volatile("" : "+r"(obstacles_zero));
    obstacles_side = obstacles_zero;
    obstacles_base = (u8 *)BATTLE_STATE_RAM;
loop_111:
    obstacles_slot = 0;
    obstacles_row = obstacles_side + 1;
    obstacles_stack[24] = obstacles_row;
    asm volatile(
        "mov r1, %1\n\t"
        "lsl r0, r1, #2\n\t"
        "add r0, r8\n\t"
        "lsl r0, r0, #3\n\t"
        "sub r0, r0, r1\n\t"
        "lsl %0, r0, #7"
        : "=&r"(obstacles_row)
        : "r"(obstacles_side)
        : "cc");
loop_112:
    if ((IsBattleUnitActive(obstacles_side, obstacles_slot) << 0x18) == 0) {
        goto block_115;
    }
    {
        register u8 *obstacles_record asm("r0");

        asm volatile(
            "lsl %0, %1, #2\n\t"
            "add %0, %0, %1\n\t"
            "lsl %0, %0, #3\n\t"
            "sub %0, %0, %1\n\t"
            "lsl %0, %0, #4\n\t"
            "add %0, %0, %2\n\t"
            "add %0, %0, %3"
            : "=&r"(obstacles_record)
            : "r"(obstacles_slot), "r"(obstacles_row), "r"(obstacles_base)
            : "cc");
        if (BATTLE_UNIT_FIELD(obstacles_record, u8, size_class) == 0) {
            goto block_115;
        }
    }
    RemoveBattleUnitFromTurnOrder(obstacles_side, obstacles_slot);
    ADD_BATTLE_EFFECT_WITH_STACK_ARGUMENTS(obstacles_side, obstacles_slot, -1, 0,
                      obstacles_zero, obstacles_zero, BATTLE_EFFECT_TURN_MARKER,
                      obstacles_zero, 1, obstacles_zero);
block_115:
    obstacles_slot += 1;
    if ((u32) obstacles_slot <= 5U) {
        goto loop_112;
    }
    {
        register s32 obstacles_reload asm("r2");
        obstacles_reload = obstacles_stack[24];
        obstacles_outer_next = (u8) obstacles_reload;
    }
    obstacles_side = obstacles_outer_next;
    if ((u32) obstacles_outer_next <= 1U) {
        goto loop_111;
    }
    return;
}
case DECK_COMMAND_COERCION - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *coercion_stack asm("sp");
    register u8 *coercion_base asm("r6");
    register s32 coercion_row asm("r5");
    register s32 coercion_outer_next asm("r0");
    s32 coercion_zero;
    {
        register s32 coercion_state_seed asm("r3") = 0;
        asm volatile("" : "+r"(coercion_state_seed));
        coercion_side = coercion_state_seed;
    }
    coercion_base = (u8 *)BATTLE_STATE_RAM;
    coercion_zero = 0;
    asm volatile("" : "+r"(coercion_zero));
loop_120:
    coercion_slot = 0;
    coercion_row = coercion_side + 1;
    coercion_stack[24] = coercion_row;
    asm volatile(
        "mov r1, %1\n\t"
        "lsl r0, r1, #2\n\t"
        "add r0, r8\n\t"
        "lsl r0, r0, #3\n\t"
        "sub r0, r0, r1\n\t"
        "lsl %0, r0, #7"
        : "=&r"(coercion_row)
        : "r"(coercion_side)
        : "cc");
loop_121:
    if ((IsBattleUnitActive(coercion_side, coercion_slot) << 0x18) == 0) {
        goto block_124;
    }
    {
        register u8 *coercion_record asm("r0");

        asm volatile(
            "lsl %0, %1, #2\n\t"
            "add %0, %0, %1\n\t"
            "lsl %0, %0, #3\n\t"
            "sub %0, %0, %1\n\t"
            "lsl %0, %0, #4\n\t"
            "add %0, %0, %2\n\t"
            "add %0, %0, %3"
            : "=&r"(coercion_record)
            : "r"(coercion_slot), "r"(coercion_row), "r"(coercion_base)
            : "cc");
        if (BATTLE_UNIT_FIELD(coercion_record, u8, size_class) == 4) {
            goto block_124;
        }
    }
    RemoveBattleUnitFromTurnOrder(coercion_side, coercion_slot);
    ADD_BATTLE_EFFECT_WITH_STACK_ARGUMENTS(coercion_side, coercion_slot, -1, 0,
                      coercion_zero, coercion_zero, BATTLE_EFFECT_TURN_MARKER,
                      coercion_zero, 1, coercion_zero);
block_124:
    coercion_slot += 1;
    if ((u32) coercion_slot <= 5U) {
        goto loop_121;
    }
    {
        register s32 coercion_reload asm("r2");
        coercion_reload = coercion_stack[24];
        coercion_outer_next = (u8) coercion_reload;
    }
    coercion_side = coercion_outer_next;
    if ((u32) coercion_outer_next <= 1U) {
        goto loop_120;
    }
    return;
}
case DECK_COMMAND_FALSE_NEGO - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *false_nego_stack asm("sp");
    register u8 *false_nego_base asm("r9");
    register u8 *false_nego_selection asm("r5");
    register s32 false_nego_stride asm("r6");
    register s32 false_nego_offset asm("r0");
    register s32 false_nego_outer_next asm("r0");
    s32 false_nego_zero;
    {
        register s32 false_nego_state_seed asm("r3") = 0;
        asm volatile("" : "+r"(false_nego_state_seed));
        false_nego_side = false_nego_state_seed;
    }
    {
        register u8 *false_nego_base_seed asm("r4") = (u8 *)BATTLE_STATE_RAM;
        asm volatile("" : "+r"(false_nego_base_seed));
        false_nego_base = false_nego_base_seed;
    }
    false_nego_zero = 0;
    asm volatile("" : "+r"(false_nego_zero));
    false_nego_stride = 0x94;
loop_128:
    false_nego_slot = 0;
    false_nego_selection = (u8 *)(false_nego_side + 1);
    false_nego_stack[24] = (s32) false_nego_selection;
    false_nego_offset = false_nego_side * false_nego_stride;
    false_nego_offset += (s32) false_nego_base;
    {
        register u32 false_nego_adjust asm("r1") = BATTLE_DECK_PREPARATION_OFFSET(payloads);
        false_nego_selection = (u8 *)false_nego_offset + false_nego_adjust;
    }
loop_129:
    if ((IsBattleUnitActive(false_nego_side, false_nego_slot) << 0x18) == 0) {
        goto block_133;
    }
    {
        register s32 false_nego_player asm("r2") = command_side;
        if (false_nego_side != false_nego_player) {
            goto block_132;
        }
    }
    {
        register u32 false_nego_selected_player asm("r3");

        asm volatile(
            "ldrb %0, [%1, #0]"
            : "=r"(false_nego_selected_player)
            : "r"(false_nego_selection)
            : "memory");
        if (false_nego_slot == false_nego_selected_player) {
            goto block_133;
        }
    }
block_132:
    RemoveBattleUnitFromTurnOrder(false_nego_side, false_nego_slot);
    ADD_BATTLE_EFFECT_WITH_STACK_ARGUMENTS(false_nego_side, false_nego_slot, -1, 0,
                      false_nego_zero, false_nego_zero, BATTLE_EFFECT_TURN_MARKER,
                      false_nego_zero, 1, false_nego_zero);
block_133:
    false_nego_slot += 1;
    if ((u32) false_nego_slot <= 5U) {
        goto loop_129;
    }
    false_nego_selection = (u8 *) false_nego_stack[24];
    false_nego_outer_next = (u8)(s32) false_nego_selection;
    false_nego_side = false_nego_outer_next;
    if ((u32) false_nego_outer_next <= 1U) {
        goto loop_128;
    }
    return;
}
case DECK_COMMAND_COVERING_FIRE - DECK_COMMAND_FRIENDSHIP: {
    register s32 covering_fire_base asm("r7") = BATTLE_STATE_RAM;
    register s32 covering_fire_work asm("r0") = BATTLE_COMBINATION_OFFSET(active_unit_slot);
    register u8 *covering_fire_state asm("r1");
    register s32 covering_fire_zero asm("r4");
    asm volatile("" : "+r"(covering_fire_base), "+r"(covering_fire_work));
    covering_fire_state = (u8 *)(covering_fire_base + covering_fire_work);
    covering_fire_work = 6;
    *covering_fire_state = (u8)covering_fire_work;
    {
        register s32 covering_fire_state_seed asm("r1") = 0;
        asm volatile("" : "+r"(covering_fire_state_seed));
        covering_fire_slot = covering_fire_state_seed;
    }
    covering_fire_zero = 0;
loop_137:
    if ((IsBattleUnitActive(command_side, covering_fire_slot) << 0x18) == 0) {
        goto block_139;
    }
    RemoveBattleUnitFromTurnOrderWide(command_side, covering_fire_slot);
    ADD_BATTLE_EFFECT_WITH_STACK_ARGUMENTS(command_side, covering_fire_slot, -1, 0,
                      covering_fire_zero, covering_fire_zero, BATTLE_EFFECT_TURN_MARKER,
                      covering_fire_zero, 1, covering_fire_zero);
block_139:
    temp_r0_27 = covering_fire_slot + 1;
    covering_fire_slot = temp_r0_27;
    if ((u32) temp_r0_27 <= 5U) {
        goto loop_137;
    }
    final_state = (s32 *)0x02030558;
    asm volatile("" : "+r"(final_state));
    next_battle_phase = 0x1300;
    goto store_battle_phase;
}
case DECK_COMMAND_DEFEND_OR_DIE - DECK_COMMAND_FRIENDSHIP: {
    register s32 defend_or_die_base asm("r2") = BATTLE_STATE_RAM;
    register s32 defend_or_die_offset asm("r3") = BATTLE_COMBINATION_OFFSET(active_unit_slot);
    register u8 *defend_or_die_state asm("r1");
    register s32 defend_or_die_value asm("r0");
    asm volatile("" : "+r"(defend_or_die_base), "+r"(defend_or_die_offset));
    defend_or_die_state = (u8 *)(defend_or_die_base + defend_or_die_offset);
    defend_or_die_value = 6;
    *defend_or_die_state = (u8)defend_or_die_value;
    final_state = (s32 *)0x02030558;
    asm volatile("" : "+r"(final_state));
    next_battle_phase = 0x1300;
    goto store_battle_phase;
}
case DECK_COMMAND_LINK_SUPPORT - DECK_COMMAND_FRIENDSHIP: {
    register s32 link_support_zero asm("r5");
    register u8 *link_support_state asm("r1");
    register s32 link_support_work asm("r0");
    register s32 link_support_side asm("r7");
    link_support_payload = (void *)BATTLE_STATE_RAM;
    link_support_zero = BATTLE_COMBINATION_OFFSET(active_unit_slot);
    asm volatile("" : "+r"(link_support_payload), "+r"(link_support_zero));
    link_support_state = link_support_payload + link_support_zero;
    link_support_zero = 0;
    *link_support_state = 6U;
    link_support_work = 0x94;
    link_support_side = command_side;
    asm volatile("" : "+r"(link_support_work), "+r"(link_support_side));
    link_support_payload = (void *)link_support_side;
    link_support_payload = (void *)((u32)link_support_payload * link_support_work);
    link_support_work = BATTLE_STATE_RAM;
    asm volatile("" : "+r"(link_support_work));
    link_support_payload += link_support_work;
    link_support_state = (u8 *)BATTLE_DECK_PREPARATION_OFFSET(payloads);
    asm volatile("" : "+r"(link_support_state));
    link_support_payload += (s32)link_support_state;
    RemoveBattleUnitFromTurnOrder(link_support_side, M2C_FIELD(link_support_payload, u8 *, 0));
    AddBattleEffect(command_side, M2C_FIELD(link_support_payload, u8 *, 0), -1, 0,
                  link_support_zero, link_support_zero, BATTLE_EFFECT_TURN_MARKER,
                  link_support_zero, 1, link_support_zero);
    final_state = (s32 *)0x02030558;
    asm volatile("" : "+r"(final_state));
    next_battle_phase = 0x1300;
    goto store_battle_phase;
}
case DECK_COMMAND_STRATEGY_MEET - DECK_COMMAND_FRIENDSHIP: {
    register s32 strategy_meet_zero asm("r4");
    {
        register s32 strategy_meet_init asm("r2") = 0;
        asm volatile("" : "+r"(strategy_meet_init));
        strategy_meet_slot = strategy_meet_init;
    }
    strategy_meet_zero = 0;
loop_144:
    if ((IsBattleUnitActive(command_side, strategy_meet_slot) << 0x18) == 0) {
        goto block_146;
    }
    RemoveBattleUnitFromTurnOrderWide(command_side, strategy_meet_slot);
    {
        register volatile s32 *strategy_meet_outgoing asm("sp");
        strategy_meet_outgoing[0] = strategy_meet_zero;
        strategy_meet_outgoing[1] = strategy_meet_zero;
        strategy_meet_outgoing[2] = BATTLE_EFFECT_TURN_MARKER;
        strategy_meet_outgoing[3] = strategy_meet_zero;
        strategy_meet_outgoing[4] = 1;
        strategy_meet_outgoing[5] = strategy_meet_zero;
        {
            register s32 strategy_meet_arg0 asm("r0") = command_side;
            register s32 strategy_meet_arg1 asm("r1") = strategy_meet_slot;
            register s32 strategy_meet_arg2 asm("r2") = -1;
            register s32 strategy_meet_arg3 asm("r3") = 0;
            AddBattleEffect(strategy_meet_arg0, strategy_meet_arg1,
                          strategy_meet_arg2, strategy_meet_arg3);
        }
    }
block_146:
    temp_r0_28 = strategy_meet_slot + 1;
    strategy_meet_slot = temp_r0_28;
    if ((u32) temp_r0_28 <= 5U) {
        goto loop_144;
    }
    return;
}
case DECK_COMMAND_DISTURBED_DATA - DECK_COMMAND_FRIENDSHIP: {
    u32 disturbed_data_count;
    register u32 disturbed_data_scan asm("r8");
    register u8 *disturbed_data_list asm("r4");
    register u8 *disturbed_data_flags asm("r2");
    register u8 *disturbed_data_store_base asm("r1");
    register u32 disturbed_data_r6_reserve asm("r6");

    {
        register s32 disturbed_data_player asm("r3") = command_side;
        if (disturbed_data_player == 0) {
            goto command_done;
        }
    }
    asm volatile("" : "=&r"(disturbed_data_r6_reserve));
    disturbed_data_count = 0;
    {
        register u32 disturbed_data_zero asm("r4") = 0;
        asm volatile("mov %0, %1"
                     : "=r"(disturbed_data_scan)
                     : "l"(disturbed_data_zero));
    }
    disturbed_data_list = (u8 *)0x02033EBA;
    disturbed_data_flags = (u8 *)0x02037300;
    asm volatile("" : "+r"(disturbed_data_list), "+r"(disturbed_data_flags));
    disturbed_data_store_base = disturbed_data_list;
loop_150:
    {
        register u32 disturbed_data_index asm("r5");
        asm volatile("mov %0, r8" : "=l"(disturbed_data_index));
        if (*(u8 *)(
                disturbed_data_index - (0U - (u32)disturbed_data_flags)) != 0) {
            register u32 disturbed_data_count_next asm("r0");
            *(u8 *)(
                disturbed_data_count - (0U - (u32)disturbed_data_store_base)) =
                (u8)disturbed_data_index;
            disturbed_data_count_next = disturbed_data_count + 1;
            disturbed_data_count = (u8)disturbed_data_count_next;
        }
    }
block_152:
    {
        register u32 disturbed_data_scan_next asm("r0");
        disturbed_data_scan_next = disturbed_data_scan + 1;
        disturbed_data_scan = (u8)disturbed_data_scan_next;
    }
    if (disturbed_data_scan <= 9U) {
        goto loop_150;
    }
    {
        register u32 disturbed_data_player_offset asm("r0") = 0x94;
        {
            register u32 disturbed_data_player asm("r1") = command_side;
            disturbed_data_player_offset *= disturbed_data_player;
        }
        {
            register u32 disturbed_data_record_base asm("r2") = BATTLE_STATE_RAM;
            asm volatile("" : "+r"(disturbed_data_record_base));
            disturbed_data_player_offset += disturbed_data_record_base;
        }
        {
            register u32 disturbed_data_state_offset asm("r3") = BATTLE_DECK_PREPARATION_OFFSET(payloads);
            asm volatile("" : "+r"(disturbed_data_state_offset));
            disturbed_data_player_offset += disturbed_data_state_offset;
        }
        disturbed_data_player_offset =
            ModuloUnsigned32(*(u8 *)disturbed_data_player_offset, disturbed_data_count);
        disturbed_data_player_offset = (u8)disturbed_data_player_offset;
        disturbed_data_player_offset += (u32)disturbed_data_list;
        disturbed_data_list = (u8 *)BATTLE_STATE_RAM;
        {
            register u32 disturbed_data_dest_offset asm("r5") = 0x27B4;
            register u8 *disturbed_data_dest asm("r1");
            asm volatile("" : "+r"(disturbed_data_list),
                         "+r"(disturbed_data_dest_offset));
            disturbed_data_dest =
                (u8 *)((u32)disturbed_data_list + disturbed_data_dest_offset);
            disturbed_data_player_offset = *(u8 *)disturbed_data_player_offset;
            disturbed_data_dest += disturbed_data_player_offset;
            *disturbed_data_dest = 0;
        }
    }
    asm volatile("" : : "r"(disturbed_data_r6_reserve));
    return;
}
case DECK_COMMAND_SWITCH - DECK_COMMAND_FRIENDSHIP: {
    register u8 *switch_second_flag asm("r6");
    register u8 *switch_second_addr asm("r0");
    register u8 *switch_wait_flag asm("r5");
    register void *switch_copy_source asm("r6");
    {
        register u32 switch_state_work asm("r0") = 0x94;
        register s32 switch_player asm("r7") = command_side;
        register u32 switch_record_base asm("r1");
        register u32 switch_state_offset asm("r2");
        switch_state_work *= switch_player;
        switch_record_base = BATTLE_STATE_RAM;
        asm volatile("" : "+r"(switch_record_base));
        switch_state_work += switch_record_base;
        switch_state_offset = BATTLE_DECK_PREPARATION_OFFSET(payloads);
        asm volatile("" : "+r"(switch_state_offset));
        switch_state_work += switch_state_offset;
        temp_r7 = *(u8 *)switch_state_work;
    }
    {
        register u32 switch_wait_base asm("r1");
        register s32 switch_player asm("r3");
        register s32 switch_player_twice asm("r2");
        switch_wait_base = 0x02032EEC;
        asm volatile("" : "+r"(switch_wait_base));
        switch_player = command_side;
        switch_player_twice = switch_player << 1;
        temp_r0_30 = (void *)(switch_player_twice + switch_player);
        temp_r0_30 = (void *)((u32)temp_r0_30 << 1);
        temp_r0_30 = (void *)(temp_r7 + (u32)temp_r0_30);
        temp_r0_30 = (void *)((u32)temp_r0_30 + switch_wait_base);
        switch_wait_base = 2;
        *(u8 *)temp_r0_30 = (u8)switch_wait_base;
        var_sl = switch_player_twice;
        switch_wait_flag = temp_r0_30;
    }
loop_156:
    YieldTaskForUpdates(1);
    if (*switch_wait_flag != 0) {
        goto loop_156;
    }
    {
        register u32 switch_record_base asm("r2") = BATTLE_STATE_RAM;
        register u32 switch_record_offset asm("r1");
        register u32 switch_row asm("r0");
        switch_record_offset = temp_r7 * 0x270;
        switch_unit_record = (void *)command_side_times_four;
        temp_r5 = (void *)command_side;
        switch_row =
            ((((u32)switch_unit_record + (u32)temp_r5) * 8)
             - (u32)temp_r5) << 7;
        switch_record_offset += switch_row;
        switch_unit_record =
            (void *)(switch_record_offset + switch_record_base);
        {
            register u32 switch_player_offset asm("r0") = 0x94;
            switch_player_offset *= (u32)temp_r5;
            temp_r5 = (void *)(
                switch_player_offset + switch_record_base);
        }
    }
    {
        register u32 switch_copy_offset asm("r0") = 0xA0A8;
        asm volatile("" : "+r"(switch_copy_offset));
        switch_copy_source =
            (void *)((u32)temp_r5 + switch_copy_offset);
        CopyBytes(switch_unit_record, switch_copy_source, 0x70);
    }
    ClearBattleUnitEffects(command_side, temp_r7);
    ApplyBattlePassiveEquipmentEffects(command_side, temp_r7);
    RecalculateBattleUnitStats(command_side, temp_r7);
    {
        register s32 switch_half asm("r0");
        asm volatile(
            "mov r1, #62\n\t"
            "ldsh r0, [r4, r1]\n\t"
            "lsr r1, r0, #31\n\t"
            "add r0, r0, r1\n\t"
            "asr r0, r0, #1"
            : "=r"(switch_half)
            : "r"(switch_unit_record)
            : "r1", "cc", "memory");
        BATTLE_UNIT_FIELD(switch_unit_record, s16, ep) = (s16)switch_half;
    }
    if (*(u8 *)0x0203055C != 1) {
        goto block_159;
    }
    BATTLE_UNIT_FIELD(switch_unit_record, u16, hp) = (u16) BATTLE_UNIT_FIELD(switch_unit_record, u16, max_hp);
block_159:
    BATTLE_UNIT_FIELD(switch_unit_record, u16, flags) = (u16) (BATTLE_UNIT_FIELD(switch_unit_record, u16, flags) | 4);
    {
        register volatile s32 *switch_call_stack asm("sp");
        register s32 switch_first_arg asm("r3");
        register s32 switch_second_arg asm("r1");
        register s32 switch_third_arg asm("r2");
        register s32 switch_direction_player asm("r5");
        register s32 switch_direction asm("r0");
        switch_first_arg = *(u8 *)switch_copy_source;
        {
            register u32 switch_second_offset asm("r2") = 0xA0A9;
            register u8 *switch_second_value asm("r0");
            asm volatile("" : "+r"(switch_second_offset));
            switch_second_value =
                (u8 *)((u32)temp_r5 + switch_second_offset);
            switch_second_arg = *switch_second_value;
        }
        {
            register u32 switch_third_offset asm("r4") = 0xA0E0;
            register u8 *switch_third_value asm("r0");
            asm volatile("" : "+r"(switch_third_offset));
            switch_third_value =
                (u8 *)((u32)temp_r5 + switch_third_offset);
            switch_third_arg = *switch_third_value;
        }
        switch_call_stack[0] = temp_r7;
        switch_direction_player = command_side;
        switch_direction =
            switch_direction_player == 0 ? 0x10000 : 0xFFFF0000;
        switch_call_stack[1] = switch_direction;
        switch_direction = 0;
        switch_call_stack[2] = switch_direction;
        {
            register s32 switch_first_call_arg asm("r0") =
                switch_first_arg;
            register s32 switch_player_call_arg asm("r3") = command_side;
            ConfigureBattleUnitSpritesWithStackArguments(
                switch_first_call_arg, switch_second_arg,
                switch_third_arg, switch_player_call_arg);
        }
    }
    {
        register u32 switch_second_base asm("r1") = 0x02032EEC;
        asm volatile("" : "+r"(switch_second_base));
        switch_second_addr = (u8 *)command_side;
        switch_second_addr =
            (u8 *)((u32)switch_second_addr + var_sl);
        switch_second_addr =
            (u8 *)((u32)switch_second_addr << 1);
        switch_second_addr =
            (u8 *)(temp_r7 + (u32)switch_second_addr);
        switch_second_addr =
            (u8 *)((u32)switch_second_addr + switch_second_base);
        switch_second_base = 1;
        *switch_second_addr = (u8)switch_second_base;
    }
    switch_second_flag = switch_second_addr;
loop_164:
    YieldTaskForUpdates(1);
    temp_r5_2 = *switch_second_flag;
    if (temp_r5_2 != 0) {
        goto loop_164;
    }
    RemoveBattleUnitFromTurnOrder(command_side, temp_r7);
    AddBattleEffect(command_side, temp_r7, -1, 0, (s32) temp_r5_2, (s32) temp_r5_2, BATTLE_EFFECT_TURN_MARKER, (s32) temp_r5_2, 1, (s32) temp_r5_2);
    return;
}
case DECK_COMMAND_REDISTRIBUTION - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *redistribution_stack asm("sp");
    register u32 redistribution_r7 asm("r7");
    register u8 *redistribution_wait_flag0 asm("r7");
    register s32 temp_r6 asm("r6");
    register s32 var_r6 asm("r6");
    s32 *redistribution_second_dest;
    s32 redistribution_player;
    register s32 redistribution_scan_offset asm("r3");
    register u32 redistribution_zero asm("r4");
    register u8 *redistribution_first_count asm("r1");
    StartBattleCameraTransition(2, command_side, 0, 0);
    {
        register s32 redistribution_side_twice asm("r7") = command_side;
        redistribution_side_twice <<= 1;
        var_sl = redistribution_side_twice;
    }
    goto loop_168;
block_167:
    YieldTaskForUpdates(1);
loop_168:
    if ((IsBattleCameraTransitionComplete() << 0x18) == 0) {
        goto block_167;
    }
    {
        register s32 redistribution_state_seed asm("r0") = 0;
        asm volatile("" : "+r"(redistribution_state_seed));
        redistribution_saved_slot = redistribution_state_seed;
    }
    {
        register u8 *redistribution_base asm("r1") = (u8 *)BATTLE_STATE_RAM;
        register s32 redistribution_players_offset asm("r2") = 0x2713;
        asm volatile("" : "+r"(redistribution_base),
                     "+r"(redistribution_players_offset));
        redistribution_players = redistribution_base + redistribution_players_offset;
    }
    {
        register u8 *redistribution_base asm("r3") = (u8 *)BATTLE_STATE_RAM;
        register s32 redistribution_slots_offset asm("r4") = 0x2714;
        asm volatile("" : "+r"(redistribution_base),
                     "+r"(redistribution_slots_offset));
        redistribution_slots = redistribution_base + redistribution_slots_offset;
    }
    redistribution_lists = (u8 *)0x02032FCA;
loop_170:
    {
        register u8 *redistribution_copy_dest_r0 asm("r0") =
            (u8 *)0x02032FE4;
        register s32 redistribution_copy_row asm("r1");
        register s32 redistribution_source_row asm("r2");
        register s32 redistribution_source_base asm("r3");
        asm volatile("" : "+r"(redistribution_copy_dest_r0));
        redistribution_r7 = redistribution_saved_slot;
        temp_r4_5 = redistribution_r7 << 2;
        redistribution_copy_row = temp_r4_5 + redistribution_r7;
        redistribution_copy_row <<= 3;
        redistribution_copy_row -= redistribution_r7;
        redistribution_copy_row <<= 4;
        redistribution_copy_dest_r0 =
            (u8 *)(redistribution_copy_row + (u32)redistribution_copy_dest_r0);
        redistribution_source_base = command_side_times_four;
        redistribution_player = command_side;
        redistribution_source_row = redistribution_source_base + redistribution_player;
        redistribution_source_row <<= 3;
        redistribution_source_row -= redistribution_player;
        redistribution_source_row <<= 7;
        redistribution_copy_row += redistribution_source_row;
        redistribution_source_row = BATTLE_STATE_RAM;
        redistribution_copy_row += redistribution_source_row;
        CopyBytes(
            redistribution_copy_dest_r0, (void *)redistribution_copy_row, 0x270);
    }
    redistribution_first_dest = (s32 *)0x02033E84;
    asm volatile("" : "+r"(redistribution_first_dest));
    redistribution_first_dest =
        (s32 *)((u32)temp_r4_5 + (u32)redistribution_first_dest);
    {
        register s32 redistribution_first_src_base asm("r0") = 0x02032E8C;
        temp_r1_6 = redistribution_player;
        asm volatile("" : "+r"(temp_r1_6));
        temp_r1_6 += var_sl;
        temp_r1_6 <<= 3;
        asm volatile("add %0, %1, %0"
                     : "+r"(temp_r1_6)
                     : "r"(temp_r4_5));
        *redistribution_first_dest =
            *(s32 *)(temp_r1_6 + redistribution_first_src_base);
    }
    {
        register s32 redistribution_record_b_dest_base asm("r0") = 0x02033E9C;
        temp_r4_5 += redistribution_record_b_dest_base;
    }
    {
        register s32 redistribution_record_b_src_base asm("r0") = 0x02032EBC;
        asm volatile("" : "+r"(redistribution_record_b_src_base));
        temp_r1_6 += redistribution_record_b_src_base;
    }
    *(s32 *)temp_r4_5 = *(s32 *)temp_r1_6;
    redistribution_first_count = (u8 *)0x02032FDC + redistribution_saved_slot;
    *redistribution_first_count = 0U;
    redistribution_turn_entry_slot = 0;
    redistribution_scan_offset = redistribution_saved_slot;
    {
        register s32 redistribution_scan_twice asm("r0") =
            redistribution_scan_offset << 1;
        asm volatile("add %0, %1, %0"
                     : "+r"(redistribution_scan_offset)
                     : "r"(redistribution_scan_twice));
    }
loop_171:
    temp_r2_6 = redistribution_turn_entry_slot * 2;
    {
        register u32 redistribution_player_value asm("r0");
        register s32 redistribution_side_view asm("r4");
        asm volatile(
            "mov %1, %3\n\t"
            "add %0, %2, %1\n\t"
            "ldrb %0, [%0]\n\t"
            "ldr %1, [sp, #24]"
            : "=&r"(redistribution_player_value), "=&r"(redistribution_side_view)
            : "r"(temp_r2_6), "r"(redistribution_players)
            : "memory");
        if (redistribution_player_value != (u32)redistribution_side_view) {
            goto block_174;
        }
    }
    {
        register u32 redistribution_slot_value asm("r0");
        asm volatile(
            "add %0, %1, %2\n\t"
            "ldrb %0, [%0]"
            : "=&r"(redistribution_slot_value)
            : "r"(temp_r2_6), "r"(redistribution_slots)
            : "memory");
        if (redistribution_slot_value != redistribution_saved_slot) {
            goto block_174;
        }
    }
    *(u8 *)(
        (*redistribution_first_count + redistribution_scan_offset)
        - (0U - (u32)redistribution_lists)) = redistribution_turn_entry_slot;
    *redistribution_first_count = (u8)(*redistribution_first_count + 1);
block_174:
    redistribution_turn_entry_slot += 1;
    if ((u32) redistribution_turn_entry_slot <= 0x23U) {
        goto loop_171;
    }
    redistribution_player = command_side;
    if (redistribution_player != 0) {
        goto block_177;
    }
    {
        register u8 *redistribution_state_dest asm("r0") =
            (u8 *)0x02033EB4 + redistribution_saved_slot;
        register u8 *redistribution_state_source asm("r1");
        register u32 redistribution_state_base asm("r2") = BATTLE_STATE_RAM;
        register u32 redistribution_state_offset asm("r3") = 0x9C;
        asm volatile("" : "+r"(redistribution_state_dest),
                     "+r"(redistribution_state_base),
                     "+r"(redistribution_state_offset));
        redistribution_state_offset <<= 6;
        redistribution_state_source =
            (u8 *)(redistribution_state_base + redistribution_state_offset + redistribution_saved_slot);
        *redistribution_state_dest = *redistribution_state_source;
    }
block_177:
    asm volatile("" : "+r"(redistribution_saved_slot));
    temp_r0_33 = redistribution_saved_slot + 1;
    redistribution_saved_slot = temp_r0_33;
    if ((u32) temp_r0_33 <= 5U) {
        goto loop_170;
    }
    asm volatile("movs %0, #0" : "=r"(redistribution_zero));
    redistribution_destination_slot = redistribution_zero;
    {
        register s32 redistribution_row_base asm("r5") = command_side_times_four;
        register s32 redistribution_row_side asm("r7") = command_side;
        register s32 redistribution_row asm("r0");
        register s32 redistribution_side_row asm("r1");
        redistribution_row = redistribution_row_base + redistribution_row_side;
        redistribution_row <<= 3;
        redistribution_row -= redistribution_row_side;
        redistribution_row <<= 7;
        redistribution_stack[8] = redistribution_row;
        redistribution_row = redistribution_row_side;
        asm volatile("" : "+r"(redistribution_row));
        redistribution_row += var_sl;
        redistribution_side_row = redistribution_row << 3;
        redistribution_stack[9] = redistribution_side_row;
        redistribution_row <<= 1;
        redistribution_stack[10] = redistribution_row;
    }
loop_179:
    {
        register u32 redistribution_base asm("r3");
        register u32 redistribution_unit_addr asm("r0") = 0x94;
        register u32 redistribution_unit_side asm("r2") = command_side;
        register u32 redistribution_field asm("r4");
        register u32 redistribution_unit_base asm("r1");
        register u32 redistribution_index asm("r7");
        register u32 redistribution_copy_dest_r0 asm("r0");
        register u32 redistribution_copy_source_r1 asm("r1");
        register u32 redistribution_copy_base_r2 asm("r2");
        redistribution_unit_addr *= redistribution_unit_side;
        redistribution_unit_addr += redistribution_destination_slot;
        redistribution_base = BATTLE_STATE_RAM;
        redistribution_field = BATTLE_DECK_PREPARATION_OFFSET(payloads);
        asm volatile("" : "+r"(redistribution_base), "+r"(redistribution_field));
        redistribution_unit_base = redistribution_base + redistribution_field;
        redistribution_unit_addr += redistribution_unit_base;
        redistribution_unit_addr = *(u8 *)redistribution_unit_addr;
        temp_r0_34 = redistribution_unit_addr;
        redistribution_index = redistribution_destination_slot;
        redistribution_second_index4 = redistribution_index << 2;
        redistribution_copy_dest_r0 = redistribution_second_index4 + redistribution_index;
        redistribution_copy_dest_r0 <<= 3;
        redistribution_copy_dest_r0 -= redistribution_index;
        redistribution_copy_dest_r0 <<= 4;
        redistribution_copy_dest_r0 += redistribution_stack[8];
        redistribution_copy_dest_r0 += redistribution_base;
        redistribution_copy_base_r2 = 0x02032FE4;
        asm volatile("" : "+r"(redistribution_copy_base_r2));
        redistribution_base = temp_r0_34;
        temp_r4_6 = redistribution_base << 2;
        redistribution_copy_source_r1 = temp_r4_6 + redistribution_base;
        redistribution_copy_source_r1 <<= 3;
        redistribution_copy_source_r1 -= redistribution_base;
        redistribution_copy_source_r1 <<= 4;
        redistribution_copy_source_r1 += redistribution_copy_base_r2;
        CopyBytes((void *)redistribution_copy_dest_r0,
                      (void *)redistribution_copy_source_r1, 0x270);
    }
    {
        register s32 redistribution_second_dest_base asm("r2") = 0x02032E8C;
        register s32 redistribution_second_row asm("r7") = redistribution_stack[9];
        asm volatile("" : "+r"(redistribution_second_dest_base));
        temp_r1_7 = redistribution_second_index4 + redistribution_second_row;
        redistribution_second_dest =
            (s32 *)(temp_r1_7 + redistribution_second_dest_base);
        {
            register u32 redistribution_record_a asm("r0") = 0x02033E84;
            asm volatile("" : "+r"(redistribution_record_a));
            redistribution_record_a = temp_r4_6 + redistribution_record_a;
            *redistribution_second_dest = *(s32 *)redistribution_record_a;
        }
    }
    {
        register u32 redistribution_record_b_dest asm("r0") = 0x02032EBC;
        temp_r1_7 += redistribution_record_b_dest;
    }
    {
        register u32 redistribution_record_b_src asm("r0") = 0x02033E9C;
        temp_r4_6 += redistribution_record_b_src;
    }
    *(s32 *)temp_r1_7 = *(s32 *)temp_r4_6;
    redistribution_r7 = (u32) *(s32 **)redistribution_second_dest;
    temp_r6 = *(s32 *)redistribution_r7 & ~0xC0;
    if (command_side != 0) {
        goto block_181;
    }
    {
        register s32 redistribution_mask_delta asm("r4") = 3;
        redistribution_mask_delta -= ModuloUnsigned32FullArguments(redistribution_destination_slot, 3U);
        redistribution_mask_delta <<= 6;
        var_r6 = temp_r6 | redistribution_mask_delta;
    }
    goto block_182;
block_181:
    var_r6 = temp_r6 | (((u32) (ModuloUnsigned32FullArguments(redistribution_destination_slot, 3U) << 0x18) >> 0x12) + 0x40);
block_182:
    *(s32 *)redistribution_r7 = var_r6;
    redistribution_r7 = 0;
    {
        register u8 *redistribution_count_ptr asm("r0") =
            (u8 *)0x02032FDC + temp_r0_34;
        redistribution_stack[24] = redistribution_destination_slot + 1;
        if (redistribution_r7 >= (u32) *redistribution_count_ptr) {
            goto block_185;
        }
    }
    var_r6 = 0x02032FCA;
    {
        register u32 redistribution_copy_unit_r2 asm("r2") = temp_r0_34;
        register u32 redistribution_copy_twice_r0 asm("r0");
        redistribution_copy_twice_r0 = redistribution_copy_unit_r2 << 1;
        asm volatile("" : "+r"(redistribution_copy_twice_r0));
        temp_r4_6 = redistribution_copy_twice_r0 + redistribution_copy_unit_r2;
    }
    {
        register u32 redistribution_copy_record_base_r1 asm("r1") = BATTLE_STATE_RAM;
        register u32 redistribution_copy_outer_r3 asm("r3") = redistribution_destination_slot;
        register u32 redistribution_copy_record_r0 asm("r0");
        asm volatile("" : "+r"(redistribution_copy_record_base_r1),
                     "+r"(redistribution_copy_outer_r3));
        redistribution_copy_record_r0 =
            redistribution_second_index4 + redistribution_copy_outer_r3;
        redistribution_copy_record_r0 <<= 3;
        redistribution_copy_record_r0 -= redistribution_copy_outer_r3;
        redistribution_copy_record_r0 <<= 4;
        redistribution_second_index4 = redistribution_stack[8];
        redistribution_copy_record_r0 += redistribution_second_index4;
        redistribution_second_index4 =
            redistribution_copy_record_r0 + redistribution_copy_record_base_r1;
    }
    asm volatile("" : : "g"(sp20));
loop_184:
    {
        register s32 redistribution_record_value asm("r0") =
            M2C_FIELD((redistribution_r7 + temp_r4_6), u8 *, var_r6);
        register s32 redistribution_record_arg asm("r3");
        register s32 redistribution_record_side asm("r1");
        register s32 redistribution_record_outer asm("r2");
        asm volatile(
            "mov r1, #10\n\t"
            "ldsh r3, [r5, r1]"
            : "=r"(redistribution_record_arg)
            : "r"(redistribution_second_index4)
            : "r1", "cc", "memory");
        redistribution_record_side = command_side;
        redistribution_record_outer = redistribution_destination_slot;
        SetBattleTurnOrderEntryWide(
            redistribution_record_value, redistribution_record_side,
            redistribution_record_outer, redistribution_record_arg);
    }
    redistribution_r7 = (u8)(redistribution_r7 + 1);
    if (redistribution_r7 < (u32) M2C_FIELD(temp_r0_34, u8 *, 0x02032FDC)) {
        goto loop_184;
    }
block_185:
    {
        register s32 redistribution_guard_r0 asm("r0");
        register s32 redistribution_guard_r1 asm("r1");
        s32 redistribution_side_guard;
        asm volatile("" :
                     "=r"(redistribution_guard_r0), "=r"(redistribution_guard_r1));
        redistribution_side_guard = command_side;
        asm volatile("" :
                     : "r"(redistribution_guard_r0), "r"(redistribution_guard_r1),
                       "r"(redistribution_side_guard));
        if (redistribution_side_guard != 0) {
            goto block_187;
        }
    }
    M2C_FIELD(redistribution_destination_slot, u8 *, 0x0203724C) = (u8) M2C_FIELD(temp_r0_34, u8 *, 0x02033EB4);
block_187:
    if ((IsBattleUnitActive(command_side, redistribution_destination_slot) << 0x18) == 0) {
        goto block_189;
    }
    {
        register u32 redistribution_flag_base asm("r0") = 0x02032EEC;
        register u32 redistribution_flag_addr asm("r1") = redistribution_stack[10];
        asm volatile("" : "+r"(redistribution_flag_base));
        redistribution_flag_addr += redistribution_destination_slot;
        redistribution_flag_addr += redistribution_flag_base;
        redistribution_flag_base = 1;
        *(u8 *)redistribution_flag_addr = (u8)redistribution_flag_base;
    }
block_189:
    {
        register u32 redistribution_exit_reload asm("r3") = redistribution_stack[24];
        register u32 redistribution_exit_normalized asm("r0");

        asm volatile(
            "lsl %0, %1, #24\n\t"
            "lsr %0, %0, #24"
            : "=r"(redistribution_exit_normalized)
            : "r"(redistribution_exit_reload));
        redistribution_destination_slot = redistribution_exit_normalized;
        if (redistribution_exit_normalized > 5U) {
            goto block_191;
        }
    }
    goto loop_179;
block_191:
    redistribution_wait_records = (u8 *)0x02032E8C;
    {
        register s32 redistribution_wait_index asm("r0") = command_side + var_sl;
        redistribution_wait_flags = (u8 *)0x02032EEC;
        {
            register s32 redistribution_wait_flag_offset asm("r1") =
                redistribution_wait_index * 2;
            redistribution_wait_flag0 = (u8 *)(
                (u32)redistribution_wait_flag_offset
                - (0U - (u32)redistribution_wait_flags));
        }
        redistribution_wait_record0 = (s32 **)(
            (u32)(redistribution_wait_index * 8)
            - (0U - (u32)redistribution_wait_records));
    }
loop_192:
    YieldTaskForUpdates(1);
    {
        register u32 redistribution_wait_zero asm("r0") = 0;
        asm volatile("" : "+r"(redistribution_wait_zero));
        redistribution_wait_slot = redistribution_wait_zero;
    }
    if (*redistribution_wait_record0 == 0) {
        goto loop_194;
    }
    {
        register u32 redistribution_wait_flag_value asm("r0");
        asm volatile("ldrb %0, [%1, #0]"
                     : "=r"(redistribution_wait_flag_value)
                     : "r"(redistribution_wait_flag0)
                     : "memory");
        if (redistribution_wait_flag_value != 0) {
            goto block_197;
        }
    }
loop_194:
    {
        register u32 redistribution_wait_next asm("r0") = redistribution_wait_slot + 1;
        register s32 redistribution_wait_row asm("r2");
        redistribution_wait_next <<= 24;
        redistribution_wait_next >>= 24;
        redistribution_wait_slot = redistribution_wait_next;
        if (redistribution_wait_next > 5U) {
            goto command_done;
        }
        redistribution_wait_next *= 4;
        {
            register s32 redistribution_wait_row8 asm("r1");
            redistribution_wait_row = command_side;
            redistribution_wait_row += var_sl;
            redistribution_wait_row8 = redistribution_wait_row * 8;
            redistribution_wait_next += redistribution_wait_row8;
        }
        redistribution_wait_next += (u32)redistribution_wait_records;
        if (*(s32 *)redistribution_wait_next == 0) {
            goto loop_194;
        }
        {
            register u32 redistribution_wait_flag_address asm("r0") =
                redistribution_wait_row * 2;
            redistribution_wait_flag_address += redistribution_wait_slot;
            redistribution_wait_flag_address += (u32)redistribution_wait_flags;
            if (*(u8 *)redistribution_wait_flag_address == 0) {
                goto loop_194;
            }
        }
    }
block_197:
    {
        register u32 redistribution_wait_check asm("r1") = redistribution_wait_slot;

        asm volatile("" : "+r"(redistribution_wait_check));
        if (redistribution_wait_check <= 5U) {
            goto loop_192;
        }
    }
    return;
}
case DECK_COMMAND_PARTS_REMOVAL - DECK_COMMAND_FRIENDSHIP: {
    register u32 parts_removal_state asm("r3");
    register u32 parts_removal_player asm("r4");
    register u32 parts_removal_row asm("r2");

    parts_removal_state = command_side_times_four;
    parts_removal_player = command_side;
    parts_removal_row = ((parts_removal_state + parts_removal_player) * 8 - parts_removal_player) << 7;
    {
        register u32 parts_removal_unit_addr asm("r0") = parts_removal_player * 0x94;
        register u32 parts_removal_base asm("r5") = BATTLE_STATE_RAM;
        register u32 parts_removal_unit asm("r1");

        parts_removal_unit_addr += parts_removal_base;
        {
            register u32 parts_removal_field asm("r7") = BATTLE_DECK_PREPARATION_OFFSET(payloads);
            parts_removal_unit_addr += parts_removal_field;
        }
        parts_removal_unit = *(u8 *)parts_removal_unit_addr;
        parts_removal_row += (parts_removal_unit * 0x270) + parts_removal_base;
    }
    {
        register u32 parts_removal_outer_zero asm("r0") = 0;
        asm volatile("" : "+r"(parts_removal_outer_zero));
        parts_removal_equipment_slot = parts_removal_outer_zero;
    }
    {
        register u32 parts_removal_zero asm("r1") = 0;
loop_201:
        {
            register u32 parts_removal_index asm("r3") = parts_removal_equipment_slot;
            register u32 parts_removal_addr asm("r0") = parts_removal_index * 4;
            asm volatile("" : "+r"(parts_removal_index));
            parts_removal_addr = parts_removal_row + parts_removal_addr;
            M2C_FIELD(parts_removal_addr, s16 *, 0x52) = parts_removal_zero;
        }
    }
    temp_r0_37 = parts_removal_equipment_slot + 1;
    parts_removal_equipment_slot = temp_r0_37;
    if ((u32) temp_r0_37 <= 3U) {
        goto loop_201;
    }
    {
        register u32 parts_removal_scale asm("r0") = 0x94;
        register u32 parts_removal_player2 asm("r5") = command_side;
        register u32 parts_removal_record asm("r4") = parts_removal_player2;

        parts_removal_record *= parts_removal_scale;
        {
            register u32 parts_removal_base2 asm("r7") = BATTLE_STATE_RAM;
            register u32 parts_removal_field2 asm("r0");

            parts_removal_record += parts_removal_base2;
            parts_removal_field2 = BATTLE_DECK_PREPARATION_OFFSET(payloads);
            asm volatile("" : "+r"(parts_removal_field2));
            parts_removal_record += parts_removal_field2;
        }
        ClearBattlePassiveEquipmentEffects(parts_removal_player2, *(u8 *)parts_removal_record);
        ApplyBattlePassiveEquipmentEffects(command_side, *(u8 *)parts_removal_record);
        RecalculateBattleUnitStats(command_side, *(u8 *)parts_removal_record);
        parts_or_decoy_unit_slot = *(u8 *)parts_removal_record;
    }
    parts_or_decoy_equipment_slot = -1;
    parts_or_decoy_zero = 0;
    {
        register volatile s32 *case3031_outgoing asm("sp");
        case3031_outgoing[0] = parts_or_decoy_zero;
        case3031_outgoing[1] = parts_or_decoy_zero;
    }
    parts_or_decoy_display_kind = BATTLE_EFFECT_PRESENTATION_DISSOLVING_ORBS;
    goto queue_parts_or_decoy_display;
}
case DECK_COMMAND_DECOY - DECK_COMMAND_FRIENDSHIP: {
    register u32 decoy_base asm("r2");
    register u32 decoy_offset asm("r3");
    register u32 decoy_player asm("r4");
    register u32 decoy_state asm("r5");
    register u32 decoy_dst asm("r1");
    register u32 decoy_src asm("r0");

    decoy_base = BATTLE_STATE_RAM;
    decoy_offset = BATTLE_DECK_REWARD_OFFSET(persistent_command_ids);
    asm volatile("" : "+r"(decoy_offset));
    decoy_dst = decoy_base + decoy_offset;
    decoy_player = command_side;
    decoy_dst = decoy_player + decoy_dst;
    decoy_state = command_side_times_four;
    decoy_src = decoy_state + decoy_base;
    {
        register u32 decoy_field asm("r7") = BATTLE_DECK_PREPARATION_OFFSET(actions[0].command_id);
        decoy_src += decoy_field;
    }
    {
        register u32 decoy_value asm("r0") = *(u8 *)decoy_src;
        parts_or_decoy_zero = 0;
        *(u8 *)decoy_dst = decoy_value;
    }
    {
        register u32 decoy_record asm("r0") = 0x94;
        register u32 decoy_record_field asm("r1");

        decoy_record *= decoy_player;
        decoy_record += decoy_base;
        decoy_record_field = BATTLE_DECK_PREPARATION_OFFSET(payloads);
        decoy_record += decoy_record_field;
        parts_or_decoy_unit_slot = *(u8 *)decoy_record;
    }
    parts_or_decoy_equipment_slot = -1;
    {
        register volatile s32 *case3031_outgoing asm("sp");
        case3031_outgoing[0] = parts_or_decoy_zero;
        case3031_outgoing[1] = parts_or_decoy_zero;
    }
    parts_or_decoy_display_kind = BATTLE_EFFECT_PRESENTATION_GREEN_STREAK;
    goto queue_parts_or_decoy_display;
}
queue_parts_or_decoy_display:
    {
        register volatile s32 *case3031_outgoing asm("sp");
        case3031_outgoing[2] = parts_or_decoy_display_kind;
        case3031_outgoing[3] = parts_or_decoy_zero;
        case3031_outgoing[4] = parts_or_decoy_zero;
        case3031_outgoing[5] = parts_or_decoy_zero;
    }
    QueueBattleEffectDisplay(command_side, parts_or_decoy_unit_slot, parts_or_decoy_equipment_slot, parts_or_decoy_zero);
    return;
case DECK_COMMAND_GODS_TERRITORY - DECK_COMMAND_FRIENDSHIP: {
    register void *gods_territory_base asm("r6");
    s32 gods_territory_zero;
    register s32 gods_territory_row asm("r5");

    {
        register u32 gods_territory_outer_zero asm("r2") = 0;
        asm volatile("" : "+r"(gods_territory_outer_zero));
        gods_territory_slot = gods_territory_outer_zero;
    }
    gods_territory_base = (void *)BATTLE_STATE_RAM;
    gods_territory_zero = 0;
    {
        register s32 gods_territory_row_base asm("r3") = command_side_times_four;
        register s32 gods_territory_row_work asm("r0");
        gods_territory_row = command_side;
        asm volatile("" : "+r"(gods_territory_row_base), "+r"(gods_territory_row));
        asm volatile("add %0, %1, %2"
                     : "=r"(gods_territory_row_work)
                     : "r"(gods_territory_row_base), "r"(gods_territory_row)
                     : "cc");
        gods_territory_row_work *= 8;
        gods_territory_row_work -= gods_territory_row;
        gods_territory_row = gods_territory_row_work << 7;
    }
loop_207:
    if ((IsBattleUnitActive(command_side, gods_territory_slot) << 0x18) == 0) {
        goto block_212;
    }
    {
        register u32 gods_territory_record_flag asm("r0");
        asm volatile(
            "mov r1, %1\n\t"
            "lsl r0, r1, #2\n\t"
            "add r0, %1\n\t"
            "lsl r0, r0, #3\n\t"
            "sub r0, r0, r1\n\t"
            "lsl r0, r0, #4\n\t"
            "add r0, %2\n\t"
            "add r0, %3\n\t"
            "add r0, #112\n\t"
            "ldrb r0, [r0]"
            : "=r"(gods_territory_record_flag)
            : "r"(gods_territory_slot), "r"(gods_territory_row), "r"(gods_territory_base)
            : "r1", "cc", "memory");
        if (gods_territory_record_flag != 1) {
            goto block_211;
        }
    }
    protagonist_slot_carrier = gods_territory_slot;
    goto block_212;
block_211:
    QUEUE_BATTLE_EFFECT_DISPLAY_WITH_STACK_ARGUMENTS(command_side, gods_territory_slot, -1, 0,
                      gods_territory_zero, gods_territory_zero, BATTLE_EFFECT_PRESENTATION_GREEN_STREAK,
                      gods_territory_zero, gods_territory_zero, gods_territory_zero);
block_212:
    temp_r0_38 = gods_territory_slot + 1;
    gods_territory_slot = temp_r0_38;
    if ((u32) temp_r0_38 <= 5U) {
        goto loop_207;
    }
    RemoveBattleUnitFromTurnOrderWide(command_side, protagonist_slot_carrier);
    {
        register volatile s32 *gods_territory_outgoing asm("sp");
        register s32 gods_territory_arg2 asm("r2");
        register s32 gods_territory_outgoing_zero asm("r1");

        gods_territory_arg2 = -1;
        gods_territory_outgoing_zero = 0;
        gods_territory_outgoing[0] = gods_territory_outgoing_zero;
        gods_territory_outgoing[1] = gods_territory_outgoing_zero;
        gods_territory_outgoing[2] = BATTLE_EFFECT_TURN_MARKER;
        gods_territory_outgoing[3] = gods_territory_outgoing_zero;
        gods_territory_outgoing[4] = 1;
        gods_territory_outgoing[5] = gods_territory_outgoing_zero;
        {
            register s32 gods_territory_arg0 asm("r0") = command_side;
            register s32 gods_territory_arg1 asm("r1") = protagonist_slot_carrier;
            register s32 gods_territory_arg3 asm("r3") = 0;

            AddBattleEffect(gods_territory_arg0, gods_territory_arg1,
                          gods_territory_arg2, gods_territory_arg3);
        }
    }
    {
        register u32 gods_territory_exit_base asm("r0") = BATTLE_STATE_RAM;
        register u32 gods_territory_exit_dest_offset asm("r2") = BATTLE_DECK_REWARD_OFFSET(persistent_command_ids);
        register s32 gods_territory_exit_dest_index asm("r3");
        register s32 gods_territory_exit_source_index asm("r4");
        register u32 gods_territory_exit_source_offset asm("r5");

        asm volatile("" : "+r"(gods_territory_exit_base),
                     "+r"(gods_territory_exit_dest_offset));
        persistent_command_destination =
            (u8 *)(gods_territory_exit_base + gods_territory_exit_dest_offset);
        gods_territory_exit_dest_index = command_side;
        persistent_command_destination = (u8 *)(
            (u32)gods_territory_exit_dest_index - (0U - (u32)persistent_command_destination));
        gods_territory_exit_source_index = command_side_times_four;
        gods_territory_exit_base =
            (u32)gods_territory_exit_source_index
            - (0U - gods_territory_exit_base);
        gods_territory_exit_source_offset = BATTLE_DECK_PREPARATION_OFFSET(actions[0].command_id);
        asm volatile("" : "+r"(gods_territory_exit_source_offset));
        persistent_command_source =
            (u8 *)(gods_territory_exit_base + gods_territory_exit_source_offset);
    }
    goto store_persistent_command;
}
case DECK_COMMAND_GRAVITY_STORM - DECK_COMMAND_FRIENDSHIP:
    {
    register volatile s32 *gravity_storm_stack asm("sp");
    u8 *gravity_storm_wait_flags;
    u8 *gravity_storm_wait_records;
    register u8 *gravity_storm_first_base asm("r5");
    register u8 *gravity_storm_first_count asm("r1");
    register s32 *gravity_storm_wait_record0 asm("r6");
    register u32 gravity_storm_side4 asm("r1");
    register u32 gravity_storm_successor asm("r2");
    register u32 gravity_storm_second_row_base asm("r4");
    {
        register u32 gravity_storm_initial_seed asm("r7") = 0;

        asm volatile("" : "+r"(gravity_storm_initial_seed));
        gravity_storm_side = gravity_storm_initial_seed;
    }
loop_215:
    {
        register u32 gravity_storm_initial_view asm("r0") = gravity_storm_side;

        asm volatile("" : "+r"(gravity_storm_initial_view));
        if (gravity_storm_initial_view != 0) {
            goto block_217;
        }
    }
    {
        register u32 gravity_storm_r6_birth asm("r6");
        asm volatile("" : "=r"(gravity_storm_r6_birth));
        opening_unit_or_gravity_storm_side = command_side;
        asm volatile("" : : "r"(gravity_storm_r6_birth));
    }
    goto block_218;
block_217:
    {
        register u32 gravity_storm_r6_birth asm("r6");
        s32 gravity_storm_side_one;

        asm volatile("" : "=r"(gravity_storm_r6_birth));
        gravity_storm_side_one = 1;
        opening_unit_or_gravity_storm_side = command_side;
        opening_unit_or_gravity_storm_side ^= gravity_storm_side_one;
        asm volatile("" : : "r"(gravity_storm_r6_birth));
    }
block_218:
    StartBattleCameraTransition(2, opening_unit_or_gravity_storm_side, 0, 0);
    gravity_storm_side4 = opening_unit_or_gravity_storm_side * 4;
    gravity_storm_stack[29] = gravity_storm_side4;
    gravity_storm_successor = gravity_storm_side;
    gravity_storm_successor += 1;
    gravity_storm_stack[24] = gravity_storm_successor;
    {
        register s32 gravity_storm_row_twice asm("r3") = opening_unit_or_gravity_storm_side * 2;

        physical_stack[28] = gravity_storm_row_twice;
        asm volatile("" : "=m"(sp70));
    }
    goto loop_220;
block_219:
    YieldTaskForUpdates(1);
loop_220:
    if ((IsBattleCameraTransitionComplete() << 0x18) == 0) {
        goto block_219;
    }
    {
        register u32 gravity_storm_outer_zero asm("r4") = 0;
        asm volatile("" : "+r"(gravity_storm_outer_zero));
        var_sl = gravity_storm_outer_zero;
    }
    {
        register u32 gravity_storm_slots_work asm("r0");
        register u32 gravity_storm_slots_offset asm("r1");
        gravity_storm_first_base = (u8 *)BATTLE_STATE_RAM;
        gravity_storm_slots_work = (u32)gravity_storm_first_base;
        gravity_storm_slots_offset = 0x2714;
        asm volatile("" : "+r"(gravity_storm_slots_offset));
        gravity_storm_slots_work += gravity_storm_slots_offset;
        gravity_storm_slots = (u8 *)gravity_storm_slots_work;
    }
    {
        register u8 *gravity_storm_lists_seed asm("r2") =
            (u8 *)0x02032FCA;
        asm volatile("" : "+r"(gravity_storm_lists_seed));
        gravity_storm_lists = gravity_storm_lists_seed;
    }
loop_222:
    {
        register u32 gravity_storm_row_index asm("r3") = var_sl;
        temp_r4_8 = gravity_storm_row_index * 4;
        temp_r1_8 =
            (((temp_r4_8 + gravity_storm_row_index) * 8)
             - gravity_storm_row_index) * 0x10;
    }
    {
        register u32 gravity_storm_copy_arg0_base asm("r2") =
            0x02032FE4;
        register void *gravity_storm_copy_arg0 asm("r0");
        register void *gravity_storm_copy_arg1 asm("r1");
        register s32 gravity_storm_first_row_reload asm("r3");
        register s32 gravity_storm_first_row_work asm("r2");

        asm volatile("add %0, %1, %2"
                     : "=r"(gravity_storm_copy_arg0)
                     : "r"(temp_r1_8),
                       "r"(gravity_storm_copy_arg0_base)
                     : "cc");
        asm volatile(
            "ldr %0, [sp, #116]\n\t"
            "add %1, %0, r7\n\t"
            "lsl %1, %1, #3\n\t"
            "sub %1, %1, r7\n\t"
            "lsl %1, %1, #7"
            : "=r"(gravity_storm_first_row_reload),
              "=r"(gravity_storm_first_row_work)
            : "r"(opening_unit_or_gravity_storm_side)
            : "cc", "memory");
        asm volatile(
            "add %0, %1, %2\n\t"
            "add %0, %0, r5"
            : "=r"(gravity_storm_copy_arg1)
            : "r"(temp_r1_8),
              "r"(gravity_storm_first_row_work),
              "r"(gravity_storm_first_base)
            : "cc");
        CopyBytes(
            gravity_storm_copy_arg0,
            gravity_storm_copy_arg1,
            0x270);
    }
    {
        register s32 gravity_storm_first_dest_base asm("r2") = 0x02033E84;

        asm volatile("add %0, %1, %0"
                     : "+r"(gravity_storm_first_dest_base)
                     : "r"(temp_r4_8));
        gravity_storm_first_dest = (s32 *)gravity_storm_first_dest_base;
    }
    {
        register s32 gravity_storm_first_src_base asm("r0") = 0x02032E8C;
        register s32 gravity_storm_first_src_reload asm("r3");
        asm volatile(
            "ldr %2, [sp, #112]\n\t"
            "add %1, %2, r7\n\t"
            "lsl %1, %1, #3\n\t"
            "add %1, r4, %1\n\t"
            "add %0, %1, %0\n\t"
            "ldr %0, [%0]\n\t"
            "str %0, [%3]"
            : "+r"(gravity_storm_first_src_base),
              "=r"(temp_r1_9),
              "=r"(gravity_storm_first_src_reload)
            : "r"(gravity_storm_first_dest),
              "r"(temp_r4_8),
              "r"(opening_unit_or_gravity_storm_side)
            : "cc", "memory");
    }
    {
        register s32 gravity_storm_second_dest_base asm("r0") =
            0x02033E9C;
        asm volatile("add %0, %0, %1"
                     : "+r"(temp_r4_8)
                     : "r"(gravity_storm_second_dest_base)
                     : "cc");
    }
    {
        register s32 gravity_storm_second_src_base asm("r0") =
            0x02032EBC;
        asm volatile(
            "add %0, %0, %1\n\t"
            "ldr %1, [%0]\n\t"
            "str %1, [%2]"
            : "+r"(temp_r1_9), "+r"(gravity_storm_second_src_base)
            : "r"(temp_r4_8)
            : "cc", "memory");
    }
    gravity_storm_first_count = (u8 *)0x02032FDC + var_sl;
    *gravity_storm_first_count = 0U;
    {
        register u32 gravity_storm_first_index asm("r6");
        register u32 gravity_storm_first_list_seed asm("r4");

        gravity_storm_first_index = 0;
        gravity_storm_first_list_seed = var_sl;
        asm volatile("" : "+r"(gravity_storm_first_list_seed));
        gravity_storm_first_list_offset = gravity_storm_first_list_seed * 3;
loop_223:
        temp_r2_7 = gravity_storm_first_index * 2;
        {
            register u8 *gravity_storm_players_base asm("r4") =
                (u8 *)0x0203725F;
            register u8 *gravity_storm_player_entry asm("r0");
            asm volatile("add %0, %1, %2"
                         : "=l"(gravity_storm_player_entry)
                         : "l"(temp_r2_7), "l"(gravity_storm_players_base));
            if (*gravity_storm_player_entry != opening_unit_or_gravity_storm_side) {
                goto block_226;
            }
        }
        {
            register u8 *gravity_storm_slots_copy asm("r4") = gravity_storm_slots;
            register u8 *gravity_storm_slot_entry asm("r0");
            asm volatile("add %0, %1, %2"
                         : "=l"(gravity_storm_slot_entry)
                         : "l"(temp_r2_7), "l"(gravity_storm_slots_copy));
            if (*gravity_storm_slot_entry != var_sl) {
                goto block_226;
            }
        }
        gravity_storm_lists[*gravity_storm_first_count + gravity_storm_first_list_offset] =
            gravity_storm_first_index;
        *gravity_storm_first_count = (u8) (*gravity_storm_first_count + 1);
block_226:
        {
            register u32 gravity_storm_first_successor asm("r0");

            gravity_storm_first_successor = gravity_storm_first_index + 1;
            gravity_storm_first_successor <<= 24;
            gravity_storm_first_index = gravity_storm_first_successor >> 24;
        }
        if (gravity_storm_first_index <= 0x23U) {
            goto loop_223;
        }
    }
    if (opening_unit_or_gravity_storm_side != 0) {
        goto block_229;
    }
    {
        register u8 *gravity_storm_side_dest asm("r0");
        register u8 *gravity_storm_side_base asm("r2");
        register u32 gravity_storm_side_offset asm("r3");
        register u8 *gravity_storm_side_source asm("r1");

        gravity_storm_side_dest = (u8 *)0x02033EB4;
        gravity_storm_side_dest += var_sl;
        gravity_storm_side_base = (u8 *)BATTLE_STATE_RAM;
        gravity_storm_side_offset = 0x9C;
        asm volatile("" : "+r"(gravity_storm_side_offset));
        gravity_storm_side_offset <<= 6;
        gravity_storm_side_source =
            gravity_storm_side_base + gravity_storm_side_offset + var_sl;
        *gravity_storm_side_dest = *gravity_storm_side_source;
    }
block_229:
    asm volatile("" : "+r"(var_sl));
    temp_r0_39 = var_sl + 1;
    var_sl = temp_r0_39;
    if ((u32) temp_r0_39 <= 5U) {
        goto loop_222;
    }
    {
        register u32 gravity_storm_second_outer_zero asm("r4") = 0;

        asm volatile("" : "+r"(gravity_storm_second_outer_zero));
        gravity_storm_destination_slot = gravity_storm_second_outer_zero;
    }
    {
        register s32 gravity_storm_second_row asm("r5") = physical_stack[28];
        register s32 gravity_storm_second_row_sum asm("r0");

        asm volatile("add %0, %1, %2"
                     : "=r"(gravity_storm_second_row_sum)
                     : "r"(gravity_storm_second_row), "r"(opening_unit_or_gravity_storm_side));
        temp_r0_40 = gravity_storm_second_row_sum;
    }
    physical_stack[11] = temp_r0_40 * 2;
    asm volatile("" : "=m"(sp2C));
    sp34 = temp_r0_40 * 8;
    {
        register s32 gravity_storm_scale_factor asm("r0") = 0x94;
        register s32 gravity_storm_scale_input asm("r3") = command_side;
        register s32 gravity_storm_scale_work asm("r2");

        asm volatile(
            "add %0, %1, #0\n\t"
            "mul %0, %2"
            : "=&l"(gravity_storm_scale_work)
            : "l"(gravity_storm_scale_input), "l"(gravity_storm_scale_factor)
            : "cc");
        physical_stack[12] = gravity_storm_scale_work;
    }
loop_231:
    {
        register s32 gravity_storm_side_input asm("r4") = command_side;

        if (gravity_storm_side_input == 0) {
            goto block_233;
        }
    }
    if (*(u8 *)0x0203055C == 1) {
        goto block_234;
    }
block_233:
    {
        register u32 gravity_storm_normal_offset asm("r0");
        register u8 *gravity_storm_normal_base asm("r1");

        asm volatile(
            "ldr %0, [sp, #44]\n\t"
            "add %0, sl\n\t"
            "ldr r5, [sp, #48]\n\t"
            "add %0, %0, r5"
            : "=&r"(gravity_storm_normal_offset)
            :
            : "cc");
        gravity_storm_normal_base = (u8 *)0x0203EBD0;
        asm volatile("add %0, %1, %2"
                     : "=r"(var_r0_5)
                     : "r"(gravity_storm_normal_offset),
                       "r"(gravity_storm_normal_base)
                     : "cc");
    }
    goto block_235;
block_234:
    {
        register u32 gravity_storm_alternate_offset asm("r0");
        register u8 *gravity_storm_alternate_base asm("r3");

        asm volatile(
            "mov %0, #1\n\t"
            "add r1, r7, #0\n\t"
            "eor r1, %0\n\t"
            "lsl r1, r1, #24\n\t"
            "lsr r1, r1, #24\n\t"
            "lsl %0, r1, #1\n\t"
            "add %0, %0, r1\n\t"
            "lsl %0, %0, #1\n\t"
            "add %0, sl\n\t"
            "ldr r2, [sp, #48]\n\t"
            "add %0, %0, r2"
            : "=&r"(gravity_storm_alternate_offset)
            :
            : "cc");
        gravity_storm_alternate_base = (u8 *)0x0203EBD0;
        asm volatile("" : "+r"(gravity_storm_alternate_base));
        var_r0_5 = gravity_storm_alternate_offset + (u32)gravity_storm_alternate_base;
    }
block_235:
    temp_r0_41 = *var_r0_5;
    {
        register u32 gravity_storm_index asm("r4") = gravity_storm_destination_slot;
        register u32 gravity_storm_copy_dest asm("r0");
        register u32 gravity_storm_row_base asm("r1");
        register u32 gravity_storm_dest_base asm("r2");
        register u32 gravity_storm_copy_source asm("r1");
        register u32 gravity_storm_unit_copy asm("r2");
        register s32 gravity_storm_copy_size asm("r2");
        temp_r5_3 = (void *)(gravity_storm_index << 2);
        gravity_storm_copy_dest = (u32)temp_r5_3 + gravity_storm_index;
        gravity_storm_copy_dest <<= 3;
        gravity_storm_copy_dest -= gravity_storm_index;
        temp_r3_2 = gravity_storm_copy_dest << 4;
        gravity_storm_row_base = gravity_storm_stack[29];
        gravity_storm_copy_dest = gravity_storm_row_base + opening_unit_or_gravity_storm_side;
        gravity_storm_copy_dest <<= 3;
        gravity_storm_copy_dest -= opening_unit_or_gravity_storm_side;
        gravity_storm_copy_dest <<= 7;
        gravity_storm_copy_dest = temp_r3_2 + gravity_storm_copy_dest;
        gravity_storm_dest_base = BATTLE_STATE_RAM;
        asm volatile("" : "+r"(gravity_storm_dest_base));
        gravity_storm_copy_dest += gravity_storm_dest_base;
        gravity_storm_copy_source = temp_r0_41;
        temp_r4_9 = gravity_storm_copy_source << 2;
        gravity_storm_copy_source = temp_r4_9 + gravity_storm_copy_source;
        gravity_storm_copy_source <<= 3;
        gravity_storm_unit_copy = temp_r0_41;
        gravity_storm_copy_source -= gravity_storm_unit_copy;
        gravity_storm_copy_source <<= 4;
        gravity_storm_unit_copy = 0x02032FE4;
        gravity_storm_copy_source += gravity_storm_unit_copy;
        gravity_storm_copy_size = 0x270;
        gravity_storm_stack[31] = temp_r3_2;
        CopyBytes((void *)gravity_storm_copy_dest,
                      (void *)gravity_storm_copy_source, gravity_storm_copy_size);
    }
    {
        register s32 gravity_storm_second_dest_base asm("r1") = 0x02032E8C;
        register s32 gravity_storm_second_row asm("r0") = sp34;
        temp_r5_3 = (void *)((u32)temp_r5_3 + gravity_storm_second_row);
        asm volatile("add %0, %1, %0"
                     : "+r"(gravity_storm_second_dest_base)
                     : "r"(temp_r5_3));
        {
            register u32 gravity_storm_record_a asm("r0") = 0x02033E84;
            asm volatile("" : "+r"(gravity_storm_record_a));
            gravity_storm_record_a = temp_r4_9 + gravity_storm_record_a;
            *(s32 *)gravity_storm_second_dest_base = *(s32 *)gravity_storm_record_a;
        }
    }
    {
        register u32 gravity_storm_record_b_dest asm("r0") = 0x02032EBC;
        temp_r5_3 = (void *)((u32)temp_r5_3 + gravity_storm_record_b_dest);
    }
    {
        register u32 gravity_storm_record_b_src asm("r0") = 0x02033E9C;
        asm volatile("" : "+r"(gravity_storm_record_b_src));
        temp_r4_9 += gravity_storm_record_b_src;
    }
    *(s32 *)temp_r5_3 = *(s32 *)temp_r4_9;
    gravity_storm_turn_entry_index = 0;
    {
        register u8 *gravity_storm_second_count_r0 asm("r0") =
            (u8 *)0x02032FDC + temp_r0_41;
        gravity_storm_stack[26] = gravity_storm_destination_slot + 1;
        asm volatile("ldr r3, [sp, #124]"
                     :
                     :
                     : "r3", "memory");
        if ((u32)gravity_storm_turn_entry_index >= (u32)*gravity_storm_second_count_r0) {
            goto block_238;
        }
    }
    {
        register u32 gravity_storm_second_list_seed asm("r2") = temp_r0_41;

        asm volatile(
            "lsl r0, %0, #1\n\t"
            "add r5, r0, %0"
            :
            : "r"(gravity_storm_second_list_seed)
            : "r0", "r5", "cc");
    }
    {
        gravity_storm_second_row_base = BATTLE_STATE_RAM;
        asm volatile(
            "mov %0, %1\n\t"
            "add %1, r3, #0"
            : "=r"(gravity_storm_lists), "+r"(gravity_storm_second_row_base)
            :
            : "cc");
    }
loop_237:
    {
        register s32 gravity_storm_call_arg0 asm("r0");
        register s32 gravity_storm_call_arg1 asm("r1");
        register s32 gravity_storm_call_arg2 asm("r2");
        register s32 gravity_storm_call_arg3 asm("r3");
        asm volatile(
            "add %0, r6, r5"
            : "=r"(gravity_storm_call_arg0));
        gravity_storm_call_arg1 = 0x02032FCA;
        asm volatile(
            "add %0, %0, %1\n\t"
            "ldrb %0, [%0]\n\t"
            "ldr r2, [sp, #116]\n\t"
            "add r1, r2, r7\n\t"
            "lsl r1, r1, #3\n\t"
            "sub r1, r1, r7\n\t"
            "lsl r1, r1, #7\n\t"
            "add r1, r4, r1\n\t"
            "add r1, r8\n\t"
            "movs r2, #10\n\t"
            "ldrsh %3, [r1, r2]\n\t"
            "add %1, r7, #0\n\t"
            "mov %2, sl"
            : "+r"(gravity_storm_call_arg0),
              "+r"(gravity_storm_call_arg1),
              "=r"(gravity_storm_call_arg2),
              "=r"(gravity_storm_call_arg3)
            :
            : "cc", "memory");
        ((void (*)())SetBattleTurnOrderEntryWide)(
            gravity_storm_call_arg0, gravity_storm_call_arg1,
            gravity_storm_call_arg2, gravity_storm_call_arg3);
    }
    gravity_storm_turn_entry_index += 1;
    if ((u32) gravity_storm_turn_entry_index < (u32) M2C_FIELD(temp_r0_41, u8 *, 0x02032FDC)) {
        goto loop_237;
    }
block_238:
    if (opening_unit_or_gravity_storm_side != 0) {
        goto block_240;
    }
    M2C_FIELD(gravity_storm_destination_slot, u8 *, 0x0203724C) = (u8) M2C_FIELD(temp_r0_41, u8 *, 0x02033EB4);
block_240:
    if ((IsBattleUnitActive(opening_unit_or_gravity_storm_side, gravity_storm_destination_slot) << 0x18) == 0) {
        goto block_242;
    }
    {
        register u8 *gravity_storm_flag_address asm("r1");

        {
            register u8 *gravity_storm_flag_base asm("r0") =
                (u8 *)0x02032EEC;

            gravity_storm_flag_address =
                (u8 *)(physical_stack[11] + gravity_storm_destination_slot);
            asm volatile("add %0, %0, %1"
                         : "+r"(gravity_storm_flag_address)
                         : "r"(gravity_storm_flag_base));
        }
        {
            register u32 gravity_storm_flag_value asm("r0") = 1;

            *gravity_storm_flag_address = gravity_storm_flag_value;
        }
    }
block_242:
    {
        register u32 gravity_storm_next_outer_reload asm("r3") =
            gravity_storm_stack[26];
        register u32 gravity_storm_next_outer_normalized asm("r0");

        asm volatile(
            "lsl %0, %1, #24\n\t"
            "lsr %0, %0, #24"
            : "=r"(gravity_storm_next_outer_normalized)
            : "r"(gravity_storm_next_outer_reload));
        gravity_storm_destination_slot = gravity_storm_next_outer_normalized;
        if (gravity_storm_next_outer_normalized > 5U) {
            goto block_244;
        }
    }
    goto loop_231;
block_244:
    gravity_storm_wait_records = (u8 *)0x02032E8C;
    {
        register s32 gravity_storm_wait_base asm("r4") = physical_stack[28];
        register s32 gravity_storm_wait_index asm("r1");
        gravity_storm_wait_index = gravity_storm_wait_base + opening_unit_or_gravity_storm_side;
        gravity_storm_wait_flags = (u8 *)0x02032EEC;
        {
            register s32 gravity_storm_flag_offset asm("r0") =
                gravity_storm_wait_index * 2;
            gravity_storm_wait_flag_address = gravity_storm_wait_flags + gravity_storm_flag_offset;
        }
        gravity_storm_wait_record0 =
            (s32 *)(gravity_storm_wait_records + (gravity_storm_wait_index * 8));
    }
loop_245:
    {
        register s32 gravity_storm_wait_call_arg asm("r0") = 1;

        asm volatile("" : "=m"(sp7C));
        gravity_storm_stack[31] = (s32)gravity_storm_wait_flag_address;
        YieldTaskForUpdates(gravity_storm_wait_call_arg);
    }
    {
        register u32 gravity_storm_wait_zero asm("r0") = 0;
        asm volatile("" : "+r"(gravity_storm_wait_zero));
        gravity_storm_wait_slot = gravity_storm_wait_zero;
    }
    {
        register s32 gravity_storm_wait_record_value asm("r0") =
            *gravity_storm_wait_record0;

        gravity_storm_wait_flag_address = (u8 *)gravity_storm_stack[31];
        if (gravity_storm_wait_record_value == 0) {
            goto loop_247;
        }
    }
    if (*gravity_storm_wait_flag_address != 0) {
        goto block_250;
    }
loop_247:
    temp_r0_43 = gravity_storm_wait_slot + 1;
    gravity_storm_wait_slot = temp_r0_43;
    if ((u32) temp_r0_43 > 5U) {
        goto block_251;
    }
    {
        register s32 gravity_storm_inner_offset asm("r0") = temp_r0_43 * 4;
        register s32 gravity_storm_row_work asm("r1") = physical_stack[28];
        temp_r2_8 = gravity_storm_row_work + opening_unit_or_gravity_storm_side;
        gravity_storm_row_work = (s32)temp_r2_8 * 8;
        gravity_storm_inner_offset += gravity_storm_row_work;
        gravity_storm_inner_offset += (s32)gravity_storm_wait_records;
        if (*(s32 *)gravity_storm_inner_offset == 0) {
            goto loop_247;
        }
    }
    {
        register s32 gravity_storm_flag_address asm("r0") =
            (s32)temp_r2_8 * 2;
        gravity_storm_flag_address += gravity_storm_wait_slot;
        gravity_storm_flag_address += (s32)gravity_storm_wait_flags;
        if (*(u8 *)gravity_storm_flag_address == 0) {
            goto loop_247;
        }
    }
block_250:
    {
        register u32 gravity_storm_wait_check asm("r2") = gravity_storm_wait_slot;
        asm volatile("" : "+r"(gravity_storm_wait_check));
        if (gravity_storm_wait_check <= 5U) {
            goto loop_245;
        }
    }
block_251:
    {
        register u32 gravity_storm_exit_state_reload asm("r3") =
            gravity_storm_stack[24];
        register u32 gravity_storm_exit_state_normalized asm("r0");

        asm volatile(
            "lsl %0, %1, #24\n\t"
            "lsr %0, %0, #24"
            : "=r"(gravity_storm_exit_state_normalized)
            : "r"(gravity_storm_exit_state_reload));
        gravity_storm_side = gravity_storm_exit_state_normalized;
        if (gravity_storm_exit_state_normalized > 1U) {
            goto block_253;
        }
    }
    goto loop_215;
block_253:
    return;
    }
case DECK_COMMAND_T_S_WARP - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *t_s_warp_stack asm("sp");
    register s32 t_s_warp_zero asm("r4");
    BuildBattleTurnOrder(1);
    t_s_warp_zero = 0;
    t_s_warp_side = t_s_warp_zero;
loop_256:
    {
        register s32 t_s_warp_guard_r5 asm("r5");
        register s32 t_s_warp_guard_r6 asm("r6");
        asm volatile("" : "=r"(t_s_warp_guard_r5), "=r"(t_s_warp_guard_r6));
        t_s_warp_display_slot = 0;
        asm volatile("" :: "r"(t_s_warp_guard_r5), "r"(t_s_warp_guard_r6));
    }
    {
        register s32 t_s_warp_successor asm("r5") = t_s_warp_side;

        t_s_warp_successor += 1;
        t_s_warp_stack[24] = t_s_warp_successor;
    }
loop_257:
    QUEUE_BATTLE_EFFECT_DISPLAY_WITH_STACK_ARGUMENTS(t_s_warp_side, t_s_warp_display_slot, -1, 0,
                      t_s_warp_zero, t_s_warp_zero, BATTLE_EFFECT_PRESENTATION_GREEN_STREAK,
                      t_s_warp_zero, t_s_warp_zero, t_s_warp_zero);
    t_s_warp_display_slot += 1;
    if ((u32) t_s_warp_display_slot <= 5U) {
        goto loop_257;
    }
    {
        register u32 t_s_warp_exit_state_reload asm("r7") =
            t_s_warp_stack[24];
        register u32 t_s_warp_exit_state_normalized asm("r0");

        asm volatile(
            "lsl %0, %1, #24\n\t"
            "lsr %0, %0, #24"
            : "=r"(t_s_warp_exit_state_normalized)
            : "r"(t_s_warp_exit_state_reload));
        t_s_warp_side = t_s_warp_exit_state_normalized;
        if (t_s_warp_exit_state_normalized <= 1U) {
            goto loop_256;
        }
    }
    return;
}
case DECK_COMMAND_CONFUSION - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *confusion_stack asm("sp");
    register s32 confusion_zero asm("r4");
    register u32 confusion_copy_current asm("r0");
    u8 *confusion_old_players;
    u8 *confusion_old_slots;
    u8 *confusion_new_players;
    u8 *confusion_new_slots;
    confusion_copy_current = 0;
    confusion_saved_entry_index = confusion_copy_current;
    confusion_old_players = (u8 *)0x02032F82;
    confusion_new_players = (u8 *)0x0203725F;
    confusion_old_slots = confusion_old_players + 1;
    confusion_new_slots = confusion_new_players + 1;
loop_261:
    temp_r1_11 = confusion_saved_entry_index * 2;
    asm volatile("" : : "r"(confusion_copy_current));
    *(u8 *)(
        (u32)temp_r1_11 - (0U - (u32)confusion_old_players)) =
        *(u8 *)(
            (u32)temp_r1_11 - (0U - (u32)confusion_new_players));
    *(u8 *)(
        (u32)temp_r1_11 - (0U - (u32)confusion_old_slots)) =
        *(u8 *)(
            (u32)temp_r1_11 - (0U - (u32)confusion_new_slots));
    asm volatile(
        "mov r0, r8\n\t"
        "add r0, #1\n\t"
        "lsl r0, r0, #24\n\t"
        "lsr %0, r0, #24"
        : "=r"(confusion_copy_current)
        :
        : "cc");
    confusion_saved_entry_index = confusion_copy_current;
    if (confusion_copy_current <= 0x23U) {
        goto loop_261;
    }
    {
    u8 *confusion_players;
    u8 *confusion_saved_players;
    u8 *confusion_selection;
    u8 *confusion_saved_slots;
    u8 *confusion_slots;
    s32 confusion_side_offset;
    register s32 confusion_side_r5 asm("r5");
    {
        register s32 confusion_state_seed asm("r3") = 0;
        asm volatile("" : "+r"(confusion_state_seed));
        confusion_destination_entry_index = confusion_state_seed;
    }
    confusion_players = (u8 *)0x0203725F;
    confusion_saved_players = (u8 *)0x02032F82;
    {
        register s32 confusion_scale_r0 asm("r0") = 0x94;
        register s32 confusion_side_offset_r4 asm("r4");

        confusion_side_r5 = command_side;
        confusion_side_offset_r4 = confusion_side_r5;
        asm volatile("" : "+r"(confusion_scale_r0),
                     "+r"(confusion_side_r5),
                     "+r"(confusion_side_offset_r4));
        confusion_side_offset_r4 *= confusion_scale_r0;
        confusion_side_offset = confusion_side_offset_r4;
    }
    confusion_selection = confusion_players + 0x7971;
    asm volatile("" : : "r"(confusion_side_r5));
    confusion_saved_slots = confusion_saved_players + 1;
    confusion_slots = confusion_players + 1;
loop_263:
    {
        register u32 confusion_output_index asm("r3") = confusion_destination_entry_index;
        register u8 *confusion_output_player asm("r3");
        register u32 confusion_selection_index asm("r0");
        register u8 *confusion_selection_address asm("r1");

        asm volatile("" : "+r"(confusion_output_index));
        temp_r2_9 = confusion_output_index * 2;
        asm volatile("add %0, %1, %2"
                     : "=r"(confusion_output_player)
                     : "r"(temp_r2_9), "r"(confusion_players));
        confusion_selection_index = confusion_destination_entry_index;
        asm volatile("add %0, %1, %2"
                     : "=r"(confusion_selection_address)
                     : "r"(confusion_selection_index),
                       "r"(confusion_side_offset));
        confusion_selection_address += (u32)confusion_selection;
        *confusion_output_player =
            confusion_saved_players[
                (u32)*confusion_selection_address * 2];
        asm volatile("add %0, %0, %1"
                     : "+r"(temp_r2_9)
                     : "r"(confusion_slots));
        *(u8 *)temp_r2_9 =
            confusion_saved_slots[
                (u32)*confusion_selection_address * 2];
    }
    temp_r0_47 = confusion_destination_entry_index + 1;
    confusion_destination_entry_index = temp_r0_47;
    if ((u32) temp_r0_47 <= 0x23U) {
        goto loop_263;
    }
    }
    {
        register u32 confusion_state_base asm("r2") = BATTLE_STATE_RAM;
        register u32 confusion_state_offset asm("r3") = 0x270F;
        register u8 *confusion_state asm("r1");
        register u32 confusion_state_value asm("r0");
        asm volatile("" : "+r"(confusion_state_base),
                      "+r"(confusion_state_offset));
        confusion_state =
            (u8 *)(confusion_state_base + confusion_state_offset);
        confusion_state_value = 2;
        *confusion_state = (u8)confusion_state_value;
    }
    confusion_zero = 0;
    confusion_display_side = confusion_zero;
loop_265:
    {
        register s32 confusion_guard_r5 asm("r5");
        register s32 confusion_guard_r6 asm("r6");
        asm volatile("" : "=r"(confusion_guard_r5), "=r"(confusion_guard_r6));
        confusion_display_unit_slot = 0;
        asm volatile("" :: "r"(confusion_guard_r5), "r"(confusion_guard_r6));
    }
    {
        register s32 confusion_successor asm("r5") = confusion_display_side;

        confusion_successor += 1;
        confusion_stack[24] = confusion_successor;
    }
loop_266:
    QUEUE_BATTLE_EFFECT_DISPLAY_WITH_STACK_ARGUMENTS(confusion_display_side, confusion_display_unit_slot, -1, 0,
                      confusion_zero, confusion_zero, BATTLE_EFFECT_PRESENTATION_GREEN_STREAK,
                      confusion_zero, confusion_zero, confusion_zero);
    confusion_display_unit_slot += 1;
    if ((u32) confusion_display_unit_slot <= 5U) {
        goto loop_266;
    }
    {
        register u32 confusion_exit_state_reload asm("r7") =
            confusion_stack[24];
        register u32 confusion_exit_state_normalized asm("r0");

        asm volatile(
            "lsl %0, %1, #24\n\t"
            "lsr %0, %0, #24"
            : "=r"(confusion_exit_state_normalized)
            : "r"(confusion_exit_state_reload));
        confusion_display_side = confusion_exit_state_normalized;
        if (confusion_exit_state_normalized <= 1U) {
            goto loop_265;
        }
    }
    return;
}
case DECK_COMMAND_FIONAS_PRAYER - DECK_COMMAND_FRIENDSHIP: {
    u8 *fionas_prayer_base;
    s32 fionas_prayer_row;
    {
        register u32 fionas_prayer_zero asm("r0") = 0;

        asm volatile("" : "+r"(fionas_prayer_zero));
        fionas_prayer_pilot_slot = fionas_prayer_zero;
    }
    fionas_prayer_base = (u8 *)BATTLE_STATE_RAM;
    {
        register s32 fionas_prayer_row_base asm("r1") = command_side_times_four;
        register s32 fionas_prayer_side asm("r3") = command_side;
        register s32 fionas_prayer_row_sum asm("r0");

        asm volatile("" : "+r"(fionas_prayer_row_base), "+r"(fionas_prayer_side));
        asm volatile("add %0, %1, %2"
                     : "=r"(fionas_prayer_row_sum)
                     : "r"(fionas_prayer_row_base), "r"(fionas_prayer_side));
        fionas_prayer_row_sum <<= 3;
        fionas_prayer_row_sum -= fionas_prayer_side;
        fionas_prayer_row = fionas_prayer_row_sum << 7;
    }
loop_271:
    {
        register u32 fionas_prayer_index_copy asm("r4") = fionas_prayer_pilot_slot;
        register u32 fionas_prayer_record_offset asm("r0");

        fionas_prayer_record_offset = fionas_prayer_index_copy << 2;
        fionas_prayer_record_offset += fionas_prayer_pilot_slot;
        fionas_prayer_record_offset <<= 3;
        fionas_prayer_record_offset -= fionas_prayer_index_copy;
        fionas_prayer_record_offset <<= 4;
        fionas_prayer_record_offset += fionas_prayer_row;
        fionas_prayer_record_offset += (u32)fionas_prayer_base;
        temp_r0_49 = BATTLE_UNIT_FIELD(fionas_prayer_record_offset, u8, pilot_id);
    }
    if (temp_r0_49 == 0xE) {
        goto block_274;
    }
    if (temp_r0_49 == 0x16) {
        goto block_274;
    }
    temp_r0_50 = fionas_prayer_pilot_slot + 1;
    fionas_prayer_pilot_slot = temp_r0_50;
    if ((u32) temp_r0_50 <= 5U) {
        goto loop_271;
    }
block_274:
    AddBattleEffect(command_side, fionas_prayer_pilot_slot, -1, 0, 0, 1, BATTLE_EFFECT_MAX_HP, 0x12C, 0, 0);
    AddBattleEffect(command_side, fionas_prayer_pilot_slot, -1, 0, 0, 1, BATTLE_EFFECT_MAX_EP, 0x1E, 0, 0);
    {
        register u32 fionas_prayer_state_address asm("r0") = BATTLE_STATE_RAM;
        register u32 fionas_prayer_state_offset asm("r5") = BATTLE_COMBINATION_OFFSET(active_unit_slot);
        register u32 fionas_prayer_state_value asm("r7");

        asm volatile("" : "+r"(fionas_prayer_state_address),
                     "+r"(fionas_prayer_state_offset));
        fionas_prayer_state_address += fionas_prayer_state_offset;
        fionas_prayer_state_value = fionas_prayer_pilot_slot;
        *(u8 *)fionas_prayer_state_address = fionas_prayer_state_value;
    }
    final_state = (s32 *)0x02030558;
    next_battle_phase = 0x1320;
    goto store_battle_phase;
}
case DECK_COMMAND_JUNOS_PRAYER - DECK_COMMAND_FRIENDSHIP: {
    register u8 *junos_prayer_first_base asm("r3");
    register u8 *junos_prayer_base asm("r2");
    {
        register u32 junos_prayer_zero asm("r0") = 0;

        asm volatile("" : "+r"(junos_prayer_zero));
        junos_prayer_pilot_slot = junos_prayer_zero;
    }
    {
        register s32 junos_prayer_row_base asm("r1") = command_side_times_four;
        register s32 junos_prayer_side asm("r2") = command_side;
        register s32 junos_prayer_row_sum asm("r0");

        asm volatile("" : "+r"(junos_prayer_row_base), "+r"(junos_prayer_side));
        asm volatile("add %0, %1, %2"
                     : "=r"(junos_prayer_row_sum)
                     : "r"(junos_prayer_row_base), "r"(junos_prayer_side));
        junos_prayer_row_sum <<= 3;
        junos_prayer_row_sum -= junos_prayer_side;
        temp_r1_13 = junos_prayer_row_sum << 7;
    }
    junos_prayer_first_base = (u8 *)BATTLE_STATE_RAM;
    {
        register u8 *junos_prayer_first_address asm("r0");

        asm volatile("add %0, %1, %2"
                     : "=r"(junos_prayer_first_address)
                     : "r"(temp_r1_13), "r"(junos_prayer_first_base));
        if (BATTLE_UNIT_FIELD(junos_prayer_first_address, u8, pilot_id) == 1) {
            goto block_280;
        }
    }
    junos_prayer_base = junos_prayer_first_base;
    asm volatile("" : "+r"(junos_prayer_base) : "r"(junos_prayer_first_base));
loop_278:
    temp_r0_51 = junos_prayer_pilot_slot + 1;
    junos_prayer_pilot_slot = temp_r0_51;
    if ((u32) temp_r0_51 > 5U) {
        goto block_280;
    }
    {
        register u32 junos_prayer_record_offset asm("r0");
        register u32 junos_prayer_index_copy asm("r4");

        junos_prayer_record_offset = temp_r0_51 << 2;
        junos_prayer_record_offset += junos_prayer_pilot_slot;
        junos_prayer_record_offset <<= 3;
        junos_prayer_index_copy = junos_prayer_pilot_slot;
        asm volatile("" : "+r"(junos_prayer_index_copy));
        junos_prayer_record_offset -= junos_prayer_index_copy;
        junos_prayer_record_offset <<= 4;
        junos_prayer_record_offset += temp_r1_13;
        junos_prayer_record_offset += (u32)junos_prayer_base;
        if (BATTLE_UNIT_FIELD(junos_prayer_record_offset, u8, pilot_id) != 1) {
            goto loop_278;
        }
    }
block_280:
    AddBattleEffect(command_side, junos_prayer_pilot_slot, -1, 0, 0, 1, BATTLE_EFFECT_MAX_HP, 0x12C, 0, 0);
    AddBattleEffect(command_side, junos_prayer_pilot_slot, -1, 0, 0, 1, BATTLE_EFFECT_MAX_EP, 0x1E, 0, 0);
    {
        register u32 junos_prayer_state_address asm("r0") = BATTLE_STATE_RAM;
        register u32 junos_prayer_state_offset asm("r5") = BATTLE_COMBINATION_OFFSET(active_unit_slot);
        register u32 junos_prayer_state_value asm("r7");

        asm volatile("" : "+r"(junos_prayer_state_address),
                     "+r"(junos_prayer_state_offset));
        junos_prayer_state_address += junos_prayer_state_offset;
        junos_prayer_state_value = junos_prayer_pilot_slot;
        *(u8 *)junos_prayer_state_address = junos_prayer_state_value;
    }
    final_state = (s32 *)0x02030558;
    asm volatile("" : "+r"(final_state));
    next_battle_phase = 0x1320;
    goto store_battle_phase;
}
case DECK_COMMAND_TWO_ARM_LIZARD_GATTAI - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *two_arm_lizard_gattai_stack asm("sp");
    register s32 two_arm_lizard_gattai_sum asm("r1");
    u8 two_arm_lizard_gattai_inner_next;
    ResetBattleCombinationPresentation();
    {
        register u32 two_arm_lizard_gattai_initial_zero asm("r0") = 0;
        asm volatile("" : "+r"(two_arm_lizard_gattai_initial_zero));
        two_arm_lizard_direction = two_arm_lizard_gattai_initial_zero;
    }
loop_283:
    two_arm_lizard_rank_base = 0;
    {
        register s32 two_arm_lizard_gattai_successor asm("r1") = two_arm_lizard_direction;

        two_arm_lizard_gattai_successor += 1;
        two_arm_lizard_gattai_stack[24] = two_arm_lizard_gattai_successor;
    }
loop_284:
    temp_r4_10 = two_arm_lizard_rank_base + 1;
    {
        register u32 two_arm_lizard_gattai_outer_view asm("r3") = two_arm_lizard_direction;
        if ((IsBattleCombinationFormationValid(
                0, command_side, (u8)(temp_r4_10 - two_arm_lizard_gattai_outer_view))
             << 0x18) == 0) {
            goto block_288;
        }
    }
    asm volatile(
        "lsl r0, %1, #24\n\t"
        "lsr r0, r0, #24\n\t"
        "mov sl, r0\n\t"
        "lsl %0, r0, #2\n\t"
        "add %0, sl\n\t"
        "lsl %0, %0, #3\n\t"
        "sub %0, %0, r0\n\t"
        "lsl %0, %0, #4"
        : "=r"(two_arm_lizard_gattai_sum)
        : "r"(temp_r4_10)
        : "r0", "sl", "cc");
    {
        register s32 two_arm_lizard_gattai_row_base asm("r4") = command_side_times_four;
        register s32 two_arm_lizard_gattai_row_side asm("r5") = command_side;
        register s32 two_arm_lizard_gattai_row_work asm("r0");

        asm volatile("" : "+r"(two_arm_lizard_gattai_row_base),
                     "+r"(two_arm_lizard_gattai_row_side));
        two_arm_lizard_gattai_row_work =
            ((two_arm_lizard_gattai_row_base + two_arm_lizard_gattai_row_side) * 8) - two_arm_lizard_gattai_row_side;
        temp_r0_53 = two_arm_lizard_gattai_row_work << 7;
    }
    sp38 = temp_r0_53;
    two_arm_lizard_gattai_sum += temp_r0_53;
    {
        register void *two_arm_lizard_gattai_record_base asm("r0") =
            (void *)BATTLE_STATE_RAM;
        asm volatile(
            "add %0, %1, %2"
            : "=l"(two_arm_lizard_host_record)
            : "l"(two_arm_lizard_gattai_sum), "l"(two_arm_lizard_gattai_record_base)
            : "cc");
    }
    {
        register u32 two_arm_lizard_gattai_initial_hp asm("r1") =
            BATTLE_UNIT_FIELD(two_arm_lizard_host_record, u16, max_hp);

        two_arm_lizard_gattai_stack[7] = two_arm_lizard_gattai_initial_hp;
    }
    two_arm_lizard_initial_hp = BATTLE_UNIT_FIELD(two_arm_lizard_host_record, u16, hp);
    asm volatile(
        "mov r0, #1\n\t"
        "mov r3, r8\n\t"
        "sub r0, r0, r3\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r7, r0\n\t"
        "lsl r0, r0, #24\n\t"
        "lsr %0, r0, #24"
        : "=l"(temp_r6_2)
        :
        : "cc");
    if (BATTLE_UNIT_FIELD(two_arm_lizard_host_record, u8, zoid_id) == 0x77) {
        goto block_287;
    }
    {
        register s32 two_arm_lizard_gattai_row_reload asm("r0");

        two_arm_lizard_component_record = temp_r6_2 * 0x270;
        two_arm_lizard_gattai_row_reload = sp38;
        two_arm_lizard_component_record += two_arm_lizard_gattai_row_reload;
        asm volatile(
            "add %0, %0, %1"
            : "+l"(two_arm_lizard_component_record)
            : "l"((u32)BATTLE_STATE_RAM),
              "l"(two_arm_lizard_gattai_row_reload)
            : "cc");
    }
    BATTLE_UNIT_FIELD(two_arm_lizard_host_record, u8, pilot_slot_or_definition_id) = (u8) BATTLE_UNIT_FIELD(two_arm_lizard_component_record, u8, pilot_slot_or_definition_id);
    CopyBytes(two_arm_lizard_host_record + 0x70, two_arm_lizard_component_record + 0x70, 0x40);
    {
        register void *two_arm_lizard_gattai_copy_dest asm("r0");
        register void *two_arm_lizard_gattai_copy_source asm("r1");
        register u32 two_arm_lizard_gattai_copy_size asm("r2");

        asm volatile(
            "add %0, %4, #0\n\t"
            "add %0, #176\n\t"
            "add %3, #176\n\t"
            "add %1, %3, #0\n\t"
            "movs %2, #52"
            : "=l"(two_arm_lizard_gattai_copy_dest),
              "=l"(two_arm_lizard_gattai_copy_source),
              "=l"(two_arm_lizard_gattai_copy_size),
              "+l"(two_arm_lizard_component_record)
            : "l"(two_arm_lizard_host_record)
            : "cc");
        CopyBytes(two_arm_lizard_gattai_copy_dest, two_arm_lizard_gattai_copy_source,
                      two_arm_lizard_gattai_copy_size);
    }
block_287:
    {
        register u32 two_arm_lizard_gattai_index_post asm("r6") = temp_r6_2;
        register s32 two_arm_lizard_gattai_call_side asm("r0");
        register u32 two_arm_lizard_gattai_call_one asm("r1");
        register u32 two_arm_lizard_gattai_call_index asm("r2");
        register void *two_arm_lizard_gattai_post_base asm("r3");
        asm volatile(
            "ldr %0, [sp, #24]\n\t"
            "mov %1, sl\n\t"
            "add %2, %3, #0"
            : "=l"(two_arm_lizard_gattai_call_side),
              "=l"(two_arm_lizard_gattai_call_one),
              "=l"(two_arm_lizard_gattai_call_index)
            : "l"(two_arm_lizard_gattai_index_post)
            : "memory");
        ((void (*)())AbsorbBattleCombinationUnit)(two_arm_lizard_gattai_call_side, two_arm_lizard_gattai_call_one,
                                    two_arm_lizard_gattai_call_index);
        asm volatile(
            "lsl %0, %1, #2\n\t"
            "add %0, %0, %1\n\t"
            "lsl %0, %0, #3\n\t"
            "sub %0, %0, %1\n\t"
            "lsl %0, %0, #4\n\t"
            "ldr r2, [sp, #56]\n\t"
            "add %0, %0, r2"
            : "=r"(temp_r1_15)
            : "r"(two_arm_lizard_gattai_index_post)
            : "cc");
        two_arm_lizard_gattai_post_base = (void *)BATTLE_STATE_RAM;
        asm volatile(
            "add %0, %0, %1"
            : "+r"(temp_r1_15)
            : "r"(two_arm_lizard_gattai_post_base)
            : "cc");
    }
    {
        register u32 two_arm_lizard_gattai_hp_reload asm("r4");
        register u32 two_arm_lizard_gattai_hp_sum asm("r0");
        register u32 two_arm_lizard_gattai_hp_addend asm("r2");
        register void *two_arm_lizard_gattai_hp_record asm("r1") = temp_r1_15;

        asm volatile(
            "ldr %0, [sp, #28]\n\t"
            "lsl %1, %0, #16\n\t"
            "asr %1, %1, #16\n\t"
            "ldrh %2, [%3, #58]\n\t"
            "add %1, %1, %2\n\t"
            "lsl %1, %1, #16\n\t"
            "lsr %1, %1, #16\n\t"
            "str %1, [sp, #28]"
            : "=&l"(two_arm_lizard_gattai_hp_reload),
              "=&l"(two_arm_lizard_gattai_hp_sum),
              "=&l"(two_arm_lizard_gattai_hp_addend)
            : "l"(two_arm_lizard_gattai_hp_record)
            : "cc", "memory");
    }
    asm volatile(
        "mov r3, %2\n\t"
        "lsl r0, r3, #16\n\t"
        "asr r0, r0, #16\n\t"
        "ldrh r1, [%1, #6]\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #16\n\t"
        "lsr r0, r0, #16\n\t"
        "mov %0, r0"
        : "=r"(two_arm_lizard_hp_total)
        : "r"(temp_r1_15), "r"(two_arm_lizard_initial_hp)
        : "cc");
    {
        register s32 two_arm_lizard_gattai_final_arg0 asm("r0") = command_side;
        register s32 two_arm_lizard_gattai_final_arg1 asm("r1");
        register s32 two_arm_lizard_gattai_final_arg2 asm("r2");

        asm volatile("mov %0, sl" : "=r"(two_arm_lizard_gattai_final_arg1));
        two_arm_lizard_gattai_final_arg2 = 0x83;
        SetBattleUnitCombinedModelWide(two_arm_lizard_gattai_final_arg0,
                           two_arm_lizard_gattai_final_arg1,
                           two_arm_lizard_gattai_final_arg2);
    }
    {
        register s32 two_arm_lizard_gattai_factor asm("r1");
        register u32 two_arm_lizard_gattai_factor_offset asm("r4");
        register u32 two_arm_lizard_gattai_total_source asm("r2");
        register s32 two_arm_lizard_gattai_product asm("r0");
        register s32 two_arm_lizard_gattai_final_hp asm("r3");
        register s32 two_arm_lizard_gattai_divisor asm("r1");

        asm volatile(
            ".syntax unified\n\t"
            "movs %1, #58\n\t"
            "ldrsh %0, [%2, %1]\n\t"
            ".syntax divided"
            : "=l"(two_arm_lizard_gattai_factor), "=l"(two_arm_lizard_gattai_factor_offset)
            : "l"(two_arm_lizard_host_record));
        asm volatile(
            "mov %0, %1"
            : "=l"(two_arm_lizard_gattai_total_source)
            : "r"(two_arm_lizard_hp_total));
        asm volatile(
            "lsl %0, %1, #16\n\t"
            "asr %0, %0, #16\n\t"
            "mul %0, %2"
            : "=&l"(two_arm_lizard_gattai_product)
            : "l"(two_arm_lizard_gattai_total_source), "l"(two_arm_lizard_gattai_factor)
            : "cc");
        two_arm_lizard_gattai_final_hp = two_arm_lizard_gattai_stack[7];
        asm volatile(
            "lsl %0, %1, #16\n\t"
            "asr %0, %0, #16"
            : "=l"(two_arm_lizard_gattai_divisor)
            : "l"(two_arm_lizard_gattai_final_hp));
        BATTLE_UNIT_FIELD(two_arm_lizard_host_record, u16, hp) =
            DivideSigned32FullArguments(two_arm_lizard_gattai_product, two_arm_lizard_gattai_divisor);
    }
block_288:
    two_arm_lizard_gattai_inner_next = two_arm_lizard_rank_base + 3;
    two_arm_lizard_rank_base = two_arm_lizard_gattai_inner_next;
    if ((u32) two_arm_lizard_rank_base <= 5U) {
        goto loop_284;
    }
    {
        register u32 two_arm_lizard_gattai_exit_state_reload asm("r4") =
            two_arm_lizard_gattai_stack[24];
        register u32 two_arm_lizard_gattai_exit_state_normalized asm("r0");

        asm volatile(
            "lsl %0, %1, #24\n\t"
            "lsr %0, %0, #24"
            : "=r"(two_arm_lizard_gattai_exit_state_normalized)
            : "r"(two_arm_lizard_gattai_exit_state_reload));
        two_arm_lizard_direction = two_arm_lizard_gattai_exit_state_normalized;
        if (two_arm_lizard_gattai_exit_state_normalized > 1U) {
            goto block_291;
        }
    }
    goto loop_283;
block_291:
    goto start_combination_presentation;
}
case DECK_COMMAND_FUZOR_DRAGON_GATTAI - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *fuzor_dragon_gattai_stack asm("sp");
    register s32 fuzor_dragon_gattai_sum asm("r1");
    register u32 fuzor_dragon_gattai_counter asm("r7");
    register u32 fuzor_dragon_gattai_one asm("sl");
    ResetBattleCombinationPresentation();
    {
        register u32 fuzor_dragon_gattai_zero asm("r5") = 0;

        asm volatile("" : "+r"(fuzor_dragon_gattai_zero));
        fuzor_dragon_direction = fuzor_dragon_gattai_zero;
    }
    {
        register s32 fuzor_dragon_gattai_initial_row_base asm("r7") = command_side_times_four;
        register s32 fuzor_dragon_gattai_initial_row_side asm("r1") = command_side;
        register s32 fuzor_dragon_gattai_initial_row_work asm("r0");

        asm volatile("" : "+r"(fuzor_dragon_gattai_initial_row_base),
                     "+r"(fuzor_dragon_gattai_initial_row_side));
        fuzor_dragon_gattai_initial_row_work =
            ((fuzor_dragon_gattai_initial_row_base + fuzor_dragon_gattai_initial_row_side) * 8) -
            fuzor_dragon_gattai_initial_row_side;
        sp3C = fuzor_dragon_gattai_initial_row_work << 7;
    }
loop_293:
    {
        register u32 fuzor_dragon_gattai_call_index asm("r2");
        asm volatile(
            "mov %0, #1\n\t"
            "mov r3, r8\n\t"
            "sub %0, %0, r3\n\t"
            "lsl %0, %0, #24\n\t"
            "lsr %0, %0, #24"
            : "=r"(fuzor_dragon_gattai_call_index)
            : "r"(fuzor_dragon_direction)
            : "r3", "cc");
        temp_r0_55 =
            IsBattleCombinationFormationValidWide(1, command_side, fuzor_dragon_gattai_call_index) << 0x18;
    }
    {
        register s32 fuzor_dragon_gattai_successor asm("r4") = fuzor_dragon_direction;

        fuzor_dragon_gattai_successor += 1;
        asm volatile("str %0, [sp, #96]"
                     :
                     : "r"(fuzor_dragon_gattai_successor)
                     : "memory");
    }
    asm volatile("" : "=m"(sp60));
    if (temp_r0_55 != 0) {
        goto block_295;
    }
    goto block_305;
block_295:
    {
        register u32 fuzor_dragon_gattai_one_seed asm("r5") = 1;

        asm volatile("" : "+r"(fuzor_dragon_gattai_one_seed));
        fuzor_dragon_gattai_one = fuzor_dragon_gattai_one_seed;
    }
    {
        register s32 fuzor_dragon_gattai_row asm("r7") = sp3C;
        register s32 fuzor_dragon_gattai_offset asm("r1") = 0x270;
        register void *fuzor_dragon_gattai_record asm("r0");
        asm volatile("" : "+r"(fuzor_dragon_gattai_row), "+r"(fuzor_dragon_gattai_offset));
        fuzor_dragon_gattai_record = fuzor_dragon_gattai_row + fuzor_dragon_gattai_offset;
        {
            register s32 fuzor_dragon_gattai_base asm("r2") = BATTLE_STATE_RAM;
            asm volatile("" : "+r"(fuzor_dragon_gattai_record), "+r"(fuzor_dragon_gattai_base));
            temp_r0_56 = fuzor_dragon_gattai_record + fuzor_dragon_gattai_base;
        }
    }
    {
        register u32 fuzor_dragon_gattai_initial_hp asm("r3") =
            BATTLE_UNIT_FIELD(temp_r0_56, u16, max_hp);
        fuzor_dragon_gattai_stack[7] = fuzor_dragon_gattai_initial_hp;
    }
    fuzor_dragon_hp_total = BATTLE_UNIT_FIELD(temp_r0_56, u16, hp);
    fuzor_dragon_gattai_counter = 0;
    {
        register s32 fuzor_dragon_gattai_row_base asm("r4") = command_side_times_four;
        register s32 fuzor_dragon_gattai_row_side asm("r5") = command_side;
        register s32 fuzor_dragon_gattai_row_work asm("r0");
        asm volatile("" : "+r"(fuzor_dragon_gattai_row_base), "+r"(fuzor_dragon_gattai_row_side));
        fuzor_dragon_gattai_row_work =
            ((fuzor_dragon_gattai_row_base + fuzor_dragon_gattai_row_side) * 8) - fuzor_dragon_gattai_row_side;
        sp40 = fuzor_dragon_gattai_row_work;
        sp44 = fuzor_dragon_gattai_row_work << 7;
    }
loop_296:
    if (fuzor_dragon_gattai_counter != 2) {
        goto block_298;
    }
    fuzor_dragon_gattai_counter = 3;
block_298:
    temp_r1_16 = fuzor_dragon_gattai_counter + 1;
    temp_r6_3 = temp_r1_16 - fuzor_dragon_direction;
    fuzor_dragon_gattai_stack[25] = temp_r1_16;
    {
        register u32 fuzor_dragon_gattai_primary_scale asm("r3");
        asm volatile("mov %0, %1\n\tlsl %0, %0, #2"
                     : "=l"(fuzor_dragon_gattai_primary_scale)
                     : "r"(fuzor_dragon_gattai_one));
        fuzor_dragon_gattai_stack[30] = fuzor_dragon_gattai_primary_scale;
    }
    asm volatile("" : "+m"(sp78));
    if (temp_r6_3 == fuzor_dragon_gattai_one) {
        goto block_303;
    }
    {
        register u32 fuzor_dragon_gattai_primary_scale asm("r3");
        register u32 fuzor_dragon_gattai_primary_base asm("r1") = BATTLE_STATE_RAM;
        register void *fuzor_dragon_gattai_primary_work asm("r0");
        asm volatile(
            "add %0, %2, #0\n\t"
            "add %0, sl\n\t"
            "lsl %0, %0, #3\n\t"
            "mov %1, sl\n\t"
            "sub %0, %0, %1\n\t"
            "lsl %0, %0, #4"
            : "=&l"(fuzor_dragon_gattai_primary_work), "=l"(fuzor_dragon_host_record)
            : "l"(fuzor_dragon_gattai_primary_scale)
            : "cc");
        {
            register s32 fuzor_dragon_gattai_primary_row_reload asm("r5");

            asm volatile(
                "ldr %0, [sp, #64]\n\t"
                "lsl %1, %0, #7"
                : "=r"(fuzor_dragon_gattai_primary_row_reload),
                  "=r"(temp_r2_10)
                :
                : "memory");
        }
        asm volatile(
            "add %0, %0, %2\n\t"
            "add %1, %0, %3"
            : "+l"(fuzor_dragon_gattai_primary_work), "+l"(fuzor_dragon_host_record)
            : "l"(temp_r2_10), "l"(fuzor_dragon_gattai_primary_base)
            : "cc");
    }
    if (BATTLE_UNIT_FIELD(fuzor_dragon_host_record, u8, zoid_id) == 0x77) {
        goto block_302;
    }
    temp_r0_59 = (temp_r6_3 * 0x270) + temp_r2_10;
    asm volatile("add %0, %1, r1"
                 : "=&l"(fuzor_dragon_pilot_component_record)
                 : "l"(temp_r0_59));
    if (BATTLE_UNIT_FIELD(fuzor_dragon_pilot_component_record, u8, zoid_id) != 0x77) {
        goto block_302;
    }
    BATTLE_UNIT_FIELD(fuzor_dragon_host_record, u8, pilot_slot_or_definition_id) = (u8) BATTLE_UNIT_FIELD(fuzor_dragon_pilot_component_record, u8, pilot_slot_or_definition_id);
    CopyBytes(fuzor_dragon_host_record + 0x70, fuzor_dragon_pilot_component_record + 0x70, 0x40);
    CopyBytes(fuzor_dragon_host_record + 0xB0, fuzor_dragon_pilot_component_record + 0xB0, 0x34);
block_302:
    AbsorbBattleCombinationUnitWide(command_side, fuzor_dragon_gattai_one, temp_r6_3);
    temp_r0_60 = (void *)(temp_r6_3 * 0x270);
    asm volatile(
        "ldr r7, [sp, #68]\n\t"
        "add %0, %0, r7"
        : "+l"(temp_r0_60)
        :
        : "memory");
    {
        register u32 fuzor_dragon_gattai_post_base asm("r1") = BATTLE_STATE_RAM;

        asm volatile(
            "add %0, %0, %1"
            : "+l"(temp_r0_60)
            : "l"(fuzor_dragon_gattai_post_base)
            : "cc");
    }
    asm volatile(
        "ldr r2, [sp, #28]\n\t"
        "lsl r1, r2, #16\n\t"
        "asr r1, r1, #16\n\t"
        "ldrh r3, [%0, #58]\n\t"
        "add r1, r1, r3\n\t"
        "lsl r1, r1, #16\n\t"
        "lsr r1, r1, #16\n\t"
        "str r1, [sp, #28]"
        :
        : "r"(temp_r0_60)
        : "cc", "memory");
    asm volatile(
        "mov r4, %0\n\t"
        "lsl r1, r4, #16\n\t"
        "asr r1, r1, #16\n\t"
        "ldrh r0, [%1, #6]\n\t"
        "add r1, r1, r0\n\t"
        "lsl r1, r1, #16\n\t"
        "lsr r1, r1, #16\n\t"
        "mov %0, r1"
        : "+r"(fuzor_dragon_hp_total)
        : "r"(temp_r0_60)
        : "cc");
block_303:
    {
        register u32 fuzor_dragon_gattai_counter_reload asm("r5");
        register u32 fuzor_dragon_gattai_counter_normalized asm("r0");

        asm volatile(
            "ldr %0, [sp, #100]\n\t"
            "lsl %1, %0, #24\n\t"
            "lsr %2, %1, #24"
            : "=r"(fuzor_dragon_gattai_counter_reload),
              "=r"(fuzor_dragon_gattai_counter_normalized),
              "=r"(fuzor_dragon_gattai_counter)
            :
            : "memory");
    }
    if (fuzor_dragon_gattai_counter <= 4U) {
        goto loop_296;
    }
    {
        SetBattleUnitCombinedModelWide(command_side, fuzor_dragon_gattai_one, 0x80);
        asm volatile(
            "ldr %0, [sp, #120]\n\t"
            "add %0, sl\n\t"
            "lsl %0, %0, #3\n\t"
            "mov r7, sl\n\t"
            "sub %0, %0, r7\n\t"
            "lsl %0, %0, #4\n\t"
            "ldr r0, [sp, #60]\n\t"
            "add %0, %0, r0"
            : "=r"(fuzor_dragon_combined_record)
            : "r"(fuzor_dragon_gattai_one)
            : "r0", "r7", "cc", "memory");
        {
            register u32 fuzor_dragon_gattai_final_base asm("r1") =
                BATTLE_STATE_RAM;

            asm volatile(
                "add %0, %0, %1"
                : "+r"(fuzor_dragon_combined_record)
                : "r"(fuzor_dragon_gattai_final_base)
                : "cc");
        }
    }
    {
        register s32 fuzor_dragon_gattai_factor asm("r1");
        register u32 fuzor_dragon_gattai_factor_offset asm("r2");
        register u32 fuzor_dragon_gattai_total_source asm("r3");
        register s32 fuzor_dragon_gattai_product asm("r0");
        register s32 fuzor_dragon_gattai_final_hp asm("r5");
        register s32 fuzor_dragon_gattai_divisor asm("r1");

        asm volatile(
            ".syntax unified\n\t"
            "movs %1, #58\n\t"
            "ldrsh %0, [%2, %1]\n\t"
            ".syntax divided"
            : "=l"(fuzor_dragon_gattai_factor), "=l"(fuzor_dragon_gattai_factor_offset)
            : "l"(fuzor_dragon_combined_record));
        asm volatile(
            "mov %0, %1"
            : "=l"(fuzor_dragon_gattai_total_source)
            : "r"(fuzor_dragon_hp_total));
        asm volatile(
            "lsl %0, %1, #16\n\t"
            "asr %0, %0, #16\n\t"
            "mul %0, %2"
            : "=&l"(fuzor_dragon_gattai_product)
            : "l"(fuzor_dragon_gattai_total_source), "l"(fuzor_dragon_gattai_factor)
            : "cc");
        fuzor_dragon_gattai_final_hp = fuzor_dragon_gattai_stack[7];
        asm volatile(
            "lsl %0, %1, #16\n\t"
            "asr %0, %0, #16"
            : "=l"(fuzor_dragon_gattai_divisor)
            : "l"(fuzor_dragon_gattai_final_hp));
        BATTLE_UNIT_FIELD(fuzor_dragon_combined_record, u16, hp) =
            DivideSigned32FullArguments(fuzor_dragon_gattai_product, fuzor_dragon_gattai_divisor);
    }
block_305:
    {
        register u32 fuzor_dragon_gattai_exit_state_reload asm("r7");
        register u32 fuzor_dragon_gattai_exit_state_normalized asm("r0");

        asm volatile("ldr %0, [sp, #96]"
                     : "=r"(fuzor_dragon_gattai_exit_state_reload)
                     :
                     : "memory");
        asm volatile(
            "lsl %0, %1, #24\n\t"
            "lsr %0, %0, #24"
            : "=r"(fuzor_dragon_gattai_exit_state_normalized)
            : "r"(fuzor_dragon_gattai_exit_state_reload));
        fuzor_dragon_direction = fuzor_dragon_gattai_exit_state_normalized;
        if (fuzor_dragon_gattai_exit_state_normalized > 1U) {
            goto block_307;
        }
    }
    goto loop_293;
block_307:
    goto start_combination_presentation;
}
case DECK_COMMAND_CHIMERA_DRAGON_GATTAI - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *chimera_dragon_gattai_stack asm("sp");
    register s32 chimera_dragon_gattai_sum asm("r1");
    register u32 chimera_dragon_gattai_counter asm("r7");
    register u32 chimera_dragon_gattai_one asm("sl");
    register s32 chimera_dragon_gattai_row_side asm("r5");
    ResetBattleCombinationPresentation();
    {
        register s32 chimera_dragon_gattai_state_seed asm("r0") = 0;

        asm volatile("" : "+r"(chimera_dragon_gattai_state_seed));
        chimera_dragon_direction = chimera_dragon_gattai_state_seed;
    }
    {
        register s32 chimera_dragon_gattai_row_base asm("r1") = command_side_times_four;
        register s32 chimera_dragon_gattai_row_side asm("r2") = command_side;
        register s32 chimera_dragon_gattai_row asm("r0");
        asm volatile("" : "+r"(chimera_dragon_gattai_row_base), "+r"(chimera_dragon_gattai_row_side));
        chimera_dragon_gattai_row = chimera_dragon_gattai_row_base + chimera_dragon_gattai_row_side;
        chimera_dragon_gattai_row = (chimera_dragon_gattai_row * 8) - chimera_dragon_gattai_row_side;
        chimera_dragon_gattai_row <<= 7;
        sp48 = chimera_dragon_gattai_row;
    }
loop_309:
    {
        register u32 chimera_dragon_gattai_side_delta asm("r2") = 1;
        register u32 chimera_dragon_gattai_outer_view asm("r3");

        chimera_dragon_gattai_outer_view = chimera_dragon_direction;
        asm volatile("" : "+r"(chimera_dragon_gattai_outer_view));
        chimera_dragon_gattai_side_delta -= chimera_dragon_gattai_outer_view;
        temp_r0_62 =
            IsBattleCombinationFormationValid(2, command_side, (u8)chimera_dragon_gattai_side_delta) << 0x18;
    }
    {
        register s32 chimera_dragon_gattai_successor asm("r4") = chimera_dragon_direction;

        chimera_dragon_gattai_successor += 1;
        chimera_dragon_gattai_stack[24] = chimera_dragon_gattai_successor;
    }
    if (temp_r0_62 != 0) {
        goto block_311;
    }
    goto block_321;
block_311:
    {
        register u32 chimera_dragon_gattai_one_seed asm("r5") = 1;

        chimera_dragon_gattai_one = chimera_dragon_gattai_one_seed;
    }
    {
        register s32 chimera_dragon_gattai_initial_row asm("r7") = sp48;
        register s32 chimera_dragon_gattai_initial_offset asm("r1") = 0x270;
        register void *chimera_dragon_gattai_initial_record asm("r0");

        asm volatile("" : "+r"(chimera_dragon_gattai_initial_row),
                     "+r"(chimera_dragon_gattai_initial_offset));
        chimera_dragon_gattai_initial_record = chimera_dragon_gattai_initial_row + chimera_dragon_gattai_initial_offset;
        {
            register u32 chimera_dragon_gattai_initial_base asm("r2") = BATTLE_STATE_RAM;

            asm volatile("" : "+r"(chimera_dragon_gattai_initial_record),
                         "+r"(chimera_dragon_gattai_initial_base));
            chimera_dragon_gattai_initial_record =
                (void *)((u32)chimera_dragon_gattai_initial_record + chimera_dragon_gattai_initial_base);
        }
        {
            register u32 chimera_dragon_gattai_initial_hp asm("r3") =
                M2C_FIELD(chimera_dragon_gattai_initial_record, u16 *, 0x3A);

            chimera_dragon_gattai_stack[7] = chimera_dragon_gattai_initial_hp;
        }
        chimera_dragon_hp_total = M2C_FIELD(chimera_dragon_gattai_initial_record, u16 *, 6);
    }
    chimera_dragon_gattai_counter = 0;
    {
        register s32 chimera_dragon_gattai_row_base asm("r4") = command_side_times_four;
        register s32 chimera_dragon_gattai_row_work asm("r0");

        chimera_dragon_gattai_row_side = command_side;
        asm volatile("" : "+r"(chimera_dragon_gattai_row_base),
                     "+r"(chimera_dragon_gattai_row_side));
        chimera_dragon_gattai_row_work =
            ((chimera_dragon_gattai_row_base + chimera_dragon_gattai_row_side) * 8) - chimera_dragon_gattai_row_side;
        sp4C = chimera_dragon_gattai_row_work;
        sp50 = chimera_dragon_gattai_row_work << 7;
    }
loop_312:
    if (chimera_dragon_gattai_counter != 2) {
        goto block_314;
    }
    chimera_dragon_gattai_counter = 3;
block_314:
    temp_r1_17 = chimera_dragon_gattai_counter + 1;
    temp_r6_4 = temp_r1_17 - chimera_dragon_direction;
    asm volatile("" : : "r"(chimera_dragon_gattai_row_side));
    chimera_dragon_gattai_stack[25] = temp_r1_17;
    {
        register u32 chimera_dragon_gattai_primary_scale asm("r3");
        asm volatile("mov %0, sl\n\tlsl %0, %0, #2"
                     : "=l"(chimera_dragon_gattai_primary_scale));
        chimera_dragon_gattai_stack[30] = chimera_dragon_gattai_primary_scale;
    }
    if (temp_r6_4 == chimera_dragon_gattai_one) {
        goto block_319;
    }
    {
        register u32 chimera_dragon_gattai_primary_scale asm("r3");
        register u32 chimera_dragon_gattai_primary_base asm("r1") = BATTLE_STATE_RAM;
        register void *chimera_dragon_gattai_primary_work asm("r0");
        asm volatile(
            "add %0, %2, #0\n\t"
            "add %0, sl\n\t"
            "lsl %0, %0, #3\n\t"
            "mov %1, sl\n\t"
            "sub %0, %0, %1\n\t"
            "lsl %0, %0, #4"
            : "=&l"(chimera_dragon_gattai_primary_work), "=l"(chimera_dragon_host_record)
            : "l"(chimera_dragon_gattai_primary_scale)
            : "cc");
        {
            register s32 chimera_dragon_gattai_primary_row_reload asm("r5");

            asm volatile(
                "ldr %0, [sp, #76]\n\t"
                "lsl %1, %0, #7"
                : "=r"(chimera_dragon_gattai_primary_row_reload),
                  "=r"(temp_r2_11)
                :
                : "memory");
        }
        asm volatile(
            "add %0, %0, %2\n\t"
            "add %1, %0, %3"
            : "+l"(chimera_dragon_gattai_primary_work), "+l"(chimera_dragon_host_record)
            : "l"(temp_r2_11), "l"(chimera_dragon_gattai_primary_base)
            : "cc");
    }
    if (BATTLE_UNIT_FIELD(chimera_dragon_host_record, u8, zoid_id) == 0x7E) {
        goto block_318;
    }
    temp_r0_66 = (temp_r6_4 * 0x270) + temp_r2_11;
    asm volatile("add %0, %1, r1"
                 : "=&l"(chimera_dragon_pilot_component_record)
                 : "l"(temp_r0_66));
    if (BATTLE_UNIT_FIELD(chimera_dragon_pilot_component_record, u8, zoid_id) != 0x7E) {
        goto block_318;
    }
    BATTLE_UNIT_FIELD(chimera_dragon_host_record, u8, pilot_slot_or_definition_id) = (u8) BATTLE_UNIT_FIELD(chimera_dragon_pilot_component_record, u8, pilot_slot_or_definition_id);
    CopyBytes(chimera_dragon_host_record + 0x70, chimera_dragon_pilot_component_record + 0x70, 0x40);
    CopyBytes(chimera_dragon_host_record + 0xB0, chimera_dragon_pilot_component_record + 0xB0, 0x34);
block_318:
    {
        register u32 chimera_dragon_gattai_call_player asm("r0") = command_side;
        register u32 chimera_dragon_gattai_call_one asm("r1");
        asm volatile("mov %0, %1"
                     : "=l"(chimera_dragon_gattai_call_one)
                     : "r"(chimera_dragon_gattai_one));
        AbsorbBattleCombinationUnitWide(chimera_dragon_gattai_call_player, chimera_dragon_gattai_call_one, temp_r6_4);
    }
    {
        register void *chimera_dragon_gattai_secondary_record asm("r0");

        chimera_dragon_gattai_secondary_record = (void *)(temp_r6_4 * 0x270);
        {
            register s32 chimera_dragon_gattai_secondary_row asm("r7") = sp50;

            chimera_dragon_gattai_secondary_record =
                (void *)((u32)chimera_dragon_gattai_secondary_record +
                         chimera_dragon_gattai_secondary_row);
        }
        {
            register u32 chimera_dragon_gattai_secondary_base asm("r1") = BATTLE_STATE_RAM;

            asm volatile("" : "+r"(chimera_dragon_gattai_secondary_base));
            chimera_dragon_gattai_secondary_record =
                (void *)((u32)chimera_dragon_gattai_secondary_record +
                         chimera_dragon_gattai_secondary_base);
        }
        {
            register u32 chimera_dragon_gattai_hp_reload asm("r2") =
                chimera_dragon_gattai_stack[7];
            register u32 chimera_dragon_gattai_other_hp asm("r3");

            chimera_dragon_gattai_sum = (s16)chimera_dragon_gattai_hp_reload;
            chimera_dragon_gattai_other_hp =
                M2C_FIELD(chimera_dragon_gattai_secondary_record, u16 *, 0x3A);
            asm volatile("" : "+r"(chimera_dragon_gattai_other_hp));
            chimera_dragon_gattai_sum += chimera_dragon_gattai_other_hp;
            chimera_dragon_gattai_sum = (u32)chimera_dragon_gattai_sum << 16;
            chimera_dragon_gattai_sum = (u32)chimera_dragon_gattai_sum >> 16;
            chimera_dragon_gattai_stack[7] = chimera_dragon_gattai_sum;
        }
        {
            register u32 chimera_dragon_gattai_total_reload asm("r4");

            asm volatile("mov %0, %1"
                         : "=l"(chimera_dragon_gattai_total_reload)
                         : "r"(chimera_dragon_hp_total));
            chimera_dragon_gattai_sum = (s16)chimera_dragon_gattai_total_reload;
            chimera_dragon_gattai_sum +=
                M2C_FIELD(chimera_dragon_gattai_secondary_record, u16 *, 6);
            chimera_dragon_gattai_sum = (u32)chimera_dragon_gattai_sum << 16;
            chimera_dragon_gattai_sum = (u32)chimera_dragon_gattai_sum >> 16;
            chimera_dragon_hp_total = chimera_dragon_gattai_sum;
        }
    }
block_319:
    {
        register u32 chimera_dragon_gattai_counter_reload asm("r5") =
            chimera_dragon_gattai_stack[25];
        register u32 chimera_dragon_gattai_counter_shift asm("r0");

        chimera_dragon_gattai_counter_shift = chimera_dragon_gattai_counter_reload << 24;
        chimera_dragon_gattai_counter = chimera_dragon_gattai_counter_shift >> 24;
        if (chimera_dragon_gattai_counter <= 4U) {
            goto loop_312;
        }
    }
    SetBattleUnitCombinedModelWide(command_side, chimera_dragon_gattai_one, 0x81);
    {
        register u32 chimera_dragon_gattai_final_work asm("r4") =
            chimera_dragon_gattai_stack[30];
        register s32 chimera_dragon_gattai_factor asm("r1");
        register u32 chimera_dragon_gattai_factor_offset asm("r2");
        register u32 chimera_dragon_gattai_total_source asm("r3");
        register s32 chimera_dragon_gattai_product asm("r0");

        chimera_dragon_gattai_final_work += chimera_dragon_gattai_one;
        chimera_dragon_gattai_final_work <<= 3;
        {
            register u32 chimera_dragon_gattai_final_one asm("r7") =
                chimera_dragon_gattai_one;

            chimera_dragon_gattai_final_work -= chimera_dragon_gattai_final_one;
        }
        chimera_dragon_gattai_final_work <<= 4;
        {
            register u32 chimera_dragon_gattai_final_row asm("r0") = sp48;

            chimera_dragon_gattai_final_work += chimera_dragon_gattai_final_row;
        }
        {
            register u32 chimera_dragon_gattai_final_base asm("r1") = BATTLE_STATE_RAM;

            asm volatile("" : "+r"(chimera_dragon_gattai_final_base));
            chimera_dragon_gattai_final_work += chimera_dragon_gattai_final_base;
        }
        asm volatile(
            ".syntax unified\n\t"
            "movs %1, #58\n\t"
            "ldrsh %0, [%2, %1]\n\t"
            ".syntax divided"
            : "=l"(chimera_dragon_gattai_factor), "=l"(chimera_dragon_gattai_factor_offset)
            : "l"(chimera_dragon_gattai_final_work));
        asm volatile("mov %0, %1"
                     : "=l"(chimera_dragon_gattai_total_source)
                     : "r"(chimera_dragon_hp_total));
        asm volatile(
            "lsl %0, %1, #16\n\t"
            "asr %0, %0, #16\n\t"
            "mul %0, %2"
            : "=&l"(chimera_dragon_gattai_product)
            : "l"(chimera_dragon_gattai_total_source), "l"(chimera_dragon_gattai_factor)
            : "cc");
        {
            register u32 chimera_dragon_gattai_final_hp_reload asm("r5") =
                chimera_dragon_gattai_stack[7];
            register s32 chimera_dragon_gattai_divisor asm("r1");

            chimera_dragon_gattai_divisor = (s16)chimera_dragon_gattai_final_hp_reload;
            M2C_FIELD(chimera_dragon_gattai_final_work, u16 *, 6) =
                DivideSigned32(chimera_dragon_gattai_product, chimera_dragon_gattai_divisor);
        }
    }
block_321:
    {
        register u32 chimera_dragon_gattai_exit_state_reload asm("r7") =
            chimera_dragon_gattai_stack[24];
        register u32 chimera_dragon_gattai_exit_state_normalized asm("r0");

        asm volatile(
            "lsl %0, %1, #24\n\t"
            "lsr %0, %0, #24"
            : "=r"(chimera_dragon_gattai_exit_state_normalized)
            : "r"(chimera_dragon_gattai_exit_state_reload));
        chimera_dragon_direction = chimera_dragon_gattai_exit_state_normalized;
        if (chimera_dragon_gattai_exit_state_normalized > 1U) {
            goto block_323;
        }
    }
    goto loop_309;
block_323:
    goto start_combination_presentation;
}
case DECK_COMMAND_GOJULOX_GATTAI - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *gojulox_gattai_stack asm("sp");
    register s32 gojulox_gattai_sum asm("r1");
    register s32 gojulox_gattai_row asm("r8");
    ResetBattleCombinationPresentation();
    {
        register u32 gojulox_gattai_one_seed asm("r0") = 1;

        asm volatile("" : "+r"(gojulox_gattai_one_seed));
        var_sl = gojulox_gattai_one_seed;
    }
    {
        register s32 gojulox_gattai_base asm("r3");

        gojulox_gattai_base = BATTLE_STATE_RAM;
        {
            register s32 gojulox_gattai_row_input asm("r2");
            register s32 gojulox_gattai_arg asm("r4");

            gojulox_gattai_row_input = command_side_times_four;
            gojulox_gattai_arg = command_side;
            temp_r1_18 = gojulox_gattai_row_input + gojulox_gattai_arg;
            temp_r1_18 <<= 3;
            temp_r1_18 -= gojulox_gattai_arg;
        }
        temp_r2_12 = temp_r1_18 << 7;
        {
            register s32 gojulox_gattai_offset asm("r5") = 0x270;
            register void *gojulox_gattai_record asm("r0");

            asm volatile("" : "+r"(gojulox_gattai_offset));
            gojulox_gattai_record = temp_r2_12 + gojulox_gattai_offset;
            temp_r0_69 = gojulox_gattai_record + gojulox_gattai_base;
        }
    }
    sp1C = (s32) BATTLE_UNIT_FIELD(temp_r0_69, u16, max_hp);
    gojulox_gattai_stack[7] = sp1C;
    gojulox_hp_total = BATTLE_UNIT_FIELD(temp_r0_69, u16, hp);
    gojulox_component_slot = 0;
    sp54 = temp_r1_18;
    gojulox_gattai_row = temp_r2_12;
loop_326:
    gojulox_gattai_copy = gojulox_component_slot;
    {
        register u32 gojulox_gattai_primary_work asm("r0");
        register u32 gojulox_gattai_primary_base asm("r1");
        register u32 gojulox_gattai_primary_one asm("r2");
        register u32 gojulox_gattai_primary_row asm("r3");
        register u32 gojulox_gattai_secondary_work asm("r0");
        register u8 *gojulox_gattai_secondary_record asm("r5");

        gojulox_gattai_primary_work = var_sl;
        asm volatile("" : "+r"(gojulox_gattai_primary_work));
        gojulox_gattai_primary_work <<= 2;
        gojulox_gattai_stack[30] = gojulox_gattai_primary_work;
        if (gojulox_component_slot == var_sl) {
            goto block_331;
        }
        gojulox_gattai_primary_base = BATTLE_STATE_RAM;
        gojulox_gattai_primary_work += var_sl;
        gojulox_gattai_primary_work <<= 3;
        gojulox_gattai_primary_one = var_sl;
        asm volatile("" : "+r"(gojulox_gattai_primary_one));
        gojulox_gattai_primary_work -= gojulox_gattai_primary_one;
        gojulox_gattai_primary_work <<= 4;
        gojulox_gattai_primary_row = sp54;
        temp_r2_13 = gojulox_gattai_primary_row << 7;
        gojulox_gattai_primary_work += temp_r2_13;
        gojulox_host_record = (void *)(gojulox_gattai_primary_work + gojulox_gattai_primary_base);
        if (BATTLE_UNIT_FIELD(gojulox_host_record, u8, zoid_id) == 0x77) {
            goto block_330;
        }
        gojulox_gattai_secondary_work = gojulox_component_slot << 2;
        gojulox_gattai_secondary_work += gojulox_component_slot;
        gojulox_gattai_secondary_work <<= 3;
        gojulox_gattai_secondary_work -= gojulox_component_slot;
        gojulox_gattai_secondary_work <<= 4;
        gojulox_gattai_secondary_work += temp_r2_13;
        gojulox_gattai_secondary_record =
            (u8 *)(gojulox_gattai_secondary_work + gojulox_gattai_primary_base);
        if (M2C_FIELD(gojulox_gattai_secondary_record, u8 *, 0) != 0x77) {
            goto block_330;
        }
        BATTLE_UNIT_FIELD(gojulox_host_record, u8, pilot_slot_or_definition_id) =
            (u8) M2C_FIELD(gojulox_gattai_secondary_record, u8 *, 2);
        CopyBytes(gojulox_host_record + 0x70,
                     gojulox_gattai_secondary_record + 0x70, 0x40);
        CopyBytes(gojulox_host_record + 0xB0,
                     gojulox_gattai_secondary_record + 0xB0, 0x34);
    }
block_330:
    AbsorbBattleCombinationUnitWide(command_side, var_sl, gojulox_gattai_copy);
    {
        register u32 gojulox_gattai_post_work asm("r0");
        register u32 gojulox_gattai_post_base asm("r4");
        register s32 gojulox_gattai_hp_reload asm("r5");

        gojulox_gattai_post_work = gojulox_gattai_copy << 2;
        gojulox_gattai_post_work += gojulox_gattai_copy;
        gojulox_gattai_post_work <<= 3;
        gojulox_gattai_post_work -= gojulox_gattai_copy;
        gojulox_gattai_post_work <<= 4;
        gojulox_gattai_post_work += gojulox_gattai_row;
        gojulox_gattai_post_base = BATTLE_STATE_RAM;
        asm volatile("" : "+r"(gojulox_gattai_post_base));
        gojulox_gattai_post_work += gojulox_gattai_post_base;
        asm volatile(
            "ldr r5, [sp, #28]\n\t"
            "lsl r1, r5, #16\n\t"
            "asr r1, r1, #16\n\t"
            "ldrh r2, [%1, #58]\n\t"
            "add r1, r1, r2\n\t"
            "lsl r1, r1, #16\n\t"
            "lsr r1, r1, #16\n\t"
            "str r1, [sp, #28]\n\t"
            "mov r3, %0\n\t"
            "lsl r1, r3, #16\n\t"
            "asr r1, r1, #16\n\t"
            "ldrh %1, [%1, #6]\n\t"
            "add r1, r1, %1\n\t"
            "lsl r1, r1, #16\n\t"
            "lsr r1, r1, #16\n\t"
            "mov %0, r1"
            : "+r"(gojulox_hp_total), "+r"(gojulox_gattai_post_work)
            :
            : "r1", "r2", "r3", "r5", "cc", "memory");
    }
block_331:
    gojulox_component_slot += 1;
    if ((u32) gojulox_component_slot <= 5U) {
        goto loop_326;
    }
    SetBattleUnitCombinedModelWide(command_side, var_sl, 0x82);
    {
        register u32 gojulox_gattai_final_work asm("r4");
        register s32 gojulox_gattai_product asm("r0");

        {
            register u32 gojulox_gattai_final_base asm("r1");
            register u32 gojulox_gattai_final_one asm("r5");
            register u32 gojulox_gattai_final_row_saved asm("r7");
            register u32 gojulox_gattai_final_arg asm("r2");
            register u32 gojulox_gattai_final_row_work asm("r0");

            gojulox_gattai_final_base = BATTLE_STATE_RAM;
            gojulox_gattai_final_work = gojulox_gattai_stack[30];
            gojulox_gattai_final_work += var_sl;
            gojulox_gattai_final_work <<= 3;
            gojulox_gattai_final_one = var_sl;
            asm volatile("" : "+r"(gojulox_gattai_final_one));
            gojulox_gattai_final_work -= gojulox_gattai_final_one;
            gojulox_gattai_final_work <<= 4;
            gojulox_gattai_final_row_saved = command_side_times_four;
            gojulox_gattai_final_arg = command_side;
            gojulox_gattai_final_row_work =
                gojulox_gattai_final_row_saved + gojulox_gattai_final_arg;
            gojulox_gattai_final_row_work <<= 3;
            gojulox_gattai_final_row_work -= gojulox_gattai_final_arg;
            gojulox_gattai_final_row_work <<= 7;
            gojulox_gattai_final_work += gojulox_gattai_final_row_work;
            gojulox_gattai_final_work += gojulox_gattai_final_base;
        }
        {
            register s32 gojulox_gattai_factor asm("r1");
            register u32 gojulox_gattai_factor_offset asm("r3");
            register u32 gojulox_gattai_total_source asm("r5");

            asm volatile(
                ".syntax unified\n\t"
                "movs %1, #58\n\t"
                "ldrsh %0, [%2, %1]\n\t"
                ".syntax divided"
                : "=l"(gojulox_gattai_factor), "=l"(gojulox_gattai_factor_offset)
                : "l"(gojulox_gattai_final_work));
            asm volatile("mov %0, %1"
                         : "=l"(gojulox_gattai_total_source)
                         : "r"(gojulox_hp_total));
            asm volatile(
                "lsl %0, %1, #16\n\t"
                "asr %0, %0, #16\n\t"
                "mul %0, %2"
                : "=&l"(gojulox_gattai_product)
                : "l"(gojulox_gattai_total_source), "l"(gojulox_gattai_factor)
                : "cc");
        }
        {
            register u32 gojulox_gattai_final_hp_reload asm("r7") =
                gojulox_gattai_stack[7];
            register s32 gojulox_gattai_divisor asm("r1");

            asm volatile(
                "lsl %0, %1, #16\n\t"
                "asr %0, %0, #16"
                : "=l"(gojulox_gattai_divisor)
                : "l"(gojulox_gattai_final_hp_reload));
            M2C_FIELD(gojulox_gattai_final_work, u16 *, 6) =
                DivideSigned32FullArguments(gojulox_gattai_product, gojulox_gattai_divisor);
        }
    }
    goto start_combination_presentation;
}
case DECK_COMMAND_GRIFFIN_GATTAI - DECK_COMMAND_FRIENDSHIP: {
    u32 griffin_gattai_index4;
    u32 griffin_gattai_kind;
    s32 griffin_gattai_sum;
    register volatile s32 *griffin_gattai_stack asm("sp");
    register u32 griffin_gattai_one asm("sl");
    register u32 griffin_gattai_post_hp_field asm("r5");
    ResetBattleCombinationPresentation();
    {
        register u32 griffin_gattai_initial_five asm("r0") = 5;

        asm volatile("" : "+r"(griffin_gattai_initial_five));
        griffin_component_slot = griffin_gattai_initial_five;
    }
    {
        register s32 griffin_gattai_row_base asm("r1") = command_side_times_four;
        register s32 griffin_gattai_row_side asm("r2") = command_side;
        register s32 griffin_gattai_row_work asm("r0");

        griffin_gattai_row_work = griffin_gattai_row_base + griffin_gattai_row_side;
        griffin_gattai_row_work <<= 3;
        griffin_gattai_row_work -= griffin_gattai_row_side;
        temp_r6_5 = griffin_gattai_row_work << 7;
    }
loop_334:
    temp_r0_73 = IsBattleCombinationFormationValidWide(6, 0, griffin_component_slot) << 0x18;
    temp_r3_3 = griffin_component_slot - 1;
    griffin_gattai_stack[23] = temp_r3_3;
    asm volatile(
        ".macro bne target\n\t"
        "beq 991f\n\t"
        ".endm\n\t"
        ".macro b target\n\t"
        ".endm");
    if (temp_r0_73 == 0) {
        goto block_342;
    }
    asm volatile(".purgem bne\n\t.purgem b");
    {
        register u32 griffin_gattai_one_seed asm("r4");
        register s32 griffin_gattai_offset asm("r5");
        register u32 griffin_gattai_base asm("r7");
        register void *griffin_gattai_record asm("r0");
        register s32 griffin_gattai_initial_hp asm("r1");
        register u32 griffin_gattai_initial_index4 asm("r1");
        register u32 griffin_gattai_current asm("r2");
        register u32 griffin_gattai_test_address asm("r0");

        asm volatile("mov %0, #1" : "=r"(griffin_gattai_one_seed));
        griffin_gattai_one = griffin_gattai_one_seed;
        griffin_gattai_offset = 0x270;
        griffin_gattai_record = temp_r6_5 + griffin_gattai_offset;
        griffin_gattai_base = BATTLE_STATE_RAM;
        temp_r0_74 = griffin_gattai_record + griffin_gattai_base;
        griffin_gattai_initial_hp = BATTLE_UNIT_FIELD(temp_r0_74, u16, max_hp);
        griffin_gattai_stack[7] = griffin_gattai_initial_hp;
        griffin_hp_total = BATTLE_UNIT_FIELD(temp_r0_74, u16, hp);
        griffin_gattai_current = griffin_component_slot;
        griffin_gattai_initial_index4 = griffin_gattai_current << 2;
        griffin_gattai_test_address = griffin_gattai_initial_index4 + griffin_gattai_current;
        griffin_gattai_test_address <<= 3;
        griffin_gattai_test_address -= griffin_gattai_current;
        griffin_gattai_test_address <<= 4;
        griffin_gattai_test_address += temp_r6_5;
        griffin_gattai_test_address += griffin_gattai_base;
        griffin_gattai_kind = (u8)(
            M2C_FIELD(griffin_gattai_test_address, u8 *, 0) - 0x77);
        griffin_gattai_index4 = griffin_gattai_initial_index4;
    }
    if (griffin_gattai_kind > 1U) {
        goto block_341;
    }
    {
    register u32 griffin_gattai_inner_index asm("r7");
    register u32 griffin_gattai_inner_normalized asm("r0");

    asm volatile(
        "lsl %0, %2, #24\n\t"
        "lsr %1, %0, #24"
        : "=r"(griffin_gattai_inner_normalized),
          "=r"(griffin_gattai_inner_index)
        : "r"(temp_r3_3));

    if (griffin_gattai_inner_index <= 2U) {
        goto block_341;
    }
    griffin_battle_state_address = BATTLE_STATE_RAM;
loop_338:
    {
        register u32 griffin_gattai_address_work asm("r0");

        asm volatile(
            "lsl %0, %2, #2\n\t"
            "add %0, %0, %2\n\t"
            "lsl %0, %0, #3\n\t"
            "sub %0, %0, %2\n\t"
            "lsl %0, %0, #4\n\t"
            "add %0, %0, %3\n\t"
            "add %1, %0, %4"
            : "=&r"(griffin_gattai_address_work), "=r"(griffin_component_record)
            : "r"(griffin_gattai_inner_index), "r"(temp_r6_5), "r"(griffin_battle_state_address));
    }
    if ((u32) (u8) (BATTLE_UNIT_FIELD(griffin_component_record, u8, zoid_id) - 0x77) > 1U) {
        goto block_340;
    }
    {
        register s32 griffin_gattai_call_side asm("r0") = command_side;
        register u32 griffin_gattai_call_one asm("r1") = griffin_gattai_one;
        register u32 griffin_gattai_call_index asm("r2") = griffin_gattai_inner_index;

        physical_stack[31] = griffin_battle_state_address;
        AbsorbBattleCombinationUnitWide(griffin_gattai_call_side, griffin_gattai_call_one,
                           griffin_gattai_call_index);
    }
    {
        asm volatile(
            "ldr r1, [sp, #28]\n\t"
            "lsl r0, r1, #16\n\t"
            "asr r0, r0, #16\n\t"
            "ldrh r2, [%0, #58]\n\t"
            "add r0, r0, r2\n\t"
            "lsl r0, r0, #16\n\t"
            "lsr r0, r0, #16\n\t"
            "str r0, [sp, #28]"
            :
            : "r"(griffin_component_record)
            : "r0", "r1", "r2", "cc", "memory");
    }
    griffin_gattai_sum = (s16)griffin_hp_total;
    griffin_gattai_sum += BATTLE_UNIT_FIELD(griffin_component_record, u16, hp);
    griffin_gattai_sum = (u32)griffin_gattai_sum << 16;
    griffin_gattai_sum = (u32)griffin_gattai_sum >> 16;
    griffin_hp_total = griffin_gattai_sum;
    {
        register s32 griffin_gattai_base_reload asm("r3") = physical_stack[31];

        griffin_battle_state_address = griffin_gattai_base_reload;
    }
block_340:
    griffin_gattai_inner_index = (u8)(griffin_gattai_inner_index - 1);
    if (griffin_gattai_inner_index > 2U) {
        goto loop_338;
    }
    }
block_341:
    AbsorbBattleCombinationUnitWide(command_side, griffin_gattai_one, griffin_component_slot);
    {
        register u32 griffin_gattai_post_base asm("r3");

        temp_r1_19 =
            (void *)(((((griffin_gattai_index4 + griffin_component_slot) * 8)
                        - griffin_component_slot) * 0x10)
                      + temp_r6_5);
        griffin_gattai_post_base = BATTLE_STATE_RAM;
        asm volatile("" : "+r"(griffin_gattai_post_base));
        temp_r1_19 =
            (void *)((u32)temp_r1_19 + griffin_gattai_post_base);
    }
    asm volatile(
        "ldr r4, [sp, #28]\n\t"
        "lsl r0, r4, #16\n\t"
        "asr r0, r0, #16\n\t"
        "ldrh %2, [%1, #58]\n\t"
        "add r0, r0, %2\n\t"
        "lsl r0, r0, #16\n\t"
        "lsr r0, r0, #16\n\t"
        "str r0, [sp, #28]\n\t"
        "mov r7, %0\n\t"
        "lsl r0, r7, #16\n\t"
        "asr r0, r0, #16\n\t"
        "ldrh %1, [%1, #6]\n\t"
        "add r0, r0, %1\n\t"
        "lsl r0, r0, #16\n\t"
        "lsr r0, r0, #16\n\t"
        "mov %0, r0"
        : "+r"(griffin_hp_total), "+r"(temp_r1_19),
          "=r"(griffin_gattai_post_hp_field)
        :
        : "r0", "r4", "r7", "cc", "memory");
    SetBattleUnitCombinedModelWide(command_side, griffin_gattai_one, 0x84);
    asm volatile(
        "mov r0, sl\n\t"
        "lsl %0, r0, #2\n\t"
        "add %0, sl\n\t"
        "lsl %0, %0, #3\n\t"
        "sub %0, %0, r0\n\t"
        "lsl %0, %0, #4\n\t"
        "add %0, %0, r6"
        : "=r"(griffin_combined_record)
        : "r"(griffin_gattai_one), "r"(temp_r6_5)
        : "r0", "cc");
    {
        register u32 griffin_gattai_final_base asm("r1") = BATTLE_STATE_RAM;

        asm volatile(
            "add %0, %0, %1"
            : "+r"(griffin_combined_record)
            : "r"(griffin_gattai_final_base)
            : "cc");
    }
    {
        register s32 griffin_gattai_factor asm("r1");
        register u32 griffin_gattai_total_source asm("r3");
        register s32 griffin_gattai_product asm("r0");
        register s32 griffin_gattai_final_hp asm("r5");
        register s32 griffin_gattai_divisor asm("r1");

        asm volatile(
            ".syntax unified\n\t"
            "ldrsh %0, [%3, %4]\n\t"
            ".syntax divided\n\t"
            "mov %1, %5\n\t"
            "lsl %2, %1, #16\n\t"
            "asr %2, %2, #16\n\t"
            "mul %2, %0"
            : "=l"(griffin_gattai_factor), "=l"(griffin_gattai_total_source),
              "=&l"(griffin_gattai_product)
            : "l"(griffin_combined_record), "l"((u32)58), "r"(griffin_hp_total),
              "r"(griffin_gattai_post_hp_field)
            : "cc");
        griffin_gattai_final_hp = griffin_gattai_stack[7];
        asm volatile(
            "lsl %0, %1, #16\n\t"
            "asr %0, %0, #16"
            : "=l"(griffin_gattai_divisor)
            : "l"(griffin_gattai_final_hp));
        BATTLE_UNIT_FIELD(griffin_combined_record, u16, hp) =
            DivideSigned32FullArguments(griffin_gattai_product, griffin_gattai_divisor);
    }
block_342:
    asm volatile("991:");
    {
        register u32 griffin_gattai_exit_reload asm("r7");
        register u32 griffin_gattai_exit_normalized asm("r0");

        asm volatile(
            "ldr %0, [sp, #92]\n\t"
            "lsl %1, %0, #24\n\t"
            "lsr %1, %1, #24"
            : "=r"(griffin_gattai_exit_reload),
              "=r"(griffin_gattai_exit_normalized)
            :
            : "memory");
        griffin_component_slot = griffin_gattai_exit_normalized;
        if (griffin_gattai_exit_normalized <= 2U) {
            goto block_344;
        }
    }
    goto loop_334;
block_344:
    goto start_combination_presentation;
}
case DECK_COMMAND_AIRRAID - DECK_COMMAND_FRIENDSHIP: {
    register s32 airraid_zero asm("r5");
    {
        register s32 airraid_base asm("r0") = BATTLE_STATE_RAM;
        register s32 airraid_offset asm("r2") = BATTLE_COMBINATION_OFFSET(active_unit_slot);
        register u8 *airraid_state asm("r1");
        asm volatile("" : "+r"(airraid_base), "+r"(airraid_offset));
        airraid_state = (u8 *)(airraid_base + airraid_offset);
        airraid_zero = 0;
        *airraid_state = 7U;
    }
    {
        register s32 airraid_work asm("r0") = 0x94;
        register s32 airraid_side asm("r3") = command_side;
        register u8 *airraid_record asm("r4") = (u8 *)airraid_side;
        register s32 airraid_base asm("r7");
        asm volatile("" : "+r"(airraid_work), "+r"(airraid_side),
            "+r"(airraid_record));
        airraid_record = (u8 *)((u32)airraid_record * airraid_work);
        airraid_base = BATTLE_STATE_RAM;
        airraid_record += airraid_base;
        airraid_work = BATTLE_DECK_PREPARATION_OFFSET(payloads);
        airraid_record += airraid_work;
        RemoveBattleUnitFromTurnOrder(airraid_side, *airraid_record);
        AddBattleEffect(command_side, *airraid_record, -1,
        0, airraid_zero, airraid_zero, BATTLE_EFFECT_TURN_MARKER, airraid_zero, 1, airraid_zero);
    }
    {
        register u32 *airraid_state_slot asm("r1") = (u32 *)0x02030558;
        register u32 airraid_state_value asm("r0") = 0x1300;
        *airraid_state_slot = airraid_state_value;
    }
    return;
}
case DECK_COMMAND_KILLER_DOME_GATTAI - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *killer_dome_gattai_stack asm("sp");
    register s32 killer_dome_gattai_base asm("r7");
    register s32 killer_dome_gattai_sum asm("r0");
    ResetBattleCombinationPresentation();
    killer_dome_front_slot = 0;
    killer_dome_gattai_base = BATTLE_STATE_RAM;
loop_348:
    if ((IsBattleCombinationFormationValidWide(4, command_side, killer_dome_front_slot) << 0x18) == 0) {
        goto block_350;
    }
    killer_dome_host_record = killer_dome_front_slot * 0x270;
    {
        register s32 killer_dome_gattai_row_base asm("r3") = command_side_times_four;
        register s32 killer_dome_gattai_side asm("r0") = command_side;
        asm volatile("" : "+r"(killer_dome_gattai_row_base), "+r"(killer_dome_gattai_side));
        asm volatile(
            "add %0, %1, %2\n\t"
            "lsl %0, %0, #3\n\t"
            "sub %0, %0, %2\n\t"
            "lsl %0, %0, #7"
            : "=r"(temp_r4_21)
            : "r"(killer_dome_gattai_row_base), "r"(killer_dome_gattai_side)
            : "cc");
    }
    asm volatile(
        "add %0, %0, %1\n\t"
        "add %0, %0, %2"
        : "+r"(killer_dome_host_record)
        : "r"(temp_r4_21), "r"(killer_dome_gattai_base)
        : "cc");
    {
        register s32 killer_dome_gattai_initial_hp asm("r1") =
            BATTLE_UNIT_FIELD(killer_dome_host_record, u16, max_hp);
        register s32 killer_dome_gattai_initial_power asm("r2");
        killer_dome_gattai_stack[7] = killer_dome_gattai_initial_hp;
        killer_dome_gattai_initial_power = BATTLE_UNIT_FIELD(killer_dome_host_record, u16, hp);
        killer_dome_initial_hp = killer_dome_gattai_initial_power;
    }
    {
        register u32 killer_dome_gattai_successor asm("r0") = killer_dome_front_slot;
        killer_dome_gattai_successor += 3;
        asm volatile(
            "lsl %1, %1, #24\n\t"
            "lsr %0, %1, #24"
            : "=l"(temp_r6_6), "+r"(killer_dome_gattai_successor));
    }
    AbsorbBattleCombinationUnitWide(command_side, killer_dome_front_slot, temp_r6_6);
    killer_dome_component_record = (temp_r6_6 * 0x270) + temp_r4_21 + killer_dome_gattai_base;
    asm volatile(
        "ldr r3, [sp, #28]\n\t"
        "lsl r0, r3, #16\n\t"
        "asr r0, r0, #16\n\t"
        "ldrh r4, [%0, #58]\n\t"
        "add r0, r0, r4\n\t"
        "lsl r0, r0, #16\n\t"
        "lsr r0, r0, #16\n\t"
        "str r0, [sp, #28]"
        :
        : "r"(killer_dome_component_record)
        : "cc", "memory");
    killer_dome_gattai_sum = (s16)killer_dome_initial_hp;
    killer_dome_gattai_sum += BATTLE_UNIT_FIELD(killer_dome_component_record, u16, hp);
    killer_dome_gattai_sum = (u32)killer_dome_gattai_sum << 16;
    killer_dome_gattai_sum = (u32)killer_dome_gattai_sum >> 16;
    killer_dome_hp_total = killer_dome_gattai_sum;
    SetBattleUnitCombinedModelWide(command_side, killer_dome_front_slot, 0x47);
    {
        register s32 killer_dome_gattai_factor asm("r1");
        register u32 killer_dome_gattai_factor_offset asm("r3");
        register u32 killer_dome_gattai_total_source asm("r4");
        register s32 killer_dome_gattai_product asm("r0");
        register s32 killer_dome_gattai_final_hp asm("r2");
        register s32 killer_dome_gattai_divisor asm("r1");

        asm volatile(
            ".syntax unified\n\t"
            "movs %1, #58\n\t"
            "ldrsh %0, [%2, %1]\n\t"
            ".syntax divided"
            : "=l"(killer_dome_gattai_factor), "=l"(killer_dome_gattai_factor_offset)
            : "l"(killer_dome_host_record));
        asm volatile(
            "mov %0, %1"
            : "=l"(killer_dome_gattai_total_source)
            : "r"(killer_dome_hp_total));
        asm volatile(
            "lsl %0, %1, #16\n\t"
            "asr %0, %0, #16\n\t"
            "mul %0, %2"
            : "=&l"(killer_dome_gattai_product)
            : "l"(killer_dome_gattai_total_source), "l"(killer_dome_gattai_factor)
            : "cc");
        killer_dome_gattai_final_hp = killer_dome_gattai_stack[7];
        asm volatile(
            "lsl %0, %1, #16\n\t"
            "asr %0, %0, #16"
            : "=l"(killer_dome_gattai_divisor)
            : "l"(killer_dome_gattai_final_hp));
        BATTLE_UNIT_FIELD(killer_dome_host_record, u16, hp) =
            DivideSigned32FullArguments(killer_dome_gattai_product, killer_dome_gattai_divisor);
    }
block_350:
    temp_r0_76 = killer_dome_front_slot + 1;
    killer_dome_front_slot = temp_r0_76;
    if ((u32) temp_r0_76 <= 2U) {
        goto loop_348;
    }
    goto start_combination_presentation;
}
case DECK_COMMAND_ARROW_PHALANX - DECK_COMMAND_FRIENDSHIP:
    RemoveBattleUnitFromTurnOrder(command_side, 3U);
    {
        register volatile s32 *arrow_phalanx_outgoing asm("sp");
        register s32 arrow_phalanx_neg_one asm("r5") = -1;
        register s32 arrow_phalanx_zero asm("r4") = 0;
        register s32 arrow_phalanx_arg6_seed asm("r3");
        register s32 arrow_phalanx_arg6_saved asm("r8");
        register s32 arrow_phalanx_arg6_second asm("r7");
        register s32 arrow_phalanx_one asm("r6");

        arrow_phalanx_outgoing[0] = arrow_phalanx_zero;
        arrow_phalanx_outgoing[1] = arrow_phalanx_zero;
        arrow_phalanx_arg6_seed = 0x1A;
        asm volatile("mov %0, %1"
                     : "=r"(arrow_phalanx_arg6_saved)
                     : "r"(arrow_phalanx_arg6_seed));
        arrow_phalanx_outgoing[2] = arrow_phalanx_arg6_seed;
        arrow_phalanx_outgoing[3] = arrow_phalanx_zero;
        arrow_phalanx_one = 1;
        arrow_phalanx_outgoing[4] = arrow_phalanx_one;
        arrow_phalanx_outgoing[5] = arrow_phalanx_zero;
        {
            register s32 arrow_phalanx_arg0 asm("r0") = command_side;
            register s32 arrow_phalanx_arg1 asm("r1") = 3;
            register s32 arrow_phalanx_arg2 asm("r2") = arrow_phalanx_neg_one;
            register s32 arrow_phalanx_arg3 asm("r3") = 0;
            AddBattleEffect(arrow_phalanx_arg0, arrow_phalanx_arg1,
                          arrow_phalanx_arg2, arrow_phalanx_arg3);
        }
        RemoveBattleUnitFromTurnOrder(command_side, 5U);
        arrow_phalanx_outgoing[0] = arrow_phalanx_zero;
        arrow_phalanx_outgoing[1] = arrow_phalanx_zero;
        asm volatile("mov %0, r8"
                     : "=r"(arrow_phalanx_arg6_second)
                     : "r"(arrow_phalanx_arg6_saved));
        arrow_phalanx_outgoing[2] = arrow_phalanx_arg6_second;
        arrow_phalanx_outgoing[3] = arrow_phalanx_zero;
        arrow_phalanx_outgoing[4] = arrow_phalanx_one;
        arrow_phalanx_outgoing[5] = arrow_phalanx_zero;
        {
            register s32 arrow_phalanx_arg0 asm("r0") = command_side;
            register s32 arrow_phalanx_arg1 asm("r1") = 5;
            register s32 arrow_phalanx_arg2 asm("r2") = arrow_phalanx_neg_one;
            register s32 arrow_phalanx_arg3 asm("r3") = 0;
            AddBattleEffect(arrow_phalanx_arg0, arrow_phalanx_arg1,
                          arrow_phalanx_arg2, arrow_phalanx_arg3);
        }
        arrow_phalanx_outgoing[0] = arrow_phalanx_zero;
        arrow_phalanx_outgoing[1] = arrow_phalanx_zero;
        arrow_phalanx_outgoing[2] = BATTLE_EFFECT_PRESENTATION_GREEN_STREAK;
        arrow_phalanx_outgoing[3] = arrow_phalanx_zero;
        arrow_phalanx_outgoing[4] = arrow_phalanx_zero;
        arrow_phalanx_outgoing[5] = arrow_phalanx_zero;
        {
            register s32 arrow_phalanx_arg0 asm("r0") = command_side;
            register s32 arrow_phalanx_arg1 asm("r1") = 1;
            register s32 arrow_phalanx_arg2 asm("r2") = arrow_phalanx_neg_one;
            register s32 arrow_phalanx_arg3 asm("r3") = 0;
            QueueBattleEffectDisplay(arrow_phalanx_arg0, arrow_phalanx_arg1,
                          arrow_phalanx_arg2, arrow_phalanx_arg3);
        }
    }
    {
        register u32 arrow_phalanx_base asm("r0");
        register u32 arrow_phalanx_dest_offset asm("r2");
        register s32 arrow_phalanx_dest_index asm("r3");
        register s32 arrow_phalanx_source_index asm("r4");
        register u32 arrow_phalanx_source_offset asm("r5");

        arrow_phalanx_base = BATTLE_STATE_RAM;
        arrow_phalanx_dest_offset = BATTLE_DECK_REWARD_OFFSET(persistent_command_ids);
        asm volatile("" : "+r"(arrow_phalanx_dest_offset));
        persistent_command_destination = (u8 *)(arrow_phalanx_base + arrow_phalanx_dest_offset);
        arrow_phalanx_dest_index = command_side;
        persistent_command_destination = (u8 *)(
            (u32)arrow_phalanx_dest_index - (0U - (u32)persistent_command_destination));
        arrow_phalanx_source_index = command_side_times_four;
        arrow_phalanx_base =
            (u32)arrow_phalanx_source_index - (0U - arrow_phalanx_base);
        arrow_phalanx_source_offset = BATTLE_DECK_PREPARATION_OFFSET(actions[0].command_id);
        persistent_command_source = (u8 *)(arrow_phalanx_base + arrow_phalanx_source_offset);
    }
    goto store_persistent_command;
case DECK_COMMAND_T_H_PHALANX - DECK_COMMAND_FRIENDSHIP:
    RemoveBattleUnitFromTurnOrder(command_side, 0U);
    {
        register volatile s32 *t_h_phalanx_outgoing asm("sp");
        register s32 t_h_phalanx_neg_one asm("r5") = -1;
        register s32 t_h_phalanx_zero asm("r4") = 0;
        register s32 t_h_phalanx_arg6_seed asm("r7");
        register s32 t_h_phalanx_arg6_saved asm("r8");
        register s32 t_h_phalanx_arg6_first asm("r0");
        register s32 t_h_phalanx_arg6_second asm("r1");
        register s32 t_h_phalanx_one asm("r6");

        t_h_phalanx_outgoing[0] = t_h_phalanx_zero;
        t_h_phalanx_outgoing[1] = t_h_phalanx_zero;
        t_h_phalanx_arg6_seed = 0x1A;
        asm volatile("mov %0, %1"
                     : "=r"(t_h_phalanx_arg6_saved)
                     : "r"(t_h_phalanx_arg6_seed));
        asm volatile("mov %0, r8"
                     : "=r"(t_h_phalanx_arg6_first)
                     : "r"(t_h_phalanx_arg6_saved));
        t_h_phalanx_outgoing[2] = t_h_phalanx_arg6_first;
        t_h_phalanx_outgoing[3] = t_h_phalanx_zero;
        t_h_phalanx_one = 1;
        t_h_phalanx_outgoing[4] = t_h_phalanx_one;
        t_h_phalanx_outgoing[5] = t_h_phalanx_zero;
        {
            register s32 t_h_phalanx_arg0 asm("r0") = command_side;
            register s32 t_h_phalanx_arg1 asm("r1") = 0;
            register s32 t_h_phalanx_arg2 asm("r2") = t_h_phalanx_neg_one;
            register s32 t_h_phalanx_arg3 asm("r3") = 0;
            AddBattleEffect(t_h_phalanx_arg0, t_h_phalanx_arg1,
                          t_h_phalanx_arg2, t_h_phalanx_arg3);
        }
        RemoveBattleUnitFromTurnOrder(command_side, 2U);
        t_h_phalanx_outgoing[0] = t_h_phalanx_zero;
        t_h_phalanx_outgoing[1] = t_h_phalanx_zero;
        asm volatile("mov %0, r8"
                     : "=r"(t_h_phalanx_arg6_second)
                     : "r"(t_h_phalanx_arg6_saved));
        t_h_phalanx_outgoing[2] = t_h_phalanx_arg6_second;
        t_h_phalanx_outgoing[3] = t_h_phalanx_zero;
        t_h_phalanx_outgoing[4] = t_h_phalanx_one;
        t_h_phalanx_outgoing[5] = t_h_phalanx_zero;
        {
            register s32 t_h_phalanx_arg0 asm("r0") = command_side;
            register s32 t_h_phalanx_arg1 asm("r1") = 2;
            register s32 t_h_phalanx_arg2 asm("r2") = t_h_phalanx_neg_one;
            register s32 t_h_phalanx_arg3 asm("r3") = 0;
            AddBattleEffect(t_h_phalanx_arg0, t_h_phalanx_arg1,
                          t_h_phalanx_arg2, t_h_phalanx_arg3);
        }
        t_h_phalanx_outgoing[0] = t_h_phalanx_zero;
        t_h_phalanx_outgoing[1] = t_h_phalanx_zero;
        t_h_phalanx_outgoing[2] = BATTLE_EFFECT_PRESENTATION_GREEN_STREAK;
        t_h_phalanx_outgoing[3] = t_h_phalanx_zero;
        t_h_phalanx_outgoing[4] = t_h_phalanx_zero;
        t_h_phalanx_outgoing[5] = t_h_phalanx_zero;
        {
            register s32 t_h_phalanx_arg0 asm("r0") = command_side;
            register s32 t_h_phalanx_arg1 asm("r1") = 4;
            register s32 t_h_phalanx_arg2 asm("r2") = t_h_phalanx_neg_one;
            register s32 t_h_phalanx_arg3 asm("r3") = 0;
            QueueBattleEffectDisplay(t_h_phalanx_arg0, t_h_phalanx_arg1,
                          t_h_phalanx_arg2, t_h_phalanx_arg3);
        }
    }
    {
        register u32 t_h_phalanx_base asm("r0");
        register u32 t_h_phalanx_dest_offset asm("r2");
        register s32 t_h_phalanx_dest_index asm("r3");
        register s32 t_h_phalanx_source_index asm("r4");
        register u32 t_h_phalanx_source_offset asm("r5");

        t_h_phalanx_base = BATTLE_STATE_RAM;
        t_h_phalanx_dest_offset = BATTLE_DECK_REWARD_OFFSET(persistent_command_ids);
        asm volatile("" : "+r"(t_h_phalanx_dest_offset));
        persistent_command_destination = (u8 *)(t_h_phalanx_base + t_h_phalanx_dest_offset);
        t_h_phalanx_dest_index = command_side;
        persistent_command_destination = (u8 *)(
            (u32)t_h_phalanx_dest_index - (0U - (u32)persistent_command_destination));
        t_h_phalanx_source_index = command_side_times_four;
        t_h_phalanx_base =
            (u32)t_h_phalanx_source_index - (0U - t_h_phalanx_base);
        t_h_phalanx_source_offset = BATTLE_DECK_PREPARATION_OFFSET(actions[0].command_id);
        asm volatile("" : "+r"(t_h_phalanx_source_offset));
        persistent_command_source = (u8 *)(t_h_phalanx_base + t_h_phalanx_source_offset);
    }
    goto store_persistent_command;
case DECK_COMMAND_CANNON_PHALANX - DECK_COMMAND_FRIENDSHIP:
    RemoveBattleUnitFromTurnOrder(command_side, 4U);
    AddBattleEffect(command_side, 4U, -1, 0, 0, 0, BATTLE_EFFECT_TURN_MARKER, 0, 1, 0);
    QueueBattleEffectDisplay(command_side, 1U, -1, 0, 0, 0, BATTLE_EFFECT_PRESENTATION_GREEN_STREAK, 0, 0, 0);
    {
        register u32 cannon_phalanx_base asm("r0");
        register u32 cannon_phalanx_dest_offset asm("r7");
        register s32 cannon_phalanx_dest_index asm("r2");
        register s32 cannon_phalanx_source_index asm("r3");
        register u32 cannon_phalanx_source_offset asm("r4");

        cannon_phalanx_base = BATTLE_STATE_RAM;
        cannon_phalanx_dest_offset = BATTLE_DECK_REWARD_OFFSET(persistent_command_ids);
        persistent_command_destination = (u8 *)(cannon_phalanx_base + cannon_phalanx_dest_offset);
        cannon_phalanx_dest_index = command_side;
        persistent_command_destination = (u8 *)(
            (u32)cannon_phalanx_dest_index - (0U - (u32)persistent_command_destination));
        cannon_phalanx_source_index = command_side_times_four;
        cannon_phalanx_base =
            (u32)cannon_phalanx_source_index - (0U - cannon_phalanx_base);
        cannon_phalanx_source_offset = BATTLE_DECK_PREPARATION_OFFSET(actions[0].command_id);
        asm volatile("" : "+r"(cannon_phalanx_source_offset));
        persistent_command_source = (u8 *)(cannon_phalanx_base + cannon_phalanx_source_offset);
    }
store_persistent_command:
    *persistent_command_destination = *persistent_command_source;
    return;
case DECK_COMMAND_GOJULAS_GIGA_CANNON_GATTAI - DECK_COMMAND_FRIENDSHIP: {
    register u8 *gojulas_giga_cannon_gattai_base asm("r7");
    s32 gojulas_giga_cannon_gattai_offset;
    register s32 gojulas_giga_cannon_gattai_zero asm("r5");
    register s32 gojulas_giga_cannon_gattai_sum asm("r0");
    register s32 gojulas_giga_cannon_gattai_row_side asm("r2");
    register void *gojulas_giga_cannon_gattai_second_record asm("r1");
    ResetBattleCombinationPresentation();
    gojulas_giga_cannon_gattai_zero = 0;
    giga_cannon_front_slot = gojulas_giga_cannon_gattai_zero;
    asm volatile("" : "+r"(gojulas_giga_cannon_gattai_zero));
    gojulas_giga_cannon_gattai_base = (u8 *)BATTLE_STATE_RAM;
loop_358:
    if ((IsBattleCombinationFormationValidWide(5, command_side, giga_cannon_front_slot) << 0x18) == 0) {
        goto block_360;
    }
    asm volatile(
        "mov r0, r8\n\t"
        "lsl %0, r0, #2\n\t"
        "add %0, r8\n\t"
        "lsl %0, %0, #3\n\t"
        "sub %0, %0, r0\n\t"
        "lsl %0, %0, #4"
        : "=l"(gojulas_giga_cannon_gattai_offset)
        :
        : "cc");
    {
        register s32 gojulas_giga_cannon_gattai_row_base asm("r1") = command_side_times_four;
        register s32 gojulas_giga_cannon_gattai_row_work asm("r4");

        gojulas_giga_cannon_gattai_row_side = command_side;
        asm volatile("" : "+r"(gojulas_giga_cannon_gattai_row_base),
                     "+r"(gojulas_giga_cannon_gattai_row_side));
        gojulas_giga_cannon_gattai_row_work = gojulas_giga_cannon_gattai_row_base + gojulas_giga_cannon_gattai_row_side;
        asm volatile("" : "+r"(gojulas_giga_cannon_gattai_row_work));
        gojulas_giga_cannon_gattai_row_work =
            (gojulas_giga_cannon_gattai_row_work * 8) - gojulas_giga_cannon_gattai_row_side;
        gojulas_giga_cannon_gattai_row_work <<= 7;
        temp_r4_22 = gojulas_giga_cannon_gattai_row_work;
    }
    giga_cannon_host_record = gojulas_giga_cannon_gattai_offset + temp_r4_22;
    asm volatile(
        "add %0, %0, %1"
        : "+l"(giga_cannon_host_record)
        : "l"(gojulas_giga_cannon_gattai_base)
        : "cc");
    {
        register s32 gojulas_giga_cannon_gattai_initial_hp asm("r3");
        asm volatile(
            "ldrh %0, [%1, #58]\n\t"
            "str %0, [sp, #28]"
            : "=r"(gojulas_giga_cannon_gattai_initial_hp)
            : "r"(giga_cannon_host_record)
            : "memory");
    }
    asm volatile("" : "=r"(sp1C));
    giga_cannon_initial_hp = BATTLE_UNIT_FIELD(giga_cannon_host_record, u16, hp);
    {
        register u32 gojulas_giga_cannon_gattai_successor asm("r0") = giga_cannon_front_slot;
        gojulas_giga_cannon_gattai_successor += 3;
        asm volatile(
            "lsl %1, %1, #24\n\t"
            "lsr %0, %1, #24"
            : "=l"(temp_r6_7), "+r"(gojulas_giga_cannon_gattai_successor));
    }
    AbsorbBattleCombinationUnitWide(gojulas_giga_cannon_gattai_row_side, giga_cannon_front_slot, temp_r6_7);
    asm volatile(
        "lsl %0, %1, #2\n\t"
        "add %0, %0, %1\n\t"
        "lsl %0, %0, #3\n\t"
        "sub %0, %0, %1\n\t"
        "lsl %0, %0, #4\n\t"
        "add %0, %0, %2\n\t"
        "add %0, %0, %3"
        : "=&r"(gojulas_giga_cannon_gattai_second_record)
        : "r"(temp_r6_7), "r"(temp_r4_22), "r"(gojulas_giga_cannon_gattai_base)
        : "cc");
    asm volatile("" : : "r"(sp1C));
    asm volatile(
        "ldr r2, [sp, #28]\n\t"
        "lsl r0, r2, #16\n\t"
        "asr r0, r0, #16\n\t"
        "ldrh r3, [%1, #58]\n\t"
        "add r0, r0, r3\n\t"
        "lsl r0, r0, #16\n\t"
        "lsr r0, r0, #16\n\t"
        "str r0, [sp, #28]\n\t"
        "mov r4, %2\n\t"
        "lsl r0, r4, #16\n\t"
        "asr r0, r0, #16\n\t"
        "ldrh %1, [%1, #6]\n\t"
        "add r0, r0, %1\n\t"
        "lsl r0, r0, #16\n\t"
        "lsr r0, r0, #16\n\t"
        "mov %0, r0"
        : "=r"(giga_cannon_hp_total), "+r"(gojulas_giga_cannon_gattai_second_record)
        : "0"(giga_cannon_initial_hp)
        : "r0", "r2", "r3", "r4", "cc", "memory");
    asm volatile("" : "=r"(sp1C));
    SetBattleUnitCombinedModelWide(command_side, giga_cannon_front_slot, 0x76);
    {
        register s32 gojulas_giga_cannon_gattai_factor asm("r1");
        register u32 gojulas_giga_cannon_gattai_factor_offset asm("r0");
        register u32 gojulas_giga_cannon_gattai_total_source asm("r2");
        register s32 gojulas_giga_cannon_gattai_product asm("r0");
        register s32 gojulas_giga_cannon_gattai_final_hp asm("r3");
        register s32 gojulas_giga_cannon_gattai_divisor asm("r1");

        asm volatile(
            ".syntax unified\n\t"
            "movs %1, #58\n\t"
            "ldrsh %0, [%2, %1]\n\t"
            ".syntax divided"
            : "=l"(gojulas_giga_cannon_gattai_factor), "=l"(gojulas_giga_cannon_gattai_factor_offset)
            : "l"(giga_cannon_host_record));
        asm volatile("mov %0, %1"
                     : "=l"(gojulas_giga_cannon_gattai_total_source)
                     : "r"(giga_cannon_hp_total));
        asm volatile(
            "lsl %0, %1, #16\n\t"
            "asr %0, %0, #16\n\t"
            "mul %0, %2"
            : "=&l"(gojulas_giga_cannon_gattai_product)
            : "l"(gojulas_giga_cannon_gattai_total_source), "l"(gojulas_giga_cannon_gattai_factor)
            : "cc");
        gojulas_giga_cannon_gattai_final_hp = physical_stack[7];
        asm volatile(
            "lsl %0, %1, #16\n\t"
            "asr %0, %0, #16"
            : "=l"(gojulas_giga_cannon_gattai_divisor)
            : "l"(gojulas_giga_cannon_gattai_final_hp));
        asm volatile("" : : "r"(sp1C));
        BATTLE_UNIT_FIELD(giga_cannon_host_record, u16, hp) =
            DivideSigned32FullArguments(gojulas_giga_cannon_gattai_product, gojulas_giga_cannon_gattai_divisor);
    }
block_360:
    temp_r0_77 = giga_cannon_front_slot + 1;
    giga_cannon_front_slot = temp_r0_77;
    if ((u32) temp_r0_77 <= 2U) {
        goto loop_358;
    }
    goto start_combination_presentation;
}
case DECK_COMMAND_LORD_GALE_GATTAI - DECK_COMMAND_FRIENDSHIP: {
    register volatile s32 *lord_gale_gattai_stack asm("sp");
    register s32 lord_gale_gattai_sum asm("r1");
    u8 lord_gale_gattai_inner_next;
    ResetBattleCombinationPresentation();
    {
        register u32 lord_gale_gattai_initial_zero asm("r4") = 0;

        asm volatile("" : "+r"(lord_gale_gattai_initial_zero));
        lord_gale_direction = lord_gale_gattai_initial_zero;
    }
loop_363:
    lord_gale_rank_base = 0;
    {
        register u32 lord_gale_gattai_successor asm("r5") = lord_gale_direction;

        lord_gale_gattai_successor += 1;
        lord_gale_gattai_stack[24] = lord_gale_gattai_successor;
    }
loop_364:
    temp_r4_23 = lord_gale_rank_base + 1;
    {
        register u32 lord_gale_gattai_outer_view asm("r0") = lord_gale_direction;

        if ((IsBattleCombinationFormationValid(
                7, command_side, (u8)(temp_r4_23 - lord_gale_gattai_outer_view))
             << 0x18) == 0) {
            goto block_368;
        }
    }
    {
        register u32 lord_gale_gattai_index_work asm("r0");
        register void *lord_gale_gattai_record_offset asm("r1");
        register s32 lord_gale_gattai_row_work asm("r0");
        register void *lord_gale_gattai_record_base asm("r4");

        asm volatile(
            "lsl %0, %1, #24\n\t"
            "lsr %0, %0, #24\n\t"
            "mov sl, %0"
            : "=r"(lord_gale_gattai_index_work)
            : "r"(temp_r4_23));
        asm volatile(
            "lsl %0, %1, #2\n\t"
            "add %0, sl\n\t"
            "lsl %0, %0, #3\n\t"
            "sub %0, %0, %1\n\t"
            "lsl %0, %0, #4"
            : "=&r"(lord_gale_gattai_record_offset)
            : "r"(lord_gale_gattai_index_work)
            : "cc");
        asm volatile(
            "ldr r2, [sp, #108]\n\t"
            "ldr r3, [sp, #24]\n\t"
            "add %0, r2, r3\n\t"
            "lsl %0, %0, #3\n\t"
            "sub %0, %0, r3\n\t"
            "lsl %0, %0, #7"
            : "=l"(lord_gale_gattai_row_work)
            :
            : "r2", "r3", "cc", "memory");
        sp58 = lord_gale_gattai_row_work;
        asm volatile(
            "add %0, %0, %1"
            : "+r"(lord_gale_gattai_record_offset)
            : "r"(lord_gale_gattai_row_work)
            : "cc");
        asm volatile(
            "ldr %0, [pc, #220]"
            : "=r"(lord_gale_gattai_record_base));
        asm volatile(
            "add %0, %1, %2"
            : "=l"(lord_gale_host_record)
            : "l"(lord_gale_gattai_record_offset), "l"(lord_gale_gattai_record_base)
            : "cc");
    }
    {
        register s32 lord_gale_gattai_initial_hp asm("r0");
        asm volatile(
            "ldrh %0, [%1, #58]\n\t"
            "str %0, [sp, #28]"
            : "=r"(lord_gale_gattai_initial_hp)
            : "r"(lord_gale_host_record)
            : "memory");
    }
    lord_gale_initial_hp = BATTLE_UNIT_FIELD(lord_gale_host_record, u16, hp);
    asm volatile(
        "mov r0, #1\n\t"
        "mov r2, r8\n\t"
        "sub r0, r0, r2\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r7, r0\n\t"
        "lsl r0, r0, #24\n\t"
        "lsr %0, r0, #24"
        : "=l"(temp_r6_8)
        :
        : "cc");
    if (BATTLE_UNIT_FIELD(lord_gale_host_record, u8, zoid_id) == 0x7E) {
        goto block_367;
    }
    {
        register void *lord_gale_gattai_alternate_base asm("r0");

        asm volatile(
            "lsl %0, %1, #2\n\t"
            "add %0, %0, %1\n\t"
            "lsl %0, %0, #3\n\t"
            "sub %0, %0, %1\n\t"
            "lsl %0, %0, #4\n\t"
            "ldr r3, [sp, #88]\n\t"
            "add %0, %0, r3"
            : "=&r"(lord_gale_pilot_component_record)
            : "r"(temp_r6_8)
            : "cc");
        asm volatile(
            "ldr %0, [pc, #176]"
            : "=r"(lord_gale_gattai_alternate_base));
        asm volatile(
            "add %0, %0, %1"
            : "+r"(lord_gale_pilot_component_record)
            : "r"(lord_gale_gattai_alternate_base)
            : "cc");
    }
    BATTLE_UNIT_FIELD(lord_gale_host_record, u8, pilot_slot_or_definition_id) = (u8) BATTLE_UNIT_FIELD(lord_gale_pilot_component_record, u8, pilot_slot_or_definition_id);
    CopyBytes(lord_gale_host_record + 0x70, lord_gale_pilot_component_record + 0x70, 0x40);
    {
        register void *lord_gale_gattai_copy_dest asm("r0");
        register void *lord_gale_gattai_copy_source asm("r1");
        register u32 lord_gale_gattai_copy_size asm("r2");

        asm volatile(
            "add %0, %4, #0\n\t"
            "add %0, #176\n\t"
            "add %3, #176\n\t"
            "add %1, %3, #0\n\t"
            "movs %2, #52"
            : "=l"(lord_gale_gattai_copy_dest),
              "=l"(lord_gale_gattai_copy_source),
              "=l"(lord_gale_gattai_copy_size),
              "+l"(lord_gale_pilot_component_record)
            : "l"(lord_gale_host_record)
            : "cc");
        CopyBytes(lord_gale_gattai_copy_dest, lord_gale_gattai_copy_source,
                      lord_gale_gattai_copy_size);
    }
block_367:
    {
        register u32 lord_gale_gattai_index_post asm("r6") = temp_r6_8;
        register s32 lord_gale_gattai_call_side asm("r0");
        register u32 lord_gale_gattai_call_one asm("r1");
        register u32 lord_gale_gattai_call_index asm("r2");
        register void *lord_gale_gattai_post_base asm("r3");
        asm volatile(
            "ldr %0, [sp, #24]\n\t"
            "mov %1, sl\n\t"
            "add %2, %3, #0"
            : "=l"(lord_gale_gattai_call_side),
              "=l"(lord_gale_gattai_call_one),
              "=l"(lord_gale_gattai_call_index)
            : "l"(lord_gale_gattai_index_post)
            : "memory");
        ((void (*)())AbsorbBattleCombinationUnit)(lord_gale_gattai_call_side, lord_gale_gattai_call_one,
                                    lord_gale_gattai_call_index);
        asm volatile(
            "lsl %0, %1, #2\n\t"
            "add %0, %0, %1\n\t"
            "lsl %0, %0, #3\n\t"
            "sub %0, %0, %1\n\t"
            "lsl %0, %0, #4\n\t"
            "ldr r2, [sp, #88]\n\t"
            "add %0, %0, r2"
            : "=r"(temp_r1_23)
            : "r"(lord_gale_gattai_index_post)
            : "cc");
        asm volatile(
            "ldr %0, [pc, #116]"
            : "=r"(lord_gale_gattai_post_base));
        asm volatile(
            "add %0, %0, %1"
            : "+r"(temp_r1_23)
            : "r"(lord_gale_gattai_post_base)
            : "cc");
    }
    {
        register u32 lord_gale_gattai_hp_reload asm("r4");
        register u32 lord_gale_gattai_hp_sum asm("r0");
        register u32 lord_gale_gattai_hp_addend asm("r2");
        register void *lord_gale_gattai_hp_record asm("r1") = temp_r1_23;

        asm volatile(
            "ldr %0, [sp, #28]\n\t"
            "lsl %1, %0, #16\n\t"
            "asr %1, %1, #16\n\t"
            "ldrh %2, [%3, #58]\n\t"
            "add %1, %1, %2\n\t"
            "lsl %1, %1, #16\n\t"
            "lsr %1, %1, #16\n\t"
            "str %1, [sp, #28]"
            : "=&l"(lord_gale_gattai_hp_reload),
              "=&l"(lord_gale_gattai_hp_sum),
              "=&l"(lord_gale_gattai_hp_addend)
            : "l"(lord_gale_gattai_hp_record)
            : "cc", "memory");
    }
    asm volatile(
        "mov r3, %2\n\t"
        "lsl r0, r3, #16\n\t"
        "asr r0, r0, #16\n\t"
        "ldrh r1, [%1, #6]\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #16\n\t"
        "lsr r0, r0, #16\n\t"
        "mov %0, r0"
        : "=r"(lord_gale_hp_total)
        : "r"(temp_r1_23), "r"(lord_gale_initial_hp)
        : "cc");
    SetBattleUnitCombinedModel(command_side, temp_r0_78, 0x97);
    {
        register s32 lord_gale_gattai_factor asm("r1");
        register u32 lord_gale_gattai_factor_offset asm("r4");
        register u32 lord_gale_gattai_total_source asm("r2");
        register s32 lord_gale_gattai_product asm("r0");
        register s32 lord_gale_gattai_final_hp asm("r3");
        register s32 lord_gale_gattai_divisor asm("r1");

        asm volatile(
            ".syntax unified\n\t"
            "movs %1, #58\n\t"
            "ldrsh %0, [%2, %1]\n\t"
            ".syntax divided"
            : "=l"(lord_gale_gattai_factor), "=l"(lord_gale_gattai_factor_offset)
            : "l"(lord_gale_host_record));
        asm volatile(
            "mov %0, %1"
            : "=l"(lord_gale_gattai_total_source)
            : "r"(lord_gale_hp_total));
        asm volatile(
            "lsl %0, %1, #16\n\t"
            "asr %0, %0, #16\n\t"
            "mul %0, %2"
            : "=&l"(lord_gale_gattai_product)
            : "l"(lord_gale_gattai_total_source), "l"(lord_gale_gattai_factor)
            : "cc");
        lord_gale_gattai_final_hp = lord_gale_gattai_stack[7];
        asm volatile(
            "lsl %0, %1, #16\n\t"
            "asr %0, %0, #16"
            : "=l"(lord_gale_gattai_divisor)
            : "l"(lord_gale_gattai_final_hp));
        BATTLE_UNIT_FIELD(lord_gale_host_record, u16, hp) =
            DivideSigned32FullArguments(lord_gale_gattai_product, lord_gale_gattai_divisor);
    }
block_368:
    lord_gale_gattai_inner_next = lord_gale_rank_base + 3;
    lord_gale_rank_base = lord_gale_gattai_inner_next;
    if ((u32) lord_gale_rank_base <= 5U) {
        goto loop_364;
    }
    {
        register u32 lord_gale_gattai_exit_reload asm("r4") = lord_gale_gattai_stack[24];
        register u32 lord_gale_gattai_exit_normalized asm("r0");

        asm volatile(
            "lsl %0, %1, #24\n\t"
            "lsr %0, %0, #24"
            : "=r"(lord_gale_gattai_exit_normalized)
            : "r"(lord_gale_gattai_exit_reload));
        lord_gale_direction = lord_gale_gattai_exit_normalized;
        if (lord_gale_gattai_exit_normalized > 1U) {
            goto start_combination_presentation;
        }
    }
    goto loop_363;
}
start_combination_presentation:
    asm volatile(
        "ldr %0, [pc, #24]"
        : "=r"(final_state));
    next_battle_phase = 0x1340;
store_battle_phase:
    *final_state = next_battle_phase;
command_done:
    return;
    }
}
