#include "m2c_prelude.h"

void *CopyBytes(u8 *destination_base, const u8 *source_base, u32 byte_count)
{
    u32 remaining_bytes = byte_count;
    u8 *destination_cursor = destination_base;
    const u8 *source_cursor = source_base;

    if (remaining_bytes > 15 && !(((u32)source_cursor | (u32)destination_base) & 3)) {
        u32 *destination_words = (u32 *)destination_base;

        do {
            *destination_words++ = *(const u32 *)source_cursor;
            source_cursor += 4;
            *destination_words++ = *(const u32 *)source_cursor;
            source_cursor += 4;
            *destination_words++ = *(const u32 *)source_cursor;
            source_cursor += 4;
            *destination_words++ = *(const u32 *)source_cursor;
            source_cursor += 4;
            remaining_bytes -= 16;
        } while (remaining_bytes > 15);

        while (remaining_bytes > 3) {
            *destination_words++ = *(const u32 *)source_cursor;
            source_cursor += 4;
            remaining_bytes -= 4;
        }
        destination_cursor = (u8 *)destination_words;
    }

    remaining_bytes--;
    if (remaining_bytes != (u32)-1) {
        do {
            *destination_cursor++ = *source_cursor++;
            remaining_bytes--;
        } while (remaining_bytes != (u32)-1);
    }
    return destination_base;
}
