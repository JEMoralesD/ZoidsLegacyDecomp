#include "m2c_prelude.h"
#include "../../graphics/screen_effects.h"
#include "../../game/game_state.h"

extern void StartTask(s32, void *) asm("func_08092D8C");
extern void StopTask(s32) asm("func_08092E0C");
extern void PlaySong(s32) asm("func_08092E84");
extern void ClearSpritePools(void) asm("func_08094330");
extern void StartScreenTransition(s32, s32) asm("func_08096308");
extern s32 IsScreenTransitionComplete(void) asm("func_0809669C");
extern void InitializeFieldMapGraphics(s32, s32, s32) asm("func_0809D938");
extern void ResetFieldActors(void) asm("func_080A9888");
extern void BiosCpuFastSet(const void *, void *, u32) asm("func_080ECD28");
extern void BiosCpuSet(const void *, void *, void *) asm("func_080ECD2C");
extern void BiosLz77ToVram(const void *, void *) asm("func_080ECD34");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");

void PlayWorldMapDistortionScene(void) asm("func_080A90DC");

void PlayWorldMapDistortionScene(void)
{
    register volatile u16 *tilemap_cursor asm("r4");
    register s32 tile_index_or_y_velocity_or_delay asm("r5");
    register u32 bg1_y_address_or_target asm("r6");
    register volatile u8 *displayed_bank_address asm("r12");
    register volatile u8 *display_update_flags_address asm("r3");
    register volatile u8 *scroll_state_address asm("r8");
    register const void *dma_source asm("r9");
    register void *dma_destination asm("r10");
    volatile s32 zero;

    *(s32 *)0x02021690 = GAME_MODE_FIELD;
    YieldTaskForUpdates(1);
    ClearSpritePools();
    ResetFieldActors();
    *(u8 *)0x020324B0 = 0;
    InitializeFieldMapGraphics(0, 0, 0);
    {
        register volatile u16 *scene asm("r0") =
            (volatile u16 *)0x0202ECF4;

        tile_index_or_y_velocity_or_delay = 0;
        *scene = tile_index_or_y_velocity_or_delay;
    }

    BiosLz77ToVram((const void *)0x0842219C, (void *)0x0600C000);
    {
        register const void *tilemap_source asm("r0") =
            (const void *)0x0842295C;

        tilemap_cursor = (volatile u16 *)0x0600F000;
        BiosLz77ToVram(tilemap_source, (void *)tilemap_cursor);
    }
    BiosLz77ToVram((const void *)0x08422934, (void *)0x05000180);
    zero = tile_index_or_y_velocity_or_delay;
    BiosCpuFastSet(&zero, (void *)0x0600EFE0, 0x01000008);

    {
        register u32 limit asm("r2") = 0x1FF;
        register u32 mask_source asm("r0") = 0xC000;
        register u32 mask asm("r1");

        asm volatile("" : "+r"(limit), "+r"(mask_source));
        mask = mask_source;
        do {
            *tilemap_cursor |= mask;
            tilemap_cursor++;
            tile_index_or_y_velocity_or_delay++;
        } while ((u32)tile_index_or_y_velocity_or_delay <= limit);
    }
    {
        register u32 limit asm("r1");
        register volatile u16 *display_control asm("r2");
        register u16 fill asm("r0");

        tile_index_or_y_velocity_or_delay = 0;
        limit = 0x5FF;
        display_control = (volatile u16 *)0x0300004C;
        bg1_y_address_or_target = SCENE_BG1_SCROLL_Y_FIXED8_RAM;
        {
            register u32 guard0 asm("r0");
            register u32 guard3 asm("r3");

            asm volatile("" : "=r"(guard0), "=r"(guard3));
            displayed_bank_address = (volatile u8 *)SCENE_BG1_SCROLL_BANK_RAM;
            asm volatile("" :: "r"(guard0), "r"(guard3));
        }
        display_update_flags_address = (volatile u8 *)0x03000074;
        scroll_state_address = (volatile u8 *)SCENE_BG1_SCROLL_STATE_RAM;
        dma_source = (const void *)SCENE_BG1_SCROLL_CALLBACK_CODE_ROM;
        dma_destination = (void *)SCENE_SCANLINE_CALLBACK_RAM;
        fill = 0x017F;
        do {
            *tilemap_cursor = fill;
            tilemap_cursor++;
            tile_index_or_y_velocity_or_delay++;
        } while ((u32)tile_index_or_y_velocity_or_delay <= limit);

        *display_control |= 0x0200;
    }

    *(u16 *)0x0400000A = 0x5E0E;
    {
        register s32 zero_value asm("r4") = 0;
        register s32 *global_base asm("r0") = (s32 *)0x03000054;

        global_base[2] = zero_value;
        *(volatile s32 *)bg1_y_address_or_target = 0x6000;
        {
            register volatile u8 *state_or_bank_address asm("r1") = displayed_bank_address;

            *state_or_bank_address = zero_value;
        }
        *display_update_flags_address |= 0x20;
        {
            register volatile u8 *state_or_bank_address asm("r2");
            register u32 flag_value asm("r0") = 1;

            state_or_bank_address = scroll_state_address;
            *state_or_bank_address = flag_value;
        }
        BiosCpuSet(dma_source, dma_destination,
            (void *)0x04000014);
        *(u8 *)SCENE_BG1_DISTORTION_PHASE_RAM = zero_value;
    }
    StartTask(4, (void *)0x080A8FAD);
    {
        register volatile u16 *position_x asm("r1") =
            (volatile u16 *)0x03005F74;

        asm volatile("" : "+r"(position_x));
        *position_x = 120;
    }
    {
        register volatile u16 *position_y asm("r1") =
            (volatile u16 *)0x03005F76;

        *position_y = 80;
    }
    StartScreenTransition(SCREEN_TRANSITION_ROTATING_SQUARE_REVEAL, 0);
    {
        register volatile u16 *control_a asm("r1") =
            (volatile u16 *)0x0300004E;
        register u32 value_a asm("r4") = 0x1542;
        register u32 store_value asm("r0");

        asm volatile("" : "+r"(control_a), "+r"(value_a));
        store_value = value_a;
        *control_a = store_value;
    }
    {
        register volatile u16 *control_b asm("r1") =
            (volatile u16 *)0x03000050;
        register u32 value_b asm("r7") = 0x0C0A;
        register u32 store_value asm("r0");

        store_value = value_b;
        *control_b = store_value;
    }

    tile_index_or_y_velocity_or_delay = -0x100;
    PlaySong(107);
    if (*(volatile s32 *)bg1_y_address_or_target > 0x1000) {
        register volatile s32 *animation asm("r4") =
            (volatile s32 *)bg1_y_address_or_target;

        bg1_y_address_or_target = 0x1000;
        do {
            *animation += tile_index_or_y_velocity_or_delay;
            YieldTaskForUpdates(1);
        } while (*animation > (s32)bg1_y_address_or_target);
    }
    if (tile_index_or_y_velocity_or_delay != 0) {
        register volatile s32 *animation asm("r4") =
            (volatile s32 *)SCENE_BG1_SCROLL_Y_FIXED8_RAM;

        do {
            *animation += tile_index_or_y_velocity_or_delay;
            tile_index_or_y_velocity_or_delay += 0x10;
            YieldTaskForUpdates(1);
        } while (tile_index_or_y_velocity_or_delay != 0);
    }

    tile_index_or_y_velocity_or_delay = 0;
    do {
        YieldTaskForUpdates(1);
        tile_index_or_y_velocity_or_delay++;
    } while ((u32)tile_index_or_y_velocity_or_delay <= 59);
    *(u8 *)SCENE_BG1_DISTORTION_PHASE_RAM = 1;
    tile_index_or_y_velocity_or_delay = 0;
    do {
        YieldTaskForUpdates(1);
        tile_index_or_y_velocity_or_delay++;
    } while ((u32)tile_index_or_y_velocity_or_delay <= 44);
    PlaySong(68);
    while (*(u8 *)SCENE_BG1_DISTORTION_PHASE_RAM == 1) {
        YieldTaskForUpdates(1);
    }
    tile_index_or_y_velocity_or_delay = 0;
    do {
        YieldTaskForUpdates(1);
        tile_index_or_y_velocity_or_delay++;
    } while ((u32)tile_index_or_y_velocity_or_delay <= 59);

    StartScreenTransition(SCREEN_TRANSITION_FADE_TO_BLACK, 16);
    while ((u8)IsScreenTransitionComplete() == 0) {
        YieldTaskForUpdates(1);
    }
    StopTask(4);
    {
        register volatile u8 *final_flag asm("r1") =
            (volatile u8 *)SCENE_BG1_SCROLL_STATE_RAM;

        *final_flag = 3;
    }
    *(s32 *)0x02021690 = -1;
    YieldTaskForUpdates(1);
}
