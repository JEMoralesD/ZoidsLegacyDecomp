#include "m2c_prelude.h"
#include "battle.h"

enum BattleItemMenuState {
    BATTLE_ITEM_MENU_OPEN = 0,
    BATTLE_ITEM_MENU_SELECT = 0x100,
    BATTLE_ITEM_MENU_OPEN_TARGET = 0x200,
    BATTLE_ITEM_MENU_SELECT_TARGET = 0x210
};

struct BattleRecoveryTarget {
    u8 pad0[6];
    s16 hp;
    u8 pad8[0x32];
    s16 max_hp;
    u8 pad3C[0x234];
};

extern u8 D_0200A880;
extern u8 D_0200A881;
extern u8 D_0200A882;
extern u8 D_020322A8[];
extern u8 D_020322B1;
extern u8 gBattleState[];

extern void PlaySong(s32) asm("func_08092E84");
extern void *GetWindow(s32) asm("func_0809716C");
extern void func_080981F0(s32, s32, s32, s32, s32);
extern void ClearWindow(s32) asm("func_080986B4");
extern void RunMenuScript(s32) asm("func_08098BB4");
extern void func_080AC7BC(s32);
extern void func_080B654C(void);
extern u8 FindBattleEffect(s32, u8, s32) asm("func_080BF464");
extern u8 func_080C05A8(s32, u8);

static inline void reject_selection(void)
{
    PlaySong(0x58);
    RunMenuScript(0x08003AA9);
}

s32 SelectBattleRecoveryItem(void)
{
    s32 message;
    s32 state;
    s32 invalid;
    s32 recovery_kind;
    s32 response;
    u8 unit_slot;
    u8 menu_row;
    u8 menu_column;
    void *window;
    struct BattleRecoveryTarget *record;

    func_080B654C();
    if (D_020322B1 == 0)
        goto no_units;

    menu_column = 0;
    menu_row = 0;

restart:
    state = BATTLE_ITEM_MENU_OPEN;

state_loop:
    switch (state) {
    case BATTLE_ITEM_MENU_OPEN:
        RunMenuScript(0x08003990);
        func_080AC7BC(1);
        window = GetWindow(1);
        *(u8 *)((u8 *)window + 0x14) = menu_column;
        *(u8 *)((u8 *)window + 0x16) = menu_row;
        state = BATTLE_ITEM_MENU_SELECT;
        goto state_loop;
    case BATTLE_ITEM_MENU_SELECT: {
        register s32 *messages asm("r1");

        ClearWindow(2);
        messages = (s32 *)0x087EEE38;
        asm volatile("" : "+r"(messages));
        func_080981F0(messages[D_020322A8[menu_row]], 0, 2, 0, 0);
        RunMenuScript(0x080039E6);
        menu_column = D_0200A881;
        menu_row = D_0200A880;
        response = D_0200A882;

        if (response == 1)
            goto response_one;
        if (response <= 1)
            goto state_loop;
        if (response == 2)
            goto response_two;
        goto state_loop;

response_one:
        if ((u32)(u8)(D_020322A8[menu_row] - 8) <= 1) {
            PlaySong(0x58);
            RunMenuScript(0x08003A60);
        } else {
            state = BATTLE_ITEM_MENU_OPEN_TARGET;
        }
        goto state_loop;

response_two:
        message = 0x080039EA;
        goto cancel;
    }
    case BATTLE_ITEM_MENU_OPEN_TARGET:
        RunMenuScript(0x08003A28);
        RunMenuScript(0x080039EA);
        unit_slot = 0;
        state = BATTLE_ITEM_MENU_SELECT_TARGET;
        goto state_loop;
    case BATTLE_ITEM_MENU_SELECT_TARGET:
        goto select;
    default:
        goto state_loop;
    }

select:
    unit_slot = func_080C05A8(0, unit_slot);
    if (unit_slot == BATTLE_UNIT_NOT_SELECTED)
        goto restart;

    invalid = 0;
    recovery_kind = D_020322A8[menu_row] - 1;
    switch (recovery_kind) {
    case BATTLE_RECOVER_HP_300 - 1:
    case BATTLE_RECOVER_HP_150 - 1:
    case BATTLE_RECOVER_HP_50 - 1:
    case BATTLE_RECOVER_HALF_MAX_HP - 1: {
        s32 hp;
        s32 max_hp;

        {
            register struct BattleRecoveryTarget *record_base asm("r1") =
                (struct BattleRecoveryTarget *)gBattleState;

            record = (struct BattleRecoveryTarget *)(
                unit_slot * sizeof(struct BattleRecoveryTarget) -
                (0u - (u32)record_base));
        }
        hp = record->hp;
        max_hp = record->max_hp;
        if (hp == max_hp) {
            reject_selection();
            goto state_loop;
        }
        break;
    }
    case BATTLE_RECOVER_FREEZE - 1:
        if (FindBattleEffect(0, unit_slot, BATTLE_EFFECT_FREEZE) == BATTLE_EFFECT_NOT_FOUND) {
            reject_selection();
            goto state_loop;
        }
        break;
    case BATTLE_RECOVER_STATUS - 1:
        if (FindBattleEffect(0, unit_slot, BATTLE_EFFECT_FREEZE) == BATTLE_EFFECT_NOT_FOUND &&
            FindBattleEffect(0, unit_slot, BATTLE_EFFECT_CONFUSION) == BATTLE_EFFECT_NOT_FOUND &&
            FindBattleEffect(0, unit_slot, BATTLE_EFFECT_PILOT_INACTIVE) == BATTLE_EFFECT_NOT_FOUND) {
            reject_selection();
            goto state_loop;
        }
        break;
    case BATTLE_RECOVER_FULL - 1:
        if (FindBattleEffect(0, unit_slot, BATTLE_EFFECT_FREEZE) == BATTLE_EFFECT_NOT_FOUND &&
            FindBattleEffect(0, unit_slot, BATTLE_EFFECT_CONFUSION) == BATTLE_EFFECT_NOT_FOUND &&
            FindBattleEffect(0, unit_slot, BATTLE_EFFECT_PILOT_INACTIVE) == BATTLE_EFFECT_NOT_FOUND) {
            s32 hp;
            s32 max_hp;

            {
                register struct BattleRecoveryTarget *record_base asm("r1") =
                    (struct BattleRecoveryTarget *)gBattleState;

                record = (struct BattleRecoveryTarget *)(
                    unit_slot * sizeof(struct BattleRecoveryTarget) -
                    (0u - (u32)record_base));
            }
            hp = record->hp;
            max_hp = record->max_hp;
            if (hp == max_hp) {
                reject_selection();
                invalid = 1;
            }
        }
        break;
    }

    if (invalid != 0)
        goto state_loop;
    gBattleState[0xA07D] = D_020322A8[menu_row];
    gBattleState[0xA07E] = unit_slot;
    return 1;

no_units:
    message = 0x080039F0;
cancel:
    RunMenuScript(message);
    return 0;
}
