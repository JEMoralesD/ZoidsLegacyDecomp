#include "m2c_prelude.h"
#include "battle_display.h"

extern u16 D_0300004C;
extern u16 D_0300004E;
extern u16 D_03000050;
extern u32 gFieldCameraScrollOffsets[] asm("D_03000054");
extern u8 gBattleState[];
extern u8 D_000027BE[];
extern u8 gBattleSceneSide asm("D_02033F36");
extern u16 D_087A2A1C[];
extern void *D_087A2A10[];
extern u8 D_08108228[];
extern u8 D_0600A740[];
extern u8 D_05000100[];
extern u8 D_08108878[];
extern u8 gSaveBuffer[];
extern u8 D_080CCF3D[];
extern u8 D_080CD05D[];
extern u8 D_00000442[];

void BiosLz77ToVram(void *, void *) asm("func_080ECD34");
void LoadMirroredBgTilemap(void *, u8, u8, void *) asm("func_0809AC30");
u32 *CreateSpriteGroup(s32, void *, void *) asm("func_08095098");
void PlaySong(s32) asm("func_08092E84");
void YieldTaskForUpdates(s32) asm("func_080ED17C");
void DestroySpriteGroup(void *) asm("func_08095114");

void PlayBattlePhalanxVolleyAnimation(void) asm("func_080CD110");

void PlayBattlePhalanxVolleyAnimation(void) {
    register u8 *deck_command_bytes asm("r4");
    register u8 *scene_side_address asm("r5");
    register u32 *background_scroll_offsets asm("r6");
    u32 *particle_group_words;
    register u32 elapsed_frames asm("r5");
    register u16 *vertical_scroll_table asm("r1");
    register s32 command_address_or_palette_variant asm("r0");
    register s32 deck_command_offset asm("r3");

    D_0300004C |= 0x200;
    *(u16 *)0x0400000A = 0x170B;
    background_scroll_offsets = gFieldCameraScrollOffsets;
    background_scroll_offsets[2] = 0;
    vertical_scroll_table = D_087A2A1C;
    deck_command_bytes = gBattleState;
    scene_side_address = &gBattleSceneSide;
    command_address_or_palette_variant = *scene_side_address;
    deck_command_offset = (s32)D_000027BE;
    deck_command_bytes += deck_command_offset;
    command_address_or_palette_variant += (s32)deck_command_bytes;
    command_address_or_palette_variant = *(u8 *)command_address_or_palette_variant;
    command_address_or_palette_variant -= BATTLE_PHALANX_FIRST_DECK_COMMAND;
    background_scroll_offsets[3] = vertical_scroll_table[command_address_or_palette_variant];
    BiosLz77ToVram(D_08108228, D_0600A740);
    BiosLz77ToVram(D_087A2A10[*(u8 *)(*scene_side_address + (s32)deck_command_bytes) - BATTLE_PHALANX_FIRST_DECK_COMMAND], D_05000100);
    LoadMirroredBgTilemap(D_08108878, 0x17, *scene_side_address, gSaveBuffer);
    particle_group_words = CreateSpriteGroup(0, D_080CCF3D, D_080CD05D);
    particle_group_words[0x23] = *(u8 *)(*scene_side_address + (s32)deck_command_bytes) - BATTLE_PHALANX_FIRST_DECK_COMMAND;
    {
        u16 *blend_control_address = &D_0300004E;
        register u32 blend_control_bits_r2 asm("r2") = (u32)D_00000442;
        u32 blend_control_bits = blend_control_bits_r2;
        asm volatile("" : "+r"(blend_control_bits_r2));
        *blend_control_address = blend_control_bits;
    }
    D_03000050 = 0x10;
    PlaySong(0x6B);

    elapsed_frames = 0;
    do {
        if (gBattleSceneSide == 0) {
            u32 scroll_x_fixed8 = background_scroll_offsets[2];
            s32 scroll_step_fixed8 = -0x1000;
            background_scroll_offsets[2] = scroll_x_fixed8 + scroll_step_fixed8;
        } else {
            u32 scroll_x_fixed8 = background_scroll_offsets[2];
            s32 scroll_step_fixed8;
            scroll_step_fixed8 = 0x1000;
            asm volatile("" : "+r"(scroll_step_fixed8));
            background_scroll_offsets[2] = scroll_x_fixed8 + scroll_step_fixed8;
        }
        if (elapsed_frames > 0x77) {
            D_03000050 = (((elapsed_frames - BATTLE_PHALANX_FADE_START) >> 1) << 8) |
                          ((BATTLE_PHALANX_FRAME_COUNT - elapsed_frames) >> 1);
        }
        YieldTaskForUpdates(1);
    } while (++elapsed_frames <= 0x97);
    D_0300004C &= 0xFDFF;
    D_0300004E = 0;
    DestroySpriteGroup(particle_group_words);
}
