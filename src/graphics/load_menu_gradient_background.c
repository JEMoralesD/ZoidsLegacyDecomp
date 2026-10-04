#include "m2c_prelude.h"
#include "graphics_resources.h"

M2C_UNK LoadCompressedPalette(M2C_UNK, s32, M2C_UNK) asm("func_0809A1BC");
M2C_UNK BiosLz77ToVram(M2C_UNK, u16 *) asm("func_080ECD34");

void LoadMenuGradientBackground(s32 bg_index, s32 char_block, s32 tile_offset, s32 palette_bank, s32 screen_block) asm("func_0809AB44");

void LoadMenuGradientBackground(s32 bg_index, s32 char_block, s32 tile_offset, s32 palette_bank, s32 screen_block)
{
    u16 *tilemap;
    u16 tile_index_offset;
    u16 row;
    u16 column;
    u8 background;
    register u32 bg_vram_base asm("r8");

    bg_index <<= 24;
    background = (u32)bg_index >> 24;
    char_block <<= 16;
    char_block = (u32)char_block >> 16;
    tile_offset <<= 16;
    tile_offset = (u32)tile_offset >> 16;
    tile_index_offset = tile_offset;
    asm volatile("" : "+r"(tile_offset), "+r"(tile_index_offset));
    palette_bank <<= 16;
    palette_bank = (u32)palette_bank >> 16;
    screen_block <<= 16;
    screen_block = (u32)screen_block >> 16;
    asm volatile("" : : "r"(palette_bank), "r"(palette_bank), "r"(palette_bank), "r"(palette_bank),
                          "r"(palette_bank), "r"(palette_bank), "r"(palette_bank), "r"(palette_bank));
    asm volatile("" : : "r"(palette_bank), "r"(palette_bank), "r"(palette_bank), "r"(palette_bank),
                          "r"(palette_bank));
    asm volatile("" : : "r"(screen_block), "r"(screen_block), "r"(screen_block), "r"(screen_block),
                          "r"(screen_block), "r"(screen_block), "r"(screen_block), "r"(screen_block));
    asm volatile("" : : "r"(screen_block), "r"(screen_block), "r"(screen_block), "r"(screen_block),
                          "r"(screen_block), "r"(screen_block), "r"(screen_block), "r"(screen_block));
    asm volatile("" : : "r"(screen_block));
    asm volatile("" : : "r"(palette_bank), "r"(palette_bank), "r"(palette_bank), "r"(palette_bank),
                          "r"(palette_bank), "r"(palette_bank), "r"(palette_bank), "r"(palette_bank));
    asm volatile("" : : "r"(char_block), "r"(char_block), "r"(char_block), "r"(char_block));
    asm volatile("" : : "r"(char_block), "r"(char_block), "r"(char_block), "r"(char_block),
                          "r"(char_block), "r"(char_block), "r"(char_block), "r"(char_block));
    asm volatile("" : : "r"(char_block), "r"(char_block), "r"(char_block), "r"(char_block),
                          "r"(char_block), "r"(char_block), "r"(char_block), "r"(char_block));
    {
        register u32 call0 asm("r0") = 0x080F1548;
        register u32 call1 asm("r1") = char_block << 14;
        register u32 offset asm("r2") = tile_offset << 5;
        register u32 base asm("r3") = 0xC0;

        base <<= 19;
        bg_vram_base = base;
        offset += bg_vram_base;
        call1 += offset;
        BiosLz77ToVram(call0, call1);
    }
    LoadCompressedPalette(0x080F1640, (palette_bank << 5) + 0x05000000, 0x02002880);
    tilemap = (screen_block << 11) + bg_vram_base;
    BiosLz77ToVram(0x080F1668, tilemap);
    {
        register volatile u16 *flags asm("ip");
        register u32 index2 asm("r1") = background;
        register u32 bg_offset asm("r8");
        register u32 index8_input asm("r3");
        register u32 state_offset asm("r2");

        row = 0;
        palette_bank <<= 12;
        flags = (u16 *)0x0300004C;
        index2 <<= 1;
        bg_offset = index2;
        screen_block <<= 8;
        char_block <<= 2;
        index8_input = background;
        state_offset = index8_input << 3;
        do {
            column = 0;
            do {
                *tilemap = (*tilemap + tile_index_offset) | palette_bank;
                tilemap++;
                column += 1;
            } while ((u32)column <= 31);
            row += 1;
        } while ((u32)row <= 19);

        asm volatile("" : "+r"(char_block), "+r"(screen_block));
        {
            register u32 mask asm("r0") = 0x80;
            register u32 shift asm("r1") = background;
            register volatile u16 *flag_ptr asm("r3");
            register u32 current asm("r1");

            mask <<= 1;
            mask <<= shift;
            flag_ptr = flags;
            current = *flag_ptr;
            mask |= current;
            *flag_ptr = mask;
        }
        *(volatile u16 *)(0x04000008 + bg_offset) = screen_block | char_block | 3;
        {
            register u32 state_base asm("r0") = 0x03000054;

            asm volatile("" : "+r"(state_base));
            state_offset += state_base;
            asm volatile("" : "+r"(state_offset) : : "memory");
        }
        {
            register u32 second_offset asm("r0") = bg_offset;

            second_offset += 1;
            second_offset <<= 2;
            {
                register u32 state_base asm("r1") = 0x03000054;

                asm volatile("" : "+r"(state_base));
                second_offset += state_base;
            }
            {
                register s32 zero asm("r1") = 0;

                *(s32 *)second_offset = zero;
                *(s32 *)state_offset = zero;
            }
        }
    }
}
