#ifndef BATTLE_H
#define BATTLE_H

enum BattleLimits {
    BATTLE_SIDE_COUNT = 2,
    BATTLE_ACTIVE_UNIT_COUNT = 6,
    BATTLE_UNIT_SLOT_COUNT = 8,
    BATTLE_EQUIPMENT_SLOT_COUNT = 8,
    BATTLE_ACTION_COUNT = 3,
    BATTLE_TARGET_CHOICE_COUNT = 6,
    BATTLE_EFFECT_SLOT_COUNT = 32,
    BATTLE_EFFECT_NOT_FOUND = 0xFF
};

enum BattleEffectKind {
    BATTLE_EFFECT_NONE = 0,
    BATTLE_EFFECT_HP_RECOVERY = 1,
    BATTLE_EFFECT_MAX_HP = 2,
    BATTLE_EFFECT_DCP = 3,
    BATTLE_EFFECT_EP_RECOVERY = 4,
    BATTLE_EFFECT_MAX_EP = 5,
    BATTLE_EFFECT_EP_REGEN = 6,
    BATTLE_EFFECT_SPEED = 7,
    BATTLE_EFFECT_MOBILITY = 8,
    BATTLE_EFFECT_DEFENSE = 9,
    BATTLE_EFFECT_ARMOR_RATE = 10,
    BATTLE_EFFECT_SENSOR_ACCURACY = 11,
    BATTLE_EFFECT_LOAD_CAPACITY = 12,
    BATTLE_EFFECT_INITIATIVE = 13,
    BATTLE_EFFECT_EVASION_SCORE = 14,
    BATTLE_EFFECT_ATTACK_POWER = 15,
    BATTLE_EFFECT_HIT_RATE = 16,
    BATTLE_EFFECT_EVASION_RATE = 17,
    BATTLE_EFFECT_DOUBLE_ATTACK_POWER = 18,
    BATTLE_EFFECT_HALF_EVASION_RATE = 19,
    BATTLE_EFFECT_SENSOR_ACCURACY_OVERRIDE = 20,
    BATTLE_EFFECT_ENERGY_SHIELD = 21,
    BATTLE_EFFECT_ANTI_AIR = 22,
    BATTLE_EFFECT_AUXILIARY_PILOT_ACTIVE = 23,
    BATTLE_EFFECT_PILOT_INACTIVE = 24,
    BATTLE_EFFECT_FREEZE = 25,
    BATTLE_EFFECT_TURN_MARKER = 26,
    BATTLE_EFFECT_CONFUSION = 27,
    BATTLE_EFFECT_EXTRA_TURNS = 28,
    BATTLE_EFFECT_DOUBLE_MELEE_POWER = 29,
    BATTLE_EFFECT_MELEE_ANTI_AIR_BONUS = 30,
    BATTLE_EFFECT_MELEE_DEFENSE_DAMAGE_CHANCE = 31,
    BATTLE_EFFECT_IGNORE_MELEE_DEFENSE = 32
};

enum BattleSideId {
    BATTLE_PLAYER_SIDE = 0,
    BATTLE_ENEMY_SIDE = 1
};

enum BattleEncounterLimits {
    BATTLE_ENCOUNTERS_PER_GROUP = 20,
    BATTLE_ENCOUNTER_TABLE_ROM = 0x087B9454,
    BATTLE_PARTY_NAME_CHARACTER_COUNT = 9,
    BATTLE_PARTY_QUOTE_CHARACTER_COUNT = 23,
    BATTLE_STATE_RAM = 0x02034B4C
};

enum BattleLinkExchangeResources {
    BATTLE_LINK_UNIT_RECORD_PART_COUNT = 3,
    BATTLE_LINK_EXCHANGE_PROGRESS_MENU = 0x08028388,
    BATTLE_LINK_EXCHANGE_CLOSE_MENU = 0x080283A8,
    BATTLE_LINK_ZOID_RECORD_MESSAGE = 0x08109450,
    BATTLE_LINK_PILOT_RECORD_MESSAGE = 0x0810945C,
    BATTLE_LINK_AUXILIARY_RECORD_MESSAGE = 0x08109468,
    BATTLE_LINK_PARTY_NAME_MESSAGE = 0x08109474,
    BATTLE_LINK_BATTLE_QUOTE_MESSAGE = 0x08109480
};

enum BattleTransportModelId {
    ZOID_GUSTAV = 147,
    ZOID_HOVER_CARGO = 148,
    ZOID_WHALE_KING = 149,
    ZOID_DRAGOON_NEST = 150
};

struct BattleEncounterUnitRecord {
    u8 zoid_id;
    u8 palette_variant;
    u8 pilot_id;
    u8 flag40_enabled;
    u8 equipment_item_ids[4];
    u32 experience_reward;
    u32 money_reward;
} __attribute__((packed));

struct BattleEncounterRecord {
    u8 song_id;
    u8 data01[3];
    struct BattleEncounterUnitRecord units[BATTLE_ACTIVE_UNIT_COUNT];
};

struct BattleEncounterGroup {
    struct BattleEncounterRecord encounters[BATTLE_ENCOUNTERS_PER_GROUP];
};

struct BattlePartyIdentityStateView {
    u8 data0000[0xA29C];
    u16 pilot_names[BATTLE_SIDE_COUNT][BATTLE_PARTY_NAME_CHARACTER_COUNT];
    u16 battle_quotes[BATTLE_SIDE_COUNT][BATTLE_PARTY_QUOTE_CHARACTER_COUNT];
    u8 original_party_stored_zoid_slots[BATTLE_ACTIVE_UNIT_COUNT];
} __attribute__((packed));

#define BATTLE_ENCOUNTER_OFFSET(field) \
    ((s32)&((struct BattleEncounterRecord *)0)->field)

#define BATTLE_ENCOUNTER_UNIT_OFFSET(field) \
    ((s32)&((struct BattleEncounterUnitRecord *)0)->field)

#define BATTLE_PARTY_IDENTITY_OFFSET(field) \
    ((s32)&((struct BattlePartyIdentityStateView *)0)->field)

#define BATTLE_PARTY_IDENTITY_ADDRESS(field) \
    (BATTLE_STATE_RAM + BATTLE_PARTY_IDENTITY_OFFSET(field))

enum BattleSelectionResult {
    BATTLE_UNIT_NOT_SELECTED = 0xFF
};

enum BattleCandidateRemoval {
    BATTLE_REMOVE_EQUIPMENT_CANDIDATE = 0xFF
};

enum BattleCandidateRemovalResult {
    BATTLE_CANDIDATE_REMOVED = 0,
    BATTLE_CANDIDATE_REMAINS = 1
};

enum BattleRuleId {
    BATTLE_RULE_UP_TO_ONE_ZOID = 1,
    BATTLE_RULE_UP_TO_TWO_ZOIDS = 2,
    BATTLE_RULE_UP_TO_THREE_ZOIDS = 3,
    BATTLE_RULE_UP_TO_FOUR_ZOIDS = 4,
    BATTLE_RULE_SIZE_S_ONLY = 5,
    BATTLE_RULE_UP_TO_SIZE_M = 6,
    BATTLE_RULE_SIZE_M_ONLY = 7,
    BATTLE_RULE_UP_TO_SIZE_L = 8,
    BATTLE_RULE_AT_LEAST_ONE_SIZE_L_OR_LARGER = 9,
    BATTLE_RULE_AT_LEAST_ONE_SIZE_XL_OR_LARGER = 10,
    BATTLE_RULE_NO_FLYING_ZOIDS = 11,
    BATTLE_RULE_FLYING_ZOIDS_ONLY = 12,
    BATTLE_RULE_LIGER_MODELS_ONLY = 13,
    BATTLE_RULE_TIGER_MODELS_ONLY = 14,
    BATTLE_RULE_WOLF_MODELS_ONLY = 15,
    BATTLE_RULE_LIGER_TIGER_OR_WOLF_MODELS_ONLY = 16,
    BATTLE_RULE_EQUIPMENT_SLOTS_4_TO_7_ONLY = 17,
    BATTLE_RULE_MELEE_ONLY = 18,
    BATTLE_RULE_NO_MELEE = 19,
    BATTLE_RULE_NO_RECOVERY = 20,
    BATTLE_RULE_NO_DECK_COMMANDS = 21,
    BATTLE_RULE_NO_ORGANOID_OR_ZOS = 22,
    BATTLE_RULE_EXACTLY_TWO_ZOIDS = 23
};

enum PilotNameEventFlag {
    PILOT_NAME_REVEAL_MYSTERY_WARRIOR = 2,
    PILOT_NAME_REVEAL_MYSTERY_WOMAN = 26
};

enum ZoidStatIndex {
    ZOID_STAT_MAX_HP = 0,
    ZOID_STAT_DCP = 1,
    ZOID_STAT_MAX_EP = 2,
    ZOID_STAT_EP_REGEN = 3,
    ZOID_STAT_SPEED = 4,
    ZOID_STAT_MOBILITY = 5,
    ZOID_STAT_DEFENSE = 6,
    ZOID_STAT_ARMOR_RATE = 7,
    ZOID_STAT_SENSOR_ACCURACY = 8,
    ZOID_STAT_LOAD_CAPACITY = 9
};

enum ZoidFormId {
    ZOID_LIGER_ZERO = 0x19,
    ZOID_ZERO_SCHNEIDER = 0x1A,
    ZOID_ZERO_JAEGER = 0x1B,
    ZOID_ZERO_PANZER = 0x1C,
    ZOID_ZERO_EMPIRE = 0x1D,
    ZOID_ZERO_X = 0x1E,
    ZOID_BERSERK_FURY = 0x40,
    ZOID_STRUM_FURY = 0x41,
    ZOID_JAGD_FURY = 0x42,
    ZOID_BERSERK_FURY_Z = 0x43
};

enum ZoidFormLimits {
    ZOID_FORM_SLOT_COUNT = 6
};

enum ZoidSizeClass {
    ZOID_SIZE_CLASS_S = 0,
    ZOID_SIZE_CLASS_M = 1,
    ZOID_SIZE_CLASS_L = 2,
    ZOID_SIZE_CLASS_XL = 3,
    ZOID_SIZE_CLASS_DOUBLE_ICON = 4
};

enum BattleEffectBits {
    BATTLE_EFFECT_KIND_MASK = 0x7F,
    BATTLE_EFFECT_SOURCE_FLAG = 0x80,
    BATTLE_EFFECT_EQUIPMENT_SLOT_MASK = 0xF00,
    BATTLE_EFFECT_PASSIVE_EQUIPMENT = 0x1000,
    BATTLE_EFFECT_LIFETIME_MASK = 0xE000
};

enum WeaponAttributes {
    WEAPON_BALLISTIC = 0x01,
    WEAPON_MISSILE = 0x02,
    WEAPON_LASER = 0x04,
    WEAPON_PARTICLE = 0x08,
    WEAPON_MELEE = 0x10,
    WEAPON_ANTI_AIR = 0x20,
    WEAPON_WATER_COMPATIBLE = 0x40,
    WEAPON_HOMING = 0x80,
    WEAPON_PENETRATE = 0x100,
    WEAPON_IGNORE_DEFENSE = 0x200,
    WEAPON_DEFENSE_DAMAGE = 0x400,
    WEAPON_PILOT_INACTIVE = 0x800,
    WEAPON_FREEZE = 0x1000,
    WEAPON_CONFUSION = 0x2000,
    WEAPON_TYPE_MASK = 0x1F
};

enum EquipmentResources {
    EQUIPMENT_RECORDS_ROM = 0x087B2524
};

enum EquipmentFlags {
    EQUIPMENT_COMMAND = 0x01,
    EQUIPMENT_PASSIVE = 0x02,
    EQUIPMENT_ATTACK = 0x04,
    EQUIPMENT_KIND_MASK = 0x07
};

enum EquipmentSlotFlags {
    EQUIPMENT_SLOT_WEAPON = 0x01,
    EQUIPMENT_SLOT_COMMAND = 0x02
};

enum EquipmentRangeKind {
    EQUIPMENT_RANGE_0_TO_1 = 0,
    EQUIPMENT_RANGE_0_TO_2 = 1,
    EQUIPMENT_RANGE_0_TO_3 = 2,
    EQUIPMENT_RANGE_EXACTLY_2 = 3,
    EQUIPMENT_RANGE_2_TO_3 = 4,
    EQUIPMENT_RANGE_EXACTLY_3 = 5
};

enum EquipmentTargetArea {
    EQUIPMENT_AREA_SINGLE = 0,
    EQUIPMENT_AREA_COLUMN = 1,
    EQUIPMENT_AREA_ROW_PAIR = 2,
    EQUIPMENT_AREA_ROW = 3,
    EQUIPMENT_AREA_TWO_COLUMNS = 4,
    EQUIPMENT_AREA_SIDE = 5,
    EQUIPMENT_AREA_SELF = 6,
    EQUIPMENT_AREA_BOTH_SIDES = 7
};

enum BattleTargetPreviewFlags {
    BATTLE_TARGET_VALID = 0x01,
    BATTLE_TARGET_DAMAGE_EXCEEDS_HP = 0x02
};

enum BattleOutcomeKind {
    BATTLE_OUTCOME_NONE = 0,
    BATTLE_OUTCOME_MISS = 1,
    BATTLE_OUTCOME_SHIELD = 2,
    BATTLE_OUTCOME_HIT = 3,
    BATTLE_OUTCOME_DIRECT_HIT = 4,
    BATTLE_OUTCOME_KIND_MASK = 0x07,
    BATTLE_OUTCOME_VARIANT_MASK = 0x38,
    BATTLE_OUTCOME_FREEZE_RESISTED = 0x40
};

enum BattleUnitFlags {
    BATTLE_UNIT_DESTROYED = 0x08
};

enum BattleEffectRemovalBank {
    BATTLE_EFFECT_REMOVAL_ROUND = 0,
    BATTLE_EFFECT_REMOVAL_FIRST_ACTION = 1,
    BATTLE_EFFECT_REMOVAL_BANK_COUNT = 4
};

struct BattleEffectRemovalMasks {
    u32 banks[BATTLE_EFFECT_REMOVAL_BANK_COUNT][BATTLE_SIDE_COUNT][BATTLE_ACTIVE_UNIT_COUNT];
};

enum EquipmentEffectKind {
    EQUIPMENT_EFFECT_DEFENSE = 1,
    EQUIPMENT_EFFECT_DEFENSE_AND_ARMOR_RATE = 2,
    EQUIPMENT_EFFECT_SPEED = 3,
    EQUIPMENT_EFFECT_MOBILITY = 4,
    EQUIPMENT_EFFECT_SPEED_AND_MOBILITY = 5,
    EQUIPMENT_EFFECT_SENSOR_ACCURACY = 6,
    EQUIPMENT_EFFECT_HIT_RATE = 7,
    EQUIPMENT_EFFECT_SENSOR_ACCURACY_AND_HIT_RATE = 8,
    EQUIPMENT_EFFECT_HIT_RATE_PENALTY = 9,
    EQUIPMENT_EFFECT_EP_REGEN = 10,
    EQUIPMENT_EFFECT_EVASION_SCORE = 11,
    EQUIPMENT_EFFECT_EVASION_RATE = 12,
    EQUIPMENT_EFFECT_MAX_HP = 13,
    EQUIPMENT_EFFECT_MAX_EP = 14,
    EQUIPMENT_EFFECT_MAX_HP_AND_EP = 15,
    EQUIPMENT_EFFECT_HP_RECOVERY = 16,
    EQUIPMENT_EFFECT_EP_RECOVERY = 17,
    EQUIPMENT_EFFECT_HP_AND_EP_RECOVERY = 18,
    EQUIPMENT_EFFECT_SENSOR_ACCURACY_OVERRIDE = 19,
    EQUIPMENT_EFFECT_ENERGY_SHIELD = 20,
    EQUIPMENT_EFFECT_KIND_MASK = 0xFF,
    EQUIPMENT_EFFECT_RADAR = 0x200,
    EQUIPMENT_EFFECT_ANTI_AIR = 0x1000
};

enum BattleEffectConditions {
    BATTLE_CONDITION_RADAR = 0x200,
    BATTLE_CONDITION_AGAINST_RADAR = 0x10000,
    BATTLE_CONDITION_AGAINST_HOMING = 0x20000,
    BATTLE_CONDITION_EVASION_MASK = 0xFF0000,
    BATTLE_CONDITION_WEAPON_TYPE_MASK = 0x1F000000
};

enum ZoidMovementFlags {
    ZOID_MOVEMENT_FLYING = 0x40
};

enum PilotAbilityKind {
    PILOT_ABILITY_ZOID_PROFICIENCY_1 = 3,
    PILOT_ABILITY_ZOID_PROFICIENCY_2 = 4,
    PILOT_ABILITY_ZOID_PROFICIENCY_3 = 5,
    PILOT_ABILITY_RANGED_EP_SAVING_1 = 6,
    PILOT_ABILITY_RANGED_EP_SAVING_2 = 7,
    PILOT_ABILITY_RANGED_EP_SAVING_3 = 8,
    PILOT_ABILITY_MELEE_EP_SAVING_1 = 9,
    PILOT_ABILITY_MELEE_EP_SAVING_2 = 10,
    PILOT_ABILITY_MELEE_EP_SAVING_3 = 11,
    PILOT_ABILITY_OPENING_INITIATIVE_200 = 12,
    PILOT_ABILITY_OPENING_INITIATIVE_500 = 13,
    PILOT_ABILITY_BLOCK_OPPONENT_OPENING_BONUS = 14,
    PILOT_ABILITY_ULTRA_REACTION_1 = 15,
    PILOT_ABILITY_ULTRA_REACTION_2 = 16,
    PILOT_ABILITY_STRATEGY_COMMAND_1 = 17,
    PILOT_ABILITY_STRATEGY_COMMAND_2 = 18,
    PILOT_ABILITY_STRATEGY_COMMAND_3 = 19,
    PILOT_ABILITY_STATUS_RESISTANCE_BONUS = 21,
    PILOT_ABILITY_STATUS_RESISTANCE_PENALTY = 22,
    PILOT_ABILITY_RANGED_ACCURACY_PENALTY = 23,
    PILOT_ABILITY_MELEE_ACCURACY_PENALTY = 24,
    PILOT_ABILITY_MISSILE_ACCURACY_PENALTY = 27,
    PILOT_ABILITY_LASER_ACCURACY_PENALTY = 28,
    PILOT_ABILITY_PARTICLE_ACCURACY_PENALTY = 29,
    PILOT_ABILITY_BALLISTIC_ACCURACY_PENALTY = 30,
    PILOT_ABILITY_MELEE_EVASION_BONUS = 31,
    PILOT_ABILITY_RANGED_EVASION_BONUS = 32,
    PILOT_ABILITY_MELEE_ACCURACY_BONUS = 33,
    PILOT_ABILITY_MISSILE_ACCURACY_BONUS = 36,
    PILOT_ABILITY_LASER_ACCURACY_BONUS = 37,
    PILOT_ABILITY_PARTICLE_ACCURACY_BONUS = 38,
    PILOT_ABILITY_BALLISTIC_ACCURACY_BONUS = 39
};

struct EquipmentSlot {
    u16 flags;
    u16 item_id;
};

struct EquipmentRecord {
    u8 family_id;
    u8 family_variant;
    u16 flags;
    u32 attributes;
    u8 range_kind;
    u8 area_type;
    u16 power_or_value;
    u16 accuracy_or_secondary_value;
    s16 accuracy_limit_or_duration;
    s16 ep_cost;
    u16 weight;
    u8 mount_constraints;
    u8 zoid_compatibility_group;
    u16 data16;
};

#define EQUIPMENT_RECORD_FIELD(record, type, field) \
    M2C_FIELD(record, type *, (s32)&((struct EquipmentRecord *)0)->field)

struct BattleTargetPreview {
    u16 flags;
    u16 hit_rate_or_value;
    s16 damage_or_secondary_value;
    u16 direct_hit_rate;
    s16 direct_hit_damage;
    u16 data0A;
};

/* Explicit load types preserve the ROM's signed accesses and compiler allocation. */
#define BATTLE_PREVIEW_FIELD(preview, type, field) \
    M2C_FIELD(preview, type *, (s32)&((struct BattleTargetPreview *)0)->field)

struct BattleTargetChoice {
    u8 target_count;
    u8 data01[3];
    struct BattleTargetPreview targets[BATTLE_SIDE_COUNT][BATTLE_ACTIVE_UNIT_COUNT];
};

struct BattleEquipmentAction {
    u16 flags;
    u16 data02;
    u32 attributes;
    s16 ep_cost;
    u8 target_choice_count;
    u8 data0B;
    struct BattleTargetChoice target_choices[BATTLE_TARGET_CHOICE_COUNT];
};

#define BATTLE_ACTION_FIELD(action, type, field) \
    M2C_FIELD(action, type *, (s32)&((struct BattleEquipmentAction *)0)->field)

struct BattleEquipmentChoices {
    struct BattleEquipmentAction actions[BATTLE_ACTION_COUNT];
};

struct BattleActionSelection {
    u8 action_count;
    u8 equipment_slots[BATTLE_ACTION_COUNT];
    u8 target_choices[BATTLE_ACTION_COUNT];
    u8 outcomes[BATTLE_ACTION_COUNT][BATTLE_SIDE_COUNT][BATTLE_ACTIVE_UNIT_COUNT];
};

struct BattleActionCandidates {
    u8 equipment_slots[BATTLE_EQUIPMENT_SLOT_COUNT];
    u8 target_choices[BATTLE_EQUIPMENT_SLOT_COUNT][BATTLE_TARGET_CHOICE_COUNT];
    u8 equipment_count;
    u8 target_choice_counts[BATTLE_EQUIPMENT_SLOT_COUNT];
} __attribute__((packed));

enum BattleEffectLifetime {
    BATTLE_EFFECT_ROUND_COUNTDOWN = 0x0000,
    BATTLE_EFFECT_ACTION_MASK = 0x2000,
    BATTLE_EFFECT_TURN_EP_COST = 0x4000,
    BATTLE_EFFECT_ROUND_MASK = 0x6000,
    BATTLE_EFFECT_UNTIL_ATTACK_END = 0x8000,
    BATTLE_EFFECT_ROUND_EP_COST = 0xA000
};

enum BattleRecoveryKind {
    BATTLE_RECOVER_HP_300 = 1,
    BATTLE_RECOVER_HP_150 = 2,
    BATTLE_RECOVER_HP_50 = 3,
    BATTLE_RECOVER_HALF_MAX_HP = 4,
    BATTLE_RECOVER_FREEZE = 5,
    BATTLE_RECOVER_STATUS = 6,
    BATTLE_RECOVER_FULL = 7,
    BATTLE_RECOVER_TOWN_TRAVEL = 8,
    BATTLE_RECOVER_EVACUATION = 9
};

enum BattlePhase {
    BATTLE_PHASE_INITIALIZE = 0x0000,
    BATTLE_PHASE_PREPARE_ROUND = 0x1000,
    BATTLE_PHASE_SELECT_NEXT_UNIT = 0x2000,
    BATTLE_PHASE_BEGIN_UNIT_TURN = 0x2010,
    BATTLE_PHASE_SELECT_ITEM = 0x2100,
    BATTLE_PHASE_SELECT_COMMAND = 0x2200,
    BATTLE_PHASE_DISPATCH_ACTION = 0x3010,
    BATTLE_PHASE_CHARGE_EP = 0x3200,
    BATTLE_PHASE_USE_ITEM = 0x3300,
    BATTLE_PHASE_CHECK_RESULT = 0x4000,
    BATTLE_PHASE_END_ROUND = 0x5000,
    BATTLE_PHASE_VICTORY = 0x6000,
    BATTLE_PHASE_DEFEAT = 0x7000
};

struct BattleEffect {
    u32 condition_flags;
    u16 kind_flags;
    s16 value;
    u16 lifetime_value;
    u16 data0A;
};

enum BattleEffectDisplayFlags {
    BATTLE_EFFECT_DISPLAY_PENDING = 0x01,
    BATTLE_EFFECT_DISPLAY_DAMAGE_RESULT = 0x02,
    BATTLE_EFFECT_DISPLAY_DAMAGE = 0x08,
    BATTLE_EFFECT_DISPLAY_DECOY = 0x10
};

enum BattleEffectDisplayCameraMode {
    BATTLE_EFFECT_CAMERA_KEEP = 0,
    BATTLE_EFFECT_CAMERA_QUEUED_UNITS = 1,
    BATTLE_EFFECT_CAMERA_FOCUS_SIDE = 2
};

enum BattleResultFlags {
    BATTLE_RESULT_PLAYER_EMPTY = 1,
    BATTLE_RESULT_ENEMY_EMPTY = 2,
    BATTLE_RESULT_SCRIPTED_END = 4
};

enum BattleStoryScenario {
    BATTLE_STORY_LEVIATHE_FIRST_DAMAGE = 1,
    BATTLE_STORY_GARD_HALF_HP_END = 4,
    BATTLE_STORY_GARD_HALF_HP_END_SCENE = 5,
    BATTLE_STORY_GARD_HALF_HP_SCENE = 6,
    BATTLE_STORY_BIT_VERSUS_LEON = 7,
    BATTLE_STORY_BIT_VERSUS_STOLLER = 8,
    BATTLE_STORY_GARD_ONE_HP_END = 10
};

enum BattleStoryPilot {
    BATTLE_STORY_BIT = 30,
    BATTLE_STORY_LEON = 34,
    BATTLE_STORY_GARD = 52,
    BATTLE_STORY_LEVIATHE = 54,
    BATTLE_STORY_LEVIATHE_VARIANT = 55,
    BATTLE_STORY_STOLLER = 59,
    BATTLE_STORY_GARD_VARIANT = 94,
    BATTLE_STORY_BIT_VARIANT = 95
};

enum BattleStorySceneFlags {
    BATTLE_STORY_BIT_FIRST_DAMAGE_SCENE = 2,
    BATTLE_STORY_BIT_DAMAGE_200_SCENE = 4,
    BATTLE_STORY_LEON_DAMAGE_200_SCENE = 8,
    BATTLE_STORY_DAMAGE_THRESHOLD = 200
};

struct BattleStorySetupView {
    u8 data00[5];
    u8 story_scenario;
    u8 played_scene_flags;
} __attribute__((packed));

struct BattleStoryDamageStateView {
    u8 data0000[0xA078];
    u16 bit_damage_taken;
    u16 leon_damage_taken_from_bit;
};

#define BATTLE_STORY_SETUP_OFFSET(field) \
    ((s32)&((struct BattleStorySetupView *)0)->field)

#define BATTLE_STORY_DAMAGE_OFFSET(field) \
    ((s32)&((struct BattleStoryDamageStateView *)0)->field)

struct BattleUnitEffectDisplayQueue {
    u16 flags;
    s16 damage;
    struct BattleEffect applied_effects[BATTLE_EFFECT_SLOT_COUNT];
    struct BattleEffect removed_effects[BATTLE_EFFECT_SLOT_COUNT];
};

struct BattleEffectDisplayQueueStateView {
    u8 data0000[0x7C28];
    struct BattleUnitEffectDisplayQueue units[BATTLE_SIDE_COUNT][BATTLE_ACTIVE_UNIT_COUNT];
};

#define BATTLE_EFFECT_DISPLAY_QUEUE_OFFSET(field) \
    ((s32)&((struct BattleEffectDisplayQueueStateView *)0)->field)

struct BattleUnit {
    u8 zoid_id;
    u8 palette_variant;
    u8 pilot_slot_or_definition_id;
    u8 form_flags;
    u16 flags;
    u16 hp;
    u16 ep;
    u16 initiative;
    u16 evasion_score;
    u16 equipment_weight;
    s16 level;
    u8 form_ep_regen_bonuses[ZOID_FORM_SLOT_COUNT];
    u8 form_defense_bonuses[ZOID_FORM_SLOT_COUNT];
    u8 form_weapon_power_bonuses[ZOID_FORM_SLOT_COUNT][4];
    u8 movement_flags;
    u8 data37;
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
    u8 data4E[2];
    struct EquipmentSlot equipment[BATTLE_EQUIPMENT_SLOT_COUNT];
    u8 pilot_id;
    u8 data71[0x73];
    struct BattleEffect effects[BATTLE_EFFECT_SLOT_COUNT];
    u8 data264[2];
    u16 effect_allocation_cursor;
    u32 experience_reward;
    u32 money_reward;
};

#define BATTLE_UNIT_FIELD(unit, type, field) \
    M2C_FIELD(unit, type *, (s32)&((struct BattleUnit *)0)->field)

#define BATTLE_UNIT_OFFSET(field) \
    ((s32)&((struct BattleUnit *)0)->field)

#define BATTLE_EFFECT_FIELD(effect, type, field) \
    M2C_FIELD(effect, type *, (s32)&((struct BattleEffect *)0)->field)

struct BattleSide {
    struct BattleUnit units[BATTLE_UNIT_SLOT_COUNT];
};

enum BattleOutcomeMode {
    BATTLE_OUTCOME_DEFEAT = 0,
    BATTLE_OUTCOME_VICTORY = 1,
    BATTLE_OUTCOME_RETREAT = 2,
    BATTLE_OUTCOME_RESTORE_ONLY = 3
};

struct BattleRewardTotalsStateView {
    u8 data0000[0xA070];
    u32 experience_total;
    u32 money_total;
};

#define BATTLE_REWARD_TOTALS_OFFSET(field) \
    ((s32)&((struct BattleRewardTotalsStateView *)0)->field)

/* These kinds select visuals without adding persistent combat modifiers. */
enum BattleEffectPresentationKind {
    BATTLE_EFFECT_PRESENTATION_GREEN_STREAK = 36,
    BATTLE_EFFECT_PRESENTATION_DISSOLVING_ORBS = 37,
    BATTLE_EFFECT_PRESENTATION_PURPLE_RING = 38
};

#endif
