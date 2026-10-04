#include "m2c_prelude.h"

u32 FindFreeWindowTiles(u32 requested_count, u16 *output_tiles)
{
    register u32 found_count asm("r10");
    register u32 word_index asm("r6");
    register u16 *output_cursor asm("r4");
    register u32 *usage_masks asm("r9");
    register u32 *tile_limit asm("r8");
    register u32 *tile_limit_view asm("r7");
    register u32 *usage_source;
    register u32 *limit_source asm("r1");
    register u32 zero asm("r1");
    register u32 request_value asm("r2");
    register u32 request asm("r12");

    request_value = requested_count;
    output_cursor = output_tiles;
    asm volatile("" : "+r"(request_value));
    request_value <<= 24;
    request_value >>= 24;
    request = request_value;
    zero = 0;
    found_count = zero;
    word_index = 0;
    usage_source = (u32 *)0x0200DD90;
    usage_masks = usage_source;
    asm volatile("" : "+r"(usage_source), "+r"(zero) : : "r0", "r2", "r3", "r5", "r8");
    limit_source = (u32 *)0x02021658;
    asm volatile("" : "+r"(limit_source) : : "r7");
    tile_limit = limit_source;
    asm volatile("" : "+r"(tile_limit));
    do {
        register u32 bit_index asm("r5");
        register u32 bits asm("r3");
        register u32 mask_offset asm("r1");

        mask_offset = word_index << 2;
        bits = *(u32 *)((u32)usage_masks + mask_offset);
        bit_index = 0;
        do {
            register u32 available asm("r1") = 1;
            register u32 emitted asm("r1");
            register u32 observed asm("r2");
            register u32 maximum asm("r1");

            if ((available & bits) == 0) {
                emitted = ((u32)word_index << 5) + bit_index;
                *output_cursor = emitted;
                observed = *output_cursor;
                tile_limit_view = tile_limit;
                maximum = *tile_limit_view;

                if (observed < maximum) {
                    goto success;
                }
                {
                    register u32 failed asm("r0") = 0;
                    asm volatile("" : "+r"(failed));
                    return failed;
                }
success:
                { register u32 next asm("r1") = found_count + 1; next <<= 24; found_count = next >> 24; }
                if (found_count == request) return 1;
                output_cursor++;
            }
            bits >>= 1;
            { register u32 next asm("r1") = bit_index + 1; next <<= 24; bit_index = next >> 24; }
        } while (bit_index <= 31);
        { register u32 next asm("r1") = word_index + 1; next <<= 24; word_index = next >> 24; }
    } while (word_index <= 31);
    {
        /* The native full-pool path returns the value left in r0. */
        register u32 terminal asm("r0");
        return terminal;
    }
}
