#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include "../battle/battle.h"

enum PlayerStorageLimits {
    PLAYER_STORED_ZOID_COUNT = 207,
    PLAYER_STORED_PILOT_COUNT = 53,
    PLAYER_TEAM_SLOT_COUNT = 6,
    PLAYER_ZOID_FORM_COUNT = ZOID_FORM_SLOT_COUNT,
    PLAYER_PILOT_ABILITY_COUNT = 10,
    PLAYER_PILOT_LEVEL_LIMIT = 99,
    PLAYER_AUXILIARY_PILOT_COUNT = 6,
    PLAYER_RECOVERY_ITEM_COUNT = 10,
    PLAYER_ZOID_CORE_COUNT = 90,
    PLAYER_EQUIPMENT_COUNT = 200,
    PLAYER_INVENTORY_QUANTITY_LIMIT = 99,
    PLAYER_MONEY_LIMIT = 9999999,
    PLAYER_ZOID_CATALOG_WORD_COUNT = 5,
    PLAYER_PILOT_CATALOG_WORD_COUNT = 4,
    PLAYER_EQUIPMENT_CATALOG_WORD_COUNT = 7,
    PLAYER_UNLOCKED_ZOID_DATA_WORD_COUNT = 5,
    PLAYER_DECK_COMMAND_WORD_COUNT = 2,
    PLAYER_SELECTED_DECK_COMMAND_COUNT = 10,
    PLAYER_STORAGE_SLOT_NOT_FOUND = 0xFF
};

enum PlayerStorageInitialization {
    PLAYER_CATALOG_FLAGS_RAM = 0x020217B4,
    PLAYER_ITEM_INVENTORY_RAM = 0x020217F4,
    PLAYER_CATALOG_CLEAR_CONTROL = 0x05000010,
    PLAYER_ITEM_INVENTORY_CLEAR_CONTROL = 0x0500001E
};

enum NewGamePlayerResources {
    NEW_GAME_PLAYER_STATE_RAM = 0x020218E4,
    NEW_GAME_PLAYER_CLEAR_CONTROL = 0x05001A82,
    NEW_GAME_STARTER_ZOID_MODEL = 0x71,
    NEW_GAME_STARTER_ZOID_SLOT = 1,
    NEW_GAME_STARTER_TEAM_SLOT = 1,
    NEW_GAME_STARTING_MONEY = 2000
};

enum PlayerStorageAddResult {
    PLAYER_STORAGE_ADD_CAPPED = 0,
    PLAYER_STORAGE_ADD_WITHIN_LIMIT = 1
};

enum PlayerStorageSubtractResult {
    PLAYER_STORAGE_SUBTRACT_DEPLETED = 0,
    PLAYER_STORAGE_SUBTRACT_REMAINS = 1
};

enum PlayerDataUnlockResult {
    PLAYER_DATA_ALREADY_UNLOCKED = 0,
    PLAYER_DATA_NEWLY_UNLOCKED = 1
};

enum PlayerPilotIdentity {
    PLAYER_PROTAGONIST_PILOT_ID = 1
};

enum PlayerRecordFlags {
    PLAYER_RECORD_NEW = 0x01,
    PLAYER_RECORD_TEMPORARY_PAIR = 0x02,
    PLAYER_RECORD_IN_TEAM = 0x04,
    PLAYER_ZOID_DESTROYED = 0x08
};

enum ZoidStatLimits {
    ZOID_MAX_HP_LIMIT = 9999,
    ZOID_DCP_LIMIT = 9999,
    ZOID_MAX_EP_LIMIT = 999,
    ZOID_EP_REGEN_LIMIT = 99,
    ZOID_SPEED_LIMIT = 9999,
    ZOID_MOBILITY_LIMIT = 999,
    ZOID_DEFENSE_LIMIT = 9999,
    ZOID_ARMOR_RATE_LIMIT = 99,
    ZOID_SENSOR_ACCURACY_LIMIT = 9999,
    ZOID_LOAD_CAPACITY_LIMIT = 999,
    ZOID_INITIATIVE_LIMIT = 9999,
    ZOID_EVASION_SCORE_LIMIT = 9999
};

enum PilotStatGrowthIndex {
    PILOT_GROWTH_MAX_HP = 0,
    PILOT_GROWTH_MOBILITY = 1,
    PILOT_GROWTH_SENSOR_ACCURACY = 2,
    PILOT_GROWTH_WEAPON_ACCURACY = 3,
    PILOT_GROWTH_DCP = 4,
    PILOT_STAT_GROWTH_COUNT = 5
};

struct ZoidBaseRecordView {
    u8 movement_flags;
    u8 data01;
    u8 size_class;
    u8 required_pilot_level;
    u16 stats[10];
    struct EquipmentSlot equipment[BATTLE_EQUIPMENT_SLOT_COUNT];
};

struct ZoidShopRecordView {
    u8 base_model_or_series_id;
    u8 required_core_id0;
    u8 required_core_id1;
    u8 reserved03;
    u32 purchase_price;
    u32 sell_value;
};

struct PlayerZoidRecordView {
    u8 model_id;
    u8 palette_variant;
    u8 pilot_slot;
    u8 form_flags;
    u16 flags;
    u16 current_hp;
    u16 current_ep;
    u16 initiative;
    u16 evasion_score;
    u16 equipment_weight;
    s16 level;
    u8 form_ep_regen_bonuses[PLAYER_ZOID_FORM_COUNT];
    u8 form_defense_bonuses[PLAYER_ZOID_FORM_COUNT];
    u8 form_weapon_power_bonuses[PLAYER_ZOID_FORM_COUNT][4];
    u8 movement_flags;
    u8 padding37;
    u8 size_class;
    u8 required_pilot_level;
    u16 max_hp;
    u16 dcp;
    u16 max_ep;
    u16 ep_regen;
    u16 speed;
    u16 mobility;
    u16 defense;
    u16 armor_rate;
    u16 sensor_accuracy;
    u16 load_capacity;
    u8 padding4E[2];
    struct EquipmentSlot equipment[BATTLE_EQUIPMENT_SLOT_COUNT];
};

enum PilotDefinitionResources {
    PILOT_BASE_RECORDS_ROM = 0x087B70E4,
    AUXILIARY_PILOT_BASE_RECORDS_ROM = 0x087B7774,
    PLAYER_NAME_RAM = 0x02021774,
    PLAYER_BATTLE_QUOTE_RAM = 0x02021786
};

struct PilotBaseRecordView {
    u8 level;
    u8 auxiliary_pilot_id;
    u8 growth_class;
    u8 padding03;
    s16 max_hp_bonus_percent;
    s16 mobility_bonus_percent;
    s16 sensor_accuracy_bonus_percent;
    s16 weapon_accuracy_bonus_percent;
    s16 dcp_bonus_percent;
    u8 padding0E[2];
};

struct AuxiliaryPilotBaseRecordView {
    u8 level_bonus;
    u8 padding01;
    s16 hp_recovery_percent;
    s16 weapon_power_bonus_percent;
    s16 sensor_accuracy_bonus_percent;
    s16 speed_bonus_percent;
    s16 defense_bonus_percent;
};

#define PILOT_BASE_RECORD_OFFSET(field) \
    ((s32)&((struct PilotBaseRecordView *)0)->field)

#define AUXILIARY_PILOT_BASE_RECORD_OFFSET(field) \
    ((s32)&((struct AuxiliaryPilotBaseRecordView *)0)->field)

struct PlayerPilotRecordView {
    u8 active_pilot_id;
    u8 zoid_slot;
    u16 flags_and_pilot_id;
    u32 experience;
    u8 ability_kinds[PLAYER_PILOT_ABILITY_COUNT];
    s16 ability_values[PLAYER_PILOT_ABILITY_COUNT];
    u16 stat_growth[PILOT_STAT_GROWTH_COUNT];
    u8 level;
    u8 auxiliary_pilot_id;
    u8 growth_class;
    u8 padding33;
    s16 max_hp_bonus_percent;
    s16 mobility_bonus_percent;
    s16 sensor_accuracy_bonus_percent;
    s16 weapon_accuracy_bonus_percent;
    s16 dcp_bonus_percent;
    u8 padding3E[2];
};

enum PulseEmotionIndex {
    PULSE_EMOTION_WHITE = 0,
    PULSE_EMOTION_RED = 1,
    PULSE_EMOTION_BLUE = 2,
    PULSE_EMOTION_BLACK = 3,
    PULSE_EMOTION_COUNT = 4
};

enum AuxiliaryPilotEffectKind {
    AUXILIARY_EFFECT_NONE = 0,
    AUXILIARY_EFFECT_COMBAT_POWER_UP = 1,
    AUXILIARY_EFFECT_ANTI_AIR_BATTLE = 2,
    AUXILIARY_EFFECT_ARMOR_DAMAGE = 3,
    AUXILIARY_EFFECT_ARMOR_PENETRATION = 4,
    AUXILIARY_EFFECT_ENERGY_GRAPPLE = 5,
    AUXILIARY_EFFECT_BRUTALITY_UP = 6,
    AUXILIARY_EFFECT_MAX_HP_UP_1 = 7,
    AUXILIARY_EFFECT_MAX_HP_UP_2 = 8,
    AUXILIARY_EFFECT_MAX_HP_UP_3 = 9,
    AUXILIARY_EFFECT_SELF_REPAIR_1 = 10,
    AUXILIARY_EFFECT_SELF_REPAIR_2 = 11,
    AUXILIARY_EFFECT_RECOVERY_FIELD = 12,
    AUXILIARY_EFFECT_MAX_EP_UP_1 = 13,
    AUXILIARY_EFFECT_MAX_EP_UP_2 = 14,
    AUXILIARY_EFFECT_MAX_EP_UP_3 = 15,
    AUXILIARY_EFFECT_GEP_UP_1 = 16,
    AUXILIARY_EFFECT_GEP_UP_2 = 17,
    AUXILIARY_EFFECT_ENERGY_SHIELD = 18,
    AUXILIARY_EFFECT_ULTRA_REACTION = 19,
    AUXILIARY_EFFECT_ULTRA_ACCELERATION = 20,
    AUXILIARY_EFFECT_ULTRA_EVASION = 21,
    AUXILIARY_EFFECT_WAR_CRY = 22,
    AUXILIARY_EFFECT_REPEAT_ATTACK = 23,
    AUXILIARY_EFFECT_ZOS_1 = 24,
    AUXILIARY_EFFECT_ZOS_2 = 25
};

enum PulseConstants {
    PULSE_PILOT_ID = 75,
    PULSE_FIELD_ACTOR_MODEL_ID = 75,
    PULSE_EMOTION_GROWTH_LIMIT = 99,
    PULSE_STAT_GROWTH_TABLE_ROM = 0x087B7988,
    PULSE_AUXILIARY_PILOT_IDS_ROM = 0x087AF5F8,
    PULSE_FIELD_PALETTE_POINTERS_ROM = 0x087A1C08
};

struct PulseStatGrowthBonuses {
    s16 hp_recovery_percent;
    s16 weapon_power_bonus_percent;
    s16 sensor_accuracy_bonus_percent;
    s16 speed_bonus_percent;
    s16 defense_bonus_percent;
} __attribute__((packed));

enum PulseEffectSchedule {
    PULSE_EFFECT_GROWTH_TABLE_ROM = 0x087B9364,
    PULSE_EFFECT_UPDATE_RESULTS_RAM = 0x02032E35
};

struct PulseEffectScheduleEntry {
    s16 emotion_growth;
    u16 kind;
    s16 value;
} __attribute__((packed));

struct PulseEffectScheduleRow {
    struct PulseEffectScheduleEntry entries[PLAYER_PILOT_ABILITY_COUNT];
};

struct PulseEffectUpdateResults {
    u8 count;
    /* Native updates can exceed ten; kind writes then overlap the value array. */
    u8 kinds[PLAYER_PILOT_ABILITY_COUNT];
    s16 values[PLAYER_PILOT_ABILITY_COUNT];
} __attribute__((packed));

struct PlayerAuxiliaryPilotRecordView {
    u8 auxiliary_pilot_id;
    u8 palette_or_portrait_variant;
    u8 emotion_growth[PULSE_EMOTION_COUNT];
    u8 processed_emotion_growth[PULSE_EMOTION_COUNT];
    u8 effect_kinds[PLAYER_PILOT_ABILITY_COUNT];
    s16 effect_values[PLAYER_PILOT_ABILITY_COUNT];
    u8 level_bonus;
    u8 padding29;
    s16 hp_recovery_percent;
    s16 weapon_power_bonus_percent;
    s16 sensor_accuracy_bonus_percent;
    s16 speed_bonus_percent;
    s16 defense_bonus_percent;
};

struct BattleUnitPilotStateView {
    u8 data0000[0x70];
    struct PlayerPilotRecordView pilot;
    struct PlayerAuxiliaryPilotRecordView auxiliary_pilot;
};

#define BATTLE_UNIT_PILOT_OFFSET(field) \
    ((s32)&((struct BattleUnitPilotStateView *)0)->field)

#define PULSE_STAT_GROWTH_OFFSET(field) \
    ((s32)&((struct PulseStatGrowthBonuses *)0)->field)

struct PilotAbilityScheduleEntry {
    s16 level;
    u16 kind;
    u16 value;
} __attribute__((packed));

struct PilotAbilityScheduleRow {
    struct PilotAbilityScheduleEntry entries[PLAYER_PILOT_ABILITY_COUNT];
};

struct PilotAbilityUpdateResults {
    u8 count;
    u8 kinds[PLAYER_PILOT_ABILITY_COUNT];
    u8 values[PLAYER_PILOT_ABILITY_COUNT];
} __attribute__((packed));

struct PlayerCatalogFlags {
    u32 zoid_models[PLAYER_ZOID_CATALOG_WORD_COUNT];
    u32 pilots[PLAYER_PILOT_CATALOG_WORD_COUNT];
    u32 equipment[PLAYER_EQUIPMENT_CATALOG_WORD_COUNT];
};

struct PlayerItemInventory {
    u8 recovery_item_quantities[PLAYER_RECOVERY_ITEM_COUNT];
    u8 zoid_core_quantities[PLAYER_ZOID_CORE_COUNT];
    u32 unlocked_zoid_data[PLAYER_UNLOCKED_ZOID_DATA_WORD_COUNT];
};

struct PlayerStateView {
    u8 party_name_display_mode;
    u8 stored_zoid_count;
    u8 padding02[2];
    struct PlayerZoidRecordView zoids[PLAYER_STORED_ZOID_COUNT];
    struct PlayerPilotRecordView pilots[PLAYER_STORED_PILOT_COUNT];
    struct PlayerAuxiliaryPilotRecordView auxiliary_pilots[PLAYER_AUXILIARY_PILOT_COUNT];
    u8 team_zoid_slots[PLAYER_TEAM_SLOT_COUNT];
    u8 selected_deck_commands[PLAYER_SELECTED_DECK_COMMAND_COUNT];
    u8 team_slot_reset_state[PLAYER_TEAM_SLOT_COUNT];
    u8 padding6922[0x12];
    u8 equipment_quantities[PLAYER_EQUIPMENT_COUNT];
    u32 unlocked_deck_commands[PLAYER_DECK_COMMAND_WORD_COUNT];
    u32 money;
};

#define PLAYER_STATE_OFFSET(field) \
    ((s32)&((struct PlayerStateView *)0)->field)

#define PLAYER_ZOID_OFFSET(field) \
    ((s32)&((struct PlayerZoidRecordView *)0)->field)

#define PLAYER_PILOT_OFFSET(field) \
    ((s32)&((struct PlayerPilotRecordView *)0)->field)

#define PULSE_PLAYER_RECORD_RAM \
    (0x020218E4 + PLAYER_STATE_OFFSET(auxiliary_pilots[1]))

#define PULSE_SCHEDULE_ENTRY_OFFSET(field) \
    ((s32)&((struct PulseEffectScheduleEntry *)0)->field)

#define PULSE_EFFECT_UPDATE_OFFSET(field) \
    ((s32)&((struct PulseEffectUpdateResults *)0)->field)

#define PLAYER_AUXILIARY_PILOT_OFFSET(field) \
    ((s32)&((struct PlayerAuxiliaryPilotRecordView *)0)->field)

#define ZOID_BASE_RECORD_OFFSET(field) \
    ((s32)&((struct ZoidBaseRecordView *)0)->field)

#define PLAYER_ITEM_INVENTORY_OFFSET(field) \
    ((s32)&((struct PlayerItemInventory *)0)->field)

#define PLAYER_CATALOG_OFFSET(field) \
    ((s32)&((struct PlayerCatalogFlags *)0)->field)

enum PilotLevelUpResources {
    PILOT_EXPERIENCE_LIMIT = 99999999,
    PILOT_EXPERIENCE_THRESHOLDS_ROM = 0x087B77C8,
    PILOT_MANUAL_GROWTH_POINTS = 10,
    PILOT_PROFICIENCY_DESCRIPTION_PREFIX_BYTES = 15,
    PILOT_PROFICIENCY_DESCRIPTION_STRIDE = 35
};

enum PilotStatGrowthClass {
    PILOT_GROWTH_CLASS_MANUAL = 5,
    PILOT_GROWTH_CLASS_RANDOM = 6
};

enum PilotRandomStatGrowthChoice {
    PILOT_RANDOM_GROWTH_MAX_HP = 0,
    PILOT_RANDOM_GROWTH_MOBILITY = 1,
    PILOT_RANDOM_GROWTH_SENSOR_ACCURACY = 2,
    PILOT_RANDOM_GROWTH_DCP = 3,
    PILOT_RANDOM_GROWTH_WEAPON_ACCURACY = 4
};


struct ChallengeBattleUnitDefinition {
    u8 model_id;
    u8 palette_variant;
    u8 pilot_id;
    u8 fixed_equipment_ids[4];
    u8 reserved07;
};

struct ChallengeBattleDefinition {
    struct ChallengeBattleUnitDefinition sides[2][6];
    u8 battle_recovery_item_quantities[10];
    u8 inventory_recovery_item_quantities[10];
    u8 reserved74[12];
};

enum ChallengeBattleResources {
    CHALLENGE_BATTLE_DEFINITIONS_ROM = 0x087A3EF0,
    CHALLENGE_SELECTED_COURSE_RAM = 0x02032E74,
    CHALLENGE_SELECTED_ROUND_RAM = 0x02032E78,
    CHALLENGE_COURSE_COUNT = 10,
    CHALLENGE_ROUNDS_PER_COURSE = 5,
    CHALLENGE_COURSE_UNLOCK_FLAGS_ROM = 0x087A3EE4,
    CHALLENGE_ROUND_COMPLETION_RAM = 0x0202EE8C,
    CHALLENGE_COURSE_SELECTABLE_RAM = 0x02032E79,
    CHALLENGE_ROUND_SELECTABLE_RAM = 0x02032E83
};

#endif
