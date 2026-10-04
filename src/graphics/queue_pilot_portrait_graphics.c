#include "m2c_prelude.h"
#include "graphics_resources.h"

extern int QueueCopy() asm("func_08095208");
extern int BiosLz77ToWram() asm("func_080ECD38");
void QueuePilotPortraitGraphics(s32 pilot_id, s32 expression_id, s32 alternate_palette_index, s32 tile_offset, s32 palette_bank, void *work_buffer) asm("func_0809A9C8");

void QueuePilotPortraitGraphics(s32 pilot_id, s32 expression_id, s32 alternate_palette_index, s32 tile_offset, s32 palette_bank, void *work_buffer) {
    u16 portrait_pilot_id;
    u32 expression_resource_address;
    u8 palette_index;
    u16 destination_tile_offset;
    u16 destination_palette_bank;
    register u32 pilot_resource_offset asm("r2");
    register u8 *base asm("r0");
    u8 *portrait_resource;
    portrait_pilot_id = (u16)pilot_id;
    expression_resource_address = expression_id << 24;
    palette_index = (u8)alternate_palette_index;
    destination_tile_offset = (u16)tile_offset;
    destination_palette_bank = (u16)palette_bank;
    pilot_resource_offset = portrait_pilot_id * 64;
    asm volatile("" : "+r"(pilot_resource_offset));
    expression_resource_address >>= 21;
    base = (u8 *)0x087ADBB8;
    asm volatile("" : "+r"(base));
    expression_resource_address += (u32)base;
    portrait_resource = (u8 *)pilot_resource_offset;
    portrait_resource += expression_resource_address;
    BiosLz77ToWram(*(void **)portrait_resource, work_buffer);
    if (portrait_pilot_id != GRAPHICS_PILOT_WITH_ALTERNATE_PORTRAIT_PALETTES) BiosLz77ToWram(*(void **)(portrait_resource + 4), work_buffer + GRAPHICS_PILOT_PORTRAIT_BYTES);
    else {
        register u8 *alternate_palettes asm("r1");
        register u32 alt_offset asm("r0");
        register void *alt_value asm("r0");
        alternate_palettes = (u8 *)0x087AF604;
        asm volatile("" : "+r"(alternate_palettes));
        alt_offset = palette_index * 4;
        asm volatile("" : "+r"(alt_offset));
        alt_offset += (u32)alternate_palettes;
        alt_value = *(void **)alt_offset;
        asm volatile("" : "+r"(alt_value));
        BiosLz77ToWram(alt_value, work_buffer + GRAPHICS_PILOT_PORTRAIT_BYTES);
    }
    QueueCopy(work_buffer, (void *)0x06010000 + destination_tile_offset * 32, GRAPHICS_PILOT_PORTRAIT_BYTES);
    QueueCopy(work_buffer + GRAPHICS_PILOT_PORTRAIT_BYTES, (void *)0x05000200 + destination_palette_bank * 32, 0x20);
}
