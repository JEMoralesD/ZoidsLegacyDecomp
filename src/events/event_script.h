#ifndef EVENT_SCRIPT_H
#define EVENT_SCRIPT_H

enum EventOpcode {
    EVENT_END = 0x00,
    EVENT_ENTER_FIELD = 0x01,
    EVENT_ENTER_BATTLE = 0x02,
    EVENT_ENTER_BATTLE_SCENE = 0x03,
    EVENT_START_BATTLE = 0x04,
    EVENT_SPAWN_ACTOR = 0x05,
    EVENT_SPAWN_ACTOR_RELATIVE = 0x06,
    EVENT_SET_FIELD_ACTOR_BEHAVIOR = 0x07,
    EVENT_REMOVE_ACTOR = 0x08,
    EVENT_START_SCRIPTS = 0x09,
    EVENT_JUMP = 0x0A,
    EVENT_RESTART = 0x0B,
    EVENT_WAIT_FOR_SCENE_ACTIONS = 0x0C,
    EVENT_WAIT_FRAMES = 0x0D,
    EVENT_YES_NO = 0x0E,
    EVENT_BRANCH_TRUE = 0x0F,
    EVENT_BRANCH_FALSE = 0x10,
    EVENT_CHOICE = 0x11,
    EVENT_CHOICE_CASE = 0x12,
    EVENT_IF_INTERACTED_ACTOR = 0x13,
    EVENT_IF_PLAYER_IN_RECT = 0x14,
    EVENT_IF_PLAYER_INTERACTION = 0x15,
    EVENT_IF_FLAG_SET = 0x16,
    EVENT_IF_FLAG_CLEAR = 0x17,
    EVENT_ALTERNATIVE = 0x18,
    EVENT_IF_PARTY_RESTRICTION = 0x19,
    EVENT_IF_PLAYER_DECK_COMMAND_UNLOCKED = 0x1A,
    EVENT_IF_PLAYER_TEAM_HAS_PILOT = 0x1B,
    EVENT_IF_PLAYER_COMBINATION_FORMATION_VALID = 0x1C,
    EVENT_ENABLE_BATTLE_RULE = 0x1D,
    EVENT_SET_FLAG = 0x1E,
    EVENT_CLEAR_FLAG = 0x1F,
    EVENT_TEXT = 0x20,
    EVENT_SHOW_PILOT_PORTRAIT = 0x21,
    EVENT_CLOSE_PORTRAIT = 0x22,
    EVENT_FADE_SCREEN_FROM_BLACK = 0x23,
    EVENT_FADE_SCREEN_TO_BLACK = 0x24,
    EVENT_FADE_SCREEN_FROM_WHITE = 0x25,
    EVENT_FADE_SCREEN_TO_WHITE = 0x26,
    EVENT_REVEAL_FIELD_WITH_ROTATING_SPLIT = 0x27,
    EVENT_CONCEAL_FIELD_WITH_ROTATING_SPLIT = 0x28,
    EVENT_REVEAL_FIELD_WITH_SCALED_Z = 0x29,
    EVENT_CONCEAL_FIELD_WITH_SCALED_Z = 0x2A,
    EVENT_REVEAL_FIELD_WITH_LEFTWARD_CHEVRON = 0x2B,
    EVENT_REVEAL_FIELD_WITH_RIGHTWARD_CHEVRON = 0x2C,
    EVENT_CONCEAL_FIELD_WITH_LEFTWARD_CHEVRON = 0x2D,
    EVENT_CONCEAL_FIELD_WITH_RIGHTWARD_CHEVRON = 0x2E,
    EVENT_REVEAL_FIELD_WITH_ROTATING_SQUARE = 0x2F,
    EVENT_CONCEAL_FIELD_WITH_ROTATING_SQUARE = 0x30,
    EVENT_REVEAL_FIELD_WITH_DITHER = 0x31,
    EVENT_CONCEAL_FIELD_WITH_DITHER = 0x32,
    EVENT_REVEAL_FIELD_WITH_CHECKERBOARD = 0x33,
    EVENT_CONCEAL_FIELD_WITH_CHECKERBOARD = 0x34,
    EVENT_FLASH_SCREEN_WHITE = 0x35,
    EVENT_MOVE_FIELD_CAMERA_IN_DIRECTION = 0x36,
    EVENT_FOCUS_FIELD_CAMERA_ON_ACTOR = 0x37,
    EVENT_QUEUE_ACTOR_WAIT = 0x38,
    EVENT_QUEUE_ACTOR_MOVE_HALF_SPEED = 0x39,
    EVENT_QUEUE_ACTOR_MOVE_NORMAL_SPEED = 0x3A,
    EVENT_QUEUE_ACTOR_MOVE_DOUBLE_SPEED = 0x3B,
    EVENT_QUEUE_ACTOR_MOVE_TO_HALF_SPEED = 0x3C,
    EVENT_QUEUE_ACTOR_MOVE_TO_NORMAL_SPEED = 0x3D,
    EVENT_QUEUE_ACTOR_MOVE_TO_DOUBLE_SPEED = 0x3E,
    EVENT_QUEUE_ACTOR_HOP = 0x3F,
    EVENT_QUEUE_ACTOR_SPECIAL_ANIMATION = 0x40,
    EVENT_LOAD_FIELD_MAP_AT_POSITION = 0x41,
    EVENT_CHANGE_MAP = 0x42,
    EVENT_SET_FIELD_VIEW_CENTER_POSITION = 0x43,
    EVENT_SHAKE_FIELD_CAMERA = 0x44,
    EVENT_STOP_CAMERA_SHAKE = 0x45,
    EVENT_ITEM_SHOP = 0x46,
    EVENT_WEAPON_SHOP = 0x47,
    EVENT_ARMOR_SHOP = 0x48,
    EVENT_OPEN_ZOIDS_LAB = 0x49,
    EVENT_PLAY_FIELD_MUSIC = 0x4A,
    EVENT_PLAY_MAP_MUSIC = 0x4B,
    EVENT_STOP_FIELD_MUSIC = 0x4C,
    EVENT_PLAY_SONG = 0x4D,
    EVENT_STOP_SONG = 0x4E,
    EVENT_SET_BATTLE_UNIT_MODEL = 0x4F,
    EVENT_CREATE_BATTLE_UNIT_SPRITE_AT_POSITION = 0x50,
    EVENT_SET_BATTLE_UNIT_MODEL_FROM_PLAYER_PILOT = 0x51,
    EVENT_SET_BATTLE_SIDE_MODELS_FROM_PLAYER_TEAM = 0x52,
    EVENT_SET_BATTLE_SIDE_MODELS_FROM_ENCOUNTER = 0x53,
    EVENT_SET_BATTLE_FUZOR_COMPONENT_MODELS = 0x54,
    EVENT_REMOVE_BATTLE_UNIT = 0x55,
    EVENT_FOCUS_BATTLE_CAMERA_ON_UNIT = 0x56,
    EVENT_SET_BATTLE_CAMERA_SIDE = 0x57,
    EVENT_CENTER_BATTLE_CAMERA = 0x58,
    EVENT_SET_BATTLE_INTRO_CAMERA = 0x59,
    EVENT_LOAD_BATTLE_SCENE_MODEL = 0x5A,
    EVENT_LOAD_BATTLE_SCENE_MODEL_FROM_PLAYER_PILOT = 0x5B,
    EVENT_CREATE_BATTLE_EQUIPMENT_SPRITE = 0x5C,
    EVENT_PLAY_BATTLE_EQUIPMENT_ANIMATION = 0x5D,
    EVENT_PLAY_BATTLE_MOUNTED_EQUIPMENT_ANIMATION = 0x5E,
    EVENT_PLAY_BATTLE_IMPACT_ANIMATION = 0x5F,
    EVENT_PLAY_BATTLE_MODEL_EQUIPMENT_IMPACT_ANIMATION = 0x60,
    EVENT_SET_BATTLE_SCENE_TERRAIN = 0x61,
    EVENT_REQUEST_ARROW_PHALANX_VOLLEY = 0x62,
    EVENT_REQUEST_T_H_PHALANX_VOLLEY = 0x63,
    EVENT_REQUEST_CANNON_PHALANX_VOLLEY = 0x64,
    EVENT_REQUEST_BATTLE_SPEED_LINES = 0x65,
    EVENT_ADD_PLAYER_ZOID = 0x66,
    EVENT_ADD_FLAGGED_PLAYER_ZOID = 0x67,
    EVENT_ADD_PLAYER_ZOID_WITH_PILOT = 0x68,
    EVENT_ADD_FLAGGED_PLAYER_ZOID_WITH_PILOT = 0x69,
    EVENT_ADD_PLAYER_PILOT = 0x6A,
    EVENT_REMOVE_PLAYER_PILOT = 0x6B,
    EVENT_ADD_TEMPORARY_PLAYER_ZOID_WITH_PILOT = 0x6C,
    EVENT_ADD_TEMPORARY_PLAYER_ZOID_WITH_PILOT_TO_TEAM = 0x6D,
    EVENT_REMOVE_PLAYER_PILOT_AND_TEMPORARY_ZOID = 0x6E,
    EVENT_INITIALIZE_PROTAGONIST_AUXILIARY_PILOT = 0x6F,
    EVENT_ADD_PLAYER_EQUIPMENT = 0x70,
    EVENT_ADD_PLAYER_RECOVERY_ITEM = 0x71,
    EVENT_UNLOCK_PLAYER_ZOID_DATA = 0x72,
    EVENT_ADD_PLAYER_ZOID_CORE = 0x73,
    EVENT_UNLOCK_PLAYER_DECK_COMMAND = 0x74,
    EVENT_ADD_PLAYER_MONEY = 0x75,
    EVENT_SET_TRANSPORT_MODEL = 0x76,
    EVENT_SPAWN_TRANSPORT_ACTOR = 0x77,
    EVENT_LOAD_SPRITE_GRAPHICS = 0x78,
    EVENT_CREATE_SPRITE = 0x79,
    EVENT_SET_SPRITE_ANIMATION = 0x7A,
    EVENT_MOVE_SPRITE = 0x7B,
    EVENT_DESTROY_SPRITE = 0x7C,
    EVENT_RESET_SPRITE_POOL = 0x7D,
    EVENT_OFFER_PULSE_EMOTION_GROWTH = 0x7E,
    EVENT_PULSE_GROWTH_ACCEPTED = 0x7F,
    EVENT_PULSE_GROWTH_DECLINED = 0x80,
    EVENT_LEVEL_UP_PULSE = 0x81,
    EVENT_GRANT_PULSE_EFFECT = 0x82,
    EVENT_RETURN_TO_TITLE = 0x83,
    EVENT_CALL_SCENE_FUNCTION = 0x84,
    EVENT_COMBINE_BATTLE_ZOIDS = 0x85,
    EVENT_OPEN_PAUSE_MENU = 0x86,
    EVENT_RESET_WORLD_MAP_BACKGROUND_EFFECTS = 0x87,
    EVENT_DISABLE_WORLD_MAP_DECORATIONS = 0x88,
    EVENT_SHOW_WORLD_MAP_BG1_OVERLAY = 0x89,
    EVENT_CLEAR_FIELD_TRAVEL_RESTRICTION = 0x8A,
    EVENT_SET_FIELD_TRAVEL_RESTRICTION = 0x8B,
    EVENT_CLEAR_ADDITIONAL_FIELD_TRAVEL_RESTRICTION = 0x8C,
    EVENT_SET_ADDITIONAL_FIELD_TRAVEL_RESTRICTION = 0x8D,
    EVENT_SHOW_WORLD_MAP_STRUCTURE_APPEARANCE = 0x8E,
    EVENT_SET_WORLD_MAP_STRUCTURE = 0x8F,
    EVENT_ANIMATE_WORLD_MAP_STRUCTURE = 0x90,
    EVENT_REMOVE_WORLD_MAP_STRUCTURE = 0x91,
    EVENT_UNLOCK_BIT_LIGER_ZERO_FORM = 0x92,
    EVENT_CHANGE_BIT_LIGER_ZERO_FORM = 0x93,
    EVENT_BACKUP_PLAYER_AND_FIELD_STATE = 0x94,
    EVENT_SAVE_CLEAR_DATA = 0x95,
    EVENT_SHOW_CREDITS = 0x96,
    EVENT_OPEN_TEAM_SETUP_IF_EMPTY = 0x97
};

enum EventScanSelector {
    EVENT_SCAN_NEXT = -1
};

enum EventScriptLimits {
    EVENT_SCRIPT_SLOT_COUNT = 70
};

enum EventHandlerResult {
    EVENT_CONTINUE = 0,
    EVENT_YIELD = 1
};

enum EventSpriteResources {
    EVENT_SPRITE_COUNT = 16,
    EVENT_SPRITE_LAST_SLOT = 15,
    EVENT_SPRITE_INITIAL_TILE_BOUNDARY = 0x398,
    EVENT_SPRITE_INITIAL_PALETTE_BOUNDARY = 9,
    EVENT_SPRITE_GRAPHICS_TABLE_ROM = 0x087AF9D4,
    EVENT_SPRITE_DEFINITIONS_ROM = 0x087AFA94,
    EVENT_SPRITE_MOVEMENT_TASK = 6
};

enum EventSpritePlaybackMode {
    EVENT_SPRITE_PLAY_ONCE = 0,
    EVENT_SPRITE_PLAY_LOOP = 1,
    EVENT_SPRITE_PLAY_AND_HOLD = 2
};

struct EventSpriteGraphicsRecord {
    const u8 *compressed_graphics;
    u32 compressed_palette_address;
};

struct EventSpritePositionView {
    u32 flags;
    u16 x;
    u16 y;
};

struct EventSpriteMovementState {
    u8 active;
    u8 x_decreases;
    u8 y_decreases;
    u8 reserved03;
    u16 x_distance;
    u16 y_distance;
    u16 x_accumulator;
    u16 y_accumulator;
    u8 duration_updates;
    u8 elapsed_updates;
    u16 reserved0E;
};

#define EVENT_SPRITE_MOVEMENT_OFFSET(field) \
    ((s32)&((struct EventSpriteMovementState *)0)->field)

struct EventSpriteResourceCommand {
    u8 opcode;
    u8 resource_id;
} __attribute__((packed));

struct EventSpriteCreateCommand {
    u8 opcode;
    u8 sprite_slot;
    u8 resource_id;
    u8 animation_id;
    u8 x_low;
    u8 x_high;
    u8 y_low;
    u8 y_high;
    u8 playback_mode;
} __attribute__((packed));

struct EventSpriteAnimationCommand {
    u8 opcode;
    u8 sprite_slot;
    u8 animation_id;
} __attribute__((packed));

struct EventSpriteMoveCommand {
    u8 opcode;
    u8 sprite_slot;
    u8 x_low;
    u8 x_high;
    u8 y_low;
    u8 y_high;
    u8 duration_updates;
    u8 reserved07;
} __attribute__((packed));

struct EventSpriteSlotCommand {
    u8 opcode;
    u8 sprite_slot;
} __attribute__((packed));

struct EventBattleCameraSideCommand {
    u8 opcode;
    u8 side_id;
} __attribute__((packed));

struct EventBattleTerrainCommand {
    u8 opcode;
    u8 terrain_id;
} __attribute__((packed));

struct EventPauseMenuCommand {
    u8 opcode;
    u8 menu_entry_mode;
} __attribute__((packed));

struct EventSceneFunctionCommand {
    u8 opcode;
    u8 function_index;
} __attribute__((packed));

struct EventTransportModelCommand {
    u8 opcode;
    u8 model_id;
} __attribute__((packed));

struct EventTransportActorCommand {
    u8 opcode;
    u8 actor_id;
    u8 map_cell_x;
    u8 map_cell_y;
    u8 direction;
    u8 behavior;
} __attribute__((packed));

struct EventPilotPortraitCommand {
    u8 opcode;
    u8 pilot_id;
    u8 portrait_variant;
} __attribute__((packed));

struct EventTeamPilotConditionCommand {
    u8 opcode;
    u8 pilot_id;
} __attribute__((packed));

struct EventCombinationConditionCommand {
    u8 opcode;
    u8 combined_model_id;
} __attribute__((packed));


struct EventBattleSceneModelCommand {
    u8 opcode;
    u8 model_id;
    u8 palette_variant;
    u8 side;
    u8 impact_flags;
} __attribute__((packed));

struct EventBattleScenePilotModelCommand {
    u8 opcode;
    u8 pilot_id;
    u8 side;
    u8 impact_flags;
} __attribute__((packed));

#define EVENT_BATTLE_SCENE_MODEL_OFFSET(field) \
    ((s32)&((struct EventBattleSceneModelCommand *)0)->field)
#define EVENT_BATTLE_SCENE_PILOT_MODEL_OFFSET(field) \
    ((s32)&((struct EventBattleScenePilotModelCommand *)0)->field)

struct EventBattleUnitModelCommand {
    u8 opcode;
    u8 model_id;
    u8 palette_variant;
    u8 side;
    u8 unit_slot;
    u8 sprite_entry_mode;
} __attribute__((packed));

struct EventBattleUnitSpritePositionCommand {
    u8 opcode;
    u8 model_id;
    u8 palette_variant;
    u8 sprite_index;
    u8 world_x_low;
    u8 world_x_high;
    u8 world_z_low;
    u8 world_z_high;
    u8 flip_x;
} __attribute__((packed));

struct EventBattleUnitPilotModelCommand {
    u8 opcode;
    u8 pilot_id;
    u8 side;
    u8 unit_slot;
    u8 sprite_entry_mode;
} __attribute__((packed));

struct EventBattlePlayerTeamModelsCommand {
    u8 opcode;
    u8 side;
    u8 sprite_entry_mode;
} __attribute__((packed));

struct EventBattleEncounterModelsCommand {
    u8 opcode;
    u8 encounter_group;
    u8 formation_index;
    u8 side;
    u8 sprite_entry_mode;
} __attribute__((packed));

struct EventBattleFuzorModelsCommand {
    u8 opcode;
    u8 sprite_entry_mode;
} __attribute__((packed));

/* The native two-byte command also reads byte 3 from the following command. */
struct EventBattleFuzorModelsLookaheadView {
    struct EventBattleFuzorModelsCommand command;
    u8 following_command_opcode;
    u8 following_command_side_byte;
} __attribute__((packed));

#define EVENT_FUZOR_DESTINATION_SIDE(command) \
    M2C_FIELD(command, u8 *, (s32)&((struct EventBattleFuzorModelsLookaheadView *)0)->following_command_side_byte)

/* Slot arithmetic keeps the four-byte player-state header before each Zoid view. */
struct EventPlayerZoidSlotView {
    u8 reserved00[4];
    u8 model_id;
    u8 palette_variant;
    u8 reserved06[2];
    u16 flags;
    u8 reserved0A[0x32];
    u8 size_class;
    u8 reserved3D[0x33];
};

struct EventEncounterModelSlotView {
    u8 reserved00[4];
    u8 model_id;
    u8 palette_variant;
};

struct EventBattleIconPositionView {
    u8 reserved00[0x28];
    s32 world_x_fixed8;
    s32 world_y_fixed8;
    s32 world_z_fixed8;
};

enum EventBattleSpriteEntryMode {
    EVENT_BATTLE_SPRITES_AT_FORMATION = 0,
    EVENT_BATTLE_SPRITES_ENTER_FROM_SIDE = 1
};

enum EventBattleModelResources {
    EVENT_FUZOR_COMPONENT_MODELS_ROM = 0x087A18E0,
    EVENT_FUZOR_COMPONENT_REQUIRED_FLAG = 0x10,
    EVENT_PLAYER_PILOTS_RAM = 0x02027378,
    EVENT_PLAYER_STATE_RAM = 0x020218E4,
    EVENT_ZOID_BASE_RECORDS_ROM = 0x087AFCC4,
    EVENT_BATTLE_UNIT_SPRITES_RAM = 0x02032E8C,
    EVENT_BATTLE_SPRITE_MOTION_RAM = 0x02032EEC,
    EVENT_ENCOUNTER_RECORDS_ROM = 0x087B9454
};

struct EventFieldActorBehaviorCommand {
    u8 opcode;
    u8 actor_id;
    u8 behavior;
} __attribute__((packed));

struct EventBattleEquipmentSpriteCommand {
    u8 opcode;
    u8 equipment_slot;
    u8 resource_variant;
} __attribute__((packed));

struct EventBattleEquipmentAnimationCommand {
    u8 opcode;
    u8 equipment_slot;
    u8 item_id_low;
    u8 item_id_high;
} __attribute__((packed));

struct EventBattleMountedEquipmentCommand {
    u8 opcode;
    u8 equipment_slot;
} __attribute__((packed));

struct EventBattleImpactCommand {
    u8 opcode;
    u8 item_id_low;
    u8 item_id_high;
} __attribute__((packed));

struct EventBattleModelEquipmentCommand {
    u8 opcode;
    u8 model_id;
    u8 equipment_slot;
} __attribute__((packed));

struct EventBitLigerFormIndexCommand {
    u8 opcode;
    u8 form_index;
} __attribute__((packed));

struct EventBitLigerFormChangeCommand {
    u8 opcode;
    u8 form_index_or_unlock_mask;
} __attribute__((packed));

struct EventWorldMapStructureCommand {
    u8 opcode;
    u8 structure_variant;
    u8 map_cell_x;
    u8 map_cell_y;
};

struct EventWorldMapStructureCursor {
    struct EventWorldMapStructureCommand *command;
};

struct WorldMapStructureSpriteOffsetBitsView {
    u8 reserved00[10];
    u16 offset_y_bits;
};

struct WorldMapStructureSpriteOffsetView {
    u32 flags;
    u8 reserved04[6];
    s16 offset_y;
};

struct WorldMapStructureAppearanceSprites {
    struct WorldMapStructureSpriteOffsetView *structure;
    struct WorldMapStructureSpriteOffsetView *dust;
};

struct WorldMapStructureScrollView {
    u32 bg0_x_fixed8;
    s32 bg0_y_fixed8;
    u32 bg1_x_fixed8;
    s32 bg1_y_fixed8;
    u32 bg2_x_fixed8;
    s32 bg2_y_fixed8;
    u32 bg3_x_fixed8;
    s32 bg3_y_fixed8;
};

enum EventBattlePhalanxResources {
    EVENT_PHALANX_ARROW_OPCODE = 0x62,
    EVENT_PHALANX_T_H_OPCODE = 0x63,
    EVENT_PHALANX_CANNON_OPCODE = 0x64,
    EVENT_PHALANX_OPCODE_TO_DECK_ADD = 0xCC
};

enum EventBitLigerFormResources {
    BIT_PILOT_ID = 30,
    BIT_ALTERNATE_PILOT_ID = 95,
    BIT_LIGER_ZERO_FIRST_MODEL = 25,
    BIT_LIGER_ZERO_LAST_MODEL = 30
};

enum WorldMapStructureResources {
    WORLD_MAP_STRUCTURE_STATE_RAM = 0x0202ECF4,
    WORLD_MAP_STRUCTURE_SPRITE_RAM = 0x020314A0,
    WORLD_MAP_STRUCTURE_GRAPHICS_TABLE_ROM = 0x087AF9D4,
    WORLD_MAP_STRUCTURE_SPRITE_TABLE_ROM = 0x087AFA94,
    WORLD_MAP_STRUCTURE_RESOURCE_SLOT = 12,
    WORLD_MAP_STRUCTURE_TILE_OFFSET = 0x398,
    WORLD_MAP_STRUCTURE_PALETTE_BANK = 10
};

struct EventFlagCommand {
    u8 opcode;
    u8 flag_id;
};

struct EventActorConditionCommand {
    u8 opcode;
    u8 actor_id;
};

struct EventBattleCommand {
    u8 opcode;
    u8 encounter_id;
    u8 formation_id;
    u8 terrain_id;
    u8 options;
};

struct EventSongCommand {
    u8 opcode;
    u8 song_id;
} __attribute__((packed));

struct EventScreenFadeCommand {
    u8 opcode;
    u8 duration_updates;
} __attribute__((packed));

struct EventBattleRuleCommand {
    u8 opcode;
    u8 rule_id;
} __attribute__((packed));

struct EventBattleCameraUnitCommand {
    u8 opcode;
    u8 side_id;
    u8 unit_id;
} __attribute__((packed));

struct EventFieldCameraMoveCommand {
    u8 opcode;
    u8 direction;
    u8 distance_pixels_low;
    u8 distance_pixels_high;
    u8 duration_updates;
} __attribute__((packed));

struct EventFieldCameraActorCommand {
    u8 opcode;
    u8 actor_id;
    u8 duration_updates;
} __attribute__((packed));

/* Short commands still read both argument bytes from the script stream. */
struct EventFieldActorCommand {
    u8 opcode;
    u8 actor_id;
    u8 argument0;
    u8 argument1;
} __attribute__((packed));

struct EventFieldMapPositionCommand {
    u8 opcode;
    u8 map_id;
    u8 map_cell_x;
    u8 map_cell_y;
} __attribute__((packed));

struct EventFieldViewCenterCommand {
    u8 opcode;
    u8 map_cell_x;
    u8 map_cell_y;
} __attribute__((packed));

struct EventFieldMapGroupView {
    u8 reserved00[0x1D];
    u8 map_group_id;
    u8 reserved1E[2];
};

struct EventFieldMapStateView {
    u16 map_id;
    u8 reserved02[0xA];
    s32 saved_world_map_x_fixed8;
    s32 saved_world_map_y_fixed8;
    u8 reserved14[0xA];
    u8 world_structure_kind;
    u8 world_structure_cell_x;
    u8 world_structure_cell_y;
    u8 reserved21;
    u16 return_map_id22;
    u16 return_map_id24;
    u8 reserved26[0x16A];
    u32 visited_map_group_bits[8];
};

struct EventFieldReturnMapEntry {
    u16 map_id;
    u8 reserved02[8];
} __attribute__((packed));

struct EventPlayerZoidCommand {
    u8 opcode;
    u8 model_id;
    u8 palette_variant;
} __attribute__((packed));

struct EventPlayerZoidPilotCommand {
    u8 opcode;
    u8 model_id;
    u8 palette_variant;
    u8 pilot_id;
} __attribute__((packed));

struct EventPlayerPilotCommand {
    u8 opcode;
    u8 pilot_id;
} __attribute__((packed));

struct EventPlayerItemCommand {
    u8 opcode;
    u8 item_id;
} __attribute__((packed));

struct EventPlayerZoidDataCommand {
    u8 opcode;
    u8 model_id;
} __attribute__((packed));

struct EventPlayerDeckCommand {
    u8 opcode;
    u8 deck_command_id;
} __attribute__((packed));

struct EventPulseEmotionGrowthCommand {
    u8 opcode;
    u8 growth_amounts[4];
} __attribute__((packed));

struct EventPulseEffectCommand {
    u8 opcode;
    u8 effect_kind;
    u8 effect_value_low;
    u8 effect_value_high;
} __attribute__((packed));

enum EventPulseResources {
    EVENT_PULSE_GROWTH_SOUND = 0x35,
    EVENT_PULSE_LEVEL_UP_SOUND = 0x34,
    EVENT_PULSE_CONFIRM_SOUND = 0x41,
    EVENT_PULSE_WHITE_GROWTH_TEXT = 0x08103D20,
    EVENT_PULSE_RED_GROWTH_TEXT = 0x08103D30,
    EVENT_PULSE_BLUE_GROWTH_TEXT = 0x08103D40,
    EVENT_PULSE_BLACK_GROWTH_TEXT = 0x08103D50,
    EVENT_PULSE_CONFIRM_EMOTION_TEXT = 0x08103D60,
    EVENT_PULSE_EFFECT_NAME_POINTERS_ROM = 0x087EF410,
    EVENT_PULSE_EMOTION_GROWTH_MENU = 0x08017A0B,
    EVENT_PULSE_CONFIRM_EMOTION_MENU = 0x08017A2D,
    EVENT_PULSE_CLOSE_EMOTION_MENU = 0x08017A37,
    EVENT_PULSE_STAT_GROWTH_MENU = 0x08017A3F,
    EVENT_PULSE_LEARNED_EFFECTS_MENU = 0x08017AB7,
    EVENT_PULSE_CLOSE_LEVEL_UP_MENU = 0x08017AC4
};

struct EventPlayerMoneyCommand {
    u8 opcode;
    u8 hundreds;
} __attribute__((packed));

enum EventPlayerAssetConstants {
    EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM = 0x02031756,
    EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = 0x0200A888,
    EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX = 0x0201,
    EVENT_PLAYER_ASSET_MESSAGE_DEFAULT_PREFIX = 0x0001,
    EVENT_PLAYER_ASSET_SOUND = 53,
    EVENT_PLAYER_MONEY_UNIT = 100
};

enum EventFieldConstants {
    FIELD_EVENT_ACTIVE_RAM = 0x02030664,
    FIELD_VIEW_CENTER_RAM = 0x02032494,
    FIELD_CAMERA_SCROLL_RAM = 0x03000054,
    FIELD_CONTROLLED_ACTOR_POINTER_RAM = 0x02032990,
    FIELD_CAMERA_MOVE_TASK = 4,
    FIELD_CAMERA_MOVE_TASK_THUMB = 0x0809FD61,
    FIELD_MAP_TRANSITION_TASK = 7,
    FIELD_MAP_TRANSITION_TASK_THUMB = 0x0809E8CD,
    FIELD_RETURN_MAP_TABLE_ROM = 0x087D223C,
    FIELD_RETURN_MAP_TABLE24_ROM = 0x087D2322,
    FIELD_MAP_GROUP_WORLD_POSITIONS_ROM = 0x087AFBB4,
    FIELD_MAP_CELL_PIXELS = 8,
    FIELD_PACKED_MAP_CELL_PIXELS = 16,
    FIELD_SCREEN_CENTER_X_FIXED8 = 0x7800,
    FIELD_SCREEN_CENTER_Y_FIXED8 = 0x5000
};

#define EVENT_COMMAND_OFFSET(type, field) ((s32)&((struct type *)0)->field)
#define EVENT_COMMAND_BYTE(command, type, field) \
    M2C_FIELD(command, u8 *, EVENT_COMMAND_OFFSET(type, field))


struct EventBattleCombinationCommand {
    u8 opcode;
    u8 model_id;
    u8 pilot_id;
    u8 side;
} __attribute__((packed));

enum EventBattleCombinationModel {
    EVENT_COMBINATION_KILLER_SPINER = 0x47,
    EVENT_COMBINATION_GOJULAS_GIGA_C = 0x76,
    EVENT_COMBINATION_FUZOR_DRAGON = 0x80,
    EVENT_COMBINATION_CHIMERA_DRAGON = 0x81,
    EVENT_COMBINATION_GOJULOX = 0x82,
    EVENT_COMBINATION_TWO_ARM_LIZARD = 0x83,
    EVENT_COMBINATION_LORD_GALE = 0x97
};

#endif
