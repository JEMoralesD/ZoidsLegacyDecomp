#include "m2c_prelude.h"
#include "battle_display.h"
#include "../graphics/camera.h"

M2C_UNK PlaySong(s32) asm("func_08092E84");                         /* extern */
void *CreateSpriteFromTable(M2C_UNK, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08094374"); /* extern */
M2C_UNK DestroySprite(void *) asm("func_08094554");                      /* extern */
M2C_UNK ResetMenuKeyRepeat() asm("func_08096F3C");                            /* extern */
M2C_UNK RequestWindowRefresh() asm("func_080972C8");                            /* extern */
M2C_UNK PrintWindowTextAt(s32, s32, s32, s32, s32) asm("func_080981F0");     /* extern */
M2C_UNK PrintWindowText(M2C_UNK, s32, s32) asm("func_08098248");           /* extern */
M2C_UNK PrintWindowNumberAt(s16, s32, u32, s32, s32, s32, s32) asm("func_0809844C"); /* extern */
M2C_UNK PrintWindowNumberAtWide(s32, s32, u32, s32, s32, s32, s32)
    asm("func_0809844C");
M2C_UNK ClearWindow(s32) asm("func_080986B4");                         /* extern */
M2C_UNK RunMenuScript(M2C_UNK) asm("func_08098BB4");                     /* extern */
M2C_UNK LoadSpriteGraphicsFromTable(M2C_UNK, s32, s32, s32) asm("func_0809AA64");      /* extern */
M2C_UNK StartBattleCameraTransition(s32, u8, u8, s32) asm("func_080BB224");            /* extern */
u8 FindBattleEffect(u8, u8, s32) asm("func_080BF464");                      /* extern */
M2C_UNK RecalculateZoidStats(M2C_UNK, void *) asm("func_080E5880");             /* extern */
s32 GetBattlePilotDisplayName(u8, u8) asm("func_080E9924");                          /* extern */
s32 IsBattleUnitActive(u8, u8) asm("func_080E9D88");                          /* extern */
s16 DivideSigned32(s16, s32) asm("func_080ECD98");                        /* extern */
s32 ModuloSigned32(s16, s32) asm("func_080ECE30");                        /* extern */
s32 ModuloSigned32FullArguments(s32, s32) asm("func_080ECE30");
s32 DivideUnsigned32(u8, s32) asm("func_080ECF00");                         /* extern */
u8 ModuloUnsigned32(u32, s32) asm("func_080ECF78");                         /* extern */
M2C_UNK CopyBytes(M2C_UNK, void *, s32) asm("func_080ED038");        /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */

asm(
    ".macro C8538_PATCH_FINAL_REFERENCE\n"
    ".macro ldr dst, src:vararg\n"
    ".purgem ldr\n"
    "ldr r2, \\src\n"
    ".endm\n"
    ".macro mov dst, src\n"
    ".purgem mov\n"
    "mov r4, #0\n"
    ".endm\n"
    ".macro ldrsh dst, src:vararg\n"
    ".purgem ldrsh\n"
    "ldrsh r1, [r2, r4]\n"
    ".endm\n"
    ".endm\n");

asm(
    ".macro C8538_PATCH_STACK_ARG reg\n"
    ".macro mov dst, src\n"
    ".purgem mov\n"
    "mov \\reg, \\src\n"
    ".endm\n"
    ".macro str src, addr:vararg\n"
    ".purgem str\n"
    "str \\reg, \\addr\n"
    ".endm\n"
    ".endm\n");

asm(
    ".macro C8538_PATCH_FIRST_NAME_CALL\n"
    ".set C8538_NAME_MOV_COUNT, 0\n"
    ".macro mov dst, src\n"
    ".if C8538_NAME_MOV_COUNT == 0\n"
    ".syntax unified\n"
    "movs r3, #0\n"
    ".syntax divided\n"
    ".elseif C8538_NAME_MOV_COUNT == 1\n"
    ".syntax unified\n"
    "movs r1, #0\n"
    ".syntax divided\n"
    ".elseif C8538_NAME_MOV_COUNT == 2\n"
    ".syntax unified\n"
    "movs r2, #3\n"
    ".syntax divided\n"
    ".else\n"
    ".purgem mov\n"
    ".endif\n"
    ".set C8538_NAME_MOV_COUNT, C8538_NAME_MOV_COUNT + 1\n"
    ".endm\n"
    ".macro str src, addr:vararg\n"
    ".purgem str\n"
    "str r3, \\addr\n"
    ".endm\n"
    ".endm\n"
    ".macro C8538_INSERT_R3_BEFORE_BL\n"
    ".macro bl target\n"
    ".purgem bl\n"
    "mov r3, #0\n"
    "bl \\target\n"
    ".endm\n"
    ".endm\n");

asm(
    ".macro C8538_DELAY_FINAL_POINTER\n"
    ".macro ldr dst, src:vararg\n"
    ".purgem ldr\n"
    ".endm\n"
    ".macro strh src, addr:vararg\n"
    ".purgem strh\n"
    "ldr r1, [sp, #20]\n"
    "strh r0, [r1, #6]\n"
    ".endm\n"
    ".endm\n");

asm(
    ".macro C8538_OPEN_INSTALL_MOV4\n"
    ".macro mov dst, src\n"
    ".purgem mov\n"
    "mov r3, \\src\n"
    ".endm\n"
    ".endm\n"
    ".macro C8538_OPEN_INSTALL_MOV3\n"
    ".macro mov dst, src\n"
    ".purgem mov\n"
    "mov \\dst, \\src\n"
    "C8538_OPEN_INSTALL_MOV4\n"
    ".endm\n"
    ".endm\n"
    ".macro C8538_OPEN_INSTALL_MOV2\n"
    ".macro mov dst, src\n"
    ".purgem mov\n"
    "mov \\dst, \\src\n"
    "C8538_OPEN_INSTALL_MOV3\n"
    ".endm\n"
    ".endm\n"
    ".macro C8538_OPEN_INSTALL_CMP4\n"
    ".macro cmp lhs, rhs\n"
    ".purgem cmp\n"
    "cmp r3, \\rhs\n"
    ".endm\n"
    ".endm\n"
    ".macro C8538_OPEN_INSTALL_CMP3\n"
    ".macro cmp lhs, rhs\n"
    ".purgem cmp\n"
    "cmp \\lhs, \\rhs\n"
    "C8538_OPEN_INSTALL_CMP4\n"
    ".endm\n"
    ".endm\n"
    ".macro C8538_OPEN_INSTALL_CMP2\n"
    ".macro cmp lhs, rhs\n"
    ".purgem cmp\n"
    "cmp \\lhs, \\rhs\n"
    "C8538_OPEN_INSTALL_CMP3\n"
    ".endm\n"
    ".endm\n"
    ".macro C8538_PATCH_OPENING_R3\n"
    ".macro mov dst, src\n"
    ".purgem mov\n"
    "mov \\dst, \\src\n"
    "C8538_OPEN_INSTALL_MOV2\n"
    ".endm\n"
    ".macro cmp lhs, rhs\n"
    ".purgem cmp\n"
    "cmp \\lhs, \\rhs\n"
    "C8538_OPEN_INSTALL_CMP2\n"
    ".endm\n"
    ".endm\n");

#define STORE_SKIP_SLOT4() ({ \
    asm volatile( \
        ".syntax unified\n\t" \
        "lsls r2, r7, #2\n\t" \
        "str r2, [sp, #40]\n\t" \
        ".syntax divided" \
        : : : "r2", "memory"); \
    1; \
})
#define VIEW_R2(value_expr) ({ \
    register s32 view asm("r2") = (value_expr); \
    asm volatile("" : "+r"(view)); \
    view; \
})
#define VIEW_R3(value_expr) ({ \
    register s32 view asm("r3") = (value_expr); \
    asm volatile("" : "+r"(view)); \
    view; \
})
#define VIEW_R4(value_expr) ({ \
    register s32 view asm("r4") = (value_expr); \
    asm volatile("" : "+r"(view)); \
    view; \
})
#define LOAD_BATTLE_SENSOR_ACCURACY(record_expr) ({ \
    register void *base_r0 asm("r0") = (record_expr); \
    register s32 value_r3 asm("r3"); \
    asm volatile( \
        ".syntax unified\n\t" \
        "adds r0, #74\n\t" \
        "movs r2, #0\n\t" \
        "ldrsh r3, [r0, r2]\n\t" \
        ".syntax divided" \
        : "+r"(base_r0), "=r"(value_r3) \
        : : "r2", "cc", "memory"); \
    value_r3; \
})
#define CALL_STAT_ROW(value_expr, relation_expr, row_imm) do { \
    register s32 call_value_r3 asm("r3") = (value_expr); \
    register u32 call_relation_r2 asm("r2") = (relation_expr); \
    asm volatile( \
        ".syntax unified\n\t" \
        "movs r0, #2\n\t" \
        "str r0, [sp, #0]\n\t" \
        "movs r1, #4\n\t" \
        "str r1, [sp, #4]\n\t" \
        "movs r0, #" #row_imm "\n\t" \
        "str r0, [sp, #8]\n\t" \
        "adds r0, %0, #0\n\t" \
        "@ relation %1\n\t" \
        "movs r3, #10\n\t" \
        "bl func_0809844C\n\t" \
        ".syntax divided" \
        : "+r"(call_value_r3), "+r"(call_relation_r2) \
        : : "r0", "r1", "lr", "cc", "memory"); \
} while (0)

void ShowBattleUnitStatusViewer(s32 initial_side_word, s32 initial_unit_slot_word) asm("func_080C8538");

void ShowBattleUnitStatusViewer(s32 initial_side_word, s32 initial_unit_slot_word) {
    volatile struct BattleUnitStatusViewerFrame {
        void *cursor_sprite;
        s32 previous_unit_slot;
        void *base_stats_record;
        s32 selected_side_times_two;
        s32 selected_side_times_four;
        s32 selected_unit_slot_times_four;
    } stack;
    volatile s32 spill;
    s16 hp_threshold_or_base_max_hp;
    s16 base_speed;
    s16 base_mobility;
    s16 base_initiative;
    s16 base_defense;
    s16 base_armor_rate;
    s16 base_dcp;
    s16 base_sensor_accuracy;
    s16 base_max_ep;
    s16 base_ep_regen;
    s32 current_max_hp;
    s32 current_sensor_accuracy;
    s32 current_max_ep;
    s32 current_ep_regen;
    s32 current_speed;
    s32 current_mobility;
    s32 current_initiative;
    s32 current_defense;
    s32 current_armor_rate;
    s32 current_dcp;
    register s32 current_hp asm("r4");
    s16 temp_r4_3;
    s16 temp_r4_4;
    s16 temp_r6;
    s16 temp_r6_2;
    s16 temp_r6_3;
    s16 temp_r6_4;
    s16 temp_r6_5;
    s16 temp_r6_6;
    s32 cursor_scale;
    s16 var_r2;
    s16 var_r5_4;
    s16 var_r5_5;
    s16 var_r5_8;
    s32 temp_r0_3;
    s32 ep_regen_comparison_difference;
    s32 speed_comparison_difference;
    s32 mobility_comparison_difference;
    s32 initiative_comparison_difference;
    s32 defense_comparison_difference;
    s32 armor_rate_comparison_difference;
    s32 dcp_comparison_difference;
    s32 sensor_accuracy_comparison_difference;
    s32 max_hp_comparison_value;
    s32 max_ep_comparison_difference;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r2_2;
    s32 var_r2_3;
    s32 var_r8;
    u16 temp_r1;
    u16 temp_r1_3;
    u16 temp_r1_4;
    u16 temp_r1_5;
    u16 unit_flag40;
    u16 var_r5;
    u16 var_r5_2;
    s16 var_r5_3;
    u32 temp_r0;
    u32 temp_r0_2;
    u32 defense_comparison_color;
    u32 armor_rate_comparison_color;
    u32 dcp_comparison_color;
    u32 sensor_accuracy_comparison_color;
    u32 max_hp_comparison_color;
    u32 max_ep_comparison_color;
    u32 ep_regen_comparison_color;
    u32 speed_comparison_color;
    u32 mobility_comparison_color;
    u32 initiative_comparison_color;
    register u32 hp_warning_color asm("r5");
    u8 temp_r1_2;
    u8 temp_r4;
    u8 temp_r4_10;
    u8 temp_r4_11;
    u8 temp_r4_12;
    u8 temp_r4_13;
    u8 temp_r4_2;
    u8 temp_r4_5;
    u8 temp_r4_6;
    u8 temp_r4_7;
    u8 temp_r4_8;
    u8 temp_r4_9;
    u8 temp_r5;
    u8 temp_r5_2;
    u8 temp_r5_3;
    u8 scan_side_1;
    u8 scan_side_2;
    u16 var_r5_6;
    u16 var_r5_7;
    u8 selected_unit_slot;
    u8 selected_side;
    register u32 previous_side_or_base_stats_address asm("sl");
    void *temp_r0_5;
    void *temp_r0_6;
    register void *selected_unit_record asm("r8");

    selected_side = initial_side_word;
    selected_unit_slot = initial_unit_slot_word;
    LoadSpriteGraphicsFromTable(BATTLE_STATUS_CURSOR_GRAPHICS_ROM, 0, 0x360, 0xD);
    stack.cursor_sprite = CreateSpriteFromTable(BATTLE_STATUS_CURSOR_SPRITES_ROM, 0, 0, 0, 0, 0x360, 0xD, 0x60, 0);
    stack.previous_unit_slot = (s32) (u8) (selected_unit_slot + 1);
    StartBattleCameraTransition(BATTLE_CAMERA_FOCUS_UNIT, selected_side, selected_unit_slot, 1);
    {
        u8 *control = (u8 *)0x030033C4;
        CAMERA_FIELD(control, s32, projection.screen_center_x) = 0xB4;
        CAMERA_FIELD(control, s32, projection.screen_center_y) = 0x58;
    }
    RunMenuScript(BATTLE_STATUS_OPEN_MENU);
    ResetMenuKeyRepeat();
    stack.base_stats_record = (void *)BATTLE_STATUS_BASE_STATS_RAM;
    do {
        {
        register u16 keys_r1 asm("r1") = *(u16 *)0x03006034;
        asm volatile("C8538_PATCH_OPENING_R3");
        if (((BATTLE_STATUS_KEY_UP & keys_r1) && (VIEW_R2(selected_side) == 0)) ||
            ((BATTLE_STATUS_KEY_DOWN & keys_r1) && (selected_side != 0))) {
            temp_r1_2 = ModuloUnsigned32((u32) selected_unit_slot, 3);
            if (temp_r1_2 != 0) {
                register s32 loop_index asm("r5");
                register s32 loop_signed asm("r6");
                register s32 loop_bound asm("r2");
                {
                    register s32 count_r1 asm("r1") = temp_r1_2;
                    asm volatile(
                        ".syntax unified\n\t"
                        "subs r0, %3, #1\n\t"
                        "subs %2, %3, %2\n\t"
                        "lsls r0, r0, #16\n\t"
                        "lsrs %0, r0, #16\n\t"
                        "lsls %2, %2, #16\n\t"
                        "asrs %1, %2, #16\n\t"
                        ".syntax divided"
                        : "=r"(loop_index), "=r"(loop_bound), "+r"(count_r1)
                        : "r"(selected_unit_slot) : "r0");
                }
                while ((s32) ({
                    asm volatile(
                        ".syntax unified\n\t"
                        "lsls r0, %1, #16\n\t"
                        "asrs %0, r0, #16\n\t"
                        ".syntax divided"
                        : "=r"(loop_signed) : "r"(loop_index) : "r0");
                    loop_signed;
                }) >= loop_bound) {
                    temp_r4 = (u8) loop_index;
                    asm volatile("" :: "r"(loop_index));
                    if ((s32) ({
                        register s32 valid asm("r0") = selected_side;
                        register s32 item_arg asm("r1") = temp_r4;
                        asm volatile(
                            ".syntax unified\n\t"
                            "str %2, [sp, #44]\n\t"
                            "bl func_080E9D88\n\t"
                            ".syntax divided"
                            : "+r"(valid), "+r"(item_arg), "+r"(loop_bound)
                            : : "r3", "lr", "cc", "memory");
                        valid <<= 0x18;
                        loop_bound = spill;
                        valid;
                    }) != 0) {
                        selected_unit_slot = temp_r4;
                        PlaySong(0x40);
                        break;
                    }
                    asm volatile(
                        ".syntax unified\n\t"
                        "subs r0, %1, #1\n\t"
                        "lsls r0, r0, #16\n\t"
                        "lsrs %0, r0, #16\n\t"
                        ".syntax divided"
                        : "=r"(loop_index) : "r"(loop_signed) : "r0");
                }
                if (selected_unit_slot == VIEW_R4(stack.previous_unit_slot)) {
                    register s32 count_r0 asm("r0");
                    register u16 fallback_index asm("r5");
                    register s32 fallback_signed asm("r6");
                    register s32 fallback_bound asm("r2");
                    register s32 fallback_base asm("r0") = 0;
                    if ((u32) selected_unit_slot <= 2U) {
                        fallback_base = 3;
                    }
                    var_r0 = fallback_base;
                    count_r0 = ModuloUnsigned32((u32) selected_unit_slot, 3);
                    {
                        register s32 neg1 asm("r1") = 0xFFFF;
                        asm volatile(
                            ".syntax unified\n\t"
                            "adds r0, %1, %2\n\t"
                            "adds r0, %3, r0\n\t"
                            "lsls r0, r0, #16\n\t"
                            "lsrs %0, r0, #16\n\t"
                            ".syntax divided"
                            : "=r"(fallback_index), "+r"(count_r0)
                            : "r"(neg1), "r"(var_r0));
                    }
                    fallback_bound = var_r0;
                    while ((s32) ({
                        asm volatile(
                            ".syntax unified\n\t"
                            "lsls r0, %1, #16\n\t"
                            "asrs %0, r0, #16\n\t"
                            ".syntax divided"
                            : "=r"(fallback_signed) : "r"(fallback_index) : "r0");
                        fallback_signed;
                    }) >= fallback_bound) {
                        temp_r4_2 = (u8) fallback_index;
                        asm volatile("" :: "r"(fallback_index));
                        if ((s32) ({
                            register s32 valid asm("r0") = selected_side;
                            register s32 item_arg asm("r1") = temp_r4_2;
                            asm volatile(
                                ".syntax unified\n\t"
                                "str %2, [sp, #44]\n\t"
                                "bl func_080E9D88\n\t"
                                ".syntax divided"
                                : "+r"(valid), "+r"(item_arg), "+r"(fallback_bound)
                                : : "r3", "lr", "cc", "memory");
                            valid <<= 0x18;
                            fallback_bound = spill;
                            valid;
                        }) != 0) {
                            selected_unit_slot = temp_r4_2;
                            PlaySong(0x40);
                            break;
                        }
                        asm volatile(
                            ".syntax unified\n\t"
                            "subs r0, %1, #1\n\t"
                            "lsls r0, r0, #16\n\t"
                            "lsrs %0, r0, #16\n\t"
                            ".syntax divided"
                            : "=r"(fallback_index) : "r"(fallback_signed) : "r0");
                    }
                }
            }
        }
        }
        {
        register u16 keys_r1 asm("r1") = *(u16 *)0x03006034;
        if ((((BATTLE_STATUS_KEY_UP & keys_r1) && (selected_side != 0)) || ((BATTLE_STATUS_KEY_DOWN & keys_r1) && (selected_side == 0))) && ((u32) ModuloUnsigned32((u32) selected_unit_slot, 3) <= 1U)) {
            register s32 upper_bound asm("r4");
            asm volatile(
                ".syntax unified\n\t"
                "adds r0, %1, #1\n\t"
                "lsls r0, r0, #16\n\t"
                "lsrs %0, r0, #16\n\t"
                ".syntax divided"
                : "=r"(var_r5_3) : "r"(selected_unit_slot) : "r0");
            upper_bound = selected_unit_slot + 3;
            asm volatile(
                ".syntax unified\n\t"
                "lsls %0, %0, #16\n\t"
                "asrs %0, %0, #16\n\t"
                ".syntax divided"
                : "+r"(upper_bound));
            upper_bound -= ModuloSigned32FullArguments(upper_bound, 3);
            asm volatile(
                ".syntax unified\n\t"
                "lsls %0, %0, #16\n\t"
                "asrs %0, %0, #16\n\t"
                ".syntax divided"
                : "+r"(upper_bound));
            while ((s32) (temp_r6_3 = var_r5_3) < upper_bound) {
                temp_r5 = (u8) var_r5_3;
                if ((IsBattleUnitActive(selected_side, temp_r5) << 0x18) != 0) {
                    selected_unit_slot = temp_r5;
                    PlaySong(0x40);
                    break;
                }
                var_r5_3 = (s16) (u16) (temp_r6_3 + 1);
            }
            if (selected_unit_slot == VIEW_R4(stack.previous_unit_slot)) {
                register s32 count_r0 asm("r0");
                register s32 fallback_index asm("r5");
                register s32 fallback_signed asm("r6");
                register s32 fallback_bound asm("r2");
                register s32 fallback_base asm("r0") = 0;
                if ((u32) selected_unit_slot <= 2U) {
                    fallback_base = 3;
                }
                var_r0_2 = fallback_base;
                count_r0 = ModuloUnsigned32((u32) selected_unit_slot, 3);
                asm volatile(
                    ".syntax unified\n\t"
                    "adds %1, %1, #1\n\t"
                    "adds %0, %2, %1\n\t"
                    ".syntax divided"
                    : "=r"(fallback_index), "+r"(count_r0) : "r"(var_r0_2));
                fallback_bound = var_r0_2 + 3;
                while ((s32) ({
                    asm volatile(
                        ".syntax unified\n\t"
                        "lsls r0, %1, #16\n\t"
                        "asrs %0, r0, #16\n\t"
                        ".syntax divided"
                        : "=r"(fallback_signed) : "r"(fallback_index) : "r0");
                    fallback_signed;
                }) < fallback_bound) {
                    temp_r4_5 = (u8) fallback_index;
                    asm volatile("" :: "r"(fallback_index));
                    if ((s32) ({
                        register s32 valid asm("r0") = selected_side;
                        register s32 item_arg asm("r1") = temp_r4_5;
                        asm volatile(
                            ".syntax unified\n\t"
                            "str %2, [sp, #44]\n\t"
                            "bl func_080E9D88\n\t"
                            ".syntax divided"
                            : "+r"(valid), "+r"(item_arg), "+r"(fallback_bound)
                            : : "r3", "lr", "cc", "memory");
                        valid <<= 0x18;
                        fallback_bound = spill;
                        valid;
                    }) != 0) {
                        selected_unit_slot = temp_r4_5;
                        PlaySong(0x40);
                        break;
                    }
                    asm volatile(
                        ".syntax unified\n\t"
                        "adds r0, %1, #1\n\t"
                        "lsls r0, r0, #16\n\t"
                        "lsrs %0, r0, #16\n\t"
                        ".syntax divided"
                        : "=r"(fallback_index) : "r"(fallback_signed) : "r0");
                }
            }
        }
        }
        {
        register u16 keys_r1 asm("r1") = *(u16 *)0x03006034;
        register s32 reload_guard_r4 asm("r4");
        asm volatile("" : "=r"(reload_guard_r4));
        if (!(BATTLE_STATUS_KEY_LEFT & keys_r1) || (selected_side != 0)) {
            if (!(BATTLE_STATUS_KEY_RIGHT & keys_r1)) {

            } else if (selected_side == 0) {

            } else {
                goto block_44;
            }
        } else {
block_44:
            asm volatile("" : : "r"(reload_guard_r4));
            var_r8 = 0;
            if ((DivideUnsigned32(selected_unit_slot, 3) << 0x18) != 0) {
                temp_r4_6 = selected_unit_slot - 3;
                if ((IsBattleUnitActive(selected_side, temp_r4_6) << 0x18) != 0) {
                    selected_unit_slot = temp_r4_6;
                    PlaySong(0x40);
                    var_r8 = 1;
                } else {
                    register s32 loop_index asm("r5") = 0;
                    register s32 loop_signed asm("r6");
                    while ((s32) ({
                        asm volatile(
                            ".syntax unified\n\t"
                            "lsls r0, %1, #16\n\t"
                            "asrs %0, r0, #16\n\t"
                            ".syntax divided"
                            : "=r"(loop_signed) : "r"(loop_index) : "r0");
                        loop_signed;
                    }) <= 2) {
                        temp_r4_7 = (u8) loop_index;
                        asm volatile("" :: "r"(loop_index));
                        if ((IsBattleUnitActive(selected_side, temp_r4_7) << 0x18) != 0) {
                            selected_unit_slot = temp_r4_7;
                            PlaySong(0x40);
                            var_r8 = 1;
                            break;
                        }
                        asm volatile(
                            ".syntax unified\n\t"
                            "adds r0, %1, #1\n\t"
                            "lsls r0, r0, #16\n\t"
                            "lsrs %0, r0, #16\n\t"
                            ".syntax divided"
                            : "=r"(loop_index) : "r"(loop_signed) : "r0");
                    }
                    if (loop_signed == 3) {
                        selected_unit_slot -= 3;
                    }
                }
            }
            if (((DivideUnsigned32(selected_unit_slot, 3) << 0x18) == 0) && (var_r8 == 0)) {
                asm volatile(
                    ".syntax unified\n\t"
                    "movs r1, #1\n\t"
                    "mov r0, r9\n\t"
                    "eors r0, r1\n\t"
                    "lsls r0, r0, #24\n\t"
                    "lsrs %0, r0, #24\n\t"
                    ".syntax divided"
                    : "=r"(temp_r4_8) : : "r0", "r1");
                temp_r5_2 = 2 - selected_unit_slot;
                if ((IsBattleUnitActive(temp_r4_8, temp_r5_2) << 0x18) != 0) {
                    selected_side = temp_r4_8;
                    selected_unit_slot = temp_r5_2;
                    asm volatile("" ::: "memory");
                    goto block_66;
                }
                goto first_scan_start;
first_scan_found:
                selected_side = scan_side_1;
                selected_unit_slot = temp_r4_9;
                PlaySong(0x40);
                goto first_scan_done;
first_scan_start:
                var_r5_6 = 0;
                scan_side_1 = temp_r4_8;
                do {
                    temp_r4_9 = var_r5_6;
                    if ((IsBattleUnitActive(scan_side_1, temp_r4_9) << 0x18) != 0) {
                        goto first_scan_found;
                    }
                    temp_r0 = (var_r5_6 << 0x10) + 0x10000;
                    var_r5_6 = (u16) (temp_r0 >> 0x10);
                } while ((s32) ((s32) temp_r0 >> 0x10) <= 2);
first_scan_done:
                if (var_r5_6 == 3) {
                    temp_r4_10 = selected_side ^ 1;
                    temp_r5_3 = 5 - selected_unit_slot;
                    if ((IsBattleUnitActive(temp_r4_10, temp_r5_3) << 0x18) != 0) {
                        selected_side = temp_r4_10;
                        selected_unit_slot = temp_r5_3;
                        goto block_66;
                    }
                    goto second_scan_start;
second_scan_found:
                    selected_side = scan_side_2;
                    selected_unit_slot = temp_r4_11;
block_66:
                    PlaySong(0x40);
                    goto second_scan_done;
second_scan_start:
                    var_r5_7 = 3;
                    scan_side_2 = temp_r4_10;
loop_68:
                    temp_r4_11 = var_r5_7;
                    if ((IsBattleUnitActive(scan_side_2, temp_r4_11) << 0x18) == 0) {
                        temp_r0_2 = (var_r5_7 << 0x10) + 0x10000;
                        var_r5_7 = (u16) (temp_r0_2 >> 0x10);
                        if ((s32) ((s32) temp_r0_2 >> 0x10) <= 5) {
                            goto loop_68;
                        }
                    } else {
                        goto second_scan_found;
                    }
second_scan_done:
                    ;
                }
        }
        }
        }
        asm volatile(
            ".syntax unified\n\t"
            "mov r3, r9\n\t"
            "lsls r3, r3, #1\n\t"
            "str r3, [sp, #32]\n\t"
            ".syntax divided"
            : : : "r3", "memory");
        if (previous_side_or_base_stats_address == selected_side) {
        register u16 keys_r1 asm("r1") = *(u16 *)0x03006034;
        register s32 reload_guard_r3 asm("r3");
        asm volatile("" : "=r"(reload_guard_r3));
        if (((BATTLE_STATUS_KEY_LEFT & keys_r1) && (previous_side_or_base_stats_address != 0)) || ((BATTLE_STATUS_KEY_RIGHT & keys_r1) && (previous_side_or_base_stats_address == 0))) {
            asm volatile("" : : "r"(reload_guard_r3));
            temp_r0_3 = DivideUnsigned32(selected_unit_slot, 3) << 0x18;
            asm volatile(
                ".syntax unified\n\t"
                "mov r1, r9\n\t"
                "lsls r1, r1, #1\n\t"
                "str r1, [sp, #32]\n\t"
                ".syntax divided"
                : : : "r1", "memory");
            if (temp_r0_3 == 0) {
                temp_r4_12 = selected_unit_slot + 3;
                if ((IsBattleUnitActive(selected_side, temp_r4_12) << 0x18) != 0) {
                    selected_unit_slot = temp_r4_12;
                    PlaySong(0x40);
                } else {
                    register s32 loop_index asm("r5") = 3;
                    register s32 loop_signed asm("r6");
                    while ((s32) ({
                        asm volatile(
                            ".syntax unified\n\t"
                            "lsls r0, %1, #16\n\t"
                            "asrs %0, r0, #16\n\t"
                            ".syntax divided"
                            : "=r"(loop_signed) : "r"(loop_index) : "r0");
                        loop_signed;
                    }) <= 5) {
                        temp_r4_13 = (u8) loop_index;
                        asm volatile("" :: "r"(loop_index));
                        if ((IsBattleUnitActive(selected_side, temp_r4_13) << 0x18) != 0) {
                            selected_unit_slot = temp_r4_13;
                            PlaySong(0x40);
                            break;
                        }
                        asm volatile(
                            ".syntax unified\n\t"
                            "adds r0, %1, #1\n\t"
                            "lsls r0, r0, #16\n\t"
                            "lsrs %0, r0, #16\n\t"
                            ".syntax divided"
                            : "=r"(loop_index) : "r"(loop_signed) : "r0");
                    }
                }
            }
        }
        }
        if ((selected_side == previous_side_or_base_stats_address) && STORE_SKIP_SLOT4() &&
            (selected_unit_slot == VIEW_R3(stack.previous_unit_slot))) {

        } else {
            register u32 work_r4 asm("r4") = selected_side;
            register u32 side4_r5 asm("r5") = work_r4 << 2;
            register u32 address_r2 asm("r2");
            register u32 slot_address_r0 asm("r0");
            register u32 base_r1 asm("r1") = 0x02034B4C;
            register void *initial_record_r2 asm("r2");
            address_r2 = side4_r5 + work_r4;
            address_r2 = ((address_r2 << 3) - work_r4) << 7;
            work_r4 = selected_unit_slot << 2;
            slot_address_r0 = work_r4 + selected_unit_slot;
            slot_address_r0 = ((slot_address_r0 << 3) - selected_unit_slot) << 4;
            slot_address_r0 += base_r1;
            address_r2 += slot_address_r0;
            selected_unit_record = (void *) address_r2;
            {
                register u32 reference_seed_r0 asm("r0") = BATTLE_STATUS_BASE_STATS_RAM;
                previous_side_or_base_stats_address = reference_seed_r0;
                CopyBytes(reference_seed_r0, selected_unit_record, 0x70);
            }
            RecalculateZoidStats(previous_side_or_base_stats_address, selected_unit_record + 0x70);
            {
                register u32 flags_r1 asm("r1");
                asm volatile(
                    ".syntax unified\n\t"
                    "mov %1, r8\n\t"
                    "ldrh %0, [%1, #4]\n\t"
                    ".syntax divided"
                    : "=r"(flags_r1), "=r"(initial_record_r2));
                unit_flag40 = 0x40 & flags_r1;
            }
            stack.selected_side_times_four = side4_r5;
            stack.selected_unit_slot_times_four = work_r4;
            if (unit_flag40 == 0) {
                {
                    current_hp = BATTLE_UNIT_FIELD(initial_record_r2, s16, hp);
                    hp_warning_color = 0;
                    hp_threshold_or_base_max_hp = DivideSigned32(BATTLE_UNIT_FIELD(initial_record_r2, s16, max_hp), 0xA);
                }
                {
                    register void *record_r2 asm("r2") = selected_unit_record;
                    if ((s32) BATTLE_UNIT_FIELD(record_r2, s16, hp) < (s32) hp_threshold_or_base_max_hp) {
                        hp_warning_color = BATTLE_STATUS_LOW_HP_COLOR;
                    }
                }
                PrintWindowNumberAtWide(current_hp, 4, hp_warning_color, 0xA, 2, 4, (s32) unit_flag40);
                {
                    register void *record_r2 asm("r2") = selected_unit_record;
                    current_max_hp = BATTLE_UNIT_FIELD(record_r2, s16, max_hp);
                }
                max_hp_comparison_value = current_max_hp;
                asm volatile("" : "+r"(max_hp_comparison_value));
                {
                    register void *reference_r2 asm("r2") = (void *) previous_side_or_base_stats_address;
                    hp_threshold_or_base_max_hp = BATTLE_UNIT_FIELD(reference_r2, s16, max_hp);
                }
                if ((s32) max_hp_comparison_value <= (s32) hp_threshold_or_base_max_hp) {
                    max_hp_comparison_value ^= hp_threshold_or_base_max_hp;
                    max_hp_comparison_color = (u32) ((0 - max_hp_comparison_value) | max_hp_comparison_value) >> 0x1F;
                } else {
                    max_hp_comparison_color = BATTLE_STATUS_CURRENT_ABOVE_BASE;
                }
                PrintWindowNumberAtWide(current_max_hp, 4, max_hp_comparison_color, 0xA, 2, 9, BATTLE_STATUS_ROW_HP);
                {
                    register void *record_r2 asm("r2") = selected_unit_record;
                    PrintWindowNumberAt(BATTLE_UNIT_FIELD(record_r2, s16, ep), 4, 0U, 0xA, 2, 4, BATTLE_STATUS_ROW_EP);
                }
                {
                    register void *record_r2 asm("r2") = selected_unit_record;
                    current_max_ep = BATTLE_UNIT_FIELD(record_r2, s16, max_ep);
                }
                {
                    register s32 current_r0 asm("r0") = current_max_ep;
                    {
                        register void *reference_r2 asm("r2") = stack.base_stats_record;
                        register s32 offset_r4 asm("r4") = BATTLE_UNIT_OFFSET(max_ep);
                        base_max_ep = M2C_FIELD(reference_r2, s16 *, offset_r4);
                    }
                    if ((s32) current_r0 <= (s32) base_max_ep) {
                        max_ep_comparison_difference = base_max_ep ^ current_r0;
                        max_ep_comparison_color = (u32) ((0 - max_ep_comparison_difference) | max_ep_comparison_difference) >> 0x1F;
                    } else {
                        max_ep_comparison_color = BATTLE_STATUS_CURRENT_ABOVE_BASE;
                    }
                }
                PrintWindowNumberAtWide(current_max_ep, 4, max_ep_comparison_color, 0xA, 2, 9, BATTLE_STATUS_ROW_EP);
                current_ep_regen = BATTLE_UNIT_FIELD(selected_unit_record, s16, ep_regen);
                {
                    register s32 current_r0 asm("r0") = current_ep_regen;
                    {
                        register s16 *reference_r4 asm("r4") = (s16 *)(BATTLE_STATUS_BASE_STATS_RAM + BATTLE_UNIT_OFFSET(ep_regen));
                        base_ep_regen = *reference_r4;
                    }
                    if ((s32) current_r0 <= (s32) base_ep_regen) {
                        ep_regen_comparison_difference = base_ep_regen ^ current_r0;
                        ep_regen_comparison_color = (u32) ((0 - ep_regen_comparison_difference) | ep_regen_comparison_difference) >> 0x1F;
                    } else {
                        ep_regen_comparison_color = BATTLE_STATUS_CURRENT_ABOVE_BASE;
                    }
                }
                PrintWindowNumberAtWide(current_ep_regen, 4, ep_regen_comparison_color, 0xA, 2, 4, BATTLE_STATUS_ROW_EP_REGEN);
                current_speed = BATTLE_UNIT_FIELD(selected_unit_record, s16, speed);
                {
                    register s32 current_r0 asm("r0") = current_speed;
                    {
                        register s16 *reference_r4 asm("r4") = (s16 *)(BATTLE_STATUS_BASE_STATS_RAM + BATTLE_UNIT_OFFSET(speed));
                        base_speed = *reference_r4;
                    }
                    if ((s32) current_r0 <= (s32) base_speed) {
                        speed_comparison_difference = base_speed ^ current_r0;
                        speed_comparison_color = (u32) ((0 - speed_comparison_difference) | speed_comparison_difference) >> 0x1F;
                    } else {
                        speed_comparison_color = BATTLE_STATUS_CURRENT_ABOVE_BASE;
                    }
                }
                PrintWindowNumberAtWide(current_speed, 4, speed_comparison_color, 0xA, 2, 4, BATTLE_STATUS_ROW_SPEED);
                current_mobility = BATTLE_UNIT_FIELD(selected_unit_record, s16, mobility);
                {
                    register s32 current_r0 asm("r0") = current_mobility;
                    {
                        register s16 *reference_r4 asm("r4") = (s16 *)0x02033F08;
                        base_mobility = *reference_r4;
                    }
                    if ((s32) current_r0 <= (s32) base_mobility) {
                        mobility_comparison_difference = base_mobility ^ current_r0;
                        mobility_comparison_color = (u32) ((0 - mobility_comparison_difference) | mobility_comparison_difference) >> 0x1F;
                    } else {
                        mobility_comparison_color = BATTLE_STATUS_CURRENT_ABOVE_BASE;
                    }
                }
                PrintWindowNumberAtWide(current_mobility, 4, mobility_comparison_color, 0xA, 2, 4, BATTLE_STATUS_ROW_MOBILITY);
                {
                    register void *record_r1 asm("r1") = selected_unit_record;
                    current_initiative = BATTLE_UNIT_FIELD(record_r1, s16, initiative);
                }
                {
                    register s32 current_r0 asm("r0") = current_initiative;
                    {
                        register void *reference_r2 asm("r2") = stack.base_stats_record;
                        base_initiative = BATTLE_UNIT_FIELD(reference_r2, s16, initiative);
                    }
                    if ((s32) current_r0 <= (s32) base_initiative) {
                        initiative_comparison_difference = base_initiative ^ current_r0;
                        initiative_comparison_color = (u32) ((0 - initiative_comparison_difference) | initiative_comparison_difference) >> 0x1F;
                    } else {
                        initiative_comparison_color = BATTLE_STATUS_CURRENT_ABOVE_BASE;
                    }
                }
                PrintWindowNumberAtWide(current_initiative, 4, initiative_comparison_color, 0xA, 2, 4, BATTLE_STATUS_ROW_INITIATIVE);
                current_defense = BATTLE_UNIT_FIELD(selected_unit_record, s16, defense);
                {
                    register s32 current_r0 asm("r0") = current_defense;
                    {
                        register s16 *reference_r2 asm("r2") = (s16 *)0x02033F0A;
                        base_defense = *reference_r2;
                    }
                    if ((s32) current_r0 <= (s32) base_defense) {
                        defense_comparison_difference = base_defense ^ current_r0;
                        defense_comparison_color = (u32) ((0 - defense_comparison_difference) | defense_comparison_difference) >> 0x1F;
                    } else {
                        defense_comparison_color = BATTLE_STATUS_CURRENT_ABOVE_BASE;
                    }
                }
                PrintWindowNumberAtWide(current_defense, 4, defense_comparison_color, 0xA, 2, 4, BATTLE_STATUS_ROW_DEFENSE);
                current_armor_rate = BATTLE_UNIT_FIELD(selected_unit_record, s16, armor_rate);
                {
                    register s32 current_r0 asm("r0") = current_armor_rate;
                    {
                        register s16 *reference_r2 asm("r2") = (s16 *)(BATTLE_STATUS_BASE_STATS_RAM + BATTLE_UNIT_OFFSET(armor_rate));
                        base_armor_rate = *reference_r2;
                    }
                    if ((s32) current_r0 <= (s32) base_armor_rate) {
                        armor_rate_comparison_difference = base_armor_rate ^ current_r0;
                        armor_rate_comparison_color = (u32) ((0 - armor_rate_comparison_difference) | armor_rate_comparison_difference) >> 0x1F;
                    } else {
                        armor_rate_comparison_color = BATTLE_STATUS_CURRENT_ABOVE_BASE;
                    }
                }
                PrintWindowNumberAtWide(current_armor_rate, 4, armor_rate_comparison_color, 0xA, 2, 4, BATTLE_STATUS_ROW_ARMOR_RATE);
                {
                    register void *record_r2 asm("r2") = selected_unit_record;
                    current_dcp = BATTLE_UNIT_FIELD(record_r2, s16, dcp);
                }
                {
                    register s32 current_r0 asm("r0") = current_dcp;
                    {
                        register void *reference_r2 asm("r2") = stack.base_stats_record;
                        register s32 offset_r4 asm("r4") = BATTLE_UNIT_OFFSET(dcp);
                        base_dcp = M2C_FIELD(reference_r2, s16 *, offset_r4);
                    }
                    if ((s32) current_r0 <= (s32) base_dcp) {
                        dcp_comparison_difference = base_dcp ^ current_r0;
                        dcp_comparison_color = (u32) ((0 - dcp_comparison_difference) | dcp_comparison_difference) >> 0x1F;
                    } else {
                        dcp_comparison_color = BATTLE_STATUS_CURRENT_ABOVE_BASE;
                    }
                }
                CALL_STAT_ROW(current_dcp, dcp_comparison_color, 0xA);
                current_sensor_accuracy = LOAD_BATTLE_SENSOR_ACCURACY(selected_unit_record);
                {
                    register s32 current_r0 asm("r0") = current_sensor_accuracy;
                    asm volatile("C8538_PATCH_FINAL_REFERENCE");
                    base_sensor_accuracy = *(s16 *)(BATTLE_STATUS_BASE_STATS_RAM + BATTLE_UNIT_OFFSET(sensor_accuracy));
                    if ((s32) current_r0 <= (s32) base_sensor_accuracy) {
                        sensor_accuracy_comparison_difference = base_sensor_accuracy ^ current_r0;
                        sensor_accuracy_comparison_color = (u32) ((0 - sensor_accuracy_comparison_difference) | sensor_accuracy_comparison_difference) >> 0x1F;
                    } else {
                        sensor_accuracy_comparison_color = BATTLE_STATUS_CURRENT_ABOVE_BASE;
                    }
                }
                CALL_STAT_ROW(current_sensor_accuracy, sensor_accuracy_comparison_color, 0xC);
            } else {
                asm volatile("C8538_PATCH_STACK_ARG r2");
                PrintWindowTextAt(BATTLE_STATUS_STAT_PLACEHOLDER_TEXT, 0, 2, 4, BATTLE_STATUS_ROW_HP);
                asm volatile("C8538_PATCH_STACK_ARG r3");
                PrintWindowTextAt(BATTLE_STATUS_STAT_PLACEHOLDER_TEXT, 0, 2, 9, BATTLE_STATUS_ROW_HP);
                PrintWindowTextAt(BATTLE_STATUS_STAT_PLACEHOLDER_TEXT, 0, 2, 4, BATTLE_STATUS_ROW_EP);
                PrintWindowTextAt(BATTLE_STATUS_STAT_PLACEHOLDER_TEXT, 0, 2, 9, BATTLE_STATUS_ROW_EP);
                asm volatile("C8538_PATCH_STACK_ARG r4");
                PrintWindowTextAt(BATTLE_STATUS_STAT_PLACEHOLDER_TEXT, 0, 2, 4, BATTLE_STATUS_ROW_EP_REGEN);
                asm volatile("C8538_PATCH_STACK_ARG r0");
                PrintWindowTextAt(BATTLE_STATUS_STAT_PLACEHOLDER_TEXT, 0, 2, 4, BATTLE_STATUS_ROW_SPEED);
                PrintWindowTextAt(BATTLE_STATUS_STAT_PLACEHOLDER_TEXT, 0, 2, 4, BATTLE_STATUS_ROW_MOBILITY);
                PrintWindowTextAt(BATTLE_STATUS_STAT_PLACEHOLDER_TEXT, 0, 2, 4, BATTLE_STATUS_ROW_INITIATIVE);
                PrintWindowTextAt(BATTLE_STATUS_STAT_PLACEHOLDER_TEXT, 0, 2, 4, BATTLE_STATUS_ROW_DEFENSE);
                PrintWindowTextAt(BATTLE_STATUS_STAT_PLACEHOLDER_TEXT, 0, 2, 4, BATTLE_STATUS_ROW_ARMOR_RATE);
                PrintWindowTextAt(BATTLE_STATUS_STAT_PLACEHOLDER_TEXT, 0, 2, 4, BATTLE_STATUS_ROW_DCP);
                PrintWindowTextAt(BATTLE_STATUS_STAT_PLACEHOLDER_TEXT, 0, 2, 4, BATTLE_STATUS_ROW_SENSOR_ACCURACY);
            }
            if (FindBattleEffect(selected_side, selected_unit_slot, BATTLE_EFFECT_FREEZE) != BATTLE_EFFECT_NOT_FOUND) {
                PrintWindowTextAt(0x08107660, 0, 2, 0, BATTLE_STATUS_ROW_FREEZE);
                PrintWindowText(0x08107664, 1, 2);
            } else {
                PrintWindowTextAt(0x0810766C, 0, 2, 0, BATTLE_STATUS_ROW_FREEZE);
            }
            if (FindBattleEffect(selected_side, selected_unit_slot, BATTLE_EFFECT_PILOT_INACTIVE) != BATTLE_EFFECT_NOT_FOUND) {
                PrintWindowTextAt(0x08107660, 0, 2, 0, BATTLE_STATUS_ROW_PILOT_INACTIVE);
                PrintWindowText(0x08107674, 1, 2);
            } else {
                PrintWindowTextAt(0x08107680, 0, 2, 0, BATTLE_STATUS_ROW_PILOT_INACTIVE);
            }
            if (FindBattleEffect(selected_side, selected_unit_slot, BATTLE_EFFECT_EXTRA_TURNS) != BATTLE_EFFECT_NOT_FOUND) {
                PrintWindowTextAt(0x08107660, 0, 2, 0, BATTLE_STATUS_ROW_EXTRA_TURNS);
                PrintWindowText(0x0810768C, 2, 2);
            } else {
                PrintWindowTextAt(0x0810766C, 0, 2, 0, BATTLE_STATUS_ROW_EXTRA_TURNS);
            }
            if (FindBattleEffect(selected_side, selected_unit_slot, BATTLE_EFFECT_ENERGY_SHIELD) != BATTLE_EFFECT_NOT_FOUND) {
                PrintWindowTextAt(0x08107660, 0, 2, 0, BATTLE_STATUS_ROW_ENERGY_SHIELD);
                PrintWindowText(0x08107694, 2, 2);
            } else {
                PrintWindowTextAt(0x081076A0, 0, 2, 0, BATTLE_STATUS_ROW_ENERGY_SHIELD);
            }
            ClearWindow(3);
            {
                register s32 *names_r1 asm("r1") = (s32 *)0x087EDD54;
                register void *record_r2 asm("r2") = selected_unit_record;
                asm volatile("" : "+r"(record_r2));
                asm volatile("C8538_PATCH_FIRST_NAME_CALL");
                PrintWindowTextAt(names_r1[BATTLE_UNIT_FIELD(record_r2, u8, zoid_id)], 0, 3, 0, 0);
            }
            ClearWindow(4);
            {
                register u8 *base_r2 asm("r2") = (u8 *)0x02034B4C;
                register u32 slot4_r4 asm("r4") = stack.selected_unit_slot_times_four;
                register u32 address_r1 asm("r1");
                register u32 side_offset_r0 asm("r0");
                register u32 side_r3 asm("r3");
                address_r1 = slot4_r4 + selected_unit_slot;
                address_r1 = ((address_r1 << 3) - selected_unit_slot) << 4;
                side_offset_r0 = stack.selected_side_times_four;
                side_offset_r0 += selected_side;
                side_offset_r0 <<= 3;
                side_r3 = selected_side;
                asm volatile("" : "+r"(side_r3));
                side_offset_r0 = (side_offset_r0 - side_r3) << 7;
                address_r1 += side_offset_r0;
                address_r1 += (u32) base_r2;
                address_r1 += 0x70;
                address_r1 = M2C_FIELD(address_r1, u8 *, 0);
                {
                    register s32 name_result_r0 asm("r0");
                    name_result_r0 = GetBattlePilotDisplayName(selected_side, address_r1);
                    asm volatile("C8538_PATCH_STACK_ARG r4\n\tC8538_INSERT_R3_BEFORE_BL"
                        : "+r"(name_result_r0));
                    PrintWindowTextAt(name_result_r0, 0, 4, 0, 0);
                }
            }
            RequestWindowRefresh();
        }
        StartBattleCameraTransition(BATTLE_CAMERA_FOCUS_UNIT, selected_side, selected_unit_slot, 0);
        {
            register u8 *entry asm("r0");
            register u8 *table asm("r1") = (u8 *)0x02032E8C;
            register u32 slot_offset asm("r2");
            asm volatile("" : "+r"(table));
            entry = (u8 *) stack.selected_side_times_two;
            entry += selected_side;
            entry = (u8 *) ((u32) entry << 3);
            slot_offset = stack.selected_unit_slot_times_four;
            entry = (u8 *)(slot_offset - (0 - (u32) entry));
            entry += (u32) table;
            {
                register u16 selected_icon_x asm("r1");
                register void *cursor_record asm("r3");
                selected_icon_x = M2C_FIELD(M2C_FIELD(entry, void **, 0), u16 *, 4);
                cursor_record = stack.cursor_sprite;
                BATTLE_SPRITE_FIELD(cursor_record, u16, x) = selected_icon_x;
            }
            entry = M2C_FIELD(entry, void **, 0);
            cursor_scale = BATTLE_SPRITE_FIELD(entry, s16, scale);
            if ((s32) cursor_scale < 0) {
                cursor_scale += 7;
            }
            asm volatile("C8538_DELAY_FINAL_POINTER");
            BATTLE_SPRITE_FIELD(stack.cursor_sprite, s16, y) =
                (s16) (BATTLE_SPRITE_FIELD(entry, u16, y) - (cursor_scale >> 3));
        }
        YieldTaskForUpdates(1);
        previous_side_or_base_stats_address = selected_side;
        stack.previous_unit_slot = (s32) selected_unit_slot;
    } while (!(BATTLE_STATUS_CLOSE_KEYS & *(u16 *)0x0300000E));
    PlaySong(0x3F);
    DestroySprite(stack.cursor_sprite);
    RunMenuScript(BATTLE_STATUS_CLOSE_MENU);
    {
        u8 *control = (u8 *)0x030033C4;
        CAMERA_FIELD(control, s32, projection.screen_center_x) = 0x78;
        CAMERA_FIELD(control, s32, projection.screen_center_y) = 0x78;
    }
}
