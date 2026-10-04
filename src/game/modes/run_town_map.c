#include "m2c_prelude.h"
#include "../../graphics/screen_effects.h"
#include "../game_state.h"

struct SceneState {
    u8 pad00[2];
    u8 kind;
    u8 active;
    u8 pad04[8];
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
};

struct Sprite {
    u8 pad00[4];
    s16 x;
    s16 y;
};

extern volatile u16 D_0300004C;
extern s32 gFieldCameraScrollOffsets[2] asm("D_03000054");
extern struct SceneState D_0202ECF4;
extern u32 D_087EF4E0[];
extern u8 D_087AFBB4[];
extern u8 D_0200A880;
extern u8 D_0200A882;
extern s32 gGameMode;

void InitializeWindowGraphics(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08096FBC");
void BiosLz77ToVram(s32, s32) asm("func_080ECD34");
void LoadWorldOverviewMapGraphics(s32, s32) asm("func_080ADBF8");
void ClearSpritePools(void) asm("func_08094330");
struct Sprite *CreateSprite(s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484");
void RunMenuScript(s32) asm("func_08098BB4");
void AppendWindowTextItem(s32, s32) asm("func_080988C8");
void StartScreenTransition(s32, s32) asm("func_08096308");
u8 IsScreenTransitionComplete(void) asm("func_0809669C");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

void RunTownMap(void)
{
    u8 selected[44];
    struct Sprite *cursor;
    register s32 count asm("r5");
    u8 id;
    register u32 *enabled asm("r8");

    D_0300004C = 0x1940;
    *(volatile u16 *)0x04000008 = 4;
    gFieldCameraScrollOffsets[1] = 0;
    gFieldCameraScrollOffsets[0] = 0;

    InitializeWindowGraphics(3, 3, 0, 0x1C0, 0x1C0, 1, 0xE, 0, 0x3E6, 0xF);
    BiosLz77ToVram(0x081046A8, 0x06015840);
    LoadWorldOverviewMapGraphics(1, 0);
    ClearSpritePools();

    {
        register u32 scaled_x asm("r0");
        register u32 scaled_y_bits asm("r1");
        register s32 sprite_data asm("r4");
        register s32 sprite_anim asm("r7");
        register s32 sprite_x asm("r3");
        s32 base_y_value;
        register s32 sprite_y asm("r0");
        register struct SceneState *scene asm("r1");
        register s32 first_x asm("r0");
        register s32 first_y asm("r1");
        register s32 y_adjust asm("r4");

        scene = &D_0202ECF4;
        first_x = scene->x0;
        if (first_x < 0) {
            first_x += 0x1FFF;
        }
        scaled_x = (u32)(first_x * 8) >> 16;
        first_y = scene->y0;
        if (first_y < 0) {
            y_adjust = 0x1FFF;
            asm volatile("" : "+r"(y_adjust));
            first_y += y_adjust;
        }
        scaled_y_bits = first_y * 8;
        sprite_data = 0x08105FD0;
        sprite_anim = 0x08106068;
        sprite_x = (s32)((scaled_x << 16) + 0xFFF80000) >> 16;
        base_y_value = scaled_y_bits >> 16;
        asm volatile("" : "+r"(base_y_value));
        sprite_y = (s32)scaled_y_bits >> 16;
        if (sprite_y > 3) {
            sprite_y -= 4;
        } else {
            sprite_y += 4;
        }
        CreateSprite(sprite_data, sprite_anim, 0, sprite_x,
            (s16)sprite_y, 0x3A8, 0xF,
            (s16)base_y_value <= 3 ? 0x10020 : 0x20, 0);
    }

    {
        register struct SceneState *scene asm("r2") = &D_0202ECF4;
        asm volatile("" : "+r"(scene));
        if (scene->active != 0 && scene->kind != 0x6C) {
            register s32 sprite_data asm("r4") = 0x08105FD0;
            register s32 sprite_anim asm("r1") = 0x08106068;
            register s32 px asm("r0");
            register s32 py asm("r0");
            register s32 x_adjust asm("r3");
            register s32 y_adjust asm("r2");
            register s32 sprite_x asm("r3");
            register s32 sprite_y asm("r0");

            asm volatile("" : "+r"(sprite_data), "+r"(sprite_anim));
            px = scene->x1;
            if (px < 0) {
                x_adjust = 0x1FFF;
                asm volatile("" : "+r"(x_adjust));
                px += x_adjust;
            }
            sprite_x = (s16)((px >> 13) - 8);
            py = scene->y1;
            if (py < 0) {
                y_adjust = 0x1FFF;
                asm volatile("" : "+r"(y_adjust));
                py += y_adjust;
            }
            sprite_y = (s16)((py >> 13) - 4);
            CreateSprite(sprite_data, sprite_anim, 2, sprite_x, sprite_y,
                0x3A8, 0xF, 0x120, 0);
        }
    }

    RunMenuScript(0x08003564);
    RunMenuScript(0x08003580);

    count = 0;
    id = 1;
    {
        register u32 *enabled_base asm("r3") = (u32 *)0x0202EE84;
        asm volatile("" : "+r"(enabled_base));
        enabled = enabled_base;
    }
    do {
        if (enabled[id >> 5] & (1 << (id & 0x1F))) {
            AppendWindowTextItem(0, D_087EF4E0[id]);
            if (count == 0) {
                s32 sprite_data = 0x08105FD0;
                s32 sprite_anim = 0x08106068;
                register s32 record_offset asm("r1");
                register u8 *positions asm("r2") = D_087AFBB4;
                register u8 *field_ptr asm("r0");
                register s32 px asm("r0");
                register s32 py asm("r0");
                register s32 sprite_x asm("r3");
                asm volatile("" : "+r"(sprite_data), "+r"(sprite_anim));
                asm volatile("" : "+r"(positions));
                record_offset = (id - 1) * 6;
                field_ptr = positions + 2;
                asm volatile("" : "+r"(field_ptr));
                field_ptr = (u8 *)((s32)record_offset + (s32)field_ptr);
                px = *(s16 *)field_ptr;
                if (px < 0) {
                    px += 3;
                }
                sprite_x = (px >> 2) - 8;
                field_ptr = positions + 4;
                asm volatile("" : "+r"(field_ptr));
                field_ptr = (u8 *)((s32)record_offset + (s32)field_ptr);
                py = *(s16 *)field_ptr;
                if (py < 0) {
                    py += 3;
                }
                cursor = CreateSprite(sprite_data, sprite_anim, 1,
                    sprite_x, (py >> 2) - 4,
                    0x3A8, 0xF, 0x220, count);
            }
            selected[count] = id;
            {
                register s32 next_count asm("r0") = count + 1;
                asm volatile("" : "+r"(next_count));
                count = (u8)next_count;
            }
        }
        id++;
    } while (id <= 0x2A);

    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_REVEAL, 0x10);
    goto wait_for_selection;
update_cursor:
    {
        register u8 *positions asm("r3") = D_087AFBB4;
        register u8 *selection_index asm("r2") = &D_0200A880;
        register s32 chosen asm("r1");
        register s32 record_offset asm("r0");
        register u8 *field_ptr asm("r1");
        register u8 *address asm("r0");
        register s32 value asm("r1");
        register s32 adjusted asm("r0");

        asm volatile("" : "+r"(positions), "+r"(selection_index));
        chosen = selected[*selection_index];
        chosen -= 1;
        asm volatile("" : "+r"(chosen));
        record_offset = chosen * 6;
        field_ptr = positions + 2;
        asm volatile("" : "+r"(field_ptr));
        address = (u8 *)((s32)record_offset + (s32)field_ptr);
        value = *(s16 *)address;
        adjusted = value;
        if (value < 0) {
            adjusted = value + 3;
        }
        cursor->x = (adjusted >> 2) - 8;
        {
            chosen = selected[*selection_index];
            chosen -= 1;
            asm volatile("" : "+r"(chosen));
            record_offset = chosen * 6;
            field_ptr = positions + 4;
            asm volatile("" : "+r"(field_ptr));
            address = (u8 *)((s32)record_offset + (s32)field_ptr);
            value = *(s16 *)address;
            adjusted = value;
            if (value < 0) {
                adjusted = value + 3;
            }
            cursor->y = (adjusted >> 2) - 4;
        }
    }
wait_for_selection:
    RunMenuScript(0x08003578);
    if (D_0200A882 != 2) {
        goto update_cursor;
    }

    gGameMode = GAME_MODE_FIELD;
    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_CONCEAL, 0x10);
    while (!IsScreenTransitionComplete()) {
        YieldTaskForUpdates(1);
    }
}
