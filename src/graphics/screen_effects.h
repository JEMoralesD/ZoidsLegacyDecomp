#ifndef SCREEN_EFFECTS_H
#define SCREEN_EFFECTS_H

enum ScanlineWindowMode {
    SCANLINE_WINDOW_DISABLED = 0,
    SCANLINE_WINDOW_FIXED = 1,
    SCANLINE_WINDOW_SCALED = 2,
    SCANLINE_WINDOW_POLYGONS = 3,
    SCANLINE_WINDOW_ROTATING_SPLIT = 4,
    SCANLINE_WINDOW_CHEVRON = 5,
    SCANLINE_WINDOW_CHECKERBOARD = 6,
    SCANLINE_WINDOW_MODE_MASK = 7
};

enum ScanlineWindowFlags {
    SCANLINE_WINDOW_HBLANK_CALLBACK = 8
};

enum ScanlineWindowLimits {
    SCANLINE_WINDOW_COUNT = 160,
    SCANLINE_WINDOW_BANK_COUNT = 2,
    SCANLINE_WINDOW_RECORD_BYTES = 8,
    SCANLINE_WINDOW_BANK_BYTES = 0x500,
    SCREEN_WIDTH = 240,
    SCREEN_HEIGHT = 160
};

enum ChevronWindowDirection {
    CHEVRON_WINDOW_LEFTWARD = 0,
    CHEVRON_WINDOW_RIGHTWARD = 1
};

enum ScreenTransitionKind {
    SCREEN_TRANSITION_NONE = 0,
    SCREEN_TRANSITION_FADE_FROM_BLACK = 1,
    SCREEN_TRANSITION_FADE_TO_BLACK = 2,
    SCREEN_TRANSITION_FADE_FROM_WHITE = 3,
    SCREEN_TRANSITION_FADE_TO_WHITE = 4,
    SCREEN_TRANSITION_ROTATING_SPLIT_REVEAL = 5,
    SCREEN_TRANSITION_ROTATING_SPLIT_CONCEAL = 6,
    SCREEN_TRANSITION_SCALED_Z_REVEAL = 7,
    SCREEN_TRANSITION_SCALED_Z_CONCEAL = 8,
    SCREEN_TRANSITION_LEFTWARD_CHEVRON_REVEAL = 9,
    SCREEN_TRANSITION_RIGHTWARD_CHEVRON_REVEAL = 10,
    SCREEN_TRANSITION_LEFTWARD_CHEVRON_CONCEAL = 11,
    SCREEN_TRANSITION_RIGHTWARD_CHEVRON_CONCEAL = 12,
    SCREEN_TRANSITION_ROTATING_SQUARE_REVEAL = 13,
    SCREEN_TRANSITION_ROTATING_SQUARE_CONCEAL = 14,
    SCREEN_TRANSITION_CHECKERBOARD_REVEAL = 15,
    SCREEN_TRANSITION_CHECKERBOARD_CONCEAL = 16,
    SCREEN_TRANSITION_DITHER_REVEAL = 17,
    SCREEN_TRANSITION_DITHER_CONCEAL = 18
};

enum ScreenTransitionFlags {
    SCREEN_TRANSITION_KIND_MASK = 0x3F,
    SCREEN_TRANSITION_SOFTWARE_PALETTE = 0x40,
    SCREEN_TRANSITION_MOSAIC = 0x80
};

enum ScreenTransitionHBlankAction {
    SCREEN_TRANSITION_HBLANK_NONE = 0,
    SCREEN_TRANSITION_HBLANK_INSTALL = 1,
    SCREEN_TRANSITION_HBLANK_REMOVE = 2
};

enum ScreenDitherAction {
    SCREEN_DITHER_FORCE_BLACK = 0,
    SCREEN_DITHER_RESTORE_REGISTERS = 1
};

enum SceneBg0ScrollState {
    SCENE_BG0_SCROLL_DISABLED = 0,
    SCENE_BG0_SCROLL_STARTING = 1,
    SCENE_BG0_SCROLL_ACTIVE = 2,
    SCENE_BG0_SCROLL_STOPPING = 3
};

enum SceneBg0ScrollResources {
    SCENE_BG0_SCROLL_CALLBACK_SLOT = 2,
    SCENE_BG0_SCROLL_CALLBACK_RAM = 0x030060FC,
    SCENE_BG0_SCROLL_BUFFER_BYTES = 0x140
};

enum SceneScanlineEffectState {
    SCENE_SCANLINE_DISABLED = 0,
    SCENE_SCANLINE_STARTING = 1,
    SCENE_SCANLINE_ACTIVE = 2,
    SCENE_SCANLINE_STOPPING = 3
};

enum SceneBg1DistortionPhase {
    SCENE_BG1_DISTORTION_VERTICAL_WAVE = 0,
    SCENE_BG1_DISTORTION_SPLIT_SCANLINES = 1,
    SCENE_BG1_DISTORTION_SPLIT_COMPLETE = 2
};

enum SceneScanlineEffectResources {
    SCENE_MOSAIC_CALLBACK_SLOT = 1,
    SCENE_MOSAIC_CALLBACK_CODE_ROM = 0x080008E4,
    SCENE_MOSAIC_STATE_RAM = 0x02032B9C,
    SCENE_MOSAIC_PHASE_RAM = 0x02032B9D,
    SCENE_MOSAIC_BANK_RAM = 0x02032B9E,
    SCENE_MOSAIC_BUFFERS_RAM = 0x02032BA0,
    SCENE_MOSAIC_BANK_BYTES = 0x140,
    SCENE_BG1_SCROLL_CALLBACK_SLOT = 2,
    SCENE_BG1_SCROLL_CALLBACK_CODE_ROM = 0x08000894,
    SCENE_BG1_SCROLL_STATE_RAM = 0x02031C98,
    SCENE_BG1_SCROLL_BANK_RAM = 0x02031C99,
    SCENE_BG1_SCROLL_BUFFERS_RAM = 0x02031C9A,
    SCENE_BG1_SCROLL_Y_FIXED8_RAM = 0x0203219C,
    SCENE_BG1_DISTORTION_PHASE_RAM = 0x020321A0,
    SCENE_BG1_SCROLL_BANK_BYTES = 0x280,
    SCENE_BG1_SCROLL_PALETTE_FRAMES_ROM = 0x08103E18,
    SCENE_SCANLINE_CALLBACK_RAM = 0x030060FC
};

struct SceneBg1ScanlineScroll {
    s16 x;
    s16 y;
};

#define SCENE_BG1_SCROLL_OFFSET(field) \
    ((s32)&((struct SceneBg1ScanlineScroll *)0)->field)

struct SceneScanlineEventView {
    u8 scanline;
    u8 flags;
    s16 reserved02;
    s32 callback;
    s32 next;
    s32 previous;
};

struct SceneTriangleWindow {
    s16 vertex_count;
    s16 center_x;
    s16 center_y;
    s16 first_x;
    s16 first_y;
    s16 second_x;
    s16 second_y;
} __attribute__((packed));

enum SceneTriangleStreamIndex {
    SCENE_TRIANGLE_VERTEX_COUNT = ((s32)&((struct SceneTriangleWindow *)0)->vertex_count) / sizeof(s16),
    SCENE_TRIANGLE_CENTER_X = ((s32)&((struct SceneTriangleWindow *)0)->center_x) / sizeof(s16),
    SCENE_TRIANGLE_CENTER_Y = ((s32)&((struct SceneTriangleWindow *)0)->center_y) / sizeof(s16),
    SCENE_TRIANGLE_FIRST_X = ((s32)&((struct SceneTriangleWindow *)0)->first_x) / sizeof(s16),
    SCENE_TRIANGLE_FIRST_Y = ((s32)&((struct SceneTriangleWindow *)0)->first_y) / sizeof(s16),
    SCENE_TRIANGLE_SECOND_X = ((s32)&((struct SceneTriangleWindow *)0)->second_x) / sizeof(s16),
    SCENE_TRIANGLE_SECOND_Y = ((s32)&((struct SceneTriangleWindow *)0)->second_y) / sizeof(s16)
};

enum ScreenBrightnessConstants {
    SCREEN_BRIGHTEN_ALL_LAYERS = 0xBF,
    SCREEN_MAX_BRIGHTNESS = 16
};

struct WindowHorizontalBounds {
    s16 left;
    s16 right;
};

struct ScanlineWindowRecord {
    u16 window0_horizontal;
    u16 window1_horizontal;
    u16 window0_vertical;
    u16 window1_vertical;
};

struct ScanlineWindowState {
    u8 flags;
    u8 data01;
    u16 horizontal_scales[2];
    u16 vertical_scales[2];
    u8 rotation_angle;
    u8 data0B;
    u16 chevron_progress;
    u8 checkerboard_progress;
    u8 previous_flags;
    u16 inside_layers;
    u16 outside_layers;
    u16 window0_horizontal;
    u16 window0_vertical;
    u16 window1_horizontal;
    u16 window1_vertical;
    struct WindowHorizontalBounds *scaled_sources[2];
    s16 *polygon_stream;
    u8 chevron_direction;
    u8 chevron_invert_bounds;
    u8 displayed_bank;
    u8 data2B;
};

struct WindowPolygon {
    u16 count;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    s16 x3;
    s16 y3;
    s16 terminator;
};

struct ScreenTransitionState {
    u8 flags;
    u8 duration_updates;
    u8 progress;
    u8 data03;
    s16 focus_x;
    s16 focus_y;
    u16 mosaic;
    u8 displayed_bank;
    u8 hblank_action;
    u8 hblank_callback[0xA4];
};

struct ScreenTransitionPaletteChannel {
    u8 increment;
    u8 decrease;
    u8 fraction;
    u8 value;
};

struct ScreenTransitionPaletteColor {
    struct ScreenTransitionPaletteChannel red;
    struct ScreenTransitionPaletteChannel green;
    struct ScreenTransitionPaletteChannel blue;
};

#define SCANLINE_WINDOW_ADDRESS(field) \
    ((u32)&((struct ScanlineWindowState *)0x03005EE8)->field)

#define SCREEN_TRANSITION_ADDRESS(field) \
    ((u32)&((struct ScreenTransitionState *)0x03005F70)->field)

#define TRANSITION_COLOR_FIELD(state, type, field) \
    M2C_FIELD(state, type *, (s32)&((struct ScreenTransitionPaletteColor *)0)->field)

#endif
