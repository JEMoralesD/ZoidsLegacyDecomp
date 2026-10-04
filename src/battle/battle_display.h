#ifndef BATTLE_DISPLAY_H
#define BATTLE_DISPLAY_H

#include "battle.h"
#include "../engine/math3d.h"

enum BattleSceneSkipWindowState {
    BATTLE_SCENE_SKIP_WINDOW_IDLE = 0,
    BATTLE_SCENE_SKIP_WINDOW_OPENING = 1,
    BATTLE_SCENE_SKIP_WINDOW_CLOSING = 2
};

struct BattleBodyAttachmentView {
    u8 reserved00[4];
    u16 shield_x;
    s16 shield_y;
    u8 reserved08[8];
    s16 phalanx_x;
    s16 phalanx_y;
    u8 reserved14[0xC];
};

struct SceneProjectedSpriteView {
    s32 flags;
    u8 reserved04[6];
    s16 offset_y;
    u8 reserved0C[0x1C];
    s32 world_x_fixed8;
    s32 world_y_fixed8;
    s32 world_z_fixed8;
};

struct BattleSceneSetupView {
    u8 reserved00;
    u8 reserved01;
    u8 terrain_id;
};

enum DeathMeteorDefeatPhase {
    DEATH_METEOR_DEFEAT_BEAM_DESCENDING = 0,
    DEATH_METEOR_DEFEAT_GENO_FLAME_MOVING = 1
};

enum BattleSpriteMotion {
    BATTLE_SPRITE_MOTION_IDLE = 0,
    BATTLE_SPRITE_MOTION_RETURN_TO_FORMATION = 1,
    BATTLE_SPRITE_MOTION_MOVE_OUTWARD = 2
};

/* ToggleBattleShieldSpriteVisibility needs its Thumb address outside the owning region. */
enum BattleShieldSpriteCallback {
    BATTLE_SHIELD_SPRITE_VISIBILITY_CALLBACK = 0x080DB965
};

enum BattleSpriteFlags {
    BATTLE_SPRITE_ACTIVE = 1,
    BATTLE_SPRITE_ANIMATION_FINISHED = 4,
    BATTLE_SPRITE_ANIMATION_PAUSED = 8,
    BATTLE_SPRITE_HOLD_LAST_FRAME = 0x10,
    BATTLE_SPRITE_LOOP_ANIMATION = 0x20,
    BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1 = 0x100,
    BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_2 = 0x200,
    BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_3 = 0x300,
    BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_MASK = 0x300,
    BATTLE_SPRITE_SEMITRANSPARENT = 0x400,
    BATTLE_SPRITE_BACKGROUND_RELATIVE = 0x1000,
    BATTLE_SPRITE_BACKGROUND_INDEX_MASK = 0x6000,
    BATTLE_SPRITE_BACKGROUND_INDEX_SHIFT = 13,
    BATTLE_SPRITE_BACKGROUND_2 = 0x4000,
    BATTLE_SPRITE_FLIP_X = 0x8000,
    BATTLE_SPRITE_HIDDEN = 0x20000,
    BATTLE_SPRITE_DOUBLE_CANVAS = 0x80000
};

struct SpriteBackgroundScrollOffsets {
    s32 x_fixed8;
    s32 y_fixed8;
};

enum BattleGaugeLevels {
    BATTLE_GAUGE_FULL = 20,
    BATTLE_GAUGE_GRAY = 0xFF
};

enum BattleCameraMode {
    BATTLE_CAMERA_CENTERED = 0,
    BATTLE_CAMERA_INTRO_OFFSET = 1,
    BATTLE_CAMERA_FOCUS_UNIT = 5,
    BATTLE_CAMERA_CENTERED_ALTERNATE = 6,
    BATTLE_CAMERA_KEEP_TARGET = 7,
    BATTLE_CAMERA_INTRO_CENTERED = 8,
    BATTLE_CAMERA_FOCUS_UNIT_CLOSE = 9
};

struct BattleDisplaySprite;
struct BattleAnimationGroup;

union BattleSpriteUserData {
    struct Vector3 position;
    struct {
        struct Vector3 position;
        s32 bob_phase;
    } bobbing_projection;
    struct {
        u32 x_fixed8;
        u32 speed_fixed8;
    } horizontal_streak;
    struct {
        s32 speed;
    } horizontal_projectile;
    struct {
        s32 x_fixed8;
        s32 y_fixed8;
        s32 angle;
        s32 speed_fixed8;
    } angled_projectile;
    struct {
        struct BattleAnimationGroup *group;
        u32 elapsed_frames;
        u32 impact_x;
    } projectile_trail;
    struct {
        struct BattleAnimationGroup *group;
        u32 elapsed_frames;
        s32 x_fixed8;
        s32 trail_resource_slot_base;
    } accelerating_missile;
    struct {
        struct BattleAnimationGroup *group;
        u32 elapsed_frames;
        s32 x_fixed8;
        s32 y_fixed8;
    } accelerating_rising_missile;
    struct {
        struct BattleAnimationGroup *group;
        u32 elapsed_frames;
        s32 impact_x;
        s32 trail_resource_slot_base;
    } missile_trail;
    struct {
        struct BattleDisplaySprite *unit_sprite;
        u32 override_flags;
        u8 data08[8];
    } gauge;
    struct {
        s32 elapsed_updates;
        s32 destroy_update;
        u8 reserved08[8];
    } popup_glyph;
    u32 words[4];
};

struct BattleDisplaySprite {
    u32 flags;
    u16 x;
    u16 y;
    u16 offset_x;
    u16 offset_y;
    s16 scale;
    u16 tile_offset;
    u8 palette_bank;
    u8 rotation;
    u16 animation_id;
    u16 animation_step;
    u16 frame_timer;
    s32 frame_table;
    s32 animation_table;
    s32 graphics;
    s32 update_callback;
    union BattleSpriteUserData user_data;
};

enum BattleStreakParticleResources {
    BATTLE_STREAK_RESOURCE_SLOT = 119,
    BATTLE_STREAK_GRAPHICS_TABLE_ROM = 0x087AC9F8,
    BATTLE_STREAK_SPRITE_TABLE_ROM = 0x087ACDD8,
    BATTLE_STREAK_TILE_OFFSET = 0x35C,
    BATTLE_STREAK_PALETTE_BANK = 10,
    BATTLE_STREAK_UPDATE_CALLBACK = 0x080CCEF1,
    BATTLE_PHALANX_FIRST_DECK_COMMAND = 46,
    BATTLE_PHALANX_FRAME_COUNT = 152,
    BATTLE_PHALANX_FADE_START = 120
};

struct BattleStreakParticleGroupView {
    u32 flags;
    s32 x;
    s32 y;
    void *sprite_slots[32];
    s32 palette_variant;
};

#define BATTLE_STREAK_GROUP_OFFSET(field) \
    ((s32)&((struct BattleStreakParticleGroupView *)0)->field)

#define BATTLE_STREAK_GROUP_FIELD(group, type, field) \
    M2C_FIELD(group, type *, BATTLE_STREAK_GROUP_OFFSET(field))

enum BattleDiagonalCloudResources {
    BATTLE_DIAGONAL_CLOUD_SCENE_ID = 0xFD,
    BATTLE_DIAGONAL_CLOUD_COUNT = 8,
    BATTLE_DIAGONAL_CLOUD_SPRITE_TABLE_ROM = 0x087AFA94,
    BATTLE_DIAGONAL_CLOUD_RESOURCE_SLOT = 9,
    BATTLE_DIAGONAL_CLOUD_TILE_OFFSET = 0x180,
    BATTLE_DIAGONAL_CLOUD_PALETTE_BANK = 3,
    BATTLE_DIAGONAL_CLOUD_UPDATE_CALLBACK = 0x080CD259
};

enum BattleTargetPreviewNumberResources {
    BATTLE_PREVIEW_NUMBER_BUFFER_RAM = 0x02030564,
    BATTLE_PREVIEW_PERCENT_TEXT_ROM = 0x08108C14,
    BATTLE_PREVIEW_UNKNOWN_TEXT_ROM = 0x08108C18,
    BATTLE_PREVIEW_GLYPH_TABLE_ROM = 0x087AC9F0,
    BATTLE_PREVIEW_GLYPH_TILE_OFFSET = 0x330,
    BATTLE_PREVIEW_GLYPH_PALETTE_BANK = 11
};

struct BattleTargetPreviewNumberGroupView {
    u32 flags;
    s32 x;
    s32 y;
    void *glyph_sprites[32];
    s32 current_values[2];
    s32 target_values[2];
};

#define BATTLE_PREVIEW_NUMBER_OFFSET(field) \
    ((s32)&((struct BattleTargetPreviewNumberGroupView *)0)->field)

struct BattleSelectedTarget {
    u8 side;
    u8 unit_slot;
} __attribute__((packed));

enum BattleSelectedTargetResources {
    BATTLE_SELECTED_TARGET_LIST_OFFSET = 0xA058,
    BATTLE_SELECTED_TARGET_FIRST_SLOT_OFFSET = 0xA059,
    BATTLE_SELECTED_TARGET_FIRST_SIDE_RAM = 0x0203EBA4,
    BATTLE_SELECTED_TARGET_OUTCOMES_RAM = 0x0203ED02,
    BATTLE_SELECTED_TARGET_COUNT = 12,
    BATTLE_SELECTED_TARGET_EMPTY = 0xFF
};

enum BattleSelectionCursorMode {
    BATTLE_SELECTION_CURSOR_TRAIL = 0,
    BATTLE_SELECTION_CURSOR_TARGET_AREA = 1
};

enum BattleSelectionCursorResources {
    BATTLE_SELECTION_CURSOR_COUNT = 7,
    BATTLE_SELECTION_CURSOR_LAST = 6,
    BATTLE_SELECTION_CURSOR_MOVE_UPDATES = 8,
    BATTLE_SELECTION_CURSOR_SPRITES_RAM = 0x02033F58,
    BATTLE_SELECTION_CURSOR_START_POSITIONS_RAM = 0x02033F74,
    BATTLE_SELECTION_CURSOR_TARGET_POSITIONS_RAM = 0x02033F90,
    BATTLE_SELECTION_CURSOR_PROGRESS_RAM = 0x02033FAC,
    BATTLE_SELECTION_CURSOR_MODE_RAM = 0x02033FB3,
    BATTLE_SELECTION_CURSOR_TILE_OFFSET = 0x300,
    BATTLE_SELECTION_CURSOR_PALETTE_BANK = 13,
    BATTLE_EQUIPMENT_CURSOR_POSITIONS_PER_MODEL = 8,
    BATTLE_SCENE_SIDE_RAM = 0x02033F36,
    BATTLE_SCENE_UNIT_SLOT_RAM = 0x02033F37
};

struct BattleSelectionCursorPosition {
    u16 x;
    u16 y;
};

/* Interpolation reads signed coordinates from the sprite's unsigned position fields. */
struct BattleSelectionCursorSignedPositionView {
    u32 flags;
    s16 x;
    s16 y;
};

#define BATTLE_SELECTION_OFFSET(field) \
    (0xA1AF + (s32)&((struct BattleActionSelection *)0)->field)

enum BattleTargetSelectionIconResources {
    BATTLE_TARGET_SELECTION_ICONS_RAM = 0x02033FB4,
    BATTLE_TARGET_SELECTION_ICON_TILE_BASE = 0x180,
    BATTLE_TARGET_SELECTION_ICON_TILES_PER_UNIT = 0x40,
    BATTLE_TARGET_SELECTION_ICON_PALETTE_BASE = 3,
    BATTLE_TARGET_SELECTION_ICON_SCALE = 0x80,
    BATTLE_TARGET_SELECTION_ICON_FRAMES_ROM = 0x0821024C,
    BATTLE_TARGET_SELECTION_ICON_ANIMATIONS_ROM = 0x08210258
};

enum BattleQueuedEquipmentWindowResources {
    BATTLE_QUEUED_EQUIPMENT_FIRST_WINDOW = 4,
    BATTLE_QUEUED_EQUIPMENT_OPEN_FIRST_MENU = 0x08004132,
    BATTLE_QUEUED_EQUIPMENT_OPEN_SECOND_MENU = 0x08004148,
    BATTLE_QUEUED_EQUIPMENT_OPEN_THIRD_MENU = 0x0800415E,
    BATTLE_QUEUED_EQUIPMENT_CLOSE_FIRST_MENU = 0x08004174,
    BATTLE_QUEUED_EQUIPMENT_CLOSE_SECOND_MENU = 0x08004177,
    BATTLE_QUEUED_EQUIPMENT_CLOSE_THIRD_MENU = 0x0800417A
};

struct BattleQueuedEquipmentStateView {
    u8 data0000[0x27A8];
    s32 acting_unit_address;
    u8 data27AC[0x7A04];
    u8 equipment_slots[BATTLE_ACTION_COUNT];
};

enum BattlePilotQuoteKind {
    BATTLE_PILOT_QUOTE_ATTACK = 0,
    BATTLE_PILOT_QUOTE_COMMAND = 1,
    BATTLE_PILOT_QUOTE_DAMAGED = 2,
    BATTLE_PILOT_QUOTE_EVADED = 3,
    BATTLE_PILOT_QUOTE_DESTROYED = 4
};

enum BattlePilotQuoteResources {
    BATTLE_PILOT_QUOTE_PILOT_COUNT = 105,
    BATTLE_PILOT_QUOTE_KIND_COUNT = 5,
    BATTLE_PILOT_QUOTES_PER_KIND = 3,
    BATTLE_PILOT_QUOTE_TABLE_ROM = 0x087EF590,
    BATTLE_PILOT_CUSTOM_QUOTES_RAM = 0x0203EE0C,
    BATTLE_PILOT_CUSTOM_QUOTE_BYTES = 46,
    BATTLE_PILOT_QUOTE_PORTRAIT_TILE_OFFSET = 0x3DC,
    BATTLE_PILOT_QUOTE_PORTRAIT_PALETTE_BANK = 15,
    BATTLE_PILOT_QUOTE_PORTRAIT_BUFFER_RAM = 0x02002880
};

struct BattlePilotQuoteEntry {
    s32 text;
    u8 portrait_variant;
    u8 data05[3];
} __attribute__((packed));

struct BattlePilotQuoteSet {
    /* A fourth quote index spills into the next kind or pilot in the ROM. */
    struct BattlePilotQuoteEntry quotes[BATTLE_PILOT_QUOTE_KIND_COUNT][BATTLE_PILOT_QUOTES_PER_KIND];
};

#define BATTLE_PILOT_QUOTE_FIELD(entry, type, field) \
    M2C_FIELD(entry, type *, (s32)&((struct BattlePilotQuoteEntry *)0)->field)

struct BattleEscapeStateView {
    struct BattleSide sides[BATTLE_SIDE_COUNT];
    u8 data2700[12];
    u8 elapsed_rounds;
    u8 escape_roll;
};

#define BATTLE_SPRITE_FIELD(sprite, type, field) \
    M2C_FIELD(sprite, type *, (s32)&((struct BattleDisplaySprite *)0)->field)

#define BATTLE_SPRITE_OFFSET(field) \
    ((s32)&((struct BattleDisplaySprite *)0)->field)

#define BATTLE_ESCAPE_FIELD(state, type, field) \
    M2C_FIELD(state, type *, (s32)&((struct BattleEscapeStateView *)0)->field)

enum BattleStatusStatRow {
    BATTLE_STATUS_ROW_HP = 0,
    BATTLE_STATUS_ROW_EP = 1,
    BATTLE_STATUS_ROW_EP_REGEN = 2,
    BATTLE_STATUS_ROW_SPEED = 4,
    BATTLE_STATUS_ROW_MOBILITY = 5,
    BATTLE_STATUS_ROW_INITIATIVE = 6,
    BATTLE_STATUS_ROW_DEFENSE = 8,
    BATTLE_STATUS_ROW_ARMOR_RATE = 9,
    BATTLE_STATUS_ROW_DCP = 10,
    BATTLE_STATUS_ROW_SENSOR_ACCURACY = 12,
    BATTLE_STATUS_ROW_FREEZE = 14,
    BATTLE_STATUS_ROW_PILOT_INACTIVE = 15,
    BATTLE_STATUS_ROW_EXTRA_TURNS = 16,
    BATTLE_STATUS_ROW_ENERGY_SHIELD = 17
};

enum BattleStatusComparisonColor {
    BATTLE_STATUS_CURRENT_EQUALS_BASE = 0,
    BATTLE_STATUS_CURRENT_BELOW_BASE = 1,
    BATTLE_STATUS_CURRENT_ABOVE_BASE = 2
};

enum BattleStatusViewerResources {
    BATTLE_STATUS_CURSOR_GRAPHICS_ROM = 0x087AC9D8,
    BATTLE_STATUS_CURSOR_SPRITES_ROM = 0x087AC9E0,
    BATTLE_STATUS_BASE_STATS_RAM = 0x02033EC4,
    BATTLE_STATUS_OPEN_MENU = 0x08003F08,
    BATTLE_STATUS_CLOSE_MENU = 0x08003F84,
    BATTLE_STATUS_KEY_RIGHT = 0x10,
    BATTLE_STATUS_KEY_LEFT = 0x20,
    BATTLE_STATUS_KEY_UP = 0x40,
    BATTLE_STATUS_KEY_DOWN = 0x80,
    BATTLE_STATUS_CLOSE_KEYS = 3,
    BATTLE_STATUS_LOW_HP_COLOR = 1,
    BATTLE_STATUS_STAT_PLACEHOLDER_TEXT = 0x08107658
};

#endif
