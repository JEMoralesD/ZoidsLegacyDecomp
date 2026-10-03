#ifndef BATTLE_ANIMATION_H
#define BATTLE_ANIMATION_H

enum BattleAnimationLimits {
    BATTLE_ANIMATION_FINISH_DELAY_FRAMES = 30,
    BATTLE_ANIMATION_ENTRY_COUNT = 8,
    BATTLE_ANIMATION_RESOURCE_COUNT = 8,
    BATTLE_ANIMATION_SOUND_COUNT = 5,
    BATTLE_ANIMATION_GROUP_COUNT = 16,
    BATTLE_ANIMATION_GROUP_SPRITE_COUNT = 32
};

enum BattleAnimationControl {
    BATTLE_ANIMATION_VARIANT_MASK = 0x03,
    BATTLE_ANIMATION_ANCHOR_MASK = 0x3C,
    BATTLE_ANIMATION_SPRITE_PRIORITY_SHIFT = 6,
    BATTLE_ANIMATION_SPRITE_PRIORITY_MASK = 0xC0
};

enum BattleAnimationAnchor {
    BATTLE_ANIMATION_ANCHOR_SCREEN = 0,
    BATTLE_ANIMATION_ANCHOR_WORLD = 4,
    BATTLE_ANIMATION_ANCHOR_EQUIPMENT = 8,
    BATTLE_ANIMATION_ANCHOR_ZOID_POINT_0 = 12,
    BATTLE_ANIMATION_ANCHOR_ZOID_POINT_1 = 16,
    BATTLE_ANIMATION_ANCHOR_ZOID_POINT_2 = 20,
    BATTLE_ANIMATION_ANCHOR_ZOID_POINT_3 = 24,
    BATTLE_ANIMATION_ANCHOR_ZOID_POINT_4 = 28,
    BATTLE_ANIMATION_ANCHOR_ZOID_POINT_5 = 32
};

enum BattleAnimationScriptControl {
    BATTLE_ANIMATION_SCRIPT_END = -1,
    BATTLE_ANIMATION_SCRIPT_LOOP = -2
};

enum BattleAnimationStatus {
    BATTLE_ANIMATION_IDLE = 0,
    BATTLE_ANIMATION_ACTIVE = 1,
    BATTLE_ANIMATION_SCRIPT_FINISHED = 2,
    BATTLE_ANIMATION_SKIPPED = 3
};

enum BattleAnimationResult {
    BATTLE_ANIMATION_RESULT_WAITING = 0,
    BATTLE_ANIMATION_RESULT_FINISHED = 1,
    BATTLE_ANIMATION_RESULT_SKIPPED = 2
};

enum BattleAnimationPresentationKind {
    BATTLE_ANIMATION_EQUIPMENT = 0,
    BATTLE_ANIMATION_IMPACT = 1,
    BATTLE_ANIMATION_NO_IMPACT = 2,
    BATTLE_ANIMATION_STATIONARY_NO_IMPACT = 3,
    BATTLE_ANIMATION_SHIELD_IMPACT = 4,
    BATTLE_ANIMATION_SCENE = 5,
    BATTLE_ANIMATION_SLIDING_IMPACT = 6
};

enum BattleScanlineWindowMode {
    BATTLE_SCANLINE_WINDOW_DISABLED = 0,
    BATTLE_SCANLINE_WINDOW_OPENING = 1,
    BATTLE_SCANLINE_WINDOW_ACTIVE = 2,
    BATTLE_SCANLINE_WINDOW_CLOSING = 3
};

enum BattleScanlineWindowPhase {
    BATTLE_SCANLINE_WINDOW_PHASE_TRANSITION = 0,
    BATTLE_SCANLINE_WINDOW_PHASE_ACTIVE = 1,
    BATTLE_SCANLINE_WINDOW_PHASE_DISABLED = 2
};

enum BattleScanlineWindowDisplayRequest {
    BATTLE_SCANLINE_WINDOW_DISPLAY_NONE = 0,
    BATTLE_SCANLINE_WINDOW_DISPLAY_START = 1,
    BATTLE_SCANLINE_WINDOW_DISPLAY_STOP = 2
};

enum BattleBackgroundShakeState {
    BATTLE_BACKGROUND_SHAKE_IDLE = 0,
    BATTLE_BACKGROUND_SHAKE_ACTIVE = 1,
    BATTLE_BACKGROUND_SHAKE_STOPPING = 2
};

enum ZoidAnimationControl {
    ZOID_ANIMATION_SCRIPT_END = -1,
    ZOID_ANIMATION_COMPLETION_MARKER = -2,
    ZOID_ANIMATION_PLAY_SOUND = -3,
    ZOID_MOUNT_ANIMATION_FINISHED = 0x04,
    ZOID_MOUNT_ANIMATION_PAUSED = 0x08
};

struct ZoidAnimationCommand {
    s32 frame_offset_or_command;
    s32 duration_or_song;
};

struct ZoidAnimationFrameHeader {
    u16 x;
    u16 y;
    u16 width;
    u16 height;
};

struct BattleAnimationEntry {
    s16 *script;
    u8 control;
    u8 data05;
    s16 x_offset;
    u16 y_offset;
    u16 data0A;
};

struct BattleAnimationRecord {
    struct BattleAnimationEntry entries[BATTLE_ANIMATION_ENTRY_COUNT];
};

struct BattleAnimationResourceHeader {
    u16 callback_kind;
    s16 resource_ids[BATTLE_ANIMATION_RESOURCE_COUNT];
    /* Lists end with -1. The loader's eight-entry bound can cross into the next header. */
    s16 sound_ids[BATTLE_ANIMATION_SOUND_COUNT];
};

struct BattleAnimationCommand {
    s16 delay_or_command;
    u16 x;
    u16 y;
    u16 sprite_priority;
};

struct BattleAnimationState {
    u8 status;
    u8 side;
    u8 zoid_id;
    u8 equipment_slot;
    u8 resource_variant;
    u8 anchor_mode;
    s16 x_offset;
    u16 y_offset;
    u16 sprite_flags;
    u8 data0C;
    s8 presentation_kind;
    u8 spawn_count;
    u8 data0F;
    s16 *script_cursor;
    s16 *loop_start;
    u16 callback_kind;
    u16 loop_count;
    u16 delay;
    u16 data1E;
    u32 groups[BATTLE_ANIMATION_GROUP_COUNT];
};

struct BattleAnimationCallbacks {
    u32 init_callback;
    u32 update_callback;
};

struct BattleAnimationGroup {
    u32 flags;
    s32 x;
    s32 y;
    u32 sprites[BATTLE_ANIMATION_GROUP_SPRITE_COUNT];
    s32 state;
    u8 data90[0x18];
    u32 sprite_flags;
    u32 init_callback;
    u32 update_callback;
};

#endif
