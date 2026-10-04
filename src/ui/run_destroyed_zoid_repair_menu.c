#include "player_selection.h"

void PlaySong(s32) asm("func_08092E84");
s32 CreateSprite() asm("func_08094484");
void DestroySprite(s32) asm("func_08094554");
void DisableDisplayWindows(void) asm("func_0809534C");
void ConfigureDisplayWindows() asm("func_0809538C");
s32 IsScreenTransitionComplete(void) asm("func_0809669C");
void PrintWindowTextAt(s32, s32, s32, s32, s32) asm("func_080981F0");
void PrintWindowText(s32, s32, s32) asm("func_08098248");
void PrintWindowNumberAt(s32, s32, s32, s32, s32, s32, s32) asm("func_0809844C");
void ClearWindow(s32) asm("func_080986B4");
void ClearWindowTextList(s32) asm("func_08098834");
void RunMenuScript(s32) asm("func_08098BB4");
void QueueZoidIconGraphics(u8, u8, s32, s32, s32) asm("func_0809A52C");
void AppendPlayerZoidSelectionRows(s32, s32) asm("func_080AC214");
void CreatePlayerSelectionStatusSprites(s32, s32, s32) asm("func_080ACA8C");
void DestroyPlayerSelectionStatusSprites(s32) asm("func_080ACBA0");
void UpdateZoidSelectionStatusSprites(s32, u32) asm("func_080ACBDC");
void BuildFilteredPlayerZoidSelection(s32, s32, s32) asm("func_080B61C8");
void SubtractPlayerMoney(u32) asm("func_080E5E90");
void RestoreDestroyedZoidHp(struct ZoidRepairRecordView *) asm("func_080E6090");
s32 GetPilotDisplayName(u8) asm("func_080E7B64");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

#define D_0200A880 (*(volatile u8 *)0x0200A880)
#define D_0200A881 (*(volatile u8 *)0x0200A881)
#define D_0200A882 (*(volatile u8 *)0x0200A882)
#define gPlayerZoidSelectionSlots ((volatile u8 *)0x020321A4)

void RunDestroyedZoidRepairMenu(void) asm("func_080B7DC0");

void RunDestroyedZoidRepairMenu(void)
{
    s32 frame[3];
    register s32 state asm("r5") = ZOID_REPAIR_INITIALIZE;
    register struct ZoidRepairRecordView *entry asm("r8");
    register u32 first_visible asm("r9");
    register u32 selected asm("sl");
    volatile u32 *money;
    register u32 repair_fee asm("r6");

    asm volatile("" : : "m"(frame[0]), "m"(frame[1]));

dispatch:
    switch (state) {
    case ZOID_REPAIR_INITIALIZE:
        RunMenuScript(0x08005074);
        CreatePlayerSelectionStatusSprites(5, 1, 1);
        /* Preserve the ROM's stack-first setup for this nine-argument call. */
        asm volatile(
            "mov r0, #72\n\t"
            "str r0, [sp, #0]\n\t"
            "str r5, [sp, #4]\n\t"
            "str r5, [sp, #8]\n\t"
            "mov r0, #8\n\t"
            "str r0, [sp, #12]\n\t"
            "str r5, [sp, #16]"
            : : : "r0", "r1", "memory");
        frame[2] = CreateSprite(
            ({ register s32 arg asm("r0") = 0x0821024C;
               asm volatile("" : "+r"(arg)); arg; }),
            ({ register s32 arg asm("r1") = 0x08210258;
               asm volatile("" : "+r"(arg)); arg; }),
            0, 0xB8);
        {
            register u32 zero asm("r1") = 0;
            asm volatile("" : "+r"(zero));
            first_visible = zero;
            selected = zero;
        }
        goto next_state;

    case ZOID_REPAIR_BUILD_LIST:
        RunMenuScript(0x080050FE);
        ClearWindowTextList(4);
        AppendPlayerZoidSelectionRows(4, 0);
        state = ZOID_REPAIR_SELECT_ZOID;
        goto continue_dispatch;

    case ZOID_REPAIR_SELECT_ZOID:
    {
        register s32 entry_offset asm("r0");
        register s32 entry_base asm("r2");
        register struct ZoidRepairRecordView *entry_low asm("r0");
        register u8 model_id asm("r0");
        register struct ZoidRepairRecordView *palette_base asm("r2");
        register u8 palette_variant asm("r1");
        register s32 six asm("r4");

        entry_offset = gPlayerZoidSelectionSlots[selected] * 0x70;
        asm volatile("" : "+r"(entry_offset));
        entry_base = 0x020218E8;
        asm volatile("" : "+r"(entry_base));
        entry_low = (struct ZoidRepairRecordView *)(entry_offset + entry_base);
        entry = entry_low;
        model_id = entry_low->model_id;
        palette_base = entry;
        palette_variant = palette_base->palette_variant;
        QueueZoidIconGraphics(model_id, palette_variant, 0, 0, 0x02002880);
        {
            register struct ZoidRepairRecordView *value_base asm("r1") = entry;
            register s32 value asm("r0");
            register s32 eight asm("r1");
            asm volatile("" : "+r"(value_base));
            value = value_base->max_hp;
            six = 6;
            PrintWindowNumberAt(value, 4, 0, 0xA, six,
                ({ eight = 8; asm volatile("" : "+r"(eight)); eight; }),
                eight);
        }
        {
            register struct ZoidRepairRecordView *value_base asm("r2") = entry;
            register s32 value asm("r0");
            asm volatile("" : "+r"(value_base));
            value = value_base->max_ep;
            PrintWindowNumberAt(value, 4, 0, 0xA, six,
                ({ register s32 eight asm("r2") = 8;
                   asm volatile("" : "+r"(eight)); eight; }),
                9);
        }
        ClearWindow(5);
        {
            register struct ZoidRepairRecordView *pilot_entry asm("r1") = entry;
            register s32 pilot_address asm("r0");
            register s32 player_base asm("r2");
            register s32 pilot_table_offset asm("r1");
            asm volatile("" : "+r"(pilot_entry));
            pilot_address = pilot_entry->pilot_slot << 6;
            asm volatile("" : "+r"(pilot_address));
            player_base = 0x020218E4;
            asm volatile("" : "+r"(player_base));
            pilot_address += player_base;
            asm volatile("" : "+r"(pilot_address));
            pilot_table_offset = 0x5A94;
            asm volatile("" : "+r"(pilot_table_offset));
            pilot_address += pilot_table_offset;
            PrintWindowText(GetPilotDisplayName(*(u8 *)pilot_address), 0, 5);
        }
        UpdateZoidSelectionStatusSprites(5, first_visible);
        RunMenuScript(0x080050BB);
        selected = D_0200A880;
        first_visible = D_0200A881;
        switch (D_0200A882) {
        case 1:
        {
            register struct ZoidRepairRecordView *load_base asm("r2") = entry;
            register u32 flags asm("r0");
            register u32 mask_base asm("r2");
            register u32 mask asm("r1");
            register struct ZoidRepairRecordView *store_base asm("r1");
            asm volatile("" : "+r"(load_base));
            flags = load_base->flags;
            mask_base = 0xFFFE;
            asm volatile("" : "+r"(mask_base));
            mask = mask_base;
            asm volatile("" : "+r"(mask));
            flags &= mask;
            store_base = entry;
            asm volatile("" : "+r"(store_base));
            store_base->flags = flags;
            state = ZOID_REPAIR_CONFIRM;
            goto continue_dispatch;
        }
        case 2:
            DestroyPlayerSelectionStatusSprites(5);
            DestroySprite(frame[2]);
            RunMenuScript(0x080050BF);
            return;
        }
        goto continue_dispatch;
    }

    case ZOID_REPAIR_CONFIRM:
    {
        register u8 *required_level_ptr asm("r5") = &entry->required_pilot_level;
        register u32 action asm("r4");
        register u32 cursor asm("r5");
        register s32 zero_r4 asm("r4");

        goto test_ready;
wait_ready:
        YieldTaskForUpdates(1);
test_ready:
        if ((IsScreenTransitionComplete() << 24) == 0) {
            goto wait_ready;
        }

        /* Preserve the ROM's stack-first setup for this eight-argument call. */
        asm volatile(
            "mov r4, #0\n\t"
            "str r4, [sp, #0]\n\t"
            "str r4, [sp, #4]\n\t"
            "mov r0, #47\n\t"
            "str r0, [sp, #8]\n\t"
            "mov r0, #63\n\t"
            "str r0, [sp, #12]"
            : "=r"(zero_r4) : : "r0", "r1", "r2", "memory");
        ConfigureDisplayWindows(
            ({ register s32 one asm("r0") = 1;
               asm volatile("" : "+r"(one)); one; }),
            ({ register s32 arg asm("r1") = 0x20D0;
               asm volatile("" : "+r"(arg)); arg; }),
            ({ register s32 arg asm("r2") = 0x2858;
               asm volatile("" : "+r"(arg)); arg; }),
            0);
        RunMenuScript(0x08005134);
        RunMenuScript(0x080050C6);
        {
            register s32 *icon_table asm("r1") = (s32 *)0x087EDD54;
            register struct ZoidRepairRecordView *icon_entry asm("r2") = entry;
            register u32 model_id asm("r0");
            asm volatile("" : "+r"(icon_table), "+r"(icon_entry));
            model_id = icon_entry->model_id;
            PrintWindowTextAt(icon_table[model_id], 2, 7, 0, zero_r4);
        }
        {
            register u32 required_level asm("r1") = *required_level_ptr;
            register u32 price_work asm("r0");
            asm volatile("" : "+r"(required_level));
            price_work = required_level * 125;
            repair_fee = price_work << 3;
        }
        PrintWindowNumberAt(repair_fee, 7, 0, 2, 7, 0xC, 2);
        RunMenuScript(0x080050F2);
        action = D_0200A882;
        if (action == 1) {
            asm volatile("" : : : "r0");
            cursor = D_0200A880;
            if (cursor == 0) {
                register s32 money_base asm("r0") = 0x020218E4;
                register s32 money_offset asm("r1") = 0x6A04;
                asm volatile("" : "+r"(money_base), "+r"(money_offset));
                money = (volatile u32 *)(money_base + money_offset);
                if (*money >= repair_fee) {
                    SubtractPlayerMoney(repair_fee);
                    RestoreDestroyedZoidHp(entry);
                    {
                        register u32 current_money asm("r0") = *money;
                        asm volatile("" : "+r"(current_money));
                        PrintWindowNumberAt(current_money, 7, 0, 2,
                            action, action, cursor);
                    }
                    RunMenuScript(0x08005150);
                    RunMenuScript(0x080050EF);
                    DisableDisplayWindows();
                    BuildFilteredPlayerZoidSelection(8, 0, 0);
                    {
                        register volatile u8 *result_ptr asm("r0") =
                            (volatile u8 *)0x02032272;
                        register u32 result asm("r0");
                        asm volatile("" : "+r"(result_ptr));
                        result = *result_ptr;
                        if (result != 0) {
                            if (result == selected) {
                                register u32 next_selected asm("r0") = selected;
                                asm volatile("" : "+r"(next_selected));
                                next_selected--;
                                next_selected <<= 24;
                                next_selected >>= 24;
                                selected = next_selected;
                            }
                            if (selected < first_visible) {
                                register u32 old_first asm("r2") = first_visible;
                                register u32 next_first asm("r0");
                                asm volatile("" : "+r"(old_first));
                                if (old_first > 4) {
                                    next_first = first_visible - 5;
                                    next_first <<= 24;
                                    next_first >>= 24;
                                } else {
                                    next_first = 0;
                                }
                                first_visible = next_first;
                            }
                            goto next_state;
                        }
                    }
                    DestroyPlayerSelectionStatusSprites(5);
                    DestroySprite(frame[2]);
                    RunMenuScript(0x080050BF);
                    RunMenuScript(0x08004FAE);
                    return;
                }
                PlaySong(0x58);
                RunMenuScript(0x08005180);
            }
        }
        RunMenuScript(0x080050EF);
        DisableDisplayWindows();
        goto next_state;
    }
    default:
        goto continue_dispatch;
    }

next_state:
    state = ZOID_REPAIR_BUILD_LIST;
continue_dispatch:
    {
        register s32 zero asm("r1") = 0;
        asm volatile("" : "+r"(zero));
        if (zero != 0) {
            return;
        }
    }
    goto dispatch;
}
