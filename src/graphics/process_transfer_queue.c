#include "m2c_prelude.h"

struct TilemapCopyRequest {
    u32 flags;
    u8 *source;
    u8 *destination;
    u16 width_halfwords;
    u16 row_count;
};

void BiosCpuSet(void *, void *, u32) asm("func_080ECD2C");

void ProcessTransferQueue(void) {
    register u32 request_index asm("r5");
    register s32 request_offset asm("r6");
    u32 next_request_index;
    register struct TilemapCopyRequest *queue asm("r3");

    request_index = 0;
    queue = (struct TilemapCopyRequest *)0x03005DE8;
    asm volatile("" : "+r"(queue));
    do {
        register s32 request_byte_offset asm("r1");
        register struct TilemapCopyRequest *transfer asm("r2");
        register u32 flags asm("r4");
        register s32 active asm("r0");

        request_byte_offset = request_index << 4;
        transfer = (struct TilemapCopyRequest *)(request_byte_offset + (s32)queue);
        flags = transfer->flags;
        active = 1;
        active &= flags;
        request_offset = request_byte_offset;
        next_request_index = request_index + 1;
        if (active != 0) {
            switch (flags & 2) {
            case 0: {
                register void *source asm("r0");
                register void *destination asm("r1");
                register u32 copy_halfword_count asm("r2");

                source = transfer->source;
                asm volatile("" : "+r"(source));
                destination = transfer->destination;
                asm volatile("" : "+r"(destination));
                copy_halfword_count = *(u32 *)&transfer->width_halfwords;
                copy_halfword_count <<= 10;
                copy_halfword_count >>= 11;
                BiosCpuSet(source, destination, copy_halfword_count);
                break;
            }
            case 2: {
                register struct TilemapCopyRequest *row_request asm("r4");
                register u32 row_index asm("r5");

                row_request = transfer;
                row_index = 0;
                goto inner_test;
inner_loop:
                    {
                        register u32 width_halfwords asm("r2");
                        register u32 source_offset asm("r1");
                        register u8 *source asm("r0");
                        register u32 destination_offset asm("r3");
                        register u8 *destination asm("r1");

                        width_halfwords = row_request->width_halfwords;
                        source_offset = row_index;
                        source_offset *= width_halfwords;
                        source_offset <<= 1;
                        source = row_request->source;
                        source = (u8 *)((s32)source + source_offset);
                        asm volatile("" : "+r"(source));
                        destination_offset = row_index << 6;
                        destination = row_request->destination;
                        destination = (u8 *)((s32)destination + destination_offset);
                        BiosCpuSet(source, destination, width_halfwords);
                    }
                    {
                        register u32 next_row_index asm("r0");

                        next_row_index = row_index + 1;
                        next_row_index <<= 24;
                        row_index = next_row_index >> 24;
                    }
inner_test:
                if (row_index < row_request->row_count) {
                    goto inner_loop;
                }
                break;
            }
            }
            {
                register struct TilemapCopyRequest *completed_request asm("r2");
                register s32 clear_flags asm("r0");
                register s32 clear_mask asm("r1");

                queue = (struct TilemapCopyRequest *)0x03005DE8;
                asm volatile("" : "+r"(queue));
                completed_request = (struct TilemapCopyRequest *)(request_offset + (s32)queue);
                clear_flags = completed_request->flags;
                clear_mask = -2;
                clear_flags &= clear_mask;
                completed_request->flags = clear_flags;
            }
        }
        {
            register u32 normalized_request_index asm("r0");

            normalized_request_index = next_request_index << 24;
            normalized_request_index >>= 24;
            request_index = normalized_request_index;
        }
    } while (request_index <= 15);
}
