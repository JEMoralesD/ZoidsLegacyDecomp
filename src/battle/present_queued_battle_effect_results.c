#include "m2c_prelude.h"
#include "popups/battle_popup.h"
#include "battle_display.h"
#include "deck_commands/deck_commands.h"
#include "combinations/battle_combination.h"
#include "../graphics/camera.h"

M2C_UNK PlaySong(s32) asm("func_08092E84");                         /* extern */
M2C_UNK LoadZoidIconGraphics(u8, u8, u16, u8) asm("func_0809A4CC");             /* extern */
M2C_UNK UpdateBattleUnitGaugeGraphics() asm("func_080BAB3C");                            /* extern */
M2C_UNK StartBattleCameraTransition(s32, s32, s32, s32) asm("func_080BB224");          /* extern */
M2C_UNK StartBattleCameraTransitionToPose(struct CameraWorldOffset *, s32 *, s32) asm("func_080BB424"); /* extern */
s32 IsBattleCameraTransitionComplete() asm("func_080BB654");                                /* extern */
M2C_UNK MarkBattleUnitDestroyed(s32, u8) asm("func_080C04DC");                     /* extern */
M2C_UNK LoadBattlePopupGraphics(s32, s32) asm("func_080C9F00");                    /* extern */
M2C_UNK StartBattleUnitPopup(s32, s32, u8, s32) asm("func_080CA0AC");           /* extern */
s32 AreBattleUnitPopupsFinished() asm("func_080CA140");                                /* extern */
s32 IsBattleUnitActive(u32, u32) asm("func_080E9D88");                        /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */
M2C_UNK jtbl_080C60E0();                            /* static */
M2C_UNK jtbl_080C6380();                            /* static */
extern u8 gBattleState[];
extern s32 gBattleUnitWorldPositions[] asm("D_087A2790");
extern s32 gBattleUnitWorldPositionsZColumn[] asm("D_087A2798");

static inline u8 *switch_entry_address(u8 *base, u32 index) {
    u32 address;

    address = index * (u32)sizeof(struct BattleEffect);
    asm volatile("add %0, %1, %0" : "+r"(address) : "r"(base));
    return (u8 *)address;
}

static inline u32 add_address_accumulator(u32 address, u32 addend) {
    asm volatile("add %0, %0, %1" : "+r"(address) : "r"(addend));
    return address;
}

void PresentQueuedBattleEffectResults(u8 camera_mode) asm("func_080C5DB4");

void PresentQueuedBattleEffectResults(u8 camera_mode) {
    struct CameraWorldOffset camera_position;
    s32 orientation_words[2];
    s32 saved_camera_mode;
    s32 applied_popup_value;
    s32 removed_popup_value;
    s32 pitch_word;
    s32 destroyed_side_offset;
    s32 destroyed_unit_offset;
    s32 camera_world_z;
    s32 applied_value_popup_kind;
    s32 applied_label_popup_kind;
    s32 removed_value_popup_kind;
    s32 removed_label_popup_kind;
    s32 no_damage_popups;
    register s32 phase_has_results asm("sl");
    u8 *applied_unit_queue;
    u8 *removed_unit_queue;
    register u16 damage_result_flags asm("r1");
    u16 applied_kind_flags;
    u16 removed_kind_flags;
    u32 camera_row_x_sum;
    u32 applied_kind_dispatch;
    u32 removed_kind_dispatch;
    u32 opponent_side;
    register u32 display_side asm("r5");
    u8 applied_unit_slot;
    u8 removed_unit_slot;
    u8 unit_slot;
    u8 applied_effect_slot;
    u8 removed_effect_slot;
    u8 destroyed_unit_slot;
    s32 destroyed_record_offset;
    void *destroyed_unit_record;
    void *opponent_unit_record;
    void *damage_unit_queue;
    void *enemy_reward_record;
    void *opponent_icon_sprite;

    saved_camera_mode = (s32) camera_mode;
    {
        register u8 *initial_side_address asm("r0");
        register u32 initial_side_offset asm("r1");

        initial_side_address = gBattleState;
        initial_side_offset = BATTLE_COMBINATION_OFFSET(active_side);
        asm volatile("add %0, %0, %1"
                     : "+r"(initial_side_address)
                     : "r"(initial_side_offset));
        display_side = *initial_side_address;
    }
    do {
        s32 side_record_offset;
        u32 side_object_offset;
        u8 *record_base;
        void **object_base;

        {
            register s32 initial_flag_zero asm("r2");

            initial_flag_zero = 0;
            asm volatile("" : "+r"(initial_flag_zero));
            phase_has_results = initial_flag_zero;
        }
        unit_slot = 0;
        record_base = gBattleState;
        side_record_offset = display_side * ((s32)sizeof(struct BattleUnitEffectDisplayQueue) * BATTLE_ACTIVE_UNIT_COUNT);
        object_base = (void **)0x02032EBC;
        side_object_offset = display_side * 0x18;
hide_queued_unit_gauges:
        if (BATTLE_EFFECT_DISPLAY_PENDING & *(u16 *)(add_address_accumulator(
                             add_address_accumulator(unit_slot * (s32)sizeof(struct BattleUnitEffectDisplayQueue),
                                                     side_record_offset),
                             (u32)record_base) +
                         BATTLE_EFFECT_DISPLAY_QUEUE_OFFSET(units[0][0].flags))) {
            *(s32 *)(*(void **)add_address_accumulator(
                           add_address_accumulator(unit_slot * 4,
                                                   side_object_offset),
                           (u32)object_base) +
                       BATTLE_SPRITE_OFFSET(user_data.gauge.override_flags)) = BATTLE_SPRITE_HIDDEN;
            phase_has_results = 1;
        }
        unit_slot += 1;
        if ((u32) unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
            goto hide_queued_unit_gauges;
        }
        YieldTaskForUpdates(1);
        if (phase_has_results != 0) {
            if (saved_camera_mode != 0) {
                if (saved_camera_mode == BATTLE_EFFECT_CAMERA_QUEUED_UNITS) {
                register s32 *position_table asm("r6");
                register s32 *average_table asm("sl");
                s32 *side_position_base;
                u32 side_position_address;
                register s32 saved_orientation_pitch_word asm("ip");
                register volatile s32 *orientation_word_address asm("r8");
                register s32 case_side_record_offset asm("r4");
                u32 rear_row_position_index;
                register u8 *case_record_base asm("r9");

                camera_position.world_x = 0;
                unit_slot = 0;
                position_table = gBattleUnitWorldPositions;
                saved_orientation_pitch_word = orientation_words[0];
                orientation_word_address = orientation_words;
                case_record_base = gBattleState;
                case_side_record_offset = display_side * ((s32)sizeof(struct BattleUnitEffectDisplayQueue) * BATTLE_ACTIVE_UNIT_COUNT);
                average_table = position_table;
                side_position_address = display_side * 0x48;
                side_position_address += (u32)position_table;
                side_position_base = (s32 *)side_position_address;
                rear_row_position_index = (display_side * 6) + 3;
find_queued_camera_rows:
                if (BATTLE_EFFECT_DISPLAY_PENDING & *(u16 *)(case_record_base +
                                 ((unit_slot * (s32)sizeof(struct BattleUnitEffectDisplayQueue)) + case_side_record_offset) +
                                 BATTLE_EFFECT_DISPLAY_QUEUE_OFFSET(units[0][0].flags))) {
                    if (unit_slot > 2U) {
                        if (camera_position.world_x != 0) {
                            camera_row_x_sum = camera_position.world_x + average_table[rear_row_position_index * 3];
                            camera_position.world_x = (s32) (camera_row_x_sum + (camera_row_x_sum >> 0x1F)) >> 1;
                            goto block_20;
                        }
                        goto block_22;
                    } else {
                        if (camera_position.world_x == 0) {
                            camera_position.world_x = *side_position_base;
                            unit_slot = 2;
                        }
                        goto block_18;
                    }
                } else {
block_18:
                    unit_slot = (u8) (unit_slot + 1);
                    if ((u32) unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                        goto find_queued_camera_rows;
                    }
                }
block_20:
                if (display_side == 0) {
                    camera_world_z = position_table[2];
                    goto block_24;
                }
                goto block_23;
block_22:
                {
                    s32 rear_row_fallback_index;

                    rear_row_fallback_index = (display_side * 6) + 3;
                    camera_position.world_x = position_table[rear_row_fallback_index * 3];
                }
                goto block_20;
block_23:
                {
                    s32 nonzero_side_z_index;
                    s32 nonzero_side_z_offset;
                    u8 *nonzero_side_z_address_or_value;

                    nonzero_side_z_index = (display_side * 6) + 2;
                    nonzero_side_z_offset = nonzero_side_z_index * (u32)sizeof(struct BattleEffect);
                    nonzero_side_z_address_or_value = (u8 *)position_table;
                    nonzero_side_z_address_or_value += 8;
                    asm volatile(
                        "add %0, %0, %1\n\t"
                        "ldr %1, [%0]"
                        : "+r"(nonzero_side_z_offset), "+r"(nonzero_side_z_address_or_value)
                        :
                        : "memory");
                    camera_world_z = (s32)(u32)nonzero_side_z_address_or_value;
                    asm volatile("" : "+r"(position_table));
                }
block_24:
                camera_position.world_z = camera_world_z;
                {
                    s32 max_side_record_offset;
                    u32 max_position_index_base;
                    s32 *max_position_table;
                    u8 *max_record_base;

                    unit_slot = 0;
                    max_record_base = gBattleState;
                    max_side_record_offset = display_side * ((s32)sizeof(struct BattleUnitEffectDisplayQueue) * BATTLE_ACTIVE_UNIT_COUNT);
                    max_position_index_base = display_side * 6;
                    max_position_table = gBattleUnitWorldPositionsZColumn;
                    do {
                        u32 max_record_address;

                        max_record_address = (unit_slot * (s32)sizeof(struct BattleUnitEffectDisplayQueue)) +
                                             max_side_record_offset;
                        max_record_address += (u32)max_record_base;
                        if (BATTLE_EFFECT_DISPLAY_PENDING & *(u16 *)(max_record_address + BATTLE_EFFECT_DISPLAY_QUEUE_OFFSET(units[0][0].flags))) {
                            if (camera_position.world_z < max_position_table[
                                    (max_position_index_base + unit_slot) * 3]) {
                                camera_position.world_z = max_position_table[
                                    (max_position_index_base + unit_slot) * 3];
                            }
                        }
                        unit_slot += 1;
                    } while ((u32) unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1);
                }
                camera_position.depth_offset = 0x8000;
                pitch_word = (0xFFFF0000 & saved_orientation_pitch_word) | 0x20;
                orientation_words[0] = pitch_word;
                {
                    volatile s32 *orientation_word_access;

                    orientation_word_access = orientation_word_address;
                    orientation_word_access[1] = (s32) ((u32) orientation_word_access[1] & 0xFFFF0000);
                }
                {
                    u32 pitch_halfword_mask;

                    pitch_halfword_mask = 0xFFFF;
                    orientation_words[0] = (s32) ((u32) orientation_words[0] & pitch_halfword_mask);
                }
                StartBattleCameraTransitionToPose(&camera_position, (s32 *)orientation_word_address, 0);
                } else if (({
                               register s32 mode_two_check asm("r2");

                               mode_two_check = saved_camera_mode;
                               asm volatile("" : "+r"(mode_two_check));
                               mode_two_check;
                           }) == BATTLE_EFFECT_CAMERA_FOCUS_SIDE) {
                    StartBattleCameraTransition(2, display_side, 0, 0);
                }
                while ((IsBattleCameraTransitionComplete() << 0x18) == 0) {
                    YieldTaskForUpdates(1);
                }
            }
            {
                s32 action_side_offset;

                {
                    register s32 action_flag_zero asm("r3");

                    action_flag_zero = 0;
                    asm volatile("" : "+r"(action_flag_zero));
                    phase_has_results = action_flag_zero;
                }
                no_damage_popups = 1;
                unit_slot = 0;
                action_side_offset = display_side * ((s32)sizeof(struct BattleUnitEffectDisplayQueue) * BATTLE_ACTIVE_UNIT_COUNT);
                do {
                    u32 action_record_address;

                    action_record_address = (unit_slot * (s32)sizeof(struct BattleUnitEffectDisplayQueue)) + 0x0203C774;
                    damage_unit_queue = (void *)(action_side_offset + action_record_address);
                    damage_result_flags = M2C_FIELD(damage_unit_queue, u16 *, 0);
                    if ((BATTLE_EFFECT_DISPLAY_PENDING & damage_result_flags) && (BATTLE_EFFECT_DISPLAY_DAMAGE_RESULT & damage_result_flags)) {
                        if (BATTLE_EFFECT_DISPLAY_DAMAGE & damage_result_flags) {
                            StartBattleUnitPopup(BATTLE_POPUP_DAMAGE, display_side, unit_slot, M2C_FIELD(damage_unit_queue, s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->damage));
                            no_damage_popups = 0;
                        } else if (BATTLE_EFFECT_DISPLAY_DECOY & damage_result_flags) {
                            StartBattleUnitPopup(BATTLE_POPUP_DECOY, display_side, unit_slot, M2C_FIELD(damage_unit_queue, s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->damage));
                        } else {
                            StartBattleUnitPopup(BATTLE_POPUP_MISS, display_side, unit_slot, M2C_FIELD(damage_unit_queue, s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->damage));
                        }
                        phase_has_results = 1;
                    }
                    unit_slot += 1;
                } while ((u32) unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1);
                if (phase_has_results != 0) {
                    LoadBattlePopupGraphics(display_side, BATTLE_POPUP_GRAPHICS_DAMAGE);
                    if (no_damage_popups != 0) {
                        PlaySong(0x58);
                    }
                    while (AreBattleUnitPopupsFinished() == 0) {
                        YieldTaskForUpdates(1);
                    }
                }
            }
            applied_effect_slot = 0;
present_applied_effect_slot:
            phase_has_results = 0;
            applied_unit_slot = 0;
            asm volatile("" ::
                "r"(applied_unit_slot), "r"(applied_unit_slot),
                "r"(applied_unit_slot), "r"(applied_unit_slot),
                "r"(applied_unit_slot), "r"(applied_unit_slot),
                "r"(applied_unit_slot), "r"(applied_unit_slot),
                "r"(applied_unit_slot), "r"(applied_unit_slot));
            asm volatile("" ::
                "r"(applied_unit_slot), "r"(applied_unit_slot),
                "r"(applied_unit_slot), "r"(applied_unit_slot));
present_applied_unit_effect:
            if ((IsBattleUnitActive(display_side, applied_unit_slot) << 0x18) == 0) {

            } else {
                register s32 applied_side_queue_offset asm("r2");
                register u32 applied_unit_queue_offset asm("r0");
                register u8 *applied_queue_base asm("r1");

                applied_side_queue_offset = display_side * ((s32)sizeof(struct BattleUnitEffectDisplayQueue) * BATTLE_ACTIVE_UNIT_COUNT);
                applied_unit_queue_offset = applied_unit_slot * (s32)sizeof(struct BattleUnitEffectDisplayQueue);
                applied_queue_base = (u8 *)0x0203C774;
                asm volatile("add %0, %0, %1"
                             : "+r"(applied_unit_queue_offset)
                             : "r"(applied_queue_base));
                asm volatile("add %0, %0, %1"
                             : "+r"(applied_side_queue_offset)
                             : "r"(applied_unit_queue_offset));
                applied_unit_queue = (u8 *)applied_side_queue_offset;
                if (!(BATTLE_EFFECT_DISPLAY_PENDING & *(u16 *)applied_unit_queue)) {

                } else {
                    applied_kind_flags = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), u16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].kind_flags);
                    if (applied_kind_flags == 0) {

                    } else {
                        applied_kind_dispatch = BATTLE_EFFECT_KIND_MASK;
                        applied_kind_dispatch &= applied_kind_flags;
                        applied_kind_dispatch -= 1;
                        switch (applied_kind_dispatch) {        /* switch 2; irregular */
                        case BATTLE_EFFECT_HP_RECOVERY - 1:                     /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_HP_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_MAX_HP - 1:                     /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_MAX_HP_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_DCP - 1:                     /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_DCP_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_EP_RECOVERY - 1:                     /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_EP_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_MAX_EP - 1:                     /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_MAX_EP_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_EP_REGEN - 1:                     /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_EP_REGEN_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_SPEED - 1:                     /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_SPEED_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_MOBILITY - 1:                     /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_MOBILITY_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_DEFENSE - 1:                     /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_DEFENSE_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_ARMOR_RATE - 1:                     /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_ARMOR_RATE_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_SENSOR_ACCURACY - 1:                    /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_SENSOR_ACCURACY_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_LOAD_CAPACITY - 1:                    /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_LOAD_CAPACITY_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_INITIATIVE - 1:                    /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_INITIATIVE_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_EVASION_SCORE - 1:                    /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_EVASION_SCORE_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_ATTACK_POWER - 1:                    /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_ATTACK_POWER_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_HIT_RATE - 1:                    /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_HIT_RATE_MODIFIER;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_EVASION_RATE - 1:                    /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_EVASION_BONUS;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_DOUBLE_ATTACK_POWER - 1:                    /* switch 2 */
                            applied_label_popup_kind = BATTLE_POPUP_DOUBLE_ATTACK_POWER;
                            goto show_applied_label_popup;
                        case BATTLE_EFFECT_HALF_EVASION_RATE - 1:                    /* switch 2 */
                            applied_label_popup_kind = BATTLE_POPUP_HALF_EVASION_RATE;
                            goto show_applied_label_popup;
                        case BATTLE_EFFECT_SENSOR_ACCURACY_OVERRIDE - 1:                    /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_SENSOR_ACCURACY_OVERRIDE;
                            goto show_applied_value_popup;
                        case BATTLE_EFFECT_ENERGY_SHIELD - 1:                    /* switch 2 */
                            applied_label_popup_kind = BATTLE_POPUP_ENERGY_SHIELD_ON;
                            goto show_applied_label_popup;
                        case BATTLE_EFFECT_ANTI_AIR - 1:                    /* switch 2 */
                            applied_label_popup_kind = BATTLE_POPUP_ANTI_AIR;
                            goto show_applied_label_popup;
                        case BATTLE_EFFECT_AUXILIARY_PILOT_ACTIVE - 1:                    /* switch 2 */
                            applied_label_popup_kind = BATTLE_POPUP_ORGANOID;
                            goto show_applied_label_popup;
                        case BATTLE_EFFECT_PILOT_INACTIVE - 1:                    /* switch 2 */
                            applied_label_popup_kind = BATTLE_POPUP_PILOT_DOWN_ARROW;
                            goto show_applied_label_popup;
                        case BATTLE_EFFECT_FREEZE - 1:                    /* switch 2 */
                            applied_label_popup_kind = BATTLE_POPUP_FREEZE;
                            goto show_applied_label_popup;
                        case BATTLE_EFFECT_TURN_MARKER - 1:                    /* switch 2 */
                            applied_label_popup_kind = BATTLE_POPUP_WAITING;
                            goto show_applied_label_popup;
                        case BATTLE_EFFECT_CONFUSION - 1:                    /* switch 2 */
                            applied_label_popup_kind = BATTLE_POPUP_CONFUSION;
                            goto show_applied_label_popup;
                        case BATTLE_EFFECT_EXTRA_TURNS - 1:                    /* switch 2 */
                            applied_popup_value = M2C_FIELD(switch_entry_address(applied_unit_queue, applied_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->applied_effects[0].value);
                            applied_value_popup_kind = BATTLE_POPUP_ZOS_LEVEL;
                            goto show_applied_value_popup;
show_applied_value_popup:
                            StartBattleUnitPopup(applied_value_popup_kind, display_side, applied_unit_slot, applied_popup_value);
                            goto mark_applied_popup_phase;
                        case 34:                    /* switch 2 */
                            applied_label_popup_kind = BATTLE_POPUP_BERSERK;
                            goto show_applied_label_popup;
                        case 35:                    /* switch 2 */
                            applied_label_popup_kind = BATTLE_POPUP_GREEN_STREAK_EFFECT;
                            goto show_applied_label_popup;
                        case 36:                    /* switch 2 */
                            applied_label_popup_kind = BATTLE_POPUP_DISSOLVING_ORB_EFFECT;
                            goto show_applied_label_popup;
show_applied_label_popup:
                            StartBattleUnitPopup(applied_label_popup_kind, display_side, applied_unit_slot, 0);
                            goto mark_applied_popup_phase;
                        case 37:                    /* switch 2 */
                            StartBattleUnitPopup(BATTLE_POPUP_PURPLE_RING_EFFECT, display_side, applied_unit_slot, 0);
                            goto mark_applied_popup_phase;
                        }
                        goto mark_applied_popup_phase;
mark_applied_popup_phase:
                        phase_has_results = 1;
                    }
                }
            }
            applied_unit_slot += 1;
            if ((u32) applied_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                goto present_applied_unit_effect;
            }
            if (phase_has_results != 0) {
                LoadBattlePopupGraphics(display_side, BATTLE_POPUP_GRAPHICS_MODIFIERS);
                while (AreBattleUnitPopupsFinished() == 0) {
                    YieldTaskForUpdates(1);
                }
            }
            asm volatile("" : "+r"(applied_effect_slot));
            applied_effect_slot += 1;
            if ((u32) applied_effect_slot <= BATTLE_EFFECT_SLOT_COUNT - 1) {
                goto present_applied_effect_slot;
            }
            removed_effect_slot = 0;
present_removed_effect_slot:
            phase_has_results = 0;
            removed_unit_slot = 0;
            asm volatile("" ::
                "r"(removed_unit_slot), "r"(removed_unit_slot),
                "r"(removed_unit_slot), "r"(removed_unit_slot),
                "r"(removed_unit_slot), "r"(removed_unit_slot),
                "r"(removed_unit_slot), "r"(removed_unit_slot),
                "r"(removed_unit_slot), "r"(removed_unit_slot));
            asm volatile("" ::
                "r"(removed_unit_slot), "r"(removed_unit_slot),
                "r"(removed_unit_slot), "r"(removed_unit_slot));
present_removed_unit_effect:
            if ((IsBattleUnitActive(display_side, removed_unit_slot) << 0x18) == 0) {

            } else {
                register s32 removed_side_queue_offset asm("r2");
                register u32 removed_unit_queue_offset asm("r0");
                register u8 *removed_queue_base asm("r1");

                removed_side_queue_offset = display_side * ((s32)sizeof(struct BattleUnitEffectDisplayQueue) * BATTLE_ACTIVE_UNIT_COUNT);
                removed_unit_queue_offset = removed_unit_slot * (s32)sizeof(struct BattleUnitEffectDisplayQueue);
                removed_queue_base = (u8 *)0x0203C774;
                asm volatile("add %0, %0, %1"
                             : "+r"(removed_unit_queue_offset)
                             : "r"(removed_queue_base));
                asm volatile("add %0, %0, %1"
                             : "+r"(removed_side_queue_offset)
                             : "r"(removed_unit_queue_offset));
                removed_unit_queue = (u8 *)removed_side_queue_offset;
                if (!(BATTLE_EFFECT_DISPLAY_PENDING & *(u16 *)removed_unit_queue)) {

                } else {
                    removed_kind_flags = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), u16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].kind_flags);
                    if (removed_kind_flags == 0) {

                    } else {
                        removed_kind_dispatch = BATTLE_EFFECT_KIND_MASK;
                        removed_kind_dispatch &= removed_kind_flags;
                        removed_kind_dispatch -= 1;
                        switch (removed_kind_dispatch) {        /* switch 3; irregular */
                        case BATTLE_EFFECT_HP_RECOVERY - 1:                     /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_HP_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_MAX_HP - 1:                     /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_MAX_HP_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_DCP - 1:                     /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_DCP_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_EP_RECOVERY - 1:                     /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_EP_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_MAX_EP - 1:                     /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_MAX_EP_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_EP_REGEN - 1:                     /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_EP_REGEN_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_SPEED - 1:                     /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_SPEED_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_MOBILITY - 1:                     /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_MOBILITY_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_DEFENSE - 1:                     /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_DEFENSE_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_ARMOR_RATE - 1:                     /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_ARMOR_RATE_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_SENSOR_ACCURACY - 1:                    /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_SENSOR_ACCURACY_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_LOAD_CAPACITY - 1:                    /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_LOAD_CAPACITY_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_INITIATIVE - 1:                    /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_INITIATIVE_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_EVASION_SCORE - 1:                    /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_EVASION_SCORE_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_ATTACK_POWER - 1:                    /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_ATTACK_POWER_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_HIT_RATE - 1:                    /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_HIT_RATE_MODIFIER;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_EVASION_RATE - 1:                    /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_popup_value = 0 - removed_popup_value;
                            removed_value_popup_kind = BATTLE_POPUP_EVASION_BONUS;
                            goto show_removed_value_popup;
                        case BATTLE_EFFECT_DOUBLE_ATTACK_POWER - 1:                    /* switch 3 */
                            removed_label_popup_kind = BATTLE_POPUP_HALF_ATTACK_POWER;
                            goto block_141;
                        case BATTLE_EFFECT_HALF_EVASION_RATE - 1:                    /* switch 3 */
                            removed_label_popup_kind = BATTLE_POPUP_DOUBLE_EVASION_RATE;
                            goto block_141;
                        case BATTLE_EFFECT_SENSOR_ACCURACY_OVERRIDE - 1:                    /* switch 3 */
                            removed_popup_value = M2C_FIELD(switch_entry_address(removed_unit_queue, removed_effect_slot), s16 *, (s32)&((struct BattleUnitEffectDisplayQueue *)0)->removed_effects[0].value);
                            removed_value_popup_kind = BATTLE_POPUP_SENSOR_ACCURACY_OVERRIDE;
                            goto show_removed_value_popup;
show_removed_value_popup:
                            StartBattleUnitPopup(removed_value_popup_kind, display_side, removed_unit_slot, removed_popup_value);
                            goto mark_removed_popup_phase;
                        case BATTLE_EFFECT_ENERGY_SHIELD - 1:                    /* switch 3 */
                            removed_label_popup_kind = BATTLE_POPUP_ENERGY_SHIELD_OFF;
                            goto block_141;
                        case BATTLE_EFFECT_PILOT_INACTIVE - 1:                    /* switch 3 */
                            removed_label_popup_kind = BATTLE_POPUP_PILOT_UP_ARROW;
                            goto block_141;
                        case BATTLE_EFFECT_FREEZE - 1:                    /* switch 3 */
                            removed_label_popup_kind = BATTLE_POPUP_WAKEUP;
                            goto block_141;
                        case BATTLE_EFFECT_CONFUSION - 1:                    /* switch 3 */
                            removed_label_popup_kind = BATTLE_POPUP_CONFUSION_END;
                            goto block_141;
block_141:
                            StartBattleUnitPopup(removed_label_popup_kind, display_side, removed_unit_slot, 0);
                            goto mark_removed_popup_phase;
                        case BATTLE_EFFECT_EXTRA_TURNS - 1:                    /* switch 3 */
                            StartBattleUnitPopup(BATTLE_POPUP_ZOS_OFF, display_side, removed_unit_slot, 0);
                            goto mark_removed_popup_phase;
                        case BATTLE_EFFECT_DOUBLE_MELEE_POWER - 1:                    /* switch 3 */
                        case 29:                    /* switch 3 */
                        case 30:                    /* switch 3 */
                        case BATTLE_EFFECT_IGNORE_MELEE_DEFENSE - 1:                    /* switch 3 */
                        case 32:                    /* switch 3 */
                        case 33:                    /* switch 3 */
                        case 34:                    /* switch 3 */
                        case 35:                    /* switch 3 */
                        case 36:                    /* switch 3 */
                        case 37:                    /* switch 3 */
                            goto mark_removed_popup_phase;
                        }
                        goto mark_removed_popup_phase;
mark_removed_popup_phase:
                        phase_has_results = 1;
                    }
                }
            }
            removed_unit_slot += 1;
            if ((u32) removed_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                goto present_removed_unit_effect;
            }
            if (phase_has_results != 0) {
                LoadBattlePopupGraphics(display_side, BATTLE_POPUP_GRAPHICS_MODIFIERS);
                while (AreBattleUnitPopupsFinished() == 0) {
                    YieldTaskForUpdates(1);
                }
            }
            asm volatile("" : "+r"(removed_effect_slot));
            removed_effect_slot += 1;
            if ((u32) removed_effect_slot <= BATTLE_EFFECT_SLOT_COUNT - 1) {
                goto present_removed_effect_slot;
            }
            {
                register u8 *cleanup_reset_address asm("r1");
                register u32 cleanup_reset_work asm("r0");

                cleanup_reset_address = gBattleState;
                cleanup_reset_work = 0x27C0;
                cleanup_reset_address += cleanup_reset_work;
                cleanup_reset_work = 0;
                *cleanup_reset_address = cleanup_reset_work;
                phase_has_results = cleanup_reset_work;
            }
            destroyed_unit_slot = 0;
            do {
                if ((IsBattleUnitActive(display_side, destroyed_unit_slot) << 0x18) != 0) {
                    u8 *cleanup_record_base;

                    cleanup_record_base = gBattleState;
                    destroyed_unit_offset = destroyed_unit_slot * (s32)sizeof(struct BattleUnit);
                    destroyed_side_offset = display_side * (s32)sizeof(struct BattleSide);
                    destroyed_record_offset = destroyed_unit_offset + destroyed_side_offset;
                    destroyed_unit_record = cleanup_record_base + destroyed_record_offset;
                    if (({
                            register s32 cleanup_signed_field asm("r0");

                            asm volatile(
                                "mov r2, #6\n\t"
                                "ldrsh %0, [%1, r2]"
                                : "=r"(cleanup_signed_field)
                                : "0"(destroyed_unit_record)
                                : "r2");
                            cleanup_signed_field;
                        }) == 0) {
                        MarkBattleUnitDestroyed(display_side, destroyed_unit_slot);
                        if (display_side != 0) {
                            register u8 *cleanup_address_r3 asm("r3");
                            register u32 cleanup_address_r0 asm("r0");

                            cleanup_address_r3 = cleanup_record_base;
                            asm volatile("add %0, %1, %2"
                                         : "=r"(cleanup_address_r0)
                                         : "r"(destroyed_unit_offset), "r"(cleanup_address_r3));
                            asm volatile("add %0, %1, %2"
                                         : "=r"(cleanup_address_r3)
                                         : "r"(destroyed_side_offset), "r"(cleanup_address_r0));
                            enemy_reward_record = cleanup_address_r3;
                            *(s32 *)(cleanup_record_base + 0xA070) += M2C_FIELD(enemy_reward_record, s32 *, BATTLE_UNIT_OFFSET(experience_reward));
                            *(s32 *)(cleanup_record_base + 0xA074) += M2C_FIELD(enemy_reward_record, s32 *, BATTLE_UNIT_OFFSET(money_reward));
                            if (!(0x40 & M2C_FIELD(destroyed_unit_record, u16 *, 4))) {
                                register u8 *cleanup_destination asm("r0");
                                register u32 cleanup_count_carrier asm("r3");
                                register u8 *cleanup_increment_ptr asm("r1");

                                cleanup_destination = cleanup_record_base + 0x27C1;
                                cleanup_count_carrier = 0x0203730C;
                                cleanup_count_carrier = *(u8 *)cleanup_count_carrier;
                                cleanup_destination += cleanup_count_carrier;
                                *cleanup_destination = *(u8 *)destroyed_unit_record;
                                cleanup_increment_ptr = (u8 *)0x0203730C;
                                *cleanup_increment_ptr = (u8)(*cleanup_increment_ptr + 1);
                            }
                        }
                        {
                            register u8 *cleanup_mode_seed asm("r0");
                            register u8 *cleanup_mode_base asm("r2");
                            register u32 arrow_phalanx_command asm("r1");

                            cleanup_mode_seed = gBattleState;
                            cleanup_mode_base = (u8 *)0x27BE;
                            asm volatile("add %0, %1, %2"
                                         : "=r"(arrow_phalanx_command)
                                         : "r"(cleanup_mode_seed),
                                           "r"(cleanup_mode_base));
                            arrow_phalanx_command = display_side + arrow_phalanx_command;
                            arrow_phalanx_command = *(u8 *)arrow_phalanx_command;
                            cleanup_mode_base = cleanup_mode_seed;
                            if ((arrow_phalanx_command == DECK_COMMAND_ARROW_PHALANX) && ((destroyed_unit_slot == 1) || (destroyed_unit_slot == 3) || (destroyed_unit_slot == 5))) {
                                register u8 *cleanup_mode1_store asm("r0");

                                cleanup_mode1_store = cleanup_mode_base + 0x27BE;
                                asm volatile("add %0, %1, %0"
                                             : "+r"(cleanup_mode1_store)
                                             : "r"(display_side));
                                *cleanup_mode1_store = 0U;
                            }
                            {
                                register u32 th_phalanx_command asm("r0");

                                th_phalanx_command = (u32)cleanup_mode_base + 0x27BE;
                                asm volatile("add %0, %1, %0"
                                             : "+r"(th_phalanx_command)
                                             : "r"(display_side));
                                th_phalanx_command = *(u8 *)th_phalanx_command;
                                if ((th_phalanx_command == DECK_COMMAND_T_H_PHALANX) && ((destroyed_unit_slot == 0) || (destroyed_unit_slot == 2) || (destroyed_unit_slot == 4))) {
                                    register u8 *cleanup_mode2_store asm("r0");

                                    cleanup_mode2_store = cleanup_mode_base + 0x27BE;
                                    asm volatile("add %0, %1, %0"
                                                 : "+r"(cleanup_mode2_store)
                                                 : "r"(display_side));
                                    *cleanup_mode2_store = 0U;
                                }
                            }
                            {
                                register u8 *cannon_phalanx_command_address asm("r1");
                                register u32 cannon_phalanx_command_base asm("r0");

                                cannon_phalanx_command_address = (u8 *)0x27BE;
                                cannon_phalanx_command_base =
                                    (u32)cleanup_mode_base + (u32)cannon_phalanx_command_address;
                                cannon_phalanx_command_address = (u8 *)(display_side + cannon_phalanx_command_base);
                                if ((*cannon_phalanx_command_address == DECK_COMMAND_CANNON_PHALANX) && ((destroyed_unit_slot == 1) || (destroyed_unit_slot == 4))) {
                                    *cannon_phalanx_command_address = 0U;
                                }
                            }
                        }
                        {
                            register s32 cleanup_success_seed asm("r2");

                            asm volatile(
                                "mov %1, #1\n\t"
                                "mov %0, %1"
                                : "=r"(phase_has_results), "=r"(cleanup_success_seed));
                        }
                    }
                }
                destroyed_unit_slot += 1;
            } while ((u32) destroyed_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1);
            if (({
                    register s32 cleanup_flag_check asm("r3");

                    cleanup_flag_check = phase_has_results;
                    asm volatile("" : "+r"(cleanup_flag_check));
                    cleanup_flag_check;
                }) != 0) {
                LoadBattlePopupGraphics(display_side, BATTLE_POPUP_GRAPHICS_DESTROYED_UNIT);
                while (AreBattleUnitPopupsFinished() == 0) {
                    YieldTaskForUpdates(1);
                }
            }
            {
                u8 *opponent_record_base;
                void **opponent_object_base;
                u8 opponent_model_id;
                u8 opponent_palette_variant;
                u32 opponent_one;

                unit_slot = 0;
                opponent_one = 1;
                opponent_side = display_side;
                asm volatile("" : "+r"(opponent_side));
                opponent_side ^= opponent_one;
                {
                    register u32 opponent_base_guard_r2 asm("r2");
                    register u32 opponent_base_guard_r3 asm("r3");

                    asm volatile("" : "=&r"(opponent_base_guard_r2),
                                          "=&r"(opponent_base_guard_r3));
                    opponent_record_base = gBattleState;
                    asm volatile("" : : "r"(opponent_base_guard_r2),
                                          "r"(opponent_base_guard_r3));
                }
                opponent_object_base = (void **)0x02032E8C;
                do {
                    if ((IsBattleUnitActive(opponent_side, unit_slot) << 0x18) != 0) {
                        s32 opponent_record_offset;
                        u32 opponent_object_address;
                        u32 opponent_object_offset;

                        asm volatile("" : "+r"(opponent_side));
                        opponent_record_offset = (unit_slot * (s32)sizeof(struct BattleUnit)) +
                                                 (opponent_side * (s32)sizeof(struct BattleSide));
                        opponent_unit_record = opponent_record_base + opponent_record_offset;
                        opponent_model_id = M2C_FIELD(opponent_unit_record, u8 *, BATTLE_UNIT_OFFSET(zoid_id));
                        opponent_palette_variant = M2C_FIELD(opponent_unit_record, u8 *, BATTLE_UNIT_OFFSET(palette_variant));
                        opponent_object_offset = (unit_slot * 4) +
                                                 (opponent_side * 0x18);
                        opponent_object_address = opponent_object_offset;
                        opponent_object_address += (u32)opponent_object_base;
                        opponent_icon_sprite = *(void **)opponent_object_address;
                        LoadZoidIconGraphics(opponent_model_id,
                                      opponent_palette_variant,
                                      M2C_FIELD(opponent_icon_sprite, u16 *, BATTLE_SPRITE_OFFSET(tile_offset)),
                                      M2C_FIELD(opponent_icon_sprite, u8 *, BATTLE_SPRITE_OFFSET(palette_bank)));
                    }
                    unit_slot += 1;
                } while ((u32) unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1);
            }
            {
                s32 final_side_record_offset;
                u32 final_side_object_offset;
                u8 *final_record_base;
                void **final_object_base;

                unit_slot = 0;
                final_record_base = gBattleState;
                final_side_record_offset = display_side * ((s32)sizeof(struct BattleUnitEffectDisplayQueue) * BATTLE_ACTIVE_UNIT_COUNT);
                final_object_base = (void **)0x02032EBC;
                final_side_object_offset = display_side * 0x18;
                do {
                    if (BATTLE_EFFECT_DISPLAY_PENDING & *(u16 *)(add_address_accumulator(
                                         add_address_accumulator(unit_slot * (s32)sizeof(struct BattleUnitEffectDisplayQueue),
                                                                 final_side_record_offset),
                                         (u32)final_record_base) +
                                     BATTLE_EFFECT_DISPLAY_QUEUE_OFFSET(units[0][0].flags))) {
                        *(s32 *)(*(void **)add_address_accumulator(
                                       add_address_accumulator(unit_slot * 4,
                                                               final_side_object_offset),
                                       (u32)final_object_base) +
                                   BATTLE_SPRITE_OFFSET(user_data.gauge.override_flags)) = 0;
                    }
                    unit_slot += 1;
                } while ((u32) unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1);
            }
            UpdateBattleUnitGaugeGraphics();
        }
        display_side ^= 1;
    } while (display_side != ({
                 register u8 *final_side_address asm("r0");
                 register u32 final_side_offset asm("r2");

                 final_side_address = gBattleState;
                 final_side_offset = BATTLE_COMBINATION_OFFSET(active_side);
                 asm volatile("add %0, %0, %1"
                              : "+r"(final_side_address)
                              : "r"(final_side_offset));
                 *final_side_address;
             }));
}
