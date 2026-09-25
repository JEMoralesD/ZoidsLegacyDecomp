#ifndef ZOIDS_TEXT_CORE_H
#define ZOIDS_TEXT_CORE_H

#include <stddef.h>
#include <stdint.h>

enum {
    TEXT_FACE_AUTO,
    TEXT_FACE_TALL,
    TEXT_FACE_COMPACT,
};

typedef enum {
    TEXT_OK,
    TEXT_END,
    TEXT_MALFORMED_INPUT,
    TEXT_MISSING_FIELD,
    TEXT_CAPACITY_FAILURE,
    TEXT_LAYOUT_OVERFLOW,
    TEXT_TILE_EXHAUSTED,
} TextStatus;

enum {
    TEXT_TOKEN_GLYPH,
    TEXT_TOKEN_COLOR,
    TEXT_TOKEN_POSITION,
    TEXT_TOKEN_NEWLINE,
    TEXT_TOKEN_END,
    TEXT_TOKEN_ALIGN,
    TEXT_TOKEN_INDENT,
    TEXT_TOKEN_ANCHOR,
    TEXT_TOKEN_REGION,
};

enum {
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT,
};

enum {
    TEXT_BOUND_ADVANCE,
    TEXT_BOUND_INK,
};

enum {
    TEXT_WRAP_NONE,
    TEXT_WRAP_WORD,
};

enum {
    TEXT_OVERFLOW_ERROR,
    TEXT_OVERFLOW_CLIP,
};

typedef enum {
    TEXT_PROFILE_PORTRAIT_DIALOGUE,
    TEXT_PROFILE_FULL_DIALOGUE,
    TEXT_PROFILE_LABEL,
    TEXT_PROFILE_NUMBER,
    TEXT_PROFILE_COMPACT_ROW,
    TEXT_PROFILE_STARTUP,
    TEXT_PROFILE_CREDITS,
    TEXT_PROFILE_BATTLE_CHOICE,
    TEXT_PROFILE_SPEAKER,
    TEXT_PROFILE_COUNT,
} TextProfileKind;

enum {
    TEXT_EVENT_GLYPH,
    TEXT_EVENT_COLOR,
    TEXT_EVENT_POSITION,
    TEXT_EVENT_LINE,
    TEXT_EVENT_PAGE,
    TEXT_EVENT_LINE_BEGIN,
    TEXT_EVENT_ALIGN,
    TEXT_EVENT_REGION,
};

typedef struct {
    const uint8_t *data;
    size_t length;
    uint8_t field_id;
} TextSpan;

typedef struct {
    uint16_t code;
    int8_t bearing_x;
    int8_t bearing_y;
    uint8_t width;
    uint8_t height;
    int8_t advance;
    uint8_t face;
    uint8_t line_height;
    uint8_t baseline;
} TextMetric;

typedef struct {
    uint16_t first;
    uint16_t count;
    uint16_t base;
    uint8_t face;
    uint8_t pad;
} TextRange;

typedef struct {
    uint16_t left;
    uint16_t right;
    int8_t adjust;
    uint8_t pad;
} TextPair;

typedef struct {
    const TextMetric *metrics;
    uint16_t metric_count;
    const TextRange *ranges;
    uint16_t range_count;
    const TextPair *pairs;
    uint16_t pair_count;
    const uint16_t *pair_offsets;
    const uint8_t *glyphs;
    uint8_t glyph_stride;
    uint16_t tall_fallback;
    uint16_t compact_fallback;
} TextFont;

typedef struct {
    const uint8_t *data[2];
    uint16_t lengths[2];
    uint16_t positions[2];
    uint8_t depth;
    uint8_t ended;
    uint8_t invalid;
    uint8_t limit_end;
} TextDecoder;

typedef union {
    struct {
        uint8_t kind;
        uint16_t code;
        uint8_t a;
        uint8_t b;
        uint16_t consumed;
    };
    struct {
        uint16_t saved_positions[2];
        uint8_t saved_depth;
        uint8_t saved_ended;
    };
} TextToken;

typedef struct {
    uint8_t *data;
    size_t capacity;
    size_t length;
    TextStatus status;
} TextWriter;

typedef enum {
    TEXT_TEMPLATE_LITERAL,
    TEXT_TEMPLATE_FIELD,
    TEXT_TEMPLATE_COLOR,
    TEXT_TEMPLATE_POSITION,
    TEXT_TEMPLATE_NEWLINE,
    TEXT_TEMPLATE_ALIGN,
    TEXT_TEMPLATE_PLAYER_REF,
    TEXT_TEMPLATE_INDENT,
    TEXT_TEMPLATE_ANCHOR,
    TEXT_TEMPLATE_REGION,
} TextTemplatePartKind;

typedef enum {
    TEXT_FIELD_PLAYER,
    TEXT_FIELD_ITEM,
    TEXT_FIELD_NUMBER,
} TextFieldKind;

typedef struct {
    TextTemplatePartKind kind;
    uint8_t field_kind;
    uint8_t field_id;
    uint8_t a;
    uint8_t b;
    TextSpan literal;
} TextTemplatePart;

typedef struct {
    TextFieldKind kind;
    uint8_t id;
    uint8_t digits;
    uint8_t mode;
    uint8_t face;
    TextSpan text;
    int32_t number;
} TextTemplateValue;

typedef struct {
    int16_t left;
    int16_t top;
    int16_t right;
    int16_t bottom;
    int8_t baseline;
    uint8_t line_height;
    uint8_t line_gap;
    uint8_t face;
    uint8_t position_scale;
    uint8_t alignment;
    uint8_t alignment_bound;
    uint8_t wrap;
    uint8_t overflow;
    uint8_t tabular_advance;
    uint8_t paginate;
    uint8_t compact_tabular_advance;
} TextProfile;

TextStatus text_profile_init(TextProfile *profile, TextProfileKind kind,
                             int16_t right, int16_t bottom);

typedef struct {
    int16_t x;
    int16_t y;
    int16_t previous;
    uint8_t color;
    uint8_t initialized;
} TextLayoutState;

typedef struct {
    uint16_t metric;
    uint16_t code;
    int16_t pen_x;
    int16_t ink_x;
    int16_t ink_y;
    uint8_t width;
    uint8_t height;
    uint8_t color;
    uint8_t missing;
    uint16_t consumed;
    uint8_t clip_left;
    uint8_t clip_right;
} TextPlacement;

typedef struct {
    uint8_t kind;
    union {
        TextPlacement glyph;
        struct {
            union {
                struct { int16_t x, y; };
                struct { int16_t advance_left, advance_right; };
            };
            uint8_t value;
            uint8_t explicit_break;
            int16_t left;
            int16_t top;
            int16_t right;
            int16_t bottom;
            uint16_t consumed;
            uint16_t next_consumed;
        };
    };
} TextEvent;

typedef TextStatus (*TextEmit)(void *context, const TextEvent *event);

typedef struct {
    uint16_t consumed;
    uint16_t glyph_count;
    uint16_t missing_count;
    int16_t ink_left;
    int16_t ink_top;
    int16_t ink_right;
    int16_t ink_bottom;
    uint8_t status;
    uint8_t overflowed;
} TextLayoutResult;

typedef struct {
    uint16_t positions[2];
    uint8_t depth;
    uint8_t ended;
} TextLayoutMark;

typedef struct {
    TextLayoutMark render_end;
    TextLayoutMark after;
    int16_t shift;
    uint16_t line_height;
    uint16_t next_consumed;
    int16_t origin_y;
    uint8_t break_kind;
    uint8_t phase;
    uint8_t pending;
    uint8_t overflowed;
    uint8_t line_color;
    uint8_t line_baseline;
    uint8_t alignment;
    uint8_t row_height;
    int16_t left;
    int16_t right;
    uint8_t end_color;
    uint8_t simple_count;
    uint8_t simple_next;
} TextLayoutCursor;

typedef uint32_t *(*TextGetTileWords)(void *context, int x, int y);

typedef struct {
    void *context;
    TextGetTileWords get;
} TextTileSurface;

void text_decoder_init_raw(TextDecoder *decoder,
                           const uint8_t *input, size_t input_length,
                           const uint8_t *field, size_t field_length,
                           uint8_t limit_end);
TextStatus text_decoder_next(TextDecoder *decoder, TextToken *token);
size_t text_decoder_consumed(const TextDecoder *decoder);

void text_writer_init(TextWriter *writer, uint8_t *data, size_t capacity);
void text_writer_init_count(TextWriter *writer, size_t capacity);
TextStatus text_writer_open(TextWriter *writer, uint8_t *data, size_t capacity);
TextStatus text_writer_byte(TextWriter *writer, uint8_t value);
TextStatus text_writer_code(TextWriter *writer, uint16_t code);
TextStatus text_writer_control(TextWriter *writer, uint8_t control,
                               uint8_t a, uint8_t b);
TextStatus text_writer_span(TextWriter *writer, TextSpan span);
TextStatus text_writer_number(TextWriter *writer, int32_t value,
                              uint8_t digits, uint8_t mode, uint8_t face);
TextStatus text_template_write(TextWriter *writer,
                               const TextTemplatePart *parts,
                               size_t part_count,
                               const TextTemplateValue *values,
                               size_t value_count);

int text_font_find(const TextFont *font, uint8_t face, uint16_t code);
int text_font_digit_advance(const TextFont *font, uint8_t face);

TextStatus text_layout_begin(const TextProfile *profile, TextDecoder *decoder,
                             TextLayoutState *state, TextLayoutCursor *cursor,
                             TextLayoutResult *result);
int text_layout_scanning(const TextLayoutCursor *cursor);
int text_blank_run(const TextFont *font, const TextProfile *profile,
                   TextLayoutState *state, uint16_t code, int count);
int text_blank_glyph(const TextFont *font, const TextProfile *profile,
                     uint16_t code);
int text_layout_simple_peek(const TextFont *font, const TextProfile *profile,
                            TextDecoder *decoder, TextLayoutState *state,
                            TextLayoutCursor *cursor, TextLayoutResult *result,
                            TextEvent *event);
int text_layout_simple_emit(const TextFont *font, const TextProfile *profile,
                            TextDecoder *decoder, TextLayoutState *state,
                            TextLayoutCursor *cursor, TextEvent *event);
int text_layout_blank_peek(const TextFont *font, const TextProfile *profile,
                           TextDecoder *decoder, TextLayoutState *state,
                           TextLayoutCursor *cursor, TextLayoutResult *result,
                           TextEvent *event, uint16_t code, uint8_t unit, int count);
void text_layout_blank_commit(const TextFont *font, const TextProfile *profile,
                              TextDecoder *decoder, TextLayoutState *state,
                              TextLayoutCursor *cursor, TextLayoutResult *result,
                              uint16_t code, uint8_t unit, int count);
int text_layout_skip_line(TextDecoder *decoder, TextLayoutState *state,
                          TextLayoutCursor *cursor);
TextStatus text_layout_scan_peek(const TextFont *font,
                                 const TextProfile *profile,
                                 TextDecoder *decoder,
                                 TextLayoutState *state,
                                 TextLayoutCursor *cursor,
                                 TextLayoutResult *result,
                                 TextEvent *event);
TextStatus text_layout_emit_peek(const TextFont *font,
                                 const TextProfile *profile,
                                 TextDecoder *decoder,
                                 TextLayoutState *state,
                                 TextLayoutCursor *cursor,
                                 TextLayoutResult *result,
                                 TextEvent *event);
TextStatus text_layout_commit(const TextFont *font, const TextProfile *profile,
                              TextDecoder *decoder, TextLayoutState *state,
                              TextLayoutCursor *cursor,
                              TextLayoutResult *result,
                              const TextEvent *event);

TextStatus text_compose_glyph_tiles(const TextFont *font,
                                    const TextPlacement *placement,
                                    TextTileSurface *surface,
                                    uint8_t variant);
TextStatus text_compose_glyph_tiles_blended(const TextFont *font,
                                            const TextPlacement *placement,
                                            TextTileSurface *surface,
                                            uint8_t variant);
TextStatus text_compose_glyph_tiles_uniform(const TextFont *font,
                                            const TextPlacement *placement,
                                            TextTileSurface *surface,
                                            uint8_t variant);

#endif
