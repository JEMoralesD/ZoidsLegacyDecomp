#ifndef WINDOW_H
#define WINDOW_H

enum WindowFlags {
    WINDOW_FLAG_OPEN = 1,
    WINDOW_FLAG_TILEMAP_DIRTY = 2,
    WINDOW_FLAG_FRAME_DECORATION = 8,
    WINDOW_FLAG_TYPEWRITER_TEXT = 0x10,
    WINDOW_FLAG_WRAP_TEXT = 0x40,
    WINDOW_FLAG_TEXT_LIST = 0x80,
    WINDOW_FLAG_YES_NO_PROMPT = 0x100
};

enum WindowTextControl {
    WINDOW_TEXT_END = 0,
    WINDOW_TEXT_SET_COLOR = 1,
    WINDOW_TEXT_SET_POSITION = 2,
    WINDOW_TEXT_PLAYER_NAME = 3,
    WINDOW_TEXT_NEWLINE = 10
};

enum NumberTextFormat {
    NUMBER_TEXT_NO_PADDING = 0,
    NUMBER_TEXT_TRAILING_SPACES = 1,
    NUMBER_TEXT_LEADING_SPACES = 2,
    NUMBER_TEXT_LEADING_ZEROES = 3,
    NUMBER_TEXT_PADDING_MASK = 3,
    NUMBER_TEXT_SIGN_PREFIX = 4,
    NUMBER_TEXT_ASCII = 8
};

enum WindowTextListLimits {
    WINDOW_COUNT = 10,
    WINDOW_RECORD_BYTES = 0x4D0,
    WINDOW_TEXT_BLOCK_COUNT = 210,
    WINDOW_TEXT_BLOCK_BYTES = 37,
    WINDOW_TEXT_BANK_BYTES = WINDOW_TEXT_BLOCK_COUNT * WINDOW_TEXT_BLOCK_BYTES
};

enum FontTileLayout {
    FONT_RANGE_COUNT = 22,
    FONT_SINGLE_BYTE_SOURCE_BYTES = 16,
    FONT_SHIFT_JIS_SOURCE_BYTES = 32,
    FONT_TILE_BYTES = 32,
    FONT_COLOR_VARIANTS_PER_PALETTE = 3,
    FONT_SINGLE_BYTE_SPACE = 0x20,
    FONT_SHIFT_JIS_SPACE = 0x8140,
    FONT_SHIFT_JIS_FALLBACK = 0x81A0
};

enum ShiftJisLatinBytes {
    SHIFT_JIS_LATIN_HIGH_BYTE = 0x82,
    SHIFT_JIS_UPPERCASE_A_LOW_BYTE = 0x60,
    SHIFT_JIS_UPPERCASE_E_LOW_BYTE = 0x64,
    SHIFT_JIS_UPPERCASE_I_LOW_BYTE = 0x68,
    SHIFT_JIS_UPPERCASE_O_LOW_BYTE = 0x6E,
    SHIFT_JIS_UPPERCASE_U_LOW_BYTE = 0x74,
    SHIFT_JIS_LOWERCASE_A_LOW_BYTE = 0x81,
    SHIFT_JIS_LOWERCASE_E_LOW_BYTE = 0x85,
    SHIFT_JIS_LOWERCASE_I_LOW_BYTE = 0x89,
    SHIFT_JIS_LOWERCASE_N_LOW_BYTE = 0x8E,
    SHIFT_JIS_LOWERCASE_O_LOW_BYTE = 0x8F,
    SHIFT_JIS_LOWERCASE_U_LOW_BYTE = 0x95
};

struct ShiftJisFontRange {
    u16 first_code;
    u8 glyph_count;
    u8 data03;
    u8 *glyphs;
};

struct SingleByteFontRange {
    u8 first_code;
    u8 glyph_count;
    u16 data02;
    u8 *glyphs;
};

struct Window {
    u32 flags;
    s16 x;
    s16 y;
    u16 width;
    u16 height;
    s16 text_column;
    s16 text_row;
    u16 page_progress_rows;
    u8 text_color;
    u8 slot;
    u8 top_item;
    u8 prior_top_item;
    u8 selected_item;
    u8 prior_selected_item;
    u8 frame_style;
    u8 frame_decoration_x;
    u8 frame_decoration_width;
    u8 key_repeat_frames;
    u16 prior_held_keys;
    u16 tiles[1];
};

struct WindowPoolRecord {
    struct Window window;
    u8 tile_storage[WINDOW_RECORD_BYTES - 0x20];
};

/* Explicit access types preserve signed loads and register allocation. */
#define WINDOW_FIELD(window, type, field) \
    M2C_FIELD(window, type *, (s32)&((struct Window *)0)->field)

#endif
