#include "m2c_prelude.h"
#include "window.h"


extern u16 D_04000008[];
extern u16 gWindowBgPaletteAttribute asm("D_02021668");
extern s32 gWindowTextTilesVram asm("D_02021654");
extern s32 gWindowTextTileCount asm("D_02021658");
extern s32 gWindowTilemapVram asm("D_0202165C");
extern s32 gWindowFrameTilesVram asm("D_02021660");
extern s32 gWindowFrameTileOffset asm("D_02021664");
extern u16 gWindowCursorTileOffset asm("D_0202166A");
extern u16 gWindowCursorPaletteBank asm("D_0202166C");
extern s32 gWindowTextTileOffset asm("D_02021670");
extern u8 gWindowTilemapRefreshPending asm("D_02021674");
extern u8 gSaveBuffer[];
extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");

void LoadCompressedPalette(s32, s32, void *) asm("func_0809A1BC");
void BiosCpuFastSet(void *, void *, s32) asm("func_080ECD28");
void BiosCpuSet(void *, void *, s32) asm("func_080ECD2C");
void BiosLz77ToVram() asm("func_080ECD34");
void ResetWindowTextLists(void) asm("func_08098804");

void InitializeWindowGraphics(s32 bg_index, s32 char_block, s32 text_tile_offset, s32 text_tile_count, s32 frame_tile_offset,
    s32 screen_block, s32 bg_palette_bank, s32 bg_control_flags, s32 cursor_tile_offset, s32 cursor_palette_bank) asm("func_08096FBC");

void InitializeWindowGraphics(s32 bg_index, s32 char_block, s32 text_tile_offset, s32 text_tile_count, s32 frame_tile_offset,
    s32 screen_block, s32 bg_palette_bank, s32 bg_control_flags, s32 cursor_tile_offset, s32 cursor_palette_bank)
{
    register u32 display asm("r0") = bg_index;
    s32 frame_tile_base = frame_tile_offset;
    register u32 destination_screen_block asm("r8") = screen_block;
    register u32 palette_bank asm("r9") = bg_palette_bank;
    register u32 control_flags asm("r6") = bg_control_flags;
    register u32 cursor_tile_base asm("sl") = cursor_tile_offset;
    register u32 cursor_palette asm("ip") = cursor_palette_bank;
    struct {
        u16 dma_value;
        u8 pad02[2];
        s32 zero_a;
        s32 zero_b;
        volatile s32 display;
    } locals;
    s32 i;
    s32 address;
    struct WindowPoolRecord *window_record;
    register s32 display_offset asm("r5");
    register u16 *display_base asm("r4");
    register s32 *destination asm("r0");
    register s32 *destination60 asm("r2");
    register u16 *destination68 asm("r2");
    register u32 value68 asm("r3");
    register u32 shifted68 asm("r0");
    register void *resource1 asm("r0");
    register u32 value6c asm("r2");
    register u32 field6_shift asm("r3");
    register void *transfer_buffer asm("r5");
    register void *resource2 asm("r0");
    register u32 transfer2 asm("r1");
    register u32 transfer2_base asm("r4");
    register void *resource3 asm("r0");
    register u32 transfer3 asm("r1");
    register u32 transfer3_base asm("r2");
    register void *zero_b_tmp asm("r3");
    register s32 loop_zero asm("r3");
    register s32 *state_base asm("r2");
    register s32 end_display asm("r4");
    register s32 *even_state asm("r1");
    register s32 odd_address asm("r0");
    void *zero_a_ptr;
    void *zero_b_ptr;
    register s32 zero asm("r5");

    asm volatile("" : "+r"(display));
    display <<= 24;
    display >>= 24;
    locals.display = display;
    char_block <<= 24;
    char_block = (u32)char_block >> 24;
    destination_screen_block <<= 24;
    destination_screen_block >>= 24;
    palette_bank <<= 24;
    palette_bank >>= 24;
    control_flags <<= 16;
    control_flags >>= 16;
    display = locals.display;
    display_offset = display << 1;
    display_base = D_04000008;
    display_offset += (s32)display_base;
    asm volatile("" : "+r"(display_offset));

    {
        u32 control = destination_screen_block << 8;

        control |= char_block << 2;
        control_flags |= control;
        *(u16 *)display_offset = control_flags;
    }
    gWindowTextTileOffset = text_tile_offset;
    gWindowTextTileCount = text_tile_count;
    destination = &gWindowTextTilesVram;
    asm volatile("" : "+r"(destination));
    char_block <<= 14;
    text_tile_offset <<= 5;
    text_tile_offset += 0x06000000;
    asm volatile("add %0, %1, %0" : "+r"(text_tile_offset) : "r"(char_block));
    *destination = text_tile_offset;
    destination = &gWindowTilemapVram;
    asm volatile("" : "+r"(destination), "+r"(destination_screen_block));
    destination_screen_block <<= 11;
    destination_screen_block += 0x06000000;
    *destination = destination_screen_block;
    destination60 = &gWindowFrameTilesVram;
    asm volatile("" : "+r"(destination60));
    address = (frame_tile_base << 5) + 0x06000000;
    char_block += address;
    *destination60 = char_block;
    gWindowFrameTileOffset = frame_tile_base;
    destination68 = &gWindowBgPaletteAttribute;
    value68 = palette_bank;
    asm volatile("" : "+r"(destination68), "+r"(value68));
    shifted68 = value68 << 12;
    *destination68 = shifted68;
    gWindowCursorTileOffset = cursor_tile_base;
    gWindowCursorPaletteBank = (value6c = cursor_palette);

    BiosLz77ToVram(0x080ED21C);
    resource1 = (void *)0x080ED368;
    asm volatile("" : "+r"(resource1));
    field6_shift = palette_bank;
    asm volatile("" : "+r"(field6_shift));
    field6_shift <<= 5;
    palette_bank = field6_shift;
    asm volatile("" : "+r"(palette_bank));
    palette_bank += 0x05000000;
    asm volatile("" : "+r"(palette_bank));
    transfer_buffer = gSaveBuffer;
    LoadCompressedPalette((s32)resource1, palette_bank, transfer_buffer);
    resource2 = (void *)0x080ED3A4;
    asm volatile("" : "+r"(resource2));
    transfer2 = gWindowCursorTileOffset;
    transfer2 <<= 5;
    transfer2_base = 0x06010000;
    asm volatile("" : "+r"(transfer2), "+r"(transfer2_base));
    transfer2 += transfer2_base;
    BiosLz77ToVram((s32)resource2, transfer2);
    resource3 = (void *)0x080ED4D8;
    asm volatile("" : "+r"(resource3));
    transfer3 = gWindowCursorPaletteBank;
    transfer3 <<= 5;
    transfer3_base = 0x05000200;
    asm volatile("" : "+r"(transfer3), "+r"(transfer3_base));
    transfer3 += transfer3_base;
    LoadCompressedPalette((s32)resource3, transfer3, transfer_buffer);

    i = 0;
    zero_a_ptr = &locals.zero_a;
    zero_b_tmp = &locals.zero_b;
    asm volatile("" : "+r"(zero_b_tmp));
    zero_b_ptr = zero_b_tmp;
    loop_zero = 0;
    asm volatile("" : "+r"(loop_zero));
    window_record = (struct WindowPoolRecord *)0x0200A8A0;
    do {
        window_record->window.flags = loop_zero;
        window_record->window.slot = i;
        window_record++;
        i++;
    } while ((u32)i < WINDOW_COUNT);

    zero = 0;
    locals.dma_value = frame_tile_base | gWindowBgPaletteAttribute;
    BiosCpuSet(&locals.dma_value, (void *)gWindowTilemapVram, 0x01000400);
    locals.zero_a = zero;
    BiosCpuFastSet(zero_a_ptr, (void *)0x0200DD90, 0x01000020);
    locals.zero_b = zero;
    BiosCpuFastSet(zero_b_ptr, (void *)0x0200DE10, 0x01000020);

    state_base = gFieldCameraScrollOffsets;
    end_display = locals.display;
    asm volatile("" : "+r"(state_base), "+r"(end_display));
    even_state = (s32 *)((end_display << 3) + (s32)state_base);
    asm volatile("" : "+r"(even_state));
    odd_address = end_display << 1;
    asm volatile("" : "+r"(odd_address));
    odd_address += 1;
    odd_address <<= 2;
    odd_address += (s32)state_base;
    asm volatile("" : "+r"(odd_address));
    *(s32 *)odd_address = zero;
    *even_state = zero;
    ResetWindowTextLists();
    gWindowTilemapRefreshPending = zero;
}
