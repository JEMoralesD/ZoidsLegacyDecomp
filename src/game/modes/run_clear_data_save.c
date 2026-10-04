#include "m2c_prelude.h"
#include "../../graphics/screen_effects.h"

extern s32 WriteSaveBlock0(void) asm("func_08094098");
extern s32 WriteSaveBlock1(void) asm("func_080940AC");
extern s32 WriteSaveBlock2(void) asm("func_080940C0");
extern s32 WriteSaveBlock3(void) asm("func_080940D4");
extern s32 WriteSaveBlock4(void) asm("func_080940E8");
extern s32 WriteSaveBlock5(void) asm("func_080940FC");
extern void ClearSpritePools(void) asm("func_08094330");
extern void StartScreenTransition(s32, s32) asm("func_08096308");
extern s32 IsScreenTransitionComplete(void) asm("func_0809669C");
extern void InitializeWindowGraphics(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08096FBC");
extern void RunMenuScript(s32) asm("func_08098BB4");
extern void LoadMenuGradientBackground(s32, s32, s32, s32, s32) asm("func_0809AB44");
extern void RunLoadGame(void) asm("func_0809C1D4");
extern void ClearEventFlag(s32) asm("func_0809F7F0");
extern void BiosLz77ToVram(s32, s32) asm("func_080ECD34");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");
extern u8 D_0200A880;
extern u8 D_0200A882;
extern u8 D_02021699;

void RunClearDataSave(void) {
    s32 zero;
    u32 clear_mask;
    u8 *first_flag;
    u8 index;
    u8 *records;
    u8 *scan_base;
    register u8 *record asm("r2");
    register u32 scan_offset asm("r0");
    u32 record_mask;
    u32 expected;

    *(s16 *)0x0300004C = 0x1840;
    InitializeWindowGraphics(3, 1, 0, 0x35C, 0x3C0, 0, 0xE, 0, 0x3E6, 0xF);
    BiosLz77ToVram(0x081046A8, 0x06015840);
    LoadMenuGradientBackground(2, 3, 0, 0, 1);
    ClearSpritePools();
    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_REVEAL, 0x10);

    zero = 0;
    clear_mask = 0xFFFD;
    first_flag = &D_02021699;

retry:
    RunMenuScript(0x08017AD6);
    if (D_0200A882 != 1) {
        goto done;
    }
    if (D_0200A880 != 0) {
        goto done;
    }
    *(u8 *)0x02021770 = 1;
    index = 1;
    records = (u8 *)0x02027378;
    scan_base = records;
    record_mask = 0x200FF;
    expected = 0x2001E;
scan:
    scan_offset = (u32)index << 6;
    record = (u8 *)(scan_offset + (u32)scan_base);
    if ((*(u32 *)record & record_mask) == expected) {
        goto found;
    }
    index++;
    if (index <= 0x34) {
        goto scan;
    }

selected:
    ClearEventFlag(0x8A);
    {
        u8 *state;
        u8 *entries;
        u8 i;
        s32 loop_zero;

        state = (u8 *)0x0202ECF4;
        *(s16 *)(state + 0) = zero;
        *(s32 *)(state + 4) = 0x81000;
        *(s32 *)(state + 8) = 0xB6000;
        i = 0;
        entries = state;
        entries += 0x38;
        loop_zero = 0;
        do {
            *(s32 *)(entries + (i * 0x14)) = loop_zero;
            i++;
        } while (i <= 0xD);
    }

    if ((WriteSaveBlock0() << 24) == 0 ||
        (WriteSaveBlock1() << 24) == 0 ||
        (WriteSaveBlock2() << 24) == 0 ||
        (WriteSaveBlock3() << 24) == 0 ||
        (WriteSaveBlock4() << 24) == 0 ||
        (WriteSaveBlock5() << 24) == 0) {
        goto checks_failed;
    }
    {
        u8 *flag_a;
        u8 *flag_b;
        u8 *flag_c;
        u8 *flag_d;
        u8 *flag_e;
        s32 one;

        flag_a = (u8 *)0x0202169A;
        flag_b = (u8 *)0x0202169B;
        flag_c = (u8 *)0x0202169C;
        flag_d = (u8 *)0x0202169D;
        flag_e = (u8 *)0x0202169E;
        one = 1;
        *flag_e = one;
        *flag_d = one;
        *flag_c = one;
        *flag_b = one;
        *flag_a = one;
        *first_flag = one;
        RunMenuScript(0x08017B38);
    }
    goto done;

found:
    {
        {
            register u32 loaded asm("r1");
            register u32 flags asm("r0");

            loaded = *(u16 *)(record + 2);
            flags = clear_mask;
            flags &= loaded;
            *(u16 *)(record + 2) = flags;
        }
        {
            register u32 id asm("r0");
            register u32 offset asm("r2");
            register s32 delta asm("r1");
            register u32 base asm("r0");
            register u32 loaded asm("r1");
            register u32 flags asm("r0");

            id = *(u8 *)(record + 1);
            offset = id << 3;
            offset -= id;
            offset <<= 4;
            delta = -0x5A90;
            base = (u32)records;
            base += delta;
            offset += base;
            loaded = *(u16 *)(offset + 4);
            flags = clear_mask;
            flags &= loaded;
            loaded = 0x10;
            flags |= loaded;
            *(u16 *)(offset + 4) = flags;
        }
    }
    goto selected;

checks_failed:
    RunMenuScript(0x08017B6B);
    goto retry;

done:
    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_CONCEAL, 0x10);
    goto poll;
wait:
    YieldTaskForUpdates(1);
poll:
    if ((IsScreenTransitionComplete() << 24) == 0) {
        goto wait;
    }
    RunLoadGame();
    *(s32 *)0x02021690 = -1;
}
