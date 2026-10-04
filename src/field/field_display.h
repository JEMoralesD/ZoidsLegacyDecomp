#ifndef FIELD_DISPLAY_H
#define FIELD_DISPLAY_H

enum FieldDisplayLimits {
    FIELD_DECORATION_SLOT_COUNT = 42,
    FIELD_ALTERNATE_DECORATION_COUNT = 3,
    FIELD_TILE_COORDINATE_MASK = 0x1FF,
    FIELD_SCREEN_TILE_MASK = 0x1F,
    FIELD_TILE_PIXELS = 8,
    FIELD_PALETTE_ANIMATION_INTERVAL = 15,
    FIELD_MAP_NAME_VISIBLE_UPDATES = 240
};

enum FieldBg3ScanlineState {
    FIELD_BG3_SCANLINE_READY = 1,
    FIELD_BG3_SCANLINE_ACTIVE = 2,
    FIELD_BG3_SCANLINE_STOPPING = 3
};

struct FieldBackgroundScrollOffsets {
    s32 bg0_x_fixed8;
    s32 bg0_y_fixed8;
    s32 bg1_x_fixed8;
    s32 bg1_y_fixed8;
    s32 bg2_x_fixed8;
    s32 bg2_y_fixed8;
    s32 bg3_x_fixed8;
    s32 bg3_y_fixed8;
};

struct FieldMapChangePositionView {
    u16 map_id;
    u16 reserved02;
    u32 center_x_fixed8;
    u32 center_y_fixed8;
};

struct FieldDecorationResourcePointerView {
    void *data;
    u32 reserved04;
};

struct FieldStreamedTileOrigin {
    u16 x;
    u16 y;
};

struct FieldStreamedTileYView {
    u16 y;
    u16 reserved02;
};

struct FieldEncounterActorView {
    u32 actor_flags;
    u8 model_id;
    u8 reserved05[3];
    s32 world_x_fixed8;
    s32 world_y_fixed8;
    s32 velocity_x_fixed8;
    s32 velocity_y_fixed8;
    u8 reserved18[6];
    u8 map_cell_value;
};

struct FieldEncounterMemberView {
    u8 reserved00[4];
    u8 zoid_id;
};

struct FieldMapNameGroupView {
    u8 reserved00[0x1D];
    u8 map_group_id;
    u8 reserved1E[2];
};

struct FieldBg3ScanlineEventView {
    s8 scanline;
    u8 flags;
    s16 reserved02;
    s32 callback;
    s32 next;
    s32 previous;
};

enum FieldDisplayAddresses {
    FIELD_CURRENT_MAP_STATE_RAM = 0x0202ECF4,
    FIELD_BG3_SCANLINE_STATE_RAM = 0x020324B8,
    FIELD_BG3_SCANLINE_HIDE_REQUEST_RAM = 0x020324B9,
    FIELD_BG3_FORCE_HIDE_RAM = 0x020324BA,
    FIELD_MESSAGE_WINDOW_STATE_RAM = 0x02030666,
    FIELD_REQUESTED_MAP_ID_RAM = 0x020324B4,
    FIELD_METATILE_MAPS_RAM = 0x02032E88,
    FIELD_METATILE_TILES_RAM = 0x02032E90,
    FIELD_METATILE_TILES_BUFFER_RAM = 0x02036E98,
    FIELD_BLEND_CONTROL_SHADOW_RAM = 0x0300004E,
    FIELD_DISPLAY_UPDATE_FLAGS_RAM = 0x03000074
};


struct FieldTravelRestrictionView {
    u8 reserved00[0x21];
    u8 travel_restrictions;
};

enum FieldTravelRestrictions {
    FIELD_TRAVEL_RESTRICTIONS_BYTE = 0x21,
    FIELD_TRAVEL_RESTRICTED = 1,
    FIELD_TRAVEL_ADDITIONALLY_RESTRICTED = 2,
    FIELD_CLEAR_TRAVEL_RESTRICTION_MASK = 0xFE,
    FIELD_CLEAR_ADDITIONAL_TRAVEL_RESTRICTION_MASK = 0xFD
};

enum WorldMapBackgroundMode {
    WORLD_MAP_BG1_DRIFT = 1,
    WORLD_MAP_BG1_SLIDE_IN = 2,
    WORLD_MAP_BG3_FOLLOWS_FIELD = 4,
    WORLD_MAP_BG1_SLIDE_GRAPHICS_ROM = 0x08420F28,
    WORLD_MAP_BG1_SLIDE_TILEMAP_ROM = 0x0842115C
};

#endif
