#ifndef TITLE_MENU_H
#define TITLE_MENU_H

enum TitleMenuGraphicsMode {
    TITLE_MENU_GRAPHICS_INTRO = 0,
    TITLE_MENU_GRAPHICS_SELECTION = 1
};

enum TitleMenuParticlePhase {
    TITLE_PARTICLE_DESCENDING = 0,
    TITLE_PARTICLE_DRIFTING = 1
};

enum TitleMenuParticleResources {
    TITLE_PARTICLE_COUNT = 16,
    TITLE_PARTICLE_FRAMES_ROM = 0x081039D0,
    TITLE_PARTICLE_ANIMATIONS_ROM = 0x08103A34,
    TITLE_PARTICLE_TILE_OFFSET = 0x3B1,
    TITLE_PARTICLE_PALETTE_BANK = 11,
    TITLE_PARTICLE_UPDATE_CALLBACK = 0x0809B971,
    TITLE_PARTICLE_INITIAL_INTERVAL = 96,
    TITLE_PARTICLE_MINIMUM_INTERVAL = 16
};

struct TitleParticlePositionView {
    u32 flags;
    u8 reserved04[0x24];
    s32 x_fixed8;
    s32 y_fixed8;
    s32 phase;
};

struct TitleParticleMotionView {
    u32 flags;
    u8 reserved04[0x24];
    s32 x_fixed8;
    s32 y_fixed8;
    s32 phase;
    s32 drift_x_fixed8;
};

#define TITLE_PARTICLE_OFFSET(field) \
    ((s32)&((struct TitleParticleMotionView *)0)->field)

#define TITLE_PARTICLE_FIELD(sprite, type, field) \
    M2C_FIELD(sprite, type *, TITLE_PARTICLE_OFFSET(field))

#endif
