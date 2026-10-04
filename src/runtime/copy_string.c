#include "m2c_prelude.h"

u8 *CopyString(u8 *destination_base, const u8 *source_base)
{
    u8 *destination_cursor = destination_base;
    const u8 *source_cursor = source_base;

    {
        u32 alignment = (u32)source_cursor;
        alignment |= (u32)destination_base;
        if (!(alignment & 3)) {
            u32 *destination_words = (u32 *)destination_cursor;
            const u32 *source_words = (const u32 *)source_cursor;

            while (((*source_words + 0xFEFEFEFF) & ~*source_words & 0x80808080) == 0)
                *destination_words++ = *source_words++;
            destination_cursor = (u8 *)destination_words;
            source_cursor = (const u8 *)source_words;
        }
    }

    while ((*destination_cursor++ = *source_cursor++) != 0)
        ;
    return destination_base;
}
