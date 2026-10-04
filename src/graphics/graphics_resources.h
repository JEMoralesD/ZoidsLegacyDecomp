#ifndef GRAPHICS_RESOURCES_H
#define GRAPHICS_RESOURCES_H

enum GraphicsResourceLimits {
    GRAPHICS_4BPP_TILE_BYTES = 32,
    GRAPHICS_PALETTE_BANK_BYTES = 32,
    GRAPHICS_BG_TILEMAP_WIDTH = 32,
    GRAPHICS_BG_TILEMAP_FLIP_X = 0x400,
    GRAPHICS_ZOID_ICON_BYTES = 0x800,
    GRAPHICS_PILOT_PORTRAIT_BYTES = 0x480,
    GRAPHICS_PILOT_WITH_ALTERNATE_PORTRAIT_PALETTES = 75
};

struct CompressedSpriteGraphics {
    void *tiles;
    void *palette;
};

struct SpriteAnimationCommand {
    s16 frame_id;
    s16 duration;
};

#define SPRITE_ANIMATION_FIELD(command, type, field) \
    M2C_FIELD(command, type *, (s32)&((struct SpriteAnimationCommand *)0)->field)

struct ZoidBodyGraphics {
    void *bg_tiles;
    void *bg_tilemap;
    void *obj_tiles;
};

struct BattleTerrainGraphics {
    void *terrain_tiles;
    void *terrain_palette;
    void *sky_tiles;
    void *sky_palette;
};

#endif
