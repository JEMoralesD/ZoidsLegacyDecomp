#include "player_selection.h"

M2C_UNK PlaySong(s32) asm("func_08092E84");                         /* extern */
void *CreateSprite(M2C_UNK, M2C_UNK, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484"); /* extern */
M2C_UNK DestroySprite(void *) asm("func_08094554");                      /* extern */
M2C_UNK ResetMenuKeyRepeat() asm("func_08096F3C");                            /* extern */
M2C_UNK RequestWindowRefresh() asm("func_080972C8");                            /* extern */
M2C_UNK PrintWindowTextAt(s32, s32, s32, s32, s32) asm("func_080981F0");     /* extern */
M2C_UNK PrintWindowNumberAt(s32, s32, s32, s32, s32, s32, s32) asm("func_0809844C"); /* extern */
M2C_UNK RunMenuScript(s32) asm("func_08098BB4");                         /* extern */
M2C_UNK QueueZoidIconGraphics(u8, s32, s32, s32, s32) asm("func_0809A52C");      /* extern */
M2C_UNK DrawEquipmentStats(s32, s32, s32) asm("func_080ACDA8");               /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */
M2C_UNK jtbl_080B0584();                            /* static */
extern u8 gEquipmentCatalog[] asm("D_087B2524");
extern s32 gZoidNameTable[];
extern s32 gEquipmentNameTable[] asm("D_087EE170");
extern u8 gZoidModelSelectionIds[] asm("D_0203237A");
extern u8 D_0200A880;
extern u8 D_08106374[];
extern u8 D_08106378[];

asm(".set D_08106374, 0x08106374\n"
    ".set D_08106378, 0x08106378");

u8 ShowZoidModelStatsAndEquipment(void *base_record, u8 play_confirmation_sound) asm("func_080B0354");

u8 ShowZoidModelStatsAndEquipment(void *base_record, u8 play_confirmation_sound) {
    s32 sp14;
    s32 sp18;
    void *sp1C;
    void *sp20;
    void *sp24;
    s16 temp_r4;
    s32 var_r2;
    s32 temp_r2;
    register s32 var_r0 asm("r0");
    s32 var_r0_2;
    register s32 var_r6 asm("r6");
    u8 *var_r5;
    register u32 temp_r0_2 asm("r0");
    register u32 temp_r0_3 asm("r0");
    u32 var_r6_2;
    s32 constructor_height;
    u8 temp_r0;
    u8 var_r7;
    register u32 var_sl asm("sl");
    register void *var_r9 asm("r9");

    sp14 = (s32) play_confirmation_sound;
    var_r6 = 0;
    sp18 = 0;
loop_1:
    if (var_r6 != 0x10) {
        goto block_3;
    }
    goto block_22;
block_3:
    if (var_r6 > 0x10) {
        goto block_6;
    }
    if (var_r6 == 0) {
        goto block_11;
    }
    goto block_68;
block_6:
    if (var_r6 != 0x1000) {
        goto block_8;
    }
    goto block_37;
block_8:
    if (var_r6 != 0x1010) {
        goto block_10;
    }
    goto block_49;
block_10:
    goto block_68;
block_11:
    RunMenuScript(0x08000FB3);
    PrintWindowTextAt(gZoidNameTable[gZoidModelSelectionIds[D_0200A880]], 0, 1, 0, var_r6);
    PrintWindowNumberAt(M2C_FIELD(base_record, s16 *, 4), 4, 0, 0xA, var_r6, 4, var_r6);
    PrintWindowNumberAt(M2C_FIELD(base_record, s16 *, 4), 4, 0, 0xA, var_r6, 9, var_r6);
    PrintWindowNumberAt(M2C_FIELD(base_record, s16 *, 8), 4, 0, 0xA, var_r6, 9, 1);
    PrintWindowNumberAt(M2C_FIELD(base_record, s16 *, 0xA), 4, 0, 0xA, var_r6, 4, 2);
    PrintWindowNumberAt(M2C_FIELD(base_record, s16 *, 0xC), 4, 0, 0xA, var_r6, 4, 4);
    PrintWindowNumberAt(M2C_FIELD(base_record, s16 *, 0xE), 4, 0, 0xA, var_r6, 4, 5);
    {
        s32 field_c = M2C_FIELD(base_record, s16 *, 0xC);
        s32 field_14 = M2C_FIELD(base_record, s16 *, 0x14);

        PrintWindowNumberAt(field_c + field_14, 4, 0, 0xA, var_r6, 4, 6);
    }
    PrintWindowNumberAt(M2C_FIELD(base_record, s16 *, 0x10), 4, 0, 0xA, var_r6, 4, 8);
    PrintWindowNumberAt(M2C_FIELD(base_record, s16 *, 0x12), 4, 0, 0xA, var_r6, 4, 9);
    PrintWindowNumberAt(M2C_FIELD(base_record, s16 *, 6), 4, 0, 0xA, var_r6, 4, 0xA);
    PrintWindowNumberAt(M2C_FIELD(base_record, s16 *, 0x14), 4, 0, 0xA, var_r6, 4, 0xC);
    PrintWindowNumberAt((s16) M2C_FIELD(base_record, u8 *, 3), 4, 0, 0xA, var_r6, 4, 0xE);
    PrintWindowNumberAt(0, 4, 0, 0xA, var_r6, 4, 0xF);
    {
        u8 *item_table = gEquipmentCatalog;

        PrintWindowNumberAt(M2C_FIELD((item_table + (M2C_FIELD(base_record, u16 *, 0x1A) * 0x18)), s16 *, 0x12) + M2C_FIELD((item_table + (M2C_FIELD(base_record, u16 *, 0x1E) * 0x18)), s16 *, 0x12) + M2C_FIELD((item_table + (M2C_FIELD(base_record, u16 *, 0x22) * 0x18)), s16 *, 0x12) + M2C_FIELD((item_table + (M2C_FIELD(base_record, u16 *, 0x26) * 0x18)), s16 *, 0x12), 4, 0, 0xA, var_r6, 4, 0x10);
    }
    PrintWindowNumberAt(M2C_FIELD(base_record, s16 *, 0x16), 4, 0, 0xA, var_r6, 9, 0x10);
    temp_r0 = M2C_FIELD(base_record, u8 *, 2);
    if ((u32) temp_r0 > 4U) {
        goto block_20;
    }
    switch (temp_r0) {                              /* jump table: jtbl_080B0584 */
case 0:
    var_r0 = 0x08106360;
    goto block_17;
case 1:
    var_r0 = 0x08106364;
    goto block_17;
case 2:
    var_r0 = 0x08106368;
    goto block_17;
case 3:
    var_r0 = 0x0810636C;
block_17:
    PrintWindowTextAt(var_r0, 0, 0, 6, 0x11);
    goto block_20;
case 4:
    PrintWindowTextAt(0x08106370, 0, 0, 6, 0x11);
    }
block_20:
    var_r7 = 0;
    var_sl = 1;
    var_r9 = CreateSprite(0x080ED830, 0x080ED864, 0, 0, (s32) *(u8 *)0x087A1DB8, 0x3EE, 0xF, 0x20, 0);
    sp20 = CreateSprite(0x08105A20, 0x08105A2C, 0, 0xD8, 0x98, 0x343, 0xF, (var_r6 = 8), 0);
    sp24 = CreateSprite(0x08105A70, 0x08105A7C, 0, 0xA8, 0x98, 0x346, 0xF, var_r6, 0);
    QueueZoidIconGraphics(gZoidModelSelectionIds[D_0200A880], 0, 0, 0, 0x02002880);
    sp1C = CreateSprite(0x0821024C, 0x08210258, 0, 0xB4, 0x58, 0, 0, var_r6, 0);
    var_r6 = 0x10;
    goto block_68;
block_22:
    ResetMenuKeyRepeat();
loop_23:
    if (var_r7 == var_sl) {
        goto block_25;
    }
    {
        register s32 *sound_table_r1 asm("r1") = (s32 *)0x087A1D80;

        asm volatile("" : "+r"(sound_table_r1));
        RunMenuScript(sound_table_r1[var_r7]);
    }
    RequestWindowRefresh();
    {
        register u32 cursor_value asm("r0");
        register u8 *window_r1 asm("r1");

        asm volatile(
            ".macro ldr dst, src\n\t"
            ".purgem ldr\n\t"
            "ldr r0, \\src\n\t"
            ".endm\n\t"
            ".macro add args:vararg\n\t"
            ".purgem add\n\t"
            ".short 0x1838\n\t"
            ".endm");
        cursor_value = M2C_FIELD(var_r7, u8 *, 0x087A1DB8);
        window_r1 = var_r9;
        M2C_FIELD(window_r1, u16 *, 6) = cursor_value;
    }
    var_sl = var_r7;
block_25:
    YieldTaskForUpdates(1);
    if (!(0x40 & *(u16 *)0x03006034)) {
        goto block_28;
    }
    if (var_r7 == 0) {
        goto block_28;
    }
    var_r7 -= 1;
    PlaySong(0x40);
block_28:
    if (!(0x80 & *(u16 *)0x03006034)) {
        goto block_31;
    }
    if ((u32) var_r7 > 0xCU) {
        goto block_31;
    }
    var_r7 += 1;
    PlaySong(0x40);
block_31:
    if (!(3 & *(u16 *)0x0300000E)) {
        goto loop_23;
    }
    DestroySprite(var_r9);
    DestroySprite(sp20);
    DestroySprite(sp24);
    if (!(1 & *(u16 *)0x0300000E)) {
        goto block_35;
    }
    PlaySong(0x3E);
    var_r6 = 0x1000;
    goto block_68;
block_35:
    RunMenuScript(0x080014D0);
    DestroySprite(sp1C);
    sp18 = 1;
    goto block_65;
block_37:
    asm volatile(
        ".set b0354_pool_ldr_count, 0\n\t"
        ".macro ldr dst, src\n\t"
        ".if b0354_pool_ldr_count == 0\n\t"
        ".short 0x4817\n\t"
        ".elseif b0354_pool_ldr_count == 1\n\t"
        ".short 0x4913\n\t"
        ".elseif b0354_pool_ldr_count == 2\n\t"
        ".short 0x6800\n\t"
        ".else\n\t"
        ".purgem ldr\n\t"
        ".short 0x4B0C\n\t"
        ".endif\n\t"
        ".set b0354_pool_ldr_count, b0354_pool_ldr_count + 1\n\t"
        ".endm\n\t"
        ".macro bne target\n\t"
        ".purgem bne\n\t"
        "bne .Lb0354_status_7c\n\t"
        ".endm");
    RunMenuScript(0x08001420);
    var_r6_2 = 0;
    var_r5 = (u8 *)base_record + 0x1A;
loop_38:
    var_r2 = (var_r6_2 * 2) + 1;
    if (var_r6_2 <= 3U) {
        goto block_40;
    }
    var_r2 += 1;
block_40:
    temp_r4 = var_r2;
    PrintWindowTextAt(gEquipmentNameTable[*(u16 *)var_r5], 0, 0, 3, (s32) temp_r4);
    temp_r0_2 = *(u16 *)var_r5;
    if (temp_r0_2 == 0) {
        goto block_47;
    }
    {
        register u8 *catalog_r3 asm("r3") = gEquipmentCatalog;
        register u32 item_id_r1 asm("r1");

        item_id_r1 = temp_r0_2;
        asm volatile("" : "+r"(item_id_r1));
        temp_r2 = item_id_r1 * 0x18;
        if (1 & *(u16 *)((u32)temp_r2 - (0 - (u32)catalog_r3) + 2)) {
            goto block_46;
        }
        {
            register s32 field_r0 asm("r0");

            asm volatile(
                ".syntax unified\n\t"
                "adds %0, %1, #4\n\t"
                "adds %0, %2, %0\n\t"
                "ldr %0, [%0]\n\t"
                ".syntax divided"
                : "=&r"(field_r0)
                : "r"(catalog_r3), "r"(temp_r2)
                : "cc", "memory");
            asm volatile(
                ".syntax unified\n\t"
                "movs r1, #16\n\t"
                "ands %0, r1\n\t"
                ".syntax divided"
                : "+r"(field_r0)
                :
                : "r1", "cc");
            asm volatile(
                ".set b0354_status_ldr_count, 0\n\t"
                ".macro ldr dst, src\n\t"
                ".if b0354_status_ldr_count == 0\n\t"
                ".else\n\t"
                ".purgem ldr\n\t"
                ".short 0x9400\n\t"
                "ldr r0, .Lb0354_status_false\n\t"
                "b .Lb0354_status_common\n\t"
                ".align 2, 0\n\t"
                ".Lb0354_pool_sound:\n\t"
                ".word 0x08001420\n\t"
                ".Lb0354_pool_rows:\n\t"
                ".word D_087EE170\n\t"
                ".Lb0354_pool_catalog:\n\t"
                ".word D_087B2524\n\t"
                ".Lb0354_status_false:\n\t"
                ".word D_08106374\n\t"
                ".endif\n\t"
                ".set b0354_status_ldr_count, b0354_status_ldr_count + 1\n\t"
                ".endm\n\t"
                ".macro str args:vararg\n\t"
                ".purgem str\n\t"
                ".short 0x9400\n\t"
                "ldr r0, .Lb0354_status_true\n\t"
                ".endm\n\t"
                ".macro add args:vararg\n\t"
                ".purgem add\n\t"
                ".Lb0354_status_common:\n\t"
                ".macro b target\n\t"
                ".purgem b\n\t"
                "b \\target\n\t"
                ".align 2, 0\n\t"
                ".Lb0354_status_true:\n\t"
                ".word D_08106378\n\t"
                ".pushsection .b0354_discard, \"\", %progbits\n\t"
                ".endm\n\t"
                ".endm");
            if (field_r0 != 0) {
                goto block_44;
            }
        }
        var_r0_2 = (s32)D_08106374;
        goto block_45;
block_44:
        var_r0_2 = (s32)D_08106378;
block_45:
        PrintWindowTextAt(var_r0_2, 0, 0, 1, (s32) temp_r4);
    }
    goto block_47;
block_46:
    asm volatile(
        ".popsection\n\t"
        ".Lb0354_status_7c:\n\t"
        ".macro ldr dst, src\n\t"
        ".purgem ldr\n\t"
        ".short 0x9400\n\t"
        "ldr \\dst, \\src\n\t"
        ".endm\n\t"
        ".macro str args:vararg\n\t"
        ".purgem str\n\t"
        ".endm");
    PrintWindowTextAt(0x0810637C, 0, 0, 1, (s32) temp_r4);
block_47:
    var_r5 += 4;
    var_r6_2 += 1;
    if (var_r6_2 <= 7U) {
        goto loop_38;
    }
    constructor_height = 8;
    var_r7 = 0;
    var_sl = 1;
    var_r9 = CreateSprite(0x080ED8F8, 0x080ED92C, 0, 0, (s32) *(u8 *)0x087A1DC6, 0x3F4, 0xF, 0x20, 0);
    sp20 = CreateSprite(0x08105AC0, 0x08105ACC, 0, 0xD0, 0x98, 0x34C, 0xF, constructor_height, 0);
    var_r6 = 0x1010;
    goto block_68;
block_49:
    ResetMenuKeyRepeat();
loop_50:
    if (var_r7 == var_sl) {
        goto block_56;
    }
    {
        register void *selected_base_r2 asm("r2");
        register u8 *selected_entry_r1 asm("r1");

        temp_r0_3 = var_r7 * 4;
        selected_base_r2 = base_record;
        asm volatile("" : "+r"(temp_r0_3), "+r"(selected_base_r2));
        selected_entry_r1 = (u8 *)selected_base_r2 + temp_r0_3;
        temp_r0_3 = *(u16 *)(selected_entry_r1 + 0x1A);
    }
    if (temp_r0_3 == 0) {
        goto block_54;
    }
    {
        register u32 item_id_r1 asm("r1") = temp_r0_3;
        u8 *catalog_pA;

        asm volatile(
            ".set b0354_catalog_add_count, 0\n\t"
            ".macro ldr dst, src\n\t"
            ".purgem ldr\n\t"
            ".set b0354_catalog_pool, \\src\n\t"
            ".endm\n\t"
            ".macro add args:vararg\n\t"
            ".if b0354_catalog_add_count == 0\n\t"
            ".short 0x1840\n\t"
            ".else\n\t"
            ".purgem add\n\t"
            "ldr r1, b0354_catalog_pool\n\t"
            ".short 0x1840\n\t"
            ".endif\n\t"
            ".set b0354_catalog_add_count, b0354_catalog_add_count + 1\n\t"
            ".endm");
        catalog_pA = gEquipmentCatalog;
        asm volatile("" : "+r"(catalog_pA));
        DrawEquipmentStats((item_id_r1 * 0x18) + (u32)catalog_pA, 2, 0);
    }
    goto block_55;
block_54:
    RunMenuScript(0x080014CB);
block_55:
    RequestWindowRefresh();
    {
        register u32 cursor_value_r0 asm("r0") = 0x087A1DC6;

        asm volatile(
            ".syntax unified\n\t"
            "adds %0, %1, %0\n\t"
            "ldrb %0, [%0]\n\t"
            ".syntax divided"
            : "+r"(cursor_value_r0)
            : "r"(var_r7)
            : "cc", "memory");
        {
            register u8 *window_r3 asm("r3") = var_r9;

            *(u16 *)(window_r3 + 6) = cursor_value_r0;
        }
    }
    var_sl = var_r7;
block_56:
    YieldTaskForUpdates(1);
    if (!(0x40 & *(u16 *)0x03006034)) {
        goto block_59;
    }
    if (var_r7 == 0) {
        goto block_59;
    }
    var_r7 -= 1;
    PlaySong(0x40);
block_59:
    if (!(0x80 & *(u16 *)0x03006034)) {
        goto block_62;
    }
    if ((u32) var_r7 > 6U) {
        goto block_62;
    }
    var_r7 += 1;
    PlaySong(0x40);
block_62:
    if (!(3 & *(u16 *)0x0300000E)) {
        goto loop_50;
    }
    DestroySprite(var_r9);
    DestroySprite(sp1C);
    DestroySprite(sp20);
    RunMenuScript(0x080014D0);
    if (!(1 & *(u16 *)0x0300000E)) {
        goto block_67;
    }
    sp18 = 2;
    if (sp14 != 0) {
        goto block_66;
    }
block_65:
    PlaySong(0x3F);
    goto block_68;
block_66:
    PlaySong(0x3E);
    goto block_68;
block_67:
    sp18 = 1;
    PlaySong(0x3F);
block_68:
    if (sp18 != 0) {
        goto block_70;
    }
    goto loop_1;
block_70:
    return (u8) (sp18 - 1);
}
