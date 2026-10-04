#include "m2c_prelude.h"

void BiosCpuSet(void *, void *, u32) asm("func_80ECD2C");

void CopyBgTilemapWithBaseAdd(s32 destination_address, s32 source_address, s32 destination_x_bits, s32 destination_y_bits,
                  s32 source_width_bits, s32 source_rows_bits, s32 tile_base_bits) asm("func_08092B8C");

void CopyBgTilemapWithBaseAdd(s32 destination_address, s32 source_address, s32 destination_x_bits, s32 destination_y_bits,
                  s32 source_width_bits, s32 source_rows_bits, s32 tile_base_bits)
{
    u16 *destination = (u16 *)destination_address;
    register u16 *source asm("r10") = (u16 *)source_address;
    register s32 source_width_argument asm("r0") = source_width_bits;
    register s32 source_rows_argument asm("r1") = source_rows_bits;
    register s32 tile_base_argument asm("r4") = tile_base_bits;
    register u32 destination_x asm("r8");
    register u32 row_index_or_destination_y asm("r5");
    register u32 source_width asm("r3");
    volatile u32 row_count;
    volatile u32 tile_base_add;
    volatile u32 source_stride;
    register u32 copy_width asm("r2");
    register u32 width asm("r6");

    destination_x = (u16)destination_x_bits;
    row_index_or_destination_y = (u16)destination_y_bits;
    source_width = (u16)source_width_argument;
    row_count = (u16)source_rows_argument;
    tile_base_add = (u16)tile_base_argument;
    copy_width = source_width;
    if (source_width > 32) {
        copy_width = 32;
    }
    width = copy_width;
    {
        register u32 x_view asm("r1") = destination_x;
        register s32 signed_x asm("r0");
        register s32 signed_y asm("r1");

        asm volatile("" : "+r"(x_view));
        signed_x = (s16)x_view;
        signed_y = (s16)row_index_or_destination_y;
        signed_y <<= 5;
        signed_x += signed_y;
        destination += signed_x;
    }

    row_index_or_destination_y = 0;
    {
        register u32 initial_rows asm("r2") = row_count;
        asm volatile("" : "+r"(initial_rows));
        if (row_index_or_destination_y >= initial_rows) {
            goto done;
        }
    }
    {
        source_stride = source_width << 1;
        do {
            u16 column;
            register u16 *next_destination asm("r12");

            BiosCpuSet(source, destination, width);
            column = 0;
            next_destination = destination + 32;
            row_index_or_destination_y++;
            if (column < width) {
                register u32 flip_bits_mask asm("r9");
                register u32 tile_palette_bits_mask asm("r8");
                register u32 flip_mask_carrier asm("r1") = 0x0C00;
                register u32 tile_palette_mask_carrier asm("r2");

                asm volatile("" : "+r"(flip_mask_carrier));
                flip_bits_mask = flip_mask_carrier;
                tile_palette_mask_carrier = 0xF3FF;
                asm volatile("" : "+r"(tile_palette_mask_carrier));
                tile_palette_bits_mask = tile_palette_mask_carrier;

                do {
                    register u16 *cell asm("r3") =
                        (u16 *)((u32)column << 1);
                    register u32 tilemap_entry asm("r2");
                    register u32 flip_bits asm("r1");
                    register u32 tile_palette_bits asm("r0");
                    register u32 tile_base asm("r2");

                    asm volatile("add %0, %0, %1"
                                 : "+r"(cell)
                                 : "r"(destination));
                    tilemap_entry = *cell;
                    flip_bits = flip_bits_mask;
                    flip_bits &= tilemap_entry;
                    tile_palette_bits = tile_palette_bits_mask;
                    tile_palette_bits &= tilemap_entry;
                    tile_base = tile_base_add;
                    tile_palette_bits = tile_base + tile_palette_bits;
                    flip_bits |= tile_palette_bits;
                    *cell = flip_bits;
                    column++;
                } while (column < width);
            }
            source = (u16 *)((u8 *)source + source_stride);
            destination = next_destination;
            {
                register u32 wrapped asm("r0") = row_index_or_destination_y << 16;
                register u32 tail_rows asm("r1");
                asm volatile("" : "+r"(wrapped));
                row_index_or_destination_y = wrapped >> 16;
                tail_rows = row_count;
                asm volatile("" : "+r"(tail_rows));
                if (row_index_or_destination_y < tail_rows) {
                    goto next_row;
                }
            }
            goto done;
next_row:
            ;
        } while (1);
    }
done:
    ;
}
