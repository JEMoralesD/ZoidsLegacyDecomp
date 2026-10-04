#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void QueueCopy(s32, s32, s32) asm("func_08095208");
void PlayBattleAnimationSound(s32) asm("func_080D2790");
void BiosCpuFastSet(s32, s32, s32) asm("func_080ECD28");
void BiosCpuSet(void *, s32, s32) asm("func_080ECD2C");

void InitializeBattleCyanParticleWhiteFlashEffect(u8 *group_bytes) asm("func_080DE918");

void InitializeBattleCyanParticleWhiteFlashEffect(u8 *group_bytes)
{
    u16 white_colors[2];
    register s32 background_palette_address asm("r9");
    register s32 sprite_palette_address asm("r8");
    register s32 background_palette_buffer asm("r5");
    register s32 sprite_palette_buffer asm("r4");
    register u32 white_color asm("r6");

    *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state)) = 0;

    background_palette_address = 0xA0;
    background_palette_address <<= 19;
    background_palette_buffer = BATTLE_WHITE_FLASH_PALETTE_ADDRESS(original_background_colors);
    BiosCpuFastSet(background_palette_address, background_palette_buffer, 0x20);

    {
        register s32 sprite_palette_source asm("r1") = 0x05000200;
        asm volatile("" : "+r"(sprite_palette_source));
        sprite_palette_address = sprite_palette_source;
        asm volatile("" : : "r"(sprite_palette_address));
    }
    sprite_palette_buffer = BATTLE_WHITE_FLASH_PALETTE_ADDRESS(original_sprite_colors);
    BiosCpuFastSet(sprite_palette_address, sprite_palette_buffer, 0x18);

    {
        register u16 *first_color asm("r0") = &white_colors[0];
        register u32 white_color_init asm("r1") = 0x7FFF;
        asm volatile("" : "+r"(first_color));
        asm volatile("" : "+r"(white_color_init));
        white_color = white_color_init;
        asm volatile("" : : "r"(white_color));
        *first_color = white_color;
        background_palette_buffer += 0x80;
        BiosCpuSet(first_color, background_palette_buffer, 0x01000040);
    }

    {
        register u16 *second_color asm("r0") = &white_colors[1];
        asm volatile("" : "+r"(second_color));
        *second_color = white_color;
        sprite_palette_buffer += 0x60;
        BiosCpuSet(second_color, sprite_palette_buffer, 0x01000030);
    }

    QueueCopy(background_palette_buffer, background_palette_address, 0x80);
    QueueCopy(sprite_palette_buffer, sprite_palette_address, 0x60);
    PlayBattleAnimationSound(0);
}
