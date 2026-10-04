#ifndef FIELD_ACTOR_H
#define FIELD_ACTOR_H

#include "m2c_prelude.h"

struct FieldTransportStateView {
    u8 data00[2];
    u8 actor_model_id;
} __attribute__((packed));

#define FIELD_TRANSPORT_STATE_OFFSET(field) \
    ((s32)&((struct FieldTransportStateView *)0)->field)

enum FieldActorLimits {
    FIELD_ACTOR_COUNT = 14,
    FIELD_ACTOR_MAX_SLOT = 13,
    FIELD_ACTOR_MAX_ID = 13,
    FIELD_ACTOR_SLOT_NOT_FOUND = 0xFF,
    FIELD_ACTOR_RECORD_BYTES = 0x48,
    FIELD_ACTOR_STATE_WORD_COUNT = 8,
    FIELD_ACTOR_DIRECTION_COUNT = 8,
    FIELD_ACTOR_TURN_INTERVAL = 3,
    FIELD_ACTOR_HISTORY_COUNT = 24,
    FIELD_ACTOR_HISTORY_LAST = 23,
    FIELD_ACTOR_HISTORY_SHIFT_LAST = 22,
    FIELD_ACTOR_ALTERNATE_PALETTE_MODEL = 0x4B,
    FIELD_ACTORS_RAM = 0x020325A0
};

enum FieldActorFlags {
    FIELD_ACTOR_ACTIVE = 1,
    FIELD_ACTOR_INPUT_LOCKED = 2,
    FIELD_ACTOR_COLLISION_DISABLED = 4,
    FIELD_ACTOR_TARGET_MOVEMENT_PENDING = 8,
    FIELD_ACTOR_TARGET_MOVEMENT_ACTIVE = 0x10
};

enum FieldActorMovementMode {
    FIELD_ACTOR_IDLE = 0,
    FIELD_ACTOR_HALF_SPEED = 1,
    FIELD_ACTOR_NORMAL_SPEED = 2,
    FIELD_ACTOR_DOUBLE_SPEED = 3,
    FIELD_ACTOR_SPECIAL_SPRITE_ANIMATION = 4
};

enum FieldActorBehavior {
    FIELD_ACTOR_PLAYER_CONTROLLED = 0,
    FIELD_ACTOR_WANDERING = 1,
    FIELD_ACTOR_STATIONARY = 2,
    FIELD_ACTOR_TURN_ONLY = 3,
    FIELD_ACTOR_DIRECTION_OFFSET = 4,
    FIELD_ACTOR_MAP_INTERACTION = 5,
    FIELD_ACTOR_SCRIPTED_CAMERA_FOLLOW = 6,
    FIELD_ACTOR_SCRIPTED = 7,
    FIELD_ACTOR_FOLLOWER = 8,
    FIELD_ACTOR_FOLLOWER_CATCH_UP = 9,
    FIELD_ACTOR_NO_REGULAR_UPDATES = 0xFF
};

enum FieldActorDirection {
    FIELD_ACTOR_NORTH = 0,
    FIELD_ACTOR_NORTH_EAST = 1,
    FIELD_ACTOR_EAST = 2,
    FIELD_ACTOR_SOUTH_EAST = 3,
    FIELD_ACTOR_SOUTH = 4,
    FIELD_ACTOR_SOUTH_WEST = 5,
    FIELD_ACTOR_WEST = 6,
    FIELD_ACTOR_NORTH_WEST = 7
};

struct FieldActorSpriteOffsetView {
    u32 flags;
    u16 position_x_pixels;
    u16 position_y_pixels;
    u16 offset_x;
    u16 offset_y;
};

struct FieldActorPosition {
    s32 world_x_fixed8;
    s32 world_y_fixed8;
};

struct FieldActorPositionHistory {
    struct FieldActorPosition positions[FIELD_ACTOR_HISTORY_COUNT];
};

union FieldActorBehaviorState {
    s32 words[FIELD_ACTOR_STATE_WORD_COUNT];
    struct {
        s32 timer;
        u8 reserved2C[0x1C];
    } wandering;
    struct {
        u32 flag_id;
        u8 reserved2C[0x1C];
    } map_interaction;
    struct {
        u8 reserved28[0x18];
        s32 world_x_fixed8;
        s32 world_y_fixed8;
    } pending_target;
    struct {
        s32 x_distance_pixels;
        s32 y_distance_pixels;
        u32 x_decreases;
        u32 y_decreases;
        u32 x_accumulator;
        u32 y_accumulator;
        u32 period_ticks;
        u32 elapsed_ticks;
    } target_movement;
};

struct FieldActor {
    u32 flags;
    u8 model_id;
    u8 actor_id;
    u16 behavior;
    s32 world_x_fixed8;
    s32 world_y_fixed8;
    s32 velocity_x_fixed8;
    s32 velocity_y_fixed8;
    u8 movement_mode;
    u8 previous_movement_mode;
    u8 requested_direction;
    u8 current_direction;
    u8 turn_timer;
    u8 map_cell_change;
    u8 map_cell_value;
    u8 previous_map_cell_value;
    struct FieldActorSpriteOffsetView *sprite;
    struct FieldActorSpriteOffsetView *secondary_sprite;
    union FieldActorBehaviorState behavior_state;
};

struct FieldActorSpriteDefinition {
    void *frame_table;
    void *animation_table;
    void *graphics;
    void *palette;
};

enum FieldActorCommandLimits {
    FIELD_ACTOR_COMMAND_COUNT = 32,
    FIELD_ACTOR_COMMAND_BYTES = 8,
    FIELD_ACTOR_COMMAND_QUEUE_BYTES = 0x100,
    FIELD_ACTOR_COMMAND_SHIFT_LAST = 30,
    FIELD_ACTOR_COMMAND_END = 0,
    FIELD_ACTOR_COMMAND_QUEUES_RAM = 0x02030668,
    FIELD_ACTOR_COMMAND_TIMERS_RAM = 0x02031468,
    FIELD_ACTOR_SPECIAL_ANIMATION_SETS_ROM = 0x087A1C18,
    FIELD_ACTOR_SPECIAL_SPRITE_DEFINITIONS_ROM = 0x087ADAF8,
    FIELD_SPRITE_BLINK_CALLBACK_THUMB = 0x080AA59D
};

enum FieldActorCommandOpcode {
    FIELD_ACTOR_CMD_WAIT = 0x38,
    FIELD_ACTOR_CMD_MOVE_HALF_SPEED = 0x39,
    FIELD_ACTOR_CMD_MOVE_NORMAL_SPEED = 0x3A,
    FIELD_ACTOR_CMD_MOVE_DOUBLE_SPEED = 0x3B,
    FIELD_ACTOR_CMD_MOVE_TO_HALF_SPEED = 0x3C,
    FIELD_ACTOR_CMD_MOVE_TO_NORMAL_SPEED = 0x3D,
    FIELD_ACTOR_CMD_MOVE_TO_DOUBLE_SPEED = 0x3E,
    FIELD_ACTOR_CMD_HOP = 0x3F,
    FIELD_ACTOR_CMD_PLAY_SPECIAL_ANIMATION = 0x40
};

struct FieldActorCommand {
    u16 opcode;
    u16 argument0;
    u16 argument1;
    u16 argument2;
};

struct FieldActorCommandQueue {
    struct FieldActorCommand commands[FIELD_ACTOR_COMMAND_COUNT];
};

struct FieldActorSpecialAnimation {
    u8 sprite_resource_id;
    u8 animation_id;
    u8 keep_secondary_sprite;
    u8 use_zero_y_offset;
};

struct FieldActorSpecialAnimationSet {
    u8 actor_model_id;
    u8 reserved01[3];
    struct FieldActorSpecialAnimation animations[8];
};

struct FieldSpriteAnimationView {
    u32 flags;
    u8 reserved04[0x14];
    void *frame_table;
    void *animation_table;
    void *graphics;
    void *update_callback;
};

enum FieldMapCollisionFlags {
    FIELD_MAP_TERRAIN_TYPE_MASK = 0x1F,
    FIELD_MAP_CONDITIONAL_COLLISION = 0x20,
    FIELD_PACKED_MAP_COLLISION_BYPASS = 0x40,
    FIELD_MAP_NON_PLAYER_COLLISION = 0x40,
    FIELD_MAP_COLLISION = 0x80,
    FIELD_MAP_ALL_COLLISION = 0xC0,
    FIELD_MAP_ENABLE_CONDITIONAL_COLLISION = 4
};

enum FieldCollisionMapId {
    FIELD_MAP_PACKED_CELLS = 0,
    FIELD_MAP_ACTOR_MODEL_TERRAIN = 0x40
};

enum FieldSpriteFlags {
    FIELD_SPRITE_ANIMATION_FINISHED = 4,
    FIELD_SPRITE_HIDDEN = 0x20000
};

struct FieldSpriteBlinkStateView {
    u32 flags;
    u8 reserved04[0x24];
    u32 blink_tick;
};

#define FIELD_ACTOR_OFFSET(field) ((s32)&((struct FieldActor *)0)->field)
#define FIELD_ACTOR_FIELD(actor, type, field) \
    M2C_FIELD(actor, type, FIELD_ACTOR_OFFSET(field))
#define FIELD_ACTOR_SPRITE_OFFSET(field) \
    ((s32)&((struct FieldActorSpriteOffsetView *)0)->field)
#define FIELD_ACTOR_SPRITE_FIELD(sprite, type, field) \
    M2C_FIELD(sprite, type, FIELD_ACTOR_SPRITE_OFFSET(field))
#define FIELD_SPRITE_ANIMATION_FIELD(sprite, type, field) \
    M2C_FIELD(sprite, type, (s32)&((struct FieldSpriteAnimationView *)0)->field)
#define FIELD_ACTOR_SPECIAL_ANIMATION_FIELD(animation, type, field) \
    M2C_FIELD(animation, type, (s32)&((struct FieldActorSpecialAnimation *)0)->field)


struct FieldTransportSpriteOffsetView {
    u8 reserved00[0xA];
    s16 offset_y;
};

struct FieldTransportActorView {
    u32 flags;
    u8 model_id;
    u8 actor_id;
    u16 behavior;
    s32 world_x_fixed8;
    s32 world_y_fixed8;
    s32 velocity_x_fixed8;
    s32 velocity_y_fixed8;
    u8 reserved18[2];
    u8 requested_direction;
    u8 reserved1B[3];
    u8 map_cell_value;
    u8 reserved1F;
    struct FieldTransportSpriteOffsetView *sprite;
};

struct FieldTransportInteractionStateView {
    u16 map_id;
    u8 actor_model_id;
    u8 active;
    u8 reserved04[0x10];
    s32 parked_world_x_fixed8;
    s32 parked_world_y_fixed8;
    u8 reserved1C;
    u8 parked_direction;
};

enum FieldTransportInteraction {
    FIELD_TRANSPORT_GUSTAV_ACTOR = 0x69,
    FIELD_TRANSPORT_HOVER_CARGO_ACTOR = 0x6A,
    FIELD_TRANSPORT_DRAGOON_NEST_ACTOR = 0x6B,
    FIELD_TRANSPORT_LANDING_ACTOR = 0x6C,
    FIELD_TRANSPORT_BOARDING_ACTOR_ID = 13,
    FIELD_TRANSPORT_PARKED_SPRITE_OFFSET_Y = -16
};

#endif
