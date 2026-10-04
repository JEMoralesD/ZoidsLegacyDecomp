#include "player_selection.h"

M2C_UNK PlaySong(s32) asm("func_08092E84");                         /* extern */
s32 CreateSprite(M2C_UNK, M2C_UNK, u8, s32, s32, s32, s32, s32, s32) asm("func_08094484"); /* extern */
M2C_UNK DestroySprite(s32) asm("func_08094554");                         /* extern */
M2C_UNK SetSpriteAnimation(s32, u8) asm("func_08094564");                     /* extern */
M2C_UNK DestroySpriteGroup(s32) asm("func_08095114");                         /* extern */
M2C_UNK QueueCopy(M2C_UNK, M2C_UNK, s32) asm("func_08095208");       /* extern */
M2C_UNK DisableDisplayWindows() asm("func_0809534C");                            /* extern */
M2C_UNK ConfigureDisplayWindows(s32, s32, s32, s32, s32, s32, s32, s32) asm("func_0809538C"); /* extern */
M2C_UNK ResetMenuKeyRepeat() asm("func_08096F3C");                            /* extern */
M2C_UNK RunMenuScript(M2C_UNK) asm("func_08098BB4");                     /* extern */
M2C_UNK LoadCompressedPalette(M2C_UNK, M2C_UNK, s32) asm("func_0809A1BC");       /* extern */
s32 LoadZoidBodyGraphicsWithWramStaging() asm("func_0809A35C");                                 /* extern */
M2C_UNK BiosLz77ToVram(M2C_UNK, M2C_UNK) asm("func_080ECD34");            /* extern */
M2C_UNK BiosLz77ToWram(s32, s32) asm("func_080ECD38");                    /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */

u8 SelectZoidPaletteVariant(u32 model_id, u32 initial_palette_variant) asm("func_080B80CC");

u8 SelectZoidPaletteVariant(u32 model_id, u32 initial_palette_variant) {
    s32 palette_label_sprite;
    register u32 selected_model_id asm("r9");
    u8 previous_variant;
    u8 palette_variant;
    void *palette_table_offset;
    register u8 *object_palette_buffer asm("r8");

    selected_model_id = (u8)model_id;
    palette_variant = initial_palette_variant;
    BiosLz77ToVram(0x081065C8, 0x060177C0);
    LoadCompressedPalette(0x081067EC, 0x050003C0, 0x02002880);
    ConfigureDisplayWindows(1, 0xF0, 0x80, 0, 0, 0, 0x37, 0x3F);
    /* The final two arguments occupy the existing outgoing stack area. */
    asm volatile(
        "str r4, [sp, #0]\n\t"
        "str r5, [sp, #4]"
        :
        :
        : "memory");
    *(s32 *)0x02033F3C = LoadZoidBodyGraphicsWithWramStaging(selected_model_id, palette_variant, 3, 1);
    *(s16 *)0x04000008 = 0x18D;
    *(u16 *)0x0300004C |= 0x100;
    {
        s32 *display_state = (s32 *)0x03000054;
        display_state[0] = 0x1000;
        display_state[1] = 0;
    }
    RunMenuScript(0x08005629);
    palette_label_sprite = CreateSprite(0x08106DD4, 0x08106E34, palette_variant, 4, 4, 0x3BE, 0xE, 8, 0);
    ResetMenuKeyRepeat();
    {
        register u32 palette2_value asm("r0") = 0x80;
        asm volatile(
            "add r0, r0, r5\n\t"
            "mov r8, r0"
            : "+r"(palette2_value), "=r"(object_palette_buffer)
            :
            : "cc");
    }
    do {
        YieldTaskForUpdates(1);
        previous_variant = palette_variant;
        if ((0x40 & *(u16 *)0x03006034) && (palette_variant != 0)) {
            palette_variant -= 1;
            PlaySong(0x40);
        }
        if ((0x80 & *(u16 *)0x03006034) && ((u32)palette_variant <= 6U)) {
            palette_variant += 1;
            PlaySong(0x40);
        }
        if (palette_variant != previous_variant) {
            s32 *background_palette_table = (s32 *)0x087A641C;
            s32 *object_palette_table;

            asm volatile("" : "+r"(background_palette_table));
            palette_table_offset = (void *)((palette_variant * 4) + (selected_model_id << 5));
            BiosLz77ToWram(*(s32 *)((u32)palette_table_offset + (u32)background_palette_table), 0x02002880);
            *(u16 *)0x02002880 = *(u16 *)0x05000000;
            QueueCopy(0x02002880, 0x05000000, 0x80);
            object_palette_table = (s32 *)0x087A8F24;
            asm volatile("" : "+r"(object_palette_table));
            palette_table_offset = (void *)((u32)palette_table_offset + (u32)object_palette_table);
            BiosLz77ToWram(*(s32 *)palette_table_offset, (s32)object_palette_buffer);
            QueueCopy((s32)object_palette_buffer, 0x05000200, 0x20);
            SetSpriteAnimation(palette_label_sprite, palette_variant);
        }
    } while (!(3 & *(u16 *)0x0300000E));
    DisableDisplayWindows();
    *(u16 *)0x0300004C = 0xFEFF & *(u16 *)0x0300004C;
    DestroySprite(palette_label_sprite);
    DestroySpriteGroup(*(s32 *)0x02033F3C);
    return palette_variant;
}
