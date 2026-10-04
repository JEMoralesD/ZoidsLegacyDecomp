#include "text_core.h"

#include <limits.h>

#if defined(__GNUC__) && !defined(__clang__)
#define TEXT_O2 __attribute__((optimize("O2,no-tree-loop-distribute-patterns")))
#define TEXT_NOIPA __attribute__((noinline,noipa))
#else
#define TEXT_O2
#define TEXT_NOIPA __attribute__((noinline))
#endif

TextStatus text_profile_init(TextProfile *profile, TextProfileKind kind,
                             int16_t right, int16_t bottom) {
    static const TextProfile profiles[TEXT_PROFILE_COUNT] = {
        {2, 0, 160, 64, 14, 16, 0, TEXT_FACE_AUTO, 8,
         TEXT_ALIGN_LEFT, TEXT_BOUND_INK, TEXT_WRAP_WORD,
         TEXT_OVERFLOW_CLIP, 0, 1, 0},
        {2, 0, 224, 64, 14, 16, 0, TEXT_FACE_AUTO, 8,
         TEXT_ALIGN_LEFT, TEXT_BOUND_INK, TEXT_WRAP_WORD,
         TEXT_OVERFLOW_CLIP, 0, 1, 0},
        {0, 0, 224, 16, 7, 8, 0, TEXT_FACE_AUTO, 8,
         TEXT_ALIGN_LEFT, TEXT_BOUND_INK, TEXT_WRAP_NONE,
         TEXT_OVERFLOW_CLIP, 0, 0, 0},
        {0, 0, 224, 16, 7, 8, 0, TEXT_FACE_AUTO, 8,
         TEXT_ALIGN_LEFT, TEXT_BOUND_INK, TEXT_WRAP_NONE,
         TEXT_OVERFLOW_CLIP, 1, 0, 0},
        {0, 0, 224, 8, 7, 8, 0, TEXT_FACE_COMPACT, 8,
         TEXT_ALIGN_LEFT, TEXT_BOUND_INK, TEXT_WRAP_NONE,
         TEXT_OVERFLOW_CLIP, 0, 0, 0},
        {0, 0, 8, 8, 7, 8, 0, TEXT_FACE_COMPACT, 8,
         TEXT_ALIGN_LEFT, TEXT_BOUND_INK, TEXT_WRAP_NONE,
         TEXT_OVERFLOW_ERROR, 0, 0, 0},
        {0, 0, 240, 16, 14, 16, 0, TEXT_FACE_TALL, 8,
         TEXT_ALIGN_LEFT, TEXT_BOUND_INK, TEXT_WRAP_NONE,
         TEXT_OVERFLOW_CLIP, 0, 0, 0},
        {0, 0, 72, 32, 14, 16, 0, TEXT_FACE_TALL, 8,
         TEXT_ALIGN_LEFT, TEXT_BOUND_INK, TEXT_WRAP_NONE,
         TEXT_OVERFLOW_ERROR, 0, 0, 0},
        {2, 0, 160, 16, 14, 16, 0, TEXT_FACE_TALL, 8,
         TEXT_ALIGN_LEFT, TEXT_BOUND_INK, TEXT_WRAP_NONE,
         TEXT_OVERFLOW_CLIP, 0, 0, 0},
    };
    if (!profile || kind >= TEXT_PROFILE_COUNT ||
            right <= 0 || bottom <= 0)
        return TEXT_MALFORMED_INPUT;
    *profile = profiles[kind];
    profile->right = right;
    profile->bottom = bottom;
    return TEXT_OK;
}

typedef uint16_t __attribute__((may_alias)) AliasU16;

typedef uint32_t __attribute__((may_alias)) AliasU32;

static __attribute__((always_inline)) inline void clear_bytes(void *target, size_t length) {
    uint8_t *bytes = target;
    if (!((uintptr_t)bytes & 3)) {
        AliasU32 *words = (AliasU32 *)bytes;
        for (; length >= 4; length -= 4)
            *words++ = 0;
        bytes = (uint8_t *)words;
    }
    if (!((uintptr_t)bytes & 1)) {
        AliasU16 *halves = (AliasU16 *)bytes;
        for (; length >= 2; length -= 2)
            *halves++ = 0;
        bytes = (uint8_t *)halves;
    }
    while (length--)
        *bytes++ = 0;
}

static void copy_bytes(void *target, const void *source, size_t length) {
    uint8_t *output = target;
    const uint8_t *input = source;
    while (length--)
        *output++ = *input++;
}

typedef struct {
    int16_t x;
    int16_t ink_left;
    int16_t ink_right;
    int16_t previous;
    uint8_t ascent;
    uint8_t descent;
    uint8_t color;
    uint8_t has_glyph;
    uint8_t baseline;
    uint8_t line_height;
} LineGeometry;

typedef TextLayoutMark DecoderMark;

typedef union {
    struct {
        DecoderMark before;
        DecoderMark start;
        TextToken token;
    } scan;
    TextEvent event;
} EventWorkspace;

typedef struct {
    EventWorkspace event;
    LineGeometry geometry;
    LineGeometry current;
} ScanWorkspace;

#ifdef TEXT_RUNTIME_SCRATCH
// Layout calls finish before frame waits or window reordering reuse this buffer.
#define SCAN_WORKSPACE ((ScanWorkspace *)(uintptr_t)TEXT_RUNTIME_SCRATCH)
#define EVENT_WORKSPACE ((EventWorkspace *)(uintptr_t)TEXT_RUNTIME_SCRATCH)
typedef char ScanWorkspaceFits[(sizeof(ScanWorkspace) <= 0x190) ? 1 : -1];
#endif

// Bits 0-8 hold the metric or color, bit 10 marks a color, and bits 11-15
// hold the signed pen adjustment before the glyph.
typedef struct {
    uint16_t packed;
    uint16_t end;
} SimpleItem;

#define SIMPLE_COLOR 0x400u

#ifdef TEXT_RUNTIME_SCRATCH
// Single-line glyph plans live after the scan workspace in the same scratch.
#define SIMPLE_ITEMS ((SimpleItem *)(uintptr_t)(TEXT_RUNTIME_SCRATCH + 0x40))
#define SIMPLE_CAPACITY 84
typedef char SimpleItemsFit[(sizeof(ScanWorkspace) <= 0x40 &&
                             0x40 + SIMPLE_CAPACITY * sizeof(SimpleItem) <= 0x190) ? 1 : -1];
#else
static SimpleItem simple_items[128];
#define SIMPLE_ITEMS simple_items
#define SIMPLE_CAPACITY 128
#endif

static void decoder_mark(const TextDecoder *decoder, DecoderMark *mark) {
    mark->positions[0] = decoder->positions[0];
    mark->positions[1] = decoder->positions[1];
    mark->depth = decoder->depth;
    mark->ended = decoder->ended;
}

static void decoder_restore(TextDecoder *decoder, const DecoderMark *mark) {
    decoder->positions[0] = mark->positions[0];
    decoder->positions[1] = mark->positions[1];
    decoder->depth = mark->depth;
    decoder->ended = mark->ended;
}

static int decoder_at(const TextDecoder *decoder, const DecoderMark *mark) {
    if (decoder->depth != mark->depth || decoder->ended != mark->ended)
        return 0;
    for (uint8_t i = 0; i <= decoder->depth; ++i)
        if (decoder->positions[i] != mark->positions[i])
            return 0;
    return 1;
}

void text_decoder_init(TextDecoder *decoder, TextSpan input,
                       const TextSpan *fields, uint8_t field_count) {
    const uint8_t *field = 0;
    size_t field_length = 0;
    for (uint8_t index = 0; fields && index < field_count; ++index) {
        if (fields[index].field_id != 1)
            continue;
        field = fields[index].data;
        field_length = fields[index].length;
        break;
    }
    text_decoder_init_raw(decoder, input.data, input.length,
                          field, field_length, 0);
}

void text_decoder_init_raw(TextDecoder *decoder,
                           const uint8_t *input, size_t input_length,
                           const uint8_t *field, size_t field_length,
                           uint8_t limit_end) {
    clear_bytes(decoder, sizeof(*decoder));
    if (!input || input_length > UINT16_MAX || (!input_length && !limit_end)) {
        decoder->invalid = 1;
        return;
    }
    decoder->data[0] = input;
    decoder->lengths[0] = (uint16_t)input_length;
    decoder->limit_end = limit_end != 0;
    if (field && field_length && field_length <= UINT16_MAX) {
        decoder->data[1] = field;
        decoder->lengths[1] = (uint16_t)field_length;
    }
}

size_t text_decoder_consumed(const TextDecoder *decoder) {
    return decoder->positions[0];
}

static TextStatus decoder_fail(TextDecoder *decoder, TextToken *token,
                               const TextLayoutMark *saved, TextStatus status) {
    token->saved_positions[0] = saved->positions[0];
    token->saved_positions[1] = saved->positions[1];
    token->saved_depth = saved->depth;
    token->saved_ended = saved->ended;
    decoder->positions[0] = saved->positions[0];
    decoder->positions[1] = saved->positions[1];
    decoder->depth = saved->depth;
    decoder->ended = saved->ended;
    return status;
}

__attribute__((always_inline)) inline TextStatus text_decoder_next(
        TextDecoder *decoder, TextToken *token) {
    TextLayoutMark saved;
    saved.positions[0] = decoder->positions[0];
    saved.positions[1] = decoder->positions[1];
    saved.depth = decoder->depth;
    saved.ended = decoder->ended;
    if (decoder->invalid)
        return decoder_fail(decoder, token, &saved, TEXT_MALFORMED_INPUT);
    if (decoder->ended) {
        clear_bytes(token, sizeof(*token));
        token->kind = TEXT_TOKEN_END;
        token->consumed = text_decoder_consumed(decoder);
        return TEXT_END;
    }
    for (;;) {
        uint16_t *position = &decoder->positions[decoder->depth];
        uint16_t length = decoder->lengths[decoder->depth];
        if (*position >= length) {
            if (!decoder->depth && decoder->limit_end) {
                decoder->ended = 1;
                clear_bytes(token, sizeof(*token));
                token->kind = TEXT_TOKEN_END;
                token->consumed = text_decoder_consumed(decoder);
                return TEXT_END;
            }
            return decoder_fail(decoder, token, &saved, TEXT_MALFORMED_INPUT);
        }
        const uint8_t *source = decoder->data[decoder->depth];
        uint8_t value = source[*position];
        if (value >= 0x20) {
            uint16_t code;
            if (value >= 0x80 && value <= 0x9f) {
                if (*position + 1 >= length || source[*position + 1] == 0)
                    return decoder_fail(decoder, token, &saved,
                                        TEXT_MALFORMED_INPUT);
                code = (uint16_t)((value << 8) | source[*position + 1]);
                *position += 2;
            } else {
                code = value;
                ++*position;
            }
            token->kind = TEXT_TOKEN_GLYPH;
            token->code = code;
            token->a = 0;
            token->b = 0;
            token->consumed = text_decoder_consumed(decoder);
            return TEXT_OK;
        }
        if (value == 0) {
            ++*position;
            if (decoder->depth) {
                --decoder->depth;
                continue;
            }
            decoder->ended = 1;
            clear_bytes(token, sizeof(*token));
            token->kind = TEXT_TOKEN_END;
            token->consumed = text_decoder_consumed(decoder);
            return TEXT_END;
        }
        if (value == 1) {
            if (*position + 1 >= length)
                return decoder_fail(decoder, token, &saved, TEXT_MALFORMED_INPUT);
            uint8_t color = source[*position + 1];
            *position += 2;
            clear_bytes(token, sizeof(*token));
            token->kind = TEXT_TOKEN_COLOR;
            token->a = color;
        } else if (value == 2) {
            if (*position + 2 >= length)
                return decoder_fail(decoder, token, &saved, TEXT_MALFORMED_INPUT);
            uint8_t x = source[*position + 1];
            uint8_t y = source[*position + 2];
            *position += 3;
            clear_bytes(token, sizeof(*token));
            token->kind = TEXT_TOKEN_POSITION;
            token->a = x;
            token->b = y;
        } else if (value == 3) {
            uint8_t field_id = 1;
            ++*position;
            if (!decoder->data[field_id])
                return decoder_fail(decoder, token, &saved, TEXT_MISSING_FIELD);
            if (decoder->depth)
                return decoder_fail(decoder, token, &saved, TEXT_MALFORMED_INPUT);
            decoder->depth = 1;
            decoder->positions[1] = 0;
            continue;
        } else if (value == 4) {
            if (*position + 1 >= length ||
                    source[*position + 1] > TEXT_ALIGN_RIGHT)
                return decoder_fail(decoder, token, &saved, TEXT_MALFORMED_INPUT);
            uint8_t alignment = source[*position + 1];
            *position += 2;
            clear_bytes(token, sizeof(*token));
            token->kind = TEXT_TOKEN_ALIGN;
            token->a = alignment;
        } else if (value == 6) {
            if (*position + 1 >= length)
                return decoder_fail(decoder, token, &saved, TEXT_MALFORMED_INPUT);
            uint8_t indent = source[*position + 1];
            *position += 2;
            clear_bytes(token, sizeof(*token));
            token->kind = TEXT_TOKEN_INDENT;
            token->a = indent;
        } else if (value == 7 || value == 8) {
            if (*position + 2 >= length)
                return decoder_fail(decoder, token, &saved, TEXT_MALFORMED_INPUT);
            uint8_t x = source[*position + 1];
            uint8_t y = source[*position + 2];
            *position += 3;
            clear_bytes(token, sizeof(*token));
            if (value == 8 && x >= y) {
                TextLayoutMark cleared = {{0, 0}, 0, 0};
                return decoder_fail(decoder, token, &cleared, TEXT_MALFORMED_INPUT);
            }
            token->kind = value == 8 ? TEXT_TOKEN_REGION : TEXT_TOKEN_ANCHOR;
            token->a = x;
            token->b = y;
        } else if (value == 10) {
            ++*position;
            clear_bytes(token, sizeof(*token));
            token->kind = TEXT_TOKEN_NEWLINE;
        } else {
            return decoder_fail(decoder, token, &saved, TEXT_MALFORMED_INPUT);
        }
        token->consumed = text_decoder_consumed(decoder);
        return TEXT_OK;
    }
}

void text_writer_init(TextWriter *writer, uint8_t *data, size_t capacity) {
    writer->data = data;
    writer->capacity = capacity;
    writer->length = 0;
    writer->status = capacity ? TEXT_OK : TEXT_CAPACITY_FAILURE;
    if (capacity)
        data[0] = 0;
}

void text_writer_init_count(TextWriter *writer, size_t capacity) {
    writer->data = 0;
    writer->capacity = capacity;
    writer->length = 0;
    writer->status = capacity ? TEXT_OK : TEXT_CAPACITY_FAILURE;
}

TEXT_O2 static TextStatus writer_unit(TextWriter *writer, const uint8_t *unit,
                              size_t length) {
    if (writer->status != TEXT_OK)
        return writer->status;
    if (writer->length + length + 1 > writer->capacity) {
        writer->status = TEXT_CAPACITY_FAILURE;
        return writer->status;
    }
    if (writer->data)
        copy_bytes(writer->data + writer->length, unit, length);
    writer->length += length;
    if (writer->data)
        writer->data[writer->length] = 0;
    return TEXT_OK;
}

TextStatus text_writer_byte(TextWriter *writer, uint8_t value) {
    if (value < 0x20 || (value >= 0x80 && value <= 0x9f))
        return writer->status = TEXT_MALFORMED_INPUT;
    return writer_unit(writer, &value, 1);
}

TextStatus text_writer_code(TextWriter *writer, uint16_t code) {
    uint8_t unit[2];
    size_t length;
    if (code > 0xff) {
        unit[0] = (uint8_t)(code >> 8);
        unit[1] = (uint8_t)code;
        length = 2;
    } else {
        unit[0] = (uint8_t)code;
        length = 1;
    }
    if (unit[0] < 0x20 ||
            (length == 1 && unit[0] >= 0x80 && unit[0] <= 0x9f) || (length == 2 &&
            (unit[0] < 0x80 || unit[0] > 0x9f || !unit[1])))
        return writer->status = TEXT_MALFORMED_INPUT;
    return writer_unit(writer, unit, length);
}

TextStatus text_writer_control(TextWriter *writer, uint8_t control,
                               uint8_t a, uint8_t b) {
    uint8_t unit[3] = {control, a, b};
    size_t length = control == 1 || control == 4 || control == 6 ? 2 :
        control == 2 || control == 7 || control == 8 ? 3 : 1;
    if (control != 1 && control != 2 && control != 3 && control != 4 &&
            control != 6 && control != 7 && control != 8 && control != 10)
        return writer->status = TEXT_MALFORMED_INPUT;
    if (control == 4 && a > TEXT_ALIGN_RIGHT)
        return writer->status = TEXT_MALFORMED_INPUT;
    if (control == 8 && a >= b)
        return writer->status = TEXT_MALFORMED_INPUT;
    return writer_unit(writer, unit, length);
}

TEXT_O2 TextStatus text_writer_span(TextWriter *writer, TextSpan span) {
    size_t position = 0;
    while (position < span.length && span.data[position]) {
        uint8_t value = span.data[position];
        size_t length;
        if (value == 1 || value == 4 || value == 6)
            length = 2;
        else if (value == 2 || value == 7 || value == 8)
            length = 3;
        else if (value == 3 || value == 10 || value >= 0x20)
            length = value >= 0x80 && value <= 0x9f ? 2 : 1;
        else
            return writer->status = TEXT_MALFORMED_INPUT;
        if (position + length >= span.length &&
                (position + length > span.length ||
                 (position + length == span.length && span.data[span.length - 1])))
            return writer->status = TEXT_MALFORMED_INPUT;
        if (length == 2 && value >= 0x80 && value <= 0x9f &&
                !span.data[position + 1])
            return writer->status = TEXT_MALFORMED_INPUT;
        if (value == 4 && span.data[position + 1] > TEXT_ALIGN_RIGHT)
            return writer->status = TEXT_MALFORMED_INPUT;
        if (value == 8 && span.data[position + 1] >= span.data[position + 2])
            return writer->status = TEXT_MALFORMED_INPUT;
        position += length;
    }
    if (position >= span.length || span.data[position] != 0)
        return writer->status = TEXT_MALFORMED_INPUT;
    return writer_unit(writer, span.data, position);
}

TextStatus text_writer_open(TextWriter *writer, uint8_t *data, size_t capacity) {
    if (!data || !capacity) {
        text_writer_init_count(writer, 0);
        return writer->status;
    }
    TextWriter counter;
    text_writer_init_count(&counter, capacity);
    TextSpan span = {data, capacity, 0xff};
    TextStatus status = text_writer_span(&counter, span);
    writer->data = data;
    writer->capacity = capacity;
    writer->length = counter.length;
    writer->status = status;
    return status;
}

TEXT_O2 static uint32_t divide_unsigned(uint32_t value, uint32_t divisor,
                                uint32_t *remainder) {
    uint32_t quotient = 0;
    uint32_t bit = 1;
    while (divisor <= (value >> 1)) {
        divisor <<= 1;
        bit <<= 1;
    }
    do {
        if (value >= divisor) {
            value -= divisor;
            quotient |= bit;
        }
        divisor >>= 1;
        bit >>= 1;
    } while (bit);
    *remainder = value;
    return quotient;
}

TEXT_O2 TextStatus text_writer_number(TextWriter *writer, int32_t value,
                              uint8_t digits, uint8_t mode, uint8_t face) {
    uint8_t compact = face == TEXT_FACE_COMPACT;
    uint32_t magnitude = value < 0 ? 0u - (uint32_t)value : (uint32_t)value;
#ifdef TEXT_RUNTIME_SCRATCH
    uint8_t *encoded = (uint8_t *)SCAN_WORKSPACE;
#else
    uint8_t encoded[24];
#endif
    uint8_t length = 0;
    uint8_t units = 0;
    if (!digits || digits > 10)
        return writer->status = TEXT_CAPACITY_FAILURE;
    if (mode & 4) {
        uint16_t sign = compact ? (value < 0 ? '-' : value > 0 ? '+' : ' ')
                                : (value < 0 ? 0x817c : value > 0 ? 0x817b : 0x817d);
        if (sign > 0xff)
            encoded[length++] = (uint8_t)(sign >> 8);
        encoded[length++] = (uint8_t)sign;
        ++units;
    }
    uint8_t pad = mode & 3;
    static const uint32_t divisors[10] = {
        1000000000u, 100000000u, 10000000u, 1000000u, 100000u,
        10000u, 1000u, 100u, 10u, 1u,
    };
    uint8_t started = 0;
    for (uint8_t position = 0; position < 10; ++position) {
        if (position >= 10 - digits) {
            uint32_t divisor = divisors[position];
            uint32_t remainder;
            uint32_t quotient = divisor == 1 ? magnitude :
                divisor == 10 && magnitude < 65536 ? (magnitude * 52429u) >> 19 :
                divide_unsigned(magnitude, divisor, &remainder);
            uint8_t digit = (uint8_t)quotient;
            if (digit || started || position == 9) {
                uint16_t code = compact ? (uint16_t)('0' + digit)
                                        : (uint16_t)(0x824f + digit);
                if (code > 0xff)
                    encoded[length++] = (uint8_t)(code >> 8);
                encoded[length++] = (uint8_t)code;
                ++units;
                magnitude -= divisor * digit;
                started = 1;
            } else if (pad == 3) {
                uint16_t code = compact ? '0' : 0x824f;
                if (code > 0xff)
                    encoded[length++] = (uint8_t)(code >> 8);
                encoded[length++] = (uint8_t)code;
                ++units;
            }
        }
    }
    while (pad == 1 && units < digits) {
        uint16_t code = compact ? ' ' : 0x8140;
        if (code > 0xff)
            encoded[length++] = (uint8_t)(code >> 8);
        encoded[length++] = (uint8_t)code;
        ++units;
    }
    return writer_unit(writer, encoded, length);
}

TEXT_O2 static TextStatus writer_plain_span(TextWriter *writer, TextSpan span) {
    if (writer->status != TEXT_OK)
        return writer->status;
    if (!span.data || !span.length)
        return writer->status = TEXT_MALFORMED_INPUT;
    size_t position = 0;
    while (position < span.length && span.data[position]) {
        uint8_t value = span.data[position];
        if (value < 0x20)
            return writer->status = TEXT_MALFORMED_INPUT;
        size_t length = value >= 0x80 && value <= 0x9f ? 2 : 1;
        if (position + length >= span.length ||
                (length == 2 && !span.data[position + 1]))
            return writer->status = TEXT_MALFORMED_INPUT;
        position += length;
    }
    if (position >= span.length)
        return writer->status = TEXT_MALFORMED_INPUT;
    return writer_unit(writer, span.data, position);
}

static const TextTemplateValue *template_value(
        const TextTemplateValue *values, size_t value_count, uint8_t id,
        uint8_t kind, TextStatus *status) {
    const TextTemplateValue *found = 0;
    for (size_t index = 0; index < value_count; ++index) {
        if (values[index].id != id)
            continue;
        if (found) {
            *status = TEXT_MALFORMED_INPUT;
            return 0;
        }
        found = &values[index];
    }
    if (!found) {
        *status = TEXT_MISSING_FIELD;
        return 0;
    }
    if (found->kind != kind) {
        *status = TEXT_MALFORMED_INPUT;
        return 0;
    }
    return found;
}

TextStatus text_template_write(TextWriter *writer,
                               const TextTemplatePart *parts,
                               size_t part_count,
                               const TextTemplateValue *values,
                               size_t value_count) {
    if (writer->status != TEXT_OK)
        return writer->status;
    if ((part_count && !parts) || (value_count && !values))
        return writer->status = TEXT_MALFORMED_INPUT;
    for (size_t index = 0; index < part_count; ++index) {
        const TextTemplatePart *part = &parts[index];
        TextStatus status;
        if (part->kind == TEXT_TEMPLATE_LITERAL) {
            status = writer_plain_span(writer, part->literal);
        } else if (part->kind == TEXT_TEMPLATE_FIELD) {
            if (part->field_kind > TEXT_FIELD_NUMBER)
                return writer->status = TEXT_MALFORMED_INPUT;
            const TextTemplateValue *value = template_value(
                values, value_count, part->field_id, part->field_kind, &status);
            if (!value)
                return writer->status = status;
            if (value->kind == TEXT_FIELD_NUMBER)
                status = text_writer_number(writer, value->number,
                    value->digits, value->mode, value->face);
            else
                status = writer_plain_span(writer, value->text);
        } else if (part->kind == TEXT_TEMPLATE_COLOR) {
            status = text_writer_control(writer, 1, part->a, 0);
        } else if (part->kind == TEXT_TEMPLATE_POSITION) {
            status = text_writer_control(writer, 2, part->a, part->b);
        } else if (part->kind == TEXT_TEMPLATE_NEWLINE) {
            status = text_writer_control(writer, 10, 0, 0);
        } else if (part->kind == TEXT_TEMPLATE_ALIGN) {
            status = text_writer_control(writer, 4, part->a, 0);
        } else if (part->kind == TEXT_TEMPLATE_PLAYER_REF) {
            status = text_writer_control(writer, 3, 0, 0);
        } else if (part->kind == TEXT_TEMPLATE_INDENT) {
            status = text_writer_control(writer, 6, part->a, 0);
        } else if (part->kind == TEXT_TEMPLATE_ANCHOR) {
            status = text_writer_control(writer, 7, part->a, part->b);
        } else if (part->kind == TEXT_TEMPLATE_REGION) {
            status = text_writer_control(writer, 8, part->a, part->b);
        } else {
            return writer->status = TEXT_MALFORMED_INPUT;
        }
        if (status != TEXT_OK)
            return status;
    }
    return TEXT_OK;
}

static __attribute__((always_inline)) inline int font_find(
        const TextFont *font, uint8_t face, uint16_t code) {
    if (face == TEXT_FACE_AUTO)
        face = code <= 0xff ? TEXT_FACE_COMPACT : TEXT_FACE_TALL;
    if (face == TEXT_FACE_COMPACT) {
        for (uint16_t i = font->range_count; i; --i) {
            const TextRange *range = &font->ranges[i - 1];
            if (range->face != face || code < range->first)
                continue;
            uint16_t offset = (uint16_t)(code - range->first);
            if (offset < range->count)
                return range->base + offset;
        }
        return -1;
    }
    for (uint16_t i = 0; i < font->range_count; ++i) {
        const TextRange *range = &font->ranges[i];
        if (range->face != face || code < range->first)
            continue;
        uint16_t offset = (uint16_t)(code - range->first);
        if (offset < range->count)
            return range->base + offset;
    }
    return -1;
}

int text_font_find(const TextFont *font, uint8_t face, uint16_t code) {
    return font_find(font, face, code);
}

static __attribute__((always_inline)) inline int font_pair(
        const TextFont *font, int left, int right) {
    if (!font->pair_count || left < 0 || left >= font->metric_count)
        return 0;
    int low = font->pair_offsets ? font->pair_offsets[left] : 0;
    int high = font->pair_offsets ? font->pair_offsets[left + 1]
                                  : font->pair_count;
    if (low >= high)
        return 0;
    while (low < high) {
        int middle = low + (high - low) / 2;
        const TextPair *pair = &font->pairs[middle];
        uint32_t candidate = font->pair_offsets ? pair->right
                                                : ((uint32_t)pair->left << 16) |
                                                  pair->right;
        uint32_t key = font->pair_offsets ? (uint16_t)right
                                          : ((uint32_t)(uint16_t)left << 16) |
                                            (uint16_t)right;
        if (candidate < key)
            low = middle + 1;
        else
            high = middle;
    }
    if (low < font->pair_count && font->pairs[low].left == left &&
            font->pairs[low].right == right)
        return font->pairs[low].adjust;
    return 0;
}

int text_font_pair(const TextFont *font, int left, int right) {
    return font_pair(font, left, right);
}

int text_font_digit_advance(const TextFont *font, uint8_t face) {
    int maximum = 1;
    for (int digit = 0; digit < 10; ++digit) {
        uint16_t code = face == TEXT_FACE_COMPACT ? (uint16_t)('0' + digit)
                                                 : (uint16_t)(0x824f + digit);
        int index = font_find(font, face, code);
        if (index >= 0 && font->metrics[index].advance > maximum)
            maximum = font->metrics[index].advance;
    }
    return maximum;
}

static __attribute__((always_inline)) inline int is_space(uint16_t code) {
    return code == 0x20 || code == 0x8140;
}

static __attribute__((always_inline)) inline int resolve_metric(
        const TextFont *font, uint8_t face, uint16_t code, int *missing) {
    int index = font_find(font, face, code);
    if (index >= 0) {
        *missing = 0;
        return index;
    }
    uint8_t selected = face == TEXT_FACE_AUTO
        ? (code <= 0xff ? TEXT_FACE_COMPACT : TEXT_FACE_TALL) : face;
    index = selected == TEXT_FACE_COMPACT ? font->compact_fallback
                                         : font->tall_fallback;
    if (index < 0 || index >= font->metric_count)
        return -1;
    *missing = 1;
    return index;
}

static __attribute__((always_inline)) inline int glyph_advance(
        const TextFont *font, const TextProfile *profile, int index) {
    const TextMetric *metric = &font->metrics[index];
    if (profile->tabular_advance &&
            ((metric->face == TEXT_FACE_TALL && metric->code >= 0x824f && metric->code <= 0x8258) ||
             (metric->face == TEXT_FACE_COMPACT && metric->code >= '0' && metric->code <= '9')))
        return metric->face == TEXT_FACE_COMPACT && profile->compact_tabular_advance
            ? profile->compact_tabular_advance : profile->tabular_advance;
    return metric->advance;
}

// Tabular digits sit centered in their fixed advance.
static __attribute__((always_inline)) inline int glyph_offset(
        const TextFont *font, const TextProfile *profile, int index) {
    const TextMetric *metric = &font->metrics[index];
    int advance = glyph_advance(font, profile, index);
    return advance > metric->advance ? (advance - metric->advance) / 2 : 0;
}

static __attribute__((always_inline)) inline int glyph_pair(
        const TextFont *font, const TextProfile *profile,
        int left, int right) {
    if (profile->compact_tabular_advance) {
        uint16_t a = font->metrics[left].code;
        uint16_t b = font->metrics[right].code;
        if ((a >= '0' && a <= '9') || (a >= 0x824f && a <= 0x8258) ||
                (b >= '0' && b <= '9') || (b >= 0x824f && b <= 0x8258))
            return 0;
        return font_pair(font, left, right);
    }
    if (profile->tabular_advance &&
            !is_space(font->metrics[left].code) &&
            !is_space(font->metrics[right].code))
        return 0;
    return font_pair(font, left, right);
}

static __attribute__((always_inline)) inline void geometry_init(
        LineGeometry *geometry, const TextLayoutState *state) {
    clear_bytes(geometry, sizeof(*geometry));
    geometry->x = state->x;
    geometry->previous = state->previous;
    geometry->ink_left = INT16_MAX;
    geometry->ink_right = INT16_MIN;
    geometry->color = state->color;
}

static __attribute__((always_inline)) inline int16_t coordinate(int value) {
    if (value < INT16_MIN)
        return INT16_MIN;
    if (value > INT16_MAX)
        return INT16_MAX;
    return (int16_t)value;
}

static __attribute__((always_inline)) inline int glyph_adjust(
        const TextFont *font, const TextProfile *profile,
        int previous, int index) {
    int previous_advance = glyph_advance(font, profile, previous);
    int step = previous_advance + glyph_pair(font, profile, previous, index);
    return (step < 1 ? 1 : step) - previous_advance;
}

static __attribute__((always_inline)) inline int geometry_glyph_adjusted(
        const TextFont *font, const TextProfile *profile,
        LineGeometry *geometry, int index, int adjust) {
    const TextMetric *metric = &font->metrics[index];
    int advance = glyph_advance(font, profile, index);
    if (geometry->previous >= 0)
        geometry->x = coordinate(geometry->x + adjust);
    int ink_left = geometry->x + glyph_offset(font, profile, index) + metric->bearing_x;
    int ink_right = ink_left + metric->width;
    if (metric->width) {
        if (ink_left < geometry->ink_left)
            geometry->ink_left = coordinate(ink_left);
        if (ink_right > geometry->ink_right)
            geometry->ink_right = coordinate(ink_right);
    }
    int ascent = -metric->bearing_y;
    int descent = metric->bearing_y + metric->height;
    if (ascent > geometry->ascent)
        geometry->ascent = ascent;
    if (descent > geometry->descent)
        geometry->descent = descent;
    if (metric->baseline > geometry->baseline)
        geometry->baseline = metric->baseline;
    if (metric->line_height > geometry->line_height)
        geometry->line_height = metric->line_height;
    geometry->x = coordinate(geometry->x + advance);
    geometry->previous = index;
    geometry->has_glyph = 1;
    return ink_right;
}

static __attribute__((always_inline)) inline int geometry_glyph(
        const TextFont *font, const TextProfile *profile,
        LineGeometry *geometry, int index) {
    return geometry_glyph_adjusted(font, profile, geometry, index,
        geometry->previous >= 0
            ? glyph_adjust(font, profile, geometry->previous, index) : 0);
}

static __attribute__((always_inline)) inline TextStatus scan_line(
        const TextFont *font, const TextProfile *profile,
        TextDecoder *cursor, const TextLayoutState *state,
        TextLayoutCursor *layout, ScanWorkspace *work) {
    decoder_mark(cursor, &work->event.scan.start);
    geometry_init(&work->current, state);
    int break_valid = 0;
    int line_right = layout->right;
    for (;;) {
        decoder_mark(cursor, &work->event.scan.before);
        if (!break_valid && profile->wrap == TEXT_WRAP_WORD)
            work->geometry = work->current;
        TextStatus status = text_decoder_next(cursor, &work->event.scan.token);
        if (status == TEXT_END) {
            layout->render_end = work->event.scan.before;
            layout->next_consumed = (uint16_t)text_decoder_consumed(cursor);
            work->geometry = work->current;
            layout->break_kind = 0;
            return TEXT_OK;
        }
        if (status != TEXT_OK)
            return status;
        if (work->event.scan.token.kind == TEXT_TOKEN_NEWLINE) {
            layout->render_end = work->event.scan.before;
            layout->next_consumed = (uint16_t)text_decoder_consumed(cursor);
            work->geometry = work->current;
            layout->break_kind = 1;
            return TEXT_OK;
        }
        if (work->event.scan.token.kind == TEXT_TOKEN_POSITION) {
            layout->render_end = work->event.scan.before;
            layout->next_consumed = work->event.scan.before.positions[0];
            work->geometry = work->current;
            layout->break_kind = 2;
            return TEXT_OK;
        }
        if (work->event.scan.token.kind == TEXT_TOKEN_INDENT) {
            layout->render_end = work->event.scan.before;
            layout->next_consumed = work->event.scan.before.positions[0];
            work->geometry = work->current;
            layout->break_kind = 6;
            return TEXT_OK;
        }
        if (work->event.scan.token.kind == TEXT_TOKEN_ANCHOR) {
            layout->render_end = work->event.scan.before;
            layout->next_consumed = work->event.scan.before.positions[0];
            work->geometry = work->current;
            layout->break_kind = 7;
            return TEXT_OK;
        }
        if (work->event.scan.token.kind == TEXT_TOKEN_REGION) {
            layout->render_end = work->event.scan.before;
            layout->next_consumed = work->event.scan.before.positions[0];
            work->geometry = work->current;
            layout->break_kind = 8;
            return TEXT_OK;
        }
        if (work->event.scan.token.kind == TEXT_TOKEN_ALIGN) {
            layout->render_end = work->event.scan.before;
            layout->next_consumed = work->event.scan.before.positions[0];
            work->geometry = work->current;
            layout->break_kind = 5;
            return TEXT_OK;
        }
        if (work->event.scan.token.kind == TEXT_TOKEN_COLOR) {
            work->current.color = work->event.scan.token.a;
            continue;
        }
        int missing;
        int index = resolve_metric(font, profile->face,
                                   work->event.scan.token.code, &missing);
        if (index < 0)
            return TEXT_MALFORMED_INPUT;
        if (profile->wrap == TEXT_WRAP_WORD && is_space(work->event.scan.token.code)) {
            if (work->current.has_glyph) {
                layout->render_end = work->event.scan.before;
                work->geometry = work->current;
                break_valid = 1;
            }
            layout->next_consumed = (uint16_t)text_decoder_consumed(cursor);
        }
        if (!work->current.has_glyph)
            layout->line_color = work->current.color;
        geometry_glyph(font, profile, &work->current, index);
        int exceeded;
        if (profile->alignment_bound == TEXT_BOUND_INK) {
            if (work->current.ink_left == INT16_MAX) {
                exceeded = 0;
            } else if (layout->alignment == TEXT_ALIGN_LEFT) {
                exceeded = work->current.ink_left < layout->left ||
                           work->current.ink_right > line_right;
            } else {
                exceeded = work->current.ink_right -
                           work->current.ink_left >
                           line_right - layout->left;
            }
        } else if (layout->alignment == TEXT_ALIGN_LEFT) {
            exceeded = state->x < layout->left ||
                       work->current.x > line_right;
        } else {
            exceeded = work->current.x - state->x >
                       line_right - layout->left;
        }
        if (!exceeded)
            continue;
        layout->overflowed = 1;
        if (profile->wrap == TEXT_WRAP_WORD &&
                work->geometry.has_glyph) {
            if (!break_valid) {
                layout->render_end = work->event.scan.before;
                layout->next_consumed = work->event.scan.before.positions[0];
            }
            layout->break_kind = break_valid ? 4 : 3;
            return TEXT_OK;
        }
        if (profile->overflow == TEXT_OVERFLOW_ERROR)
            return TEXT_LAYOUT_OVERFLOW;
    }
}

static __attribute__((always_inline)) inline int line_shift(
        const TextProfile *profile, const TextLayoutState *state,
        const LineGeometry *geometry, const TextLayoutCursor *layout) {
    if (layout->alignment == TEXT_ALIGN_LEFT)
        return 0;
    int left;
    int right;
    if (profile->alignment_bound == TEXT_BOUND_INK &&
            geometry->ink_left != INT16_MAX) {
        left = geometry->ink_left;
        right = geometry->ink_right;
    } else {
        left = state->x;
        right = geometry->x;
    }
    int width = right - left;
    int target = layout->alignment == TEXT_ALIGN_CENTER
        ? layout->left + (layout->right - layout->left - width) / 2
        : layout->right - width;
    return target - left;
}

static void result_bounds(TextLayoutResult *result, int left, int top,
                          int right, int bottom) {
    if (left < result->ink_left)
        result->ink_left = left;
    if (top < result->ink_top)
        result->ink_top = top;
    if (right > result->ink_right)
        result->ink_right = right;
    if (bottom > result->ink_bottom)
        result->ink_bottom = bottom;
}

enum {
    LAYOUT_SCAN,
    LAYOUT_EMIT,
    LAYOUT_DONE,
};

static TextStatus layout_status(TextDecoder *decoder,
                                TextLayoutResult *result,
                                TextStatus status) {
    result->status = (uint8_t)status;
    result->consumed = (uint16_t)text_decoder_consumed(decoder);
    return status;
}

TextStatus text_layout_begin(const TextProfile *profile, TextDecoder *decoder,
                             TextLayoutState *state, TextLayoutCursor *cursor,
                             TextLayoutResult *result) {
    clear_bytes(cursor, sizeof(*cursor));
    cursor->alignment = profile->alignment;
    cursor->left = profile->left;
    cursor->right = profile->right;
    clear_bytes(result, sizeof(*result));
    result->ink_left = INT16_MAX;
    result->ink_top = INT16_MAX;
    result->ink_right = INT16_MIN;
    result->ink_bottom = INT16_MIN;
    if (profile->right < profile->left || profile->bottom < profile->top) {
        cursor->phase = LAYOUT_DONE;
        return layout_status(decoder, result, TEXT_LAYOUT_OVERFLOW);
    }
    if (!state->initialized) {
        state->x = profile->left;
        state->y = profile->top;
        state->previous = -1;
        state->color = 0;
        state->initialized = 1;
    }
    cursor->origin_y = state->y;
    return TEXT_OK;
}

static __attribute__((always_inline)) inline TextStatus begin_line_event(
        const TextProfile *profile, TextDecoder *decoder,
        const TextLayoutState *state, TextLayoutCursor *cursor,
        TextLayoutResult *result, TextEvent *event,
        const LineGeometry *geometry) {
        int shift = line_shift(profile, state, geometry,
                               cursor);
        int line_height = geometry->has_glyph
            ? geometry->line_height : profile->line_height;
        int natural_height = geometry->ascent + geometry->descent;
        if (line_height < natural_height)
            line_height = natural_height;
        int baseline = geometry->has_glyph
            ? geometry->baseline : profile->baseline;
        int vertical_overflow = state->y < profile->top ||
                                state->y + line_height > profile->bottom;
        if (geometry->ink_left != INT16_MAX) {
            int ink_top = state->y + baseline - geometry->ascent;
            int ink_bottom = state->y + baseline + geometry->descent;
            vertical_overflow |= ink_top < profile->top ||
                                 ink_bottom > profile->bottom;
        }
        if (geometry->has_glyph && vertical_overflow) {
            if (profile->overflow == TEXT_OVERFLOW_ERROR) {
                cursor->phase = LAYOUT_DONE;
                return layout_status(decoder, result, TEXT_LAYOUT_OVERFLOW);
            }
        }
        cursor->shift = (int16_t)shift;
        cursor->end_color = geometry->color;
        cursor->line_height = (uint16_t)line_height;
        cursor->line_baseline = (uint8_t)baseline;
        cursor->overflowed |= (uint8_t)(geometry->has_glyph &&
                                        vertical_overflow);
        clear_bytes(event, sizeof(*event));
        event->kind = TEXT_EVENT_LINE_BEGIN;
        event->advance_left = (int16_t)(state->x + shift);
        event->advance_right = (int16_t)(geometry->x + shift);
        event->left = (int16_t)(geometry->ink_left == INT16_MAX
            ? state->x + shift : geometry->ink_left + shift);
        event->right = (int16_t)(geometry->ink_right == INT16_MIN
            ? state->x + shift : geometry->ink_right + shift);
        if (cursor->left != profile->left || cursor->right != profile->right) {
            if (event->left < cursor->left)
                event->left = cursor->left;
            if (event->right > cursor->right)
                event->right = cursor->right;
            if (event->advance_left < cursor->left)
                event->advance_left = cursor->left;
            if (event->advance_right > cursor->right)
                event->advance_right = cursor->right;
        }
        event->top = state->y;
        event->bottom = (int16_t)(state->y + line_height);
        event->value = geometry->has_glyph
            ? cursor->line_color : geometry->color;
        event->consumed = text_decoder_consumed(decoder);
        event->next_consumed = cursor->next_consumed;
        cursor->pending = TEXT_EVENT_LINE_BEGIN + 1;
        return TEXT_OK;
}

__attribute__((noinline)) TEXT_O2 TextStatus text_layout_scan_peek(
        const TextFont *font, const TextProfile *profile,
        TextDecoder *decoder, TextLayoutState *state,
        TextLayoutCursor *cursor, TextLayoutResult *result,
        TextEvent *event) {
#ifdef TEXT_RUNTIME_SCRATCH
        ScanWorkspace *work = SCAN_WORKSPACE;
#else
        ScanWorkspace work_storage;
        ScanWorkspace *work = &work_storage;
#endif
        cursor->overflowed = 0;
        TextStatus status = scan_line(font, profile, decoder, state,
                                      cursor, work);
        decoder_restore(decoder, &work->event.scan.start);
        if (status != TEXT_OK) {
            cursor->phase = LAYOUT_DONE;
            return layout_status(decoder, result, status);
        }
        return begin_line_event(profile, decoder, state, cursor, result,
                                event, &work->geometry);
}

// Returns the metric of an inkless glyph that text_blank_run accepts, or -1.
TEXT_O2 int text_blank_glyph(const TextFont *font, const TextProfile *profile,
                             uint16_t code) {
    int missing;
    int index = resolve_metric(font, profile->face, code, &missing);
    return index < 0 || missing || font->metrics[index].width ? -1 : index;
}

// Advances state over count copies of one inkless glyph as layout would;
// returns the line height, or 0 when the glyph has ink or no metric.
TEXT_O2 int text_blank_run(const TextFont *font, const TextProfile *profile,
                           TextLayoutState *state, uint16_t code, int count) {
    int missing;
    int index = resolve_metric(font, profile->face, code, &missing);
    if (index < 0 || missing || font->metrics[index].width || count < 1)
        return 0;
    const TextMetric *metric = &font->metrics[index];
    int advance = glyph_advance(font, profile, index);
    int x = state->x;
    int previous = state->previous;
    int repeat = 0;
    for (int i = 0; i < count; ++i) {
        if (previous >= 0) {
            if (previous != index)
                x = coordinate(x + glyph_adjust(font, profile, previous, index));
            else {
                if (i == 1 || (i == 0 && !repeat))
                    repeat = glyph_adjust(font, profile, index, index);
                x = coordinate(x + repeat);
            }
        }
        x = coordinate(x + advance);
        previous = index;
    }
    state->x = (int16_t)x;
    state->previous = (int16_t)previous;
    int ascent = -metric->bearing_y;
    int descent = metric->bearing_y + metric->height;
    int natural = (ascent > 0 ? ascent : 0) + (descent > 0 ? descent : 0);
    return metric->line_height > natural ? metric->line_height : natural;
}

// text_layout_scan_peek for a line of count copies of one inkless glyph unit
// at the decoder start; returns -1 without changes that matter when it cannot.
TEXT_O2 int text_layout_blank_peek(
        const TextFont *font, const TextProfile *profile,
        TextDecoder *decoder, TextLayoutState *state,
        TextLayoutCursor *cursor, TextLayoutResult *result,
        TextEvent *event, uint16_t code, uint8_t unit, int count) {
    if (cursor->phase != LAYOUT_SCAN || cursor->pending || decoder->depth ||
            decoder->ended || decoder->invalid || profile->wrap != TEXT_WRAP_NONE ||
            count < 1)
        return -1;
    int missing;
    int index = resolve_metric(font, profile->face, code, &missing);
    if (index < 0 || font->metrics[index].width)
        return -1;
#ifdef TEXT_RUNTIME_SCRATCH
    LineGeometry *line = &SCAN_WORKSPACE->current;
#else
    LineGeometry line_storage;
    LineGeometry *line = &line_storage;
#endif
    geometry_init(line, state);
    int line_right = cursor->right;
    uint8_t overflowed = 0;
    for (int i = 0; i < count; ++i) {
        if (!line->has_glyph)
            cursor->line_color = line->color;
        geometry_glyph(font, profile, line, index);
        int exceeded;
        if (profile->alignment_bound == TEXT_BOUND_INK) {
            if (line->ink_left == INT16_MAX) {
                exceeded = 0;
            } else if (cursor->alignment == TEXT_ALIGN_LEFT) {
                exceeded = line->ink_left < cursor->left ||
                           line->ink_right > line_right;
            } else {
                exceeded = line->ink_right - line->ink_left >
                           line_right - cursor->left;
            }
        } else if (cursor->alignment == TEXT_ALIGN_LEFT) {
            exceeded = state->x < cursor->left || line->x > line_right;
        } else {
            exceeded = line->x - state->x > line_right - cursor->left;
        }
        if (exceeded) {
            if (profile->overflow == TEXT_OVERFLOW_ERROR)
                return -1;
            overflowed = 1;
        }
    }
    uint16_t end = (uint16_t)(decoder->positions[0] + count * unit);
    cursor->overflowed = overflowed;
    cursor->render_end.positions[0] = end;
    cursor->render_end.positions[1] = decoder->positions[1];
    cursor->render_end.depth = 0;
    cursor->render_end.ended = 0;
    cursor->next_consumed = (uint16_t)(end + 1);
    cursor->break_kind = 0;
    return begin_line_event(profile, decoder, state, cursor, result, event,
                            line);
}

// text_layout_scan_peek for one line of glyph, color, and newline tokens,
// read straight from the input; the plan feeds text_layout_simple_emit.
// Returns -1 without changes that matter when the line needs the decoder.
TEXT_O2 int text_layout_simple_peek(
        const TextFont *font, const TextProfile *profile,
        TextDecoder *decoder, TextLayoutState *state,
        TextLayoutCursor *cursor, TextLayoutResult *result,
        TextEvent *event) {
    if (cursor->phase != LAYOUT_SCAN || cursor->pending || decoder->depth ||
            decoder->ended || decoder->invalid || decoder->limit_end)
        return -1;
    const uint8_t *data = decoder->data[0];
    uint16_t length = decoder->lengths[0];
    uint16_t position = decoder->positions[0];
    int word = profile->wrap == TEXT_WRAP_WORD;
#ifdef TEXT_RUNTIME_SCRATCH
    LineGeometry *line = &SCAN_WORKSPACE->current;
    LineGeometry *kept = &SCAN_WORKSPACE->geometry;
#else
    LineGeometry line_storage;
    LineGeometry kept_storage;
    LineGeometry *line = &line_storage;
    LineGeometry *kept = &kept_storage;
#endif
    geometry_init(line, state);
    int line_right = cursor->right;
    uint8_t overflowed = 0;
    uint8_t line_color = cursor->line_color;
    int break_valid = 0;
    int count = 0;
    int kept_count = 0;
    uint16_t render_end = 0;
    uint16_t next_consumed = 0;
    uint8_t break_kind;
    for (;;) {
        if (!break_valid && word) {
            *kept = *line;
            kept_count = count;
        }
        if (position >= length || count >= SIMPLE_CAPACITY)
            return -1;
        uint16_t before = position;
        uint8_t value = data[position];
        SimpleItem *item = &SIMPLE_ITEMS[count];
        uint16_t code;
        if (value >= 0x20) {
            if (value >= 0x80 && value <= 0x9f) {
                if (position + 1 >= length || !data[position + 1])
                    return -1;
                code = (uint16_t)((value << 8) | data[position + 1]);
                position += 2;
            } else {
                code = value;
                ++position;
            }
        } else if (value == 1) {
            if (position + 1 >= length)
                return -1;
            line->color = data[position + 1];
            position += 2;
            item->packed = (uint16_t)(SIMPLE_COLOR | line->color);
            item->end = position;
            ++count;
            continue;
        } else if (value == 0 || value == 10) {
            render_end = before;
            next_consumed = (uint16_t)(position + 1);
            *kept = *line;
            kept_count = count;
            break_kind = value == 10;
            break;
        } else {
            return -1;
        }
        int missing;
        int index = resolve_metric(font, profile->face, code, &missing);
        if (index < 0 || missing || index > 0x1ff)
            return -1;
        if (word && is_space(code)) {
            if (line->has_glyph) {
                render_end = before;
                *kept = *line;
                kept_count = count;
                break_valid = 1;
            }
            next_consumed = position;
        }
        if (!line->has_glyph)
            line_color = line->color;
        int adjust = line->previous >= 0
            ? glyph_adjust(font, profile, line->previous, index) : 0;
        if (adjust < -16 || adjust > 15)
            return -1;
        geometry_glyph_adjusted(font, profile, line, index, adjust);
        item->packed = (uint16_t)(index | ((uint32_t)adjust << 11));
        item->end = position;
        ++count;
        int exceeded;
        if (profile->alignment_bound == TEXT_BOUND_INK) {
            if (line->ink_left == INT16_MAX) {
                exceeded = 0;
            } else if (cursor->alignment == TEXT_ALIGN_LEFT) {
                exceeded = line->ink_left < cursor->left ||
                           line->ink_right > line_right;
            } else {
                exceeded = line->ink_right - line->ink_left >
                           line_right - cursor->left;
            }
        } else if (cursor->alignment == TEXT_ALIGN_LEFT) {
            exceeded = state->x < cursor->left || line->x > line_right;
        } else {
            exceeded = line->x - state->x > line_right - cursor->left;
        }
        if (!exceeded)
            continue;
        overflowed = 1;
        if (word && kept->has_glyph) {
            if (!break_valid) {
                render_end = before;
                next_consumed = before;
            }
            break_kind = break_valid ? 4 : 3;
            break;
        }
        if (profile->overflow == TEXT_OVERFLOW_ERROR)
            return -1;
    }
    cursor->line_color = line_color;
    cursor->overflowed = overflowed;
    cursor->render_end.positions[0] = render_end;
    cursor->render_end.positions[1] = decoder->positions[1];
    cursor->render_end.depth = 0;
    cursor->render_end.ended = 0;
    cursor->next_consumed = next_consumed;
    cursor->break_kind = break_kind;
    cursor->simple_count = (uint8_t)kept_count;
    cursor->simple_next = 0;
    return begin_line_event(profile, decoder, state, cursor, result, event,
                            kept);
}

// text_layout_emit_peek for the next token of a text_layout_simple_peek plan;
// returns -1 at the line end so the decoder finishes the line.
TEXT_O2 int text_layout_simple_emit(
        const TextFont *font, const TextProfile *profile,
        TextDecoder *decoder, TextLayoutState *state,
        TextLayoutCursor *cursor, TextEvent *event) {
    if (cursor->simple_next >= cursor->simple_count ||
            cursor->phase != LAYOUT_EMIT || cursor->pending)
        return -1;
    const SimpleItem *item = &SIMPLE_ITEMS[cursor->simple_next++];
    cursor->after.positions[0] = item->end;
    cursor->after.positions[1] = decoder->positions[1];
    cursor->after.depth = 0;
    cursor->after.ended = 0;
    if (item->packed & SIMPLE_COLOR) {
        event->kind = TEXT_EVENT_COLOR;
        event->value = (uint8_t)item->packed;
        event->consumed = item->end;
        event->next_consumed = item->end;
        cursor->pending = TEXT_EVENT_COLOR + 1;
        return TEXT_OK;
    }
    int index = item->packed & 0x1ff;
    const TextMetric *metric = &font->metrics[index];
    int pen_x = state->x;
    if (state->previous >= 0)
        pen_x += (int16_t)item->packed >> 11;
    TextPlacement *glyph = &event->glyph;
    event->kind = TEXT_EVENT_GLYPH;
    glyph->metric = (uint16_t)index;
    glyph->code = metric->code;
    glyph->pen_x = coordinate(pen_x);
    glyph->ink_x = coordinate(pen_x + glyph_offset(font, profile, index) + metric->bearing_x);
    glyph->ink_y = coordinate(
        state->y + cursor->line_baseline + metric->bearing_y);
    glyph->width = metric->width;
    glyph->height = metric->height;
    glyph->color = state->color;
    glyph->missing = 0;
    glyph->consumed = item->end;
    glyph->clip_left = 0;
    glyph->clip_right = 0;
    if (cursor->left != profile->left || cursor->right != profile->right) {
        int left = cursor->left - glyph->ink_x;
        int right = glyph->ink_x + metric->width - cursor->right;
        glyph->clip_left = (uint8_t)(left < 0 ? 0 :
            left > metric->width ? metric->width : left);
        glyph->clip_right = (uint8_t)(right < 0 ? 0 :
            right > metric->width ? metric->width : right);
    }
    cursor->pending = TEXT_EVENT_GLYPH + 1;
    return TEXT_OK;
}

// Commits count copies of one blank glyph as text_layout_commit would.
TEXT_O2 void text_layout_blank_commit(
        const TextFont *font, const TextProfile *profile,
        TextDecoder *decoder, TextLayoutState *state,
        TextLayoutCursor *cursor, TextLayoutResult *result,
        uint16_t code, uint8_t unit, int count) {
    int missing = 0;
    int index = resolve_metric(font, profile->face, code, &missing);
    int advance = glyph_advance(font, profile, index);
    for (int i = 0; i < count; ++i) {
        int pen_x = state->x;
        if (state->previous >= 0) {
            int previous_advance = glyph_advance(font, profile,
                                                 state->previous);
            int step = previous_advance +
                       glyph_pair(font, profile, state->previous, index);
            pen_x += (step < 1 ? 1 : step) - previous_advance;
        }
        state->x = coordinate(coordinate(pen_x) + advance);
        state->previous = (int16_t)index;
        decoder->positions[0] = (uint16_t)(decoder->positions[0] + unit);
        ++result->glyph_count;
        result->missing_count += (uint16_t)missing;
    }
    decoder_mark(decoder, &cursor->after);
    result->consumed = (uint16_t)text_decoder_consumed(decoder);
}

__attribute__((noinline)) TEXT_O2 TextStatus text_layout_emit_peek(
        const TextFont *font, const TextProfile *profile,
        TextDecoder *decoder, TextLayoutState *state,
        TextLayoutCursor *cursor, TextLayoutResult *result,
        TextEvent *event) {
#ifdef TEXT_RUNTIME_SCRATCH
    EventWorkspace *work = EVENT_WORKSPACE;
#else
    EventWorkspace work_storage;
    EventWorkspace *work = &work_storage;
#endif
    TextToken *token = &work->scan.token;
    if (!decoder_at(decoder, &cursor->render_end)) {
        decoder_mark(decoder, &work->scan.before);
        TextStatus status = text_decoder_next(decoder, token);
        decoder_mark(decoder, &cursor->after);
        decoder_restore(decoder, &work->scan.before);
        if (status != TEXT_OK) {
            cursor->phase = LAYOUT_DONE;
            return layout_status(decoder, result, status);
        }
        if (token->kind == TEXT_TOKEN_COLOR) {
            uint8_t color = token->a;
            uint16_t consumed = token->consumed;
            clear_bytes(event, sizeof(*event));
            event->kind = TEXT_EVENT_COLOR;
            event->value = color;
            event->consumed = consumed;
            event->next_consumed = consumed;
            cursor->pending = TEXT_EVENT_COLOR + 1;
            return TEXT_OK;
        }
        if (token->kind != TEXT_TOKEN_GLYPH) {
            cursor->phase = LAYOUT_DONE;
            return layout_status(decoder, result, TEXT_MALFORMED_INPUT);
        }
        int missing;
        int index = resolve_metric(font, profile->face, token->code, &missing);
        if (index < 0) {
            cursor->phase = LAYOUT_DONE;
            return layout_status(decoder, result, TEXT_MALFORMED_INPUT);
        }
        const TextMetric *metric = &font->metrics[index];
        int pen_x = state->x;
        if (state->previous >= 0) {
            int previous_advance = glyph_advance(font, profile,
                                                 state->previous);
            int step = previous_advance +
                       glyph_pair(font, profile, state->previous, index);
            pen_x += (step < 1 ? 1 : step) - previous_advance;
        }
        uint16_t code = token->code;
        uint16_t consumed = token->consumed;
        event->kind = TEXT_EVENT_GLYPH;
        event->glyph.clip_left = 0;
        event->glyph.clip_right = 0;
        event->glyph.metric = (uint16_t)index;
        event->glyph.code = code;
        event->glyph.pen_x = coordinate(pen_x);
        event->glyph.ink_x = coordinate(pen_x + glyph_offset(font, profile, index) + metric->bearing_x);
        event->glyph.ink_y = coordinate(
            state->y + cursor->line_baseline + metric->bearing_y);
        event->glyph.width = metric->width;
        event->glyph.height = metric->height;
        event->glyph.color = state->color;
        event->glyph.missing = (uint8_t)missing;
        event->glyph.consumed = consumed;
        if (cursor->left != profile->left || cursor->right != profile->right) {
            int left = cursor->left - event->glyph.ink_x;
            int right = event->glyph.ink_x + metric->width - cursor->right;
            event->glyph.clip_left = (uint8_t)(left < 0 ? 0 :
                left > metric->width ? metric->width : left);
            event->glyph.clip_right = (uint8_t)(right < 0 ? 0 :
                right > metric->width ? metric->width : right);
        }
        cursor->pending = TEXT_EVENT_GLYPH + 1;
        return TEXT_OK;
    }
    if (!cursor->break_kind) {
        decoder_restore(decoder, &cursor->render_end);
        TextStatus status = text_decoder_next(decoder, token);
        if (status != TEXT_END) {
            cursor->phase = LAYOUT_DONE;
            return layout_status(decoder, result,
                status == TEXT_OK ? TEXT_MALFORMED_INPUT : status);
        }
        cursor->phase = LAYOUT_DONE;
        if (result->ink_left == INT16_MAX) {
            result->ink_left = result->ink_right = state->x;
            result->ink_top = result->ink_bottom = state->y;
        }
        return layout_status(decoder, result, TEXT_END);
    }
    if (cursor->break_kind == 2 || cursor->break_kind == 5 ||
            cursor->break_kind == 6 || cursor->break_kind == 7 ||
            cursor->break_kind == 8) {
        decoder_mark(decoder, &work->scan.before);
        TextStatus status = text_decoder_next(decoder, token);
        decoder_mark(decoder, &cursor->after);
        decoder_restore(decoder, &work->scan.before);
        uint8_t expected = cursor->break_kind == 2 ? TEXT_TOKEN_POSITION :
            cursor->break_kind == 6 ? TEXT_TOKEN_INDENT :
            cursor->break_kind == 7 ? TEXT_TOKEN_ANCHOR :
            cursor->break_kind == 8 ? TEXT_TOKEN_REGION : TEXT_TOKEN_ALIGN;
        if (status != TEXT_OK || token->kind != expected) {
            cursor->phase = LAYOUT_DONE;
            return layout_status(decoder, result,
                status == TEXT_OK ? TEXT_MALFORMED_INPUT : status);
        }
        uint8_t x = token->a;
        uint8_t y = token->b;
        uint16_t consumed = token->consumed;
        clear_bytes(event, sizeof(*event));
        event->kind = cursor->break_kind == 8 ? TEXT_EVENT_REGION :
            cursor->break_kind == 5 ? TEXT_EVENT_ALIGN : TEXT_EVENT_POSITION;
        if (event->kind == TEXT_EVENT_REGION) {
            event->left = profile->left + x;
            event->right = profile->left + y;
            if (event->right > profile->right)
                return layout_status(decoder, result, TEXT_LAYOUT_OVERFLOW);
        }
        if (event->kind == TEXT_EVENT_POSITION) {
            event->explicit_break = cursor->break_kind == 6;
            event->x = cursor->break_kind == 6 ?
                (int16_t)(state->x + x) :
                (int16_t)(profile->left + x * profile->position_scale);
            event->y = cursor->break_kind == 6 ? state->y :
                cursor->break_kind == 7 ?
                    (int16_t)(cursor->origin_y + y * profile->position_scale) :
                    (int16_t)(profile->top + y * profile->position_scale);
        } else {
            event->value = x;
        }
        event->consumed = consumed;
        event->next_consumed = consumed;
        cursor->pending = event->kind + 1;
        return TEXT_OK;
    }
    int advance = cursor->row_height + profile->line_gap;
    int next_y = state->y + advance;
    int page = profile->paginate && next_y >= profile->bottom;
    event->kind = page ? TEXT_EVENT_PAGE : TEXT_EVENT_LINE;
    event->y = (int16_t)(page ? profile->top : next_y);
    event->value = (uint8_t)(advance > 255 ? 255 : advance);
    event->explicit_break = cursor->break_kind == 1;
    event->consumed = cursor->render_end.positions[0];
    event->next_consumed = cursor->next_consumed;
    cursor->pending = event->kind + 1;
    return TEXT_OK;
}

TEXT_O2 TextStatus text_layout_peek(
                           const TextFont *font, const TextProfile *profile,
                            TextDecoder *decoder, TextLayoutState *state,
                            TextLayoutCursor *cursor, TextLayoutResult *result,
                            TextEvent *event) {
    if (cursor->pending)
        return layout_status(decoder, result, TEXT_MALFORMED_INPUT);
    if (cursor->phase == LAYOUT_DONE)
        return (TextStatus)result->status;
    if (cursor->phase == LAYOUT_SCAN)
        return text_layout_scan_peek(font, profile, decoder, state,
                                     cursor, result, event);
    return text_layout_emit_peek(font, profile, decoder, state,
                                 cursor, result, event);
}

int text_layout_scanning(const TextLayoutCursor *cursor) {
    return cursor->phase == LAYOUT_SCAN;
}

int text_layout_skip_line(TextDecoder *decoder, TextLayoutState *state,
                          TextLayoutCursor *cursor) {
    if (cursor->phase != LAYOUT_EMIT || cursor->pending ||
            (cursor->break_kind != 0 && cursor->break_kind != 1 &&
             cursor->break_kind != 3 && cursor->break_kind != 4))
        return 0;
    decoder_restore(decoder, &cursor->render_end);
    state->color = cursor->end_color;
    return 1;
}

TEXT_O2 TextStatus text_layout_commit(const TextFont *font, const TextProfile *profile,
                              TextDecoder *decoder, TextLayoutState *state,
                              TextLayoutCursor *cursor,
                              TextLayoutResult *result,
                              const TextEvent *event) {
    if (cursor->pending != event->kind + 1)
        return layout_status(decoder, result, TEXT_MALFORMED_INPUT);
    cursor->pending = 0;
    if (event->kind == TEXT_EVENT_GLYPH) {
        if (event->glyph.metric >= font->metric_count)
            return layout_status(decoder, result, TEXT_MALFORMED_INPUT);
        decoder_restore(decoder, &cursor->after);
        const TextMetric *metric = &font->metrics[event->glyph.metric];
        state->x = coordinate(event->glyph.pen_x +
                              glyph_advance(font, profile,
                                            event->glyph.metric));
        state->previous = (int16_t)event->glyph.metric;
        int left = event->glyph.ink_x + event->glyph.clip_left;
        int right = event->glyph.ink_x + metric->width - event->glyph.clip_right;
        if (right > left && metric->height)
            result_bounds(result, left, event->glyph.ink_y, right,
                          event->glyph.ink_y + metric->height);
        ++result->glyph_count;
        result->missing_count += event->glyph.missing;
        result->consumed = (uint16_t)text_decoder_consumed(decoder);
        return TEXT_OK;
    }
    if (event->kind == TEXT_EVENT_LINE_BEGIN) {
        state->x = coordinate(state->x + cursor->shift);
        if (cursor->line_height > cursor->row_height)
            cursor->row_height = (uint8_t)cursor->line_height;
        result->overflowed |= cursor->overflowed;
        cursor->phase = LAYOUT_EMIT;
        return TEXT_OK;
    }
    if (event->kind == TEXT_EVENT_COLOR) {
        decoder_restore(decoder, &cursor->after);
        state->color = event->value;
        result->consumed = (uint16_t)text_decoder_consumed(decoder);
        return TEXT_OK;
    }
    if (event->kind == TEXT_EVENT_GLYPH) {
        if (event->glyph.metric >= font->metric_count)
            return layout_status(decoder, result, TEXT_MALFORMED_INPUT);
        decoder_restore(decoder, &cursor->after);
        const TextMetric *metric = &font->metrics[event->glyph.metric];
        state->x = coordinate(event->glyph.pen_x +
                              glyph_advance(font, profile,
                                            event->glyph.metric));
        state->previous = (int16_t)event->glyph.metric;
        int left = event->glyph.ink_x + event->glyph.clip_left;
        int right = event->glyph.ink_x + metric->width - event->glyph.clip_right;
        if (right > left && metric->height)
            result_bounds(result, left, event->glyph.ink_y, right,
                          event->glyph.ink_y + metric->height);
        ++result->glyph_count;
        result->missing_count += event->glyph.missing;
        result->consumed = (uint16_t)text_decoder_consumed(decoder);
        return TEXT_OK;
    }
    if (event->kind == TEXT_EVENT_POSITION) {
        decoder_restore(decoder, &cursor->after);
        state->x = event->x;
        if (!event->explicit_break) {
            cursor->left = profile->left;
            cursor->right = profile->right;
            cursor->alignment = profile->alignment;
        }
        if (state->y != event->y)
            cursor->row_height = 0;
        state->y = event->y;
        state->previous = -1;
        cursor->phase = LAYOUT_SCAN;
        result->consumed = (uint16_t)text_decoder_consumed(decoder);
        return TEXT_OK;
    }
    if (event->kind == TEXT_EVENT_REGION) {
        decoder_restore(decoder, &cursor->after);
        cursor->left = event->left;
        cursor->right = event->right;
        cursor->alignment = TEXT_ALIGN_LEFT;
        state->x = cursor->left;
        state->previous = -1;
        cursor->phase = LAYOUT_SCAN;
        result->consumed = (uint16_t)text_decoder_consumed(decoder);
        return TEXT_OK;
    }
    if (event->kind == TEXT_EVENT_ALIGN) {
        decoder_restore(decoder, &cursor->after);
        cursor->alignment = event->value;
        state->x = cursor->left;
        state->previous = -1;
        cursor->phase = LAYOUT_SCAN;
        result->consumed = (uint16_t)text_decoder_consumed(decoder);
        return TEXT_OK;
    }
    if (event->kind == TEXT_EVENT_LINE || event->kind == TEXT_EVENT_PAGE) {
        decoder_restore(decoder, &cursor->render_end);
        if (cursor->break_kind == 1 || cursor->break_kind == 4) {
            TextToken token;
            TextStatus status = text_decoder_next(decoder, &token);
            int valid = cursor->break_kind == 1
                ? token.kind == TEXT_TOKEN_NEWLINE
                : token.kind == TEXT_TOKEN_GLYPH && is_space(token.code);
            if (status != TEXT_OK || !valid)
                return layout_status(decoder, result,
                    status == TEXT_OK ? TEXT_MALFORMED_INPUT : status);
        }
        state->x = profile->left;
        state->y = event->y;
        state->previous = -1;
        cursor->alignment = profile->alignment;
        cursor->left = profile->left;
        cursor->right = profile->right;
        cursor->row_height = 0;
        cursor->phase = LAYOUT_SCAN;
        result->consumed = (uint16_t)text_decoder_consumed(decoder);
        return TEXT_OK;
    }
    return layout_status(decoder, result, TEXT_MALFORMED_INPUT);
}

TextStatus text_layout(const TextFont *font, const TextProfile *profile,
                       TextDecoder *decoder, TextLayoutState *state,
                       TextEmit emit, void *context, TextLayoutResult *result) {
    TextLayoutCursor cursor;
    TextEvent event;
    TextStatus status = text_layout_begin(profile, decoder, state,
                                          &cursor, result);
    while (status == TEXT_OK) {
        status = text_layout_peek(font, profile, decoder, state,
                                  &cursor, result, &event);
        if (status != TEXT_OK)
            break;
        if (event.kind == TEXT_EVENT_COLOR ||
                event.kind == TEXT_EVENT_POSITION ||
                event.kind == TEXT_EVENT_ALIGN ||
                event.kind == TEXT_EVENT_REGION ||
                event.kind == TEXT_EVENT_LINE ||
                event.kind == TEXT_EVENT_PAGE) {
            status = text_layout_commit(font, profile, decoder, state,
                                        &cursor, result, &event);
            if (status != TEXT_OK)
                break;
            if (emit && (status = emit(context, &event)) != TEXT_OK)
                break;
        } else {
            if (emit && (status = emit(context, &event)) != TEXT_OK)
                break;
            status = text_layout_commit(font, profile, decoder, state,
                                        &cursor, result, &event);
        }
    }
    if (status != TEXT_END)
        layout_status(decoder, result, status);
    return status;
}

#define GLYPH_QUAD(x) x, x+1, x+2, x+3, x+0x10, x+0x11, x+0x12, x+0x13, \
    x+0x20, x+0x21, x+0x22, x+0x23, x+0x30, x+0x31, x+0x32, x+0x33
const uint16_t text_glyph_pixels[256] = {
    GLYPH_QUAD(0x0000), GLYPH_QUAD(0x0100),
    GLYPH_QUAD(0x0200), GLYPH_QUAD(0x0300),
    GLYPH_QUAD(0x1000), GLYPH_QUAD(0x1100),
    GLYPH_QUAD(0x1200), GLYPH_QUAD(0x1300),
    GLYPH_QUAD(0x2000), GLYPH_QUAD(0x2100),
    GLYPH_QUAD(0x2200), GLYPH_QUAD(0x2300),
    GLYPH_QUAD(0x3000), GLYPH_QUAD(0x3100),
    GLYPH_QUAD(0x3200), GLYPH_QUAD(0x3300),
};
#undef GLYPH_QUAD

static __attribute__((always_inline)) inline uint32_t expand_glyph_row(
        uint32_t packed) {
    return text_glyph_pixels[packed & 255] |
           ((uint32_t)text_glyph_pixels[(packed >> 8) & 255] << 16);
}

static __attribute__((always_inline)) inline uint32_t blend_word(
        uint32_t old, uint32_t mask, uint32_t pixels, uint8_t variant) {
    uint32_t old_ink = old & 0x33333333u;
    uint32_t new_ink = pixels & 0x33333333u;
    uint32_t greater = ~((new_ink | 0x44444444u) - old_ink) & 0x44444444u;
    uint32_t variant_bits = (uint32_t)(variant * 4) * 0x11111111u;
    uint32_t different = (old ^ variant_bits) & 0xccccccccu;
    different = (different | (different >> 1) |
                 (different >> 2) | (different >> 3)) & 0x11111111u;
    uint32_t keep = (greater >> 2) & ~different & 0x11111111u;
    keep |= keep << 1;
    keep |= keep << 2;
    keep &= mask;
    return (old & (~mask | keep)) | (pixels & ~keep);
}

static __attribute__((always_inline)) inline uint32_t glyph_pixel_mask(
        const TextMetric *metric, const TextPlacement *placement) {
    int left = placement->clip_left;
    int right = metric->width - placement->clip_right;
    if (right <= left)
        return 0;
    return ((1u << (right * 2)) - 1) & ~((1u << (left * 2)) - 1);
}

#ifdef TEXT_RUNTIME_SCRATCH
// blend_word in place with a 12-byte frame for the refresh stack budget.
void blend_into(uint32_t *word, uint32_t mask, uint32_t pixels, uint8_t variant);
__asm__(
    ".text\n"
    ".syntax unified\n"
    ".balign 2\n"
    ".thumb_func\n"
    ".type blend_into, %function\n"
    "blend_into:\n"
    "push {r4, r5, r6}\n"
    "mov ip, r0\n"
    "ldr r4, [r0]\n"
    "ldr r5, 1f\n"
    "muls r3, r5\n"
    "eors r3, r4\n"
    "ldr r5, 2f\n"
    "ands r3, r5\n"
    "lsrs r5, r3, #1\n"
    "orrs r5, r3\n"
    "lsrs r6, r3, #2\n"
    "orrs r5, r6\n"
    "lsrs r6, r3, #3\n"
    "orrs r5, r6\n"
    "ldr r6, 3f\n"
    "ands r5, r6\n"
    "ldr r3, 4f\n"
    "movs r0, r2\n"
    "ands r0, r3\n"
    "ldr r6, 1f\n"
    "orrs r0, r6\n"
    "ands r3, r4\n"
    "subs r0, r0, r3\n"
    "mvns r0, r0\n"
    "ands r0, r6\n"
    "lsrs r0, r0, #2\n"
    "bics r0, r5\n"
    "ldr r6, 3f\n"
    "ands r0, r6\n"
    "lsls r3, r0, #1\n"
    "orrs r0, r3\n"
    "lsls r3, r0, #2\n"
    "orrs r0, r3\n"
    "ands r0, r1\n"
    "mvns r3, r1\n"
    "orrs r3, r0\n"
    "ands r4, r3\n"
    "bics r2, r0\n"
    "orrs r4, r2\n"
    "mov r0, ip\n"
    "str r4, [r0]\n"
    "pop {r4, r5, r6}\n"
    "bx lr\n"
    ".balign 4\n"
    "1: .word 0x44444444\n"
    "2: .word 0xcccccccc\n"
    "3: .word 0x11111111\n"
    "4: .word 0x33333333\n"
    ".syntax divided\n");
#else
static TEXT_NOIPA TEXT_O2 void blend_into(uint32_t *word, uint32_t mask,
                                          uint32_t pixels, uint8_t variant) {
    *word = blend_word(*word, mask, pixels, variant);
}
#endif

typedef struct {
    const uint8_t *glyph;
    uint32_t *left;
    uint32_t *right;
    uint32_t mask;
    uint32_t background;
    int offset;
    int crosses;
    int first_y;
    int height;
    int stride;
} GlyphWorkspace;

#ifdef TEXT_RUNTIME_SCRATCH
#define GLYPH_WORKSPACE ((GlyphWorkspace *)(uintptr_t)(TEXT_RUNTIME_SCRATCH + 0x100))
typedef char GlyphWorkspaceFits[(sizeof(GlyphWorkspace) <= 0x90) ? 1 : -1];
#endif

TEXT_NOIPA TEXT_O2 TextStatus text_compose_glyph_tiles_blended(
        const TextFont *font, const TextPlacement *placement,
        TextTileSurface *surface, uint8_t variant) {
    if (placement->metric >= font->metric_count)
        return TEXT_MALFORMED_INPUT;
    const TextMetric *metric = &font->metrics[placement->metric];
    int width = metric->width;
    if (width > 8)
        return TEXT_MALFORMED_INPUT;
    int height = metric->height;
    int stride = font->glyph_stride;
    uint32_t mask = glyph_pixel_mask(metric, placement);
    if (!mask)
        return TEXT_OK;
    const uint8_t *glyph = font->glyphs +
                           (size_t)placement->metric * stride * 16;
    int ink_x = placement->ink_x;
    int first_y = placement->ink_y;
    int offset = ink_x & 7;
    int shift = offset * 4;
    int crosses = offset + width > 8;
    uint32_t *left;
    uint32_t *right = 0;
    if (!crosses && height && first_y >= 0 &&
            (first_y >> 3) == ((first_y + height - 1) >> 3)) {
        left = surface->get(surface->context, ink_x, first_y);
        uint32_t fill = (uint32_t)(variant * 4) * 0x11111111u;
        int fresh = left != 0;
        for (int y = 0; fresh && y < 8; ++y)
            fresh = left[y] == fill;
        if (fresh) {
            for (int y = 0; y < height; ++y) {
                uint32_t packed = *(const uint16_t *)(glyph + y * stride);
                left[(first_y + y) & 7] |=
                    expand_glyph_row(packed & mask) << shift;
            }
            return TEXT_OK;
        }
    }
    left = 0;
    const uint8_t *row = glyph;
    int end_y = first_y + height;
    for (int destination_y = first_y; destination_y < end_y;
            ++destination_y, row += stride) {
        if (destination_y == first_y || !(destination_y & 7)) {
            left = surface->get(surface->context, ink_x, destination_y);
            right = crosses
                ? surface->get(surface->context, ink_x + 8 - offset,
                               destination_y) : 0;
        }
        uint32_t packed = *(const uint16_t *)row & mask;
        if (!packed)
            continue;
        uint32_t pixels = expand_glyph_row(packed);
        uint32_t present = (pixels | (pixels >> 1)) & 0x11111111u;
        uint32_t ink = present * 15u;
        pixels |= present * (variant * 4u);
        uint32_t side_mask = ink << shift;
        if (left && side_mask) {
            uint32_t *word = &left[destination_y & 7];
            uint32_t old = *word;
            if (old & side_mask & 0x33333333u)
                blend_into(word, side_mask, pixels << shift, variant);
            else
                *word = (old & ~side_mask) | (pixels << shift);
        }
        if (right) {
            side_mask = ink >> (32 - shift);
            if (side_mask) {
                uint32_t *word = &right[destination_y & 7];
                uint32_t old = *word;
                if (old & side_mask & 0x33333333u)
                    blend_into(word, side_mask, pixels >> (32 - shift), variant);
                else
                    *word = (old & ~side_mask) | (pixels >> (32 - shift));
            }
        }
    }
    return TEXT_OK;
}

static TEXT_NOIPA TextStatus compose_fresh_glyph_tile(
        const TextFont *font, const TextPlacement *placement,
        TextTileSurface *surface) {
    const TextMetric *metric = &font->metrics[placement->metric];
    uint32_t glyph_mask = glyph_pixel_mask(metric, placement);
    const uint8_t *glyph = font->glyphs +
        (size_t)placement->metric * font->glyph_stride * 16;
    int offset = placement->ink_x & 7;
    uint32_t *tile = surface->context;
    if (!tile)
        return TEXT_OK;
    for (int y = 0; y < metric->height; ++y) {
        uint32_t packed = *(const uint16_t *)(
            glyph + y * font->glyph_stride);
        packed &= glyph_mask;
        tile[(placement->ink_y + y) & 7] |=
            expand_glyph_row(packed) << (offset * 4);
    }
    return TEXT_OK;
}

TextStatus text_compose_glyph_tiles(
        const TextFont *font, const TextPlacement *placement,
        TextTileSurface *surface, uint8_t variant) {
    if (!surface->get && placement->metric < font->metric_count) {
        return compose_fresh_glyph_tile(font, placement, surface);
    }
    return text_compose_glyph_tiles_blended(
        font, placement, surface, variant);
}

static __attribute__((always_inline)) inline uint32_t blend_uniform_word(
        uint32_t old, uint32_t mask, uint32_t pixels) {
    uint32_t old_ink = old & 0x33333333u;
    uint32_t new_ink = pixels & 0x33333333u;
    uint32_t greater = ~((new_ink | 0x44444444u) - old_ink) & 0x44444444u;
    uint32_t keep = (greater >> 2) & 0x11111111u;
    keep |= keep << 1;
    keep |= keep << 2;
    keep &= mask;
    return (old & (~mask | keep)) | (pixels & ~keep);
}

TEXT_NOIPA TEXT_O2 TextStatus text_compose_glyph_tiles_uniform(
        const TextFont *font, const TextPlacement *placement,
        TextTileSurface *surface, uint8_t variant) {
    if (placement->metric >= font->metric_count)
        return TEXT_MALFORMED_INPUT;
    const TextMetric *metric = &font->metrics[placement->metric];
    if (metric->width > 8)
        return TEXT_MALFORMED_INPUT;
    uint32_t glyph_mask = glyph_pixel_mask(metric, placement);
    const uint8_t *glyph = font->glyphs +
                           (size_t)placement->metric * font->glyph_stride * 16;
    int offset = placement->ink_x & 7;
    int crosses = offset + metric->width > 8;
    uint32_t *left_tile = 0;
    uint32_t *right_tile = 0;
    for (int y = 0; y < metric->height; ++y) {
        int destination_y = placement->ink_y + y;
        if (!y || !(destination_y & 7)) {
            left_tile = surface->get(surface->context,
                placement->ink_x, destination_y);
            right_tile = crosses
                ? surface->get(surface->context,
                    placement->ink_x + 8 - offset, destination_y) : 0;
        }
        uint32_t packed = *(const uint16_t *)(glyph + y * font->glyph_stride);
        packed &= glyph_mask;
        uint32_t pixels = expand_glyph_row(packed);
        uint32_t present = (pixels | (pixels >> 1)) & 0x11111111u;
        uint32_t mask = present * 15u;
        pixels |= present * (variant * 4u);
        int shift = offset * 4;
        uint32_t left_mask = mask << shift;
        if (left_tile && left_mask) {
            uint32_t *word = &left_tile[destination_y & 7];
            uint32_t shifted = pixels << shift;
            *word = (*word & left_mask & 0x33333333u)
                ? blend_uniform_word(*word, left_mask, shifted)
                : *word | shifted;
        }
        if (right_tile) {
            shift = (8 - offset) * 4;
            uint32_t right_mask = mask >> shift;
            if (right_mask) {
                uint32_t *word = &right_tile[destination_y & 7];
                uint32_t shifted = pixels >> shift;
                *word = (*word & right_mask & 0x33333333u)
                    ? blend_uniform_word(*word, right_mask, shifted)
                    : *word | shifted;
            }
        }
    }
    return TEXT_OK;
}

static TEXT_NOIPA TextStatus compose_pixels(
        const TextFont *font, const TextPlacement *placement,
        TextSurface *surface, uint8_t variant) {
    const TextMetric *metric = &font->metrics[placement->metric];
    const uint8_t *glyph = font->glyphs +
                           (size_t)placement->metric * font->glyph_stride * 16;
    for (int y = 0; y < metric->height; ++y) {
        int destination_y = placement->ink_y + y;
        if (destination_y < 0 || destination_y >= surface->height)
            continue;
        for (int x = placement->clip_left; x < metric->width - placement->clip_right; ++x) {
            int destination_x = placement->ink_x + x;
            if (destination_x < 0 || destination_x >= surface->width)
                continue;
            uint8_t packed = glyph[y * font->glyph_stride + (x >> 2)];
            uint8_t ink = (uint8_t)((packed >> ((x & 3) * 2)) & 3);
            if (!ink)
                continue;
            uint8_t value = (uint8_t)(variant * 4 + ink);
            uint8_t old = surface->access.pixels.get(
                surface->context, destination_x, destination_y);
            if ((old >> 2) == variant && (old & 3) > ink)
                value = old;
            surface->access.pixels.set(surface->context, destination_x,
                                       destination_y, value);
        }
    }
    return TEXT_OK;
}

static TEXT_NOIPA TextStatus compose_clipped_tiles(
        const TextFont *font, const TextPlacement *placement,
        TextSurface *surface, uint8_t variant) {
    const TextMetric *metric = &font->metrics[placement->metric];
    const uint8_t *glyph = font->glyphs +
                           (size_t)placement->metric * font->glyph_stride * 16;
    for (int y = 0; y < metric->height; ++y) {
        int destination_y = placement->ink_y + y;
        if (destination_y < 0 || destination_y >= surface->height)
            continue;
        for (int x = placement->clip_left; x < metric->width - placement->clip_right; ++x) {
            int destination_x = placement->ink_x + x;
            if (destination_x < 0 || destination_x >= surface->width)
                continue;
            uint8_t packed = glyph[y * font->glyph_stride + (x >> 2)];
            uint8_t ink = (uint8_t)((packed >> ((x & 3) * 2)) & 3);
            if (!ink)
                continue;
            uint32_t *tile = surface->access.tiles.get(
                surface->context, destination_x, destination_y);
            if (!tile)
                continue;
            uint32_t *word = &tile[destination_y & 7];
            int shift = (destination_x & 7) * 4;
            uint8_t old = (uint8_t)((*word >> shift) & 15);
            uint8_t value = (uint8_t)(variant * 4 + ink);
            if ((old >> 2) == variant && (old & 3) > ink)
                value = old;
            *word = (*word & ~(15u << shift)) | ((uint32_t)value << shift);
        }
    }
    return TEXT_OK;
}

TextStatus text_compose_glyph(const TextFont *font,
                              const TextPlacement *placement,
                              TextSurface *surface, uint8_t variant) {
    if (placement->metric >= font->metric_count)
        return TEXT_MALFORMED_INPUT;
    const TextMetric *metric = &font->metrics[placement->metric];
    if (surface->access.tiles.marker || !surface->access.tiles.get)
        return compose_pixels(font, placement, surface, variant);
    if (metric->width > 8)
        return TEXT_MALFORMED_INPUT;
    if (placement->ink_x < 0 || placement->ink_y < 0 ||
            placement->ink_x + metric->width > surface->width ||
            placement->ink_y + metric->height > surface->height)
        return compose_clipped_tiles(font, placement, surface, variant);
    TextTileSurface tiles = {surface->context, surface->access.tiles.get};
    return text_compose_glyph_tiles(font, placement, &tiles, variant);
}

void text_fill(TextSurface *surface, int left, int top,
               int right, int bottom, uint8_t value) {
    if (left < 0)
        left = 0;
    if (top < 0)
        top = 0;
    if (right > surface->width)
        right = surface->width;
    if (bottom > surface->height)
        bottom = surface->height;
    for (int y = top; y < bottom; ++y)
        for (int x = left; x < right; ++x)
            surface->access.pixels.set(surface->context, x, y, value);
}

#ifndef TEXT_RUNTIME_SCRATCH
typedef struct {
    const uint8_t *text;
    const uint8_t *player;
    uint16_t text_length;
    uint16_t player_length;
    int16_t left;
    int16_t right;
    int16_t bottom;
    int16_t x;
    int16_t y;
    uint8_t kind;
    uint8_t wrap;
    uint8_t overflow;
    uint8_t resume;
    uint8_t color;
    uint8_t lines;
    uint8_t missing_count;
    uint16_t missing[128];
} TextCheck;

TextStatus text_check(const TextFont *font, TextCheck *check);

TextStatus text_check(const TextFont *font, TextCheck *check) {
    TextProfile profile;
    TextStatus status = text_profile_init(&profile, (TextProfileKind)check->kind,
                                          check->right, check->bottom);
    if (status != TEXT_OK)
        return status;
    if (check->left >= 0)
        profile.left = check->left;
    profile.wrap = check->wrap;
    profile.overflow = check->overflow;
    TextDecoder decoder;
    text_decoder_init_raw(&decoder, check->text, check->text_length,
                          check->player, check->player_length, 0);
    TextLayoutState state = {check->x, check->y, -1, check->color, check->resume};
    TextLayoutCursor cursor;
    TextLayoutResult result;
    TextEvent event;
    status = text_layout_begin(&profile, &decoder, &state, &cursor, &result);
    while (status == TEXT_OK) {
        if (cursor.pending)
            status = layout_status(&decoder, &result, TEXT_MALFORMED_INPUT);
        else if (cursor.phase == LAYOUT_DONE)
            status = (TextStatus)result.status;
        else if (cursor.phase == LAYOUT_SCAN)
            status = text_layout_scan_peek(font, &profile, &decoder, &state,
                                           &cursor, &result, &event);
        else
            status = text_layout_emit_peek(font, &profile, &decoder, &state,
                                           &cursor, &result, &event);
        if (status != TEXT_OK)
            break;
        if (event.kind == TEXT_EVENT_GLYPH && event.glyph.missing) {
            uint8_t index = 0;
            while (index < check->missing_count &&
                   check->missing[index] != event.glyph.code)
                ++index;
            if (index == 128)
                return TEXT_CAPACITY_FAILURE;
            if (index == check->missing_count)
                check->missing[check->missing_count++] = event.glyph.code;
        } else if (event.kind == TEXT_EVENT_LINE || event.kind == TEXT_EVENT_PAGE)
            ++check->lines;
        status = text_layout_commit(font, &profile, &decoder, &state,
                                    &cursor, &result, &event);
    }
    check->color = state.color;
    return status;
}
#endif
