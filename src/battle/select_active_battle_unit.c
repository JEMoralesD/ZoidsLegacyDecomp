#include "m2c_prelude.h"
#include "battle_display.h"

M2C_UNK PlaySong(s32) asm("func_08092E84");                         /* extern */
void *CreateSpriteFromTable(M2C_UNK, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08094374"); /* extern */
M2C_UNK DestroySprite(void *) asm("func_08094554");                      /* extern */
M2C_UNK ResetMenuKeyRepeat() asm("func_08096F3C");                            /* extern */
M2C_UNK LoadSpriteGraphicsFromTable(M2C_UNK, s32, s32, s32) asm("func_0809AA64");      /* extern */
M2C_UNK StartBattleCameraTransition(s32, u8, s32, s32) asm("func_080BB224");           /* extern */
s32 IsBattleUnitActive(u8, u8) asm("func_080E9D88");                          /* extern */
s32 ModuloSigned32(s32, s32) asm("func_080ECE30");                        /* extern */
s32 DivideUnsigned32(u8, s32) asm("func_080ECF00");                         /* extern */
u8 ModuloUnsigned32(u32, s32) asm("func_080ECF78");                         /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */

#define CARRY_R0(value) ({ register s32 v asm("r0") = (value); asm volatile("" : "+r"(v)); v; })
#define CARRY_R1(value) ({ register s32 v asm("r1") = (value); asm volatile("" : "+r"(v)); v; })
#define CARRY_R2(value) ({ register s32 v asm("r2") = (value); asm volatile("" : "+r"(v)); v; })
#define CARRY_R3(value) ({ register s32 v asm("r3") = (value); asm volatile("" : "+r"(v)); v; })

enum BattleUnitSelectionResources {
    BATTLE_UNIT_SELECTION_GRAPHICS = 0x087AC9D8,
    BATTLE_UNIT_SELECTION_SPRITES = 0x087AC9E0,
    BATTLE_UNIT_SELECTION_TILE_BASE = 0x360,
    BATTLE_UNIT_SELECTION_PALETTE_BANK = 0xD,
    BATTLE_UNIT_SELECTION_CURSOR_FLAGS = 0x60
};

enum BattleUnitSelectionInput {
    BATTLE_UNIT_SELECTION_KEY_CONFIRM = 0x01,
    BATTLE_UNIT_SELECTION_KEY_CANCEL = 0x02,
    BATTLE_UNIT_SELECTION_KEY_RIGHT = 0x10,
    BATTLE_UNIT_SELECTION_KEY_LEFT = 0x20,
    BATTLE_UNIT_SELECTION_KEY_UP = 0x40,
    BATTLE_UNIT_SELECTION_KEY_DOWN = 0x80,
    BATTLE_UNIT_SELECTION_CONFIRM_SOUND = 0x3E,
    BATTLE_UNIT_SELECTION_CANCEL_SOUND = 0x3F,
    BATTLE_UNIT_SELECTION_MOVE_SOUND = 0x40
};

u8 SelectActiveBattleUnit(s32 side, s32 preferred_unit_slot) asm("func_080C05A8");

u8 SelectActiveBattleUnit(s32 side, s32 preferred_unit_slot) {
    volatile s32 saved_group_boundary;
    s16 next_candidate_signed;
    s32 sprite_scale;
    s16 next_candidate;
    s32 next_group_probe;
    s32 previous_other_group_start;
    s32 next_other_group_start;
    u16 unused_initial_keys;
    u16 unused_next_keys;
    u16 unused_first_group_keys;
    u16 unused_second_group_keys;
    u16 unused_confirmation_keys;
    u8 side_byte;
    u8 slot_within_group;
    u8 initial_active_slot;
    u8 second_group_candidate_byte;
    u8 previous_candidate_byte;
    u8 previous_other_candidate_byte;
    u8 next_other_candidate_byte;
    u8 first_group_paired_slot;
    u8 first_group_candidate_byte;
    u8 second_group_paired_slot;
    u8 next_candidate_byte;
    u8 preferred_unit_slot_byte;
    u8 previous_selected_slot;
    u16 initial_candidate;
    u8 selected_unit_slot;
    void *selected_unit_sprite;
    void *selection_cursor;

    side_byte = side;
    preferred_unit_slot_byte = preferred_unit_slot;
    LoadSpriteGraphicsFromTable(BATTLE_UNIT_SELECTION_GRAPHICS, 0, BATTLE_UNIT_SELECTION_TILE_BASE, BATTLE_UNIT_SELECTION_PALETTE_BANK);
    selection_cursor = CreateSpriteFromTable(BATTLE_UNIT_SELECTION_SPRITES, 0, 0, 0, 0, BATTLE_UNIT_SELECTION_TILE_BASE, BATTLE_UNIT_SELECTION_PALETTE_BANK, BATTLE_UNIT_SELECTION_CURSOR_FLAGS, 0);
    initial_candidate = preferred_unit_slot_byte;
    while ((IsBattleUnitActive(side_byte, initial_active_slot = initial_candidate) << 0x18) == 0) {
        initial_candidate = ModuloSigned32((s16) initial_candidate + 1, 6);
        if ((s16) initial_candidate == preferred_unit_slot_byte) {
            asm volatile(
                ".syntax unified\n\t"
                "movs r0, #255\n\t"
                "b .Lsub_080C05A8_return\n\t"
                ".syntax divided"
                : : : "r0");
        }
    }
    selected_unit_slot = initial_active_slot;
    StartBattleCameraTransition(2, side_byte, 0, 0);
    ResetMenuKeyRepeat();
selection_input_loop:
        previous_selected_slot = selected_unit_slot;
        asm volatile("" :: "r"(previous_selected_slot));
        {
        register u16 keys asm("r1") = *(u16 *)0x03006034;
        if (((BATTLE_UNIT_SELECTION_KEY_UP & keys) && (side_byte == 0)) || ((BATTLE_UNIT_SELECTION_KEY_DOWN & keys) && (side_byte != 0))) {
            slot_within_group = ModuloUnsigned32((u32) selected_unit_slot, 3);
            if (slot_within_group != 0) {
                register s32 previous_candidate asm("r5");
                register s32 previous_candidate_signed asm("r6");
                register s32 current_group_start asm("r2");
                asm volatile(
                    ".syntax unified\n\t"
                    "subs r0, %1, #1\n\t"
                    "lsls r0, r0, #16\n\t"
                    "lsrs %0, r0, #16\n\t"
                    ".syntax divided"
                    : "=r"(previous_candidate) : "r"(selected_unit_slot) : "r0");
                current_group_start = selected_unit_slot - slot_within_group;
                while ((s32) ({
                    asm volatile(
                        ".syntax unified\n\t"
                        "lsls r0, %1, #16\n\t"
                        "asrs %0, r0, #16\n\t"
                        ".syntax divided"
                        : "=r"(previous_candidate_signed) : "r"(previous_candidate) : "r0");
                    previous_candidate_signed;
                }) >= current_group_start) {
                    previous_candidate_byte = (u8) previous_candidate;
                    asm volatile("" :: "r"(previous_candidate));
                    if ((s32) ({
                        register s32 active_result asm("r0") = side_byte;
                        register s32 candidate_unit_slot asm("r1") = previous_candidate_byte;
                        asm volatile(
                            ".syntax unified\n\t"
                            "str %2, [sp, #20]\n\t"
                            "bl func_080E9D88\n\t"
                            ".syntax divided"
                            : "+r"(active_result), "+r"(candidate_unit_slot), "+r"(current_group_start)
                            : : "r3", "lr", "cc", "memory");
                        active_result <<= 0x18;
                        current_group_start = saved_group_boundary;
                        active_result;
                    }) != 0) {
                        selected_unit_slot = previous_candidate_byte;
                        PlaySong(BATTLE_UNIT_SELECTION_MOVE_SOUND);
                        break;
                    }
                    asm volatile(
                        ".syntax unified\n\t"
                        "subs r0, %1, #1\n\t"
                        "lsls r0, r0, #16\n\t"
                        "lsrs %0, r0, #16\n\t"
                        ".syntax divided"
                        : "=r"(previous_candidate) : "r"(previous_candidate_signed) : "r0");
                }
                if (selected_unit_slot == previous_selected_slot) {
                    register s32 count asm("r0");
                    register u16 previous_other_candidate asm("r5");
                    register s32 previous_other_candidate_signed asm("r6");
                    register s32 previous_other_group_boundary asm("r2");
                    register s32 base asm("r0") = 0;
                    if ((u32) selected_unit_slot <= 2U) {
                        base = 3;
                    }
                    asm volatile("" : "+r"(base));
                    previous_other_group_start = base;
                    count = ModuloUnsigned32((u32) selected_unit_slot, 3);
                    {
                        register s32 previous_slot_delta asm("r2") = 0xFFFF;
                        asm volatile(
                            ".syntax unified\n\t"
                            "adds r0, %1, %2\n\t"
                            "adds r0, %3, r0\n\t"
                            "lsls r0, r0, #16\n\t"
                            "lsrs %0, r0, #16\n\t"
                            ".syntax divided"
                            : "=r"(previous_other_candidate), "+r"(count)
                            : "r"(previous_slot_delta), "r"(previous_other_group_start));
                    }
                    previous_other_group_boundary = previous_other_group_start;
                    while ((s32) ({
                        asm volatile(
                            ".syntax unified\n\t"
                            "lsls r0, %1, #16\n\t"
                            "asrs %0, r0, #16\n\t"
                            ".syntax divided"
                            : "=r"(previous_other_candidate_signed) : "r"(previous_other_candidate) : "r0");
                        previous_other_candidate_signed;
                    }) >= previous_other_group_boundary) {
                        previous_other_candidate_byte = (u8) previous_other_candidate;
                        asm volatile("" :: "r"(previous_other_candidate));
                        if ((s32) ({
                            register s32 active_result asm("r0") = side_byte;
                            register s32 candidate_unit_slot asm("r1") = previous_other_candidate_byte;
                            asm volatile(
                                ".syntax unified\n\t"
                                "str %2, [sp, #20]\n\t"
                                "bl func_080E9D88\n\t"
                                ".syntax divided"
                                : "+r"(active_result), "+r"(candidate_unit_slot), "+r"(previous_other_group_boundary)
                                : : "r3", "lr", "cc", "memory");
                            active_result <<= 0x18;
                            previous_other_group_boundary = saved_group_boundary;
                            active_result;
                        }) != 0) {
                            selected_unit_slot = previous_other_candidate_byte;
                            PlaySong(BATTLE_UNIT_SELECTION_MOVE_SOUND);
                            break;
                        }
                        asm volatile(
                            ".syntax unified\n\t"
                            "subs r0, %1, #1\n\t"
                            "lsls r0, r0, #16\n\t"
                            "lsrs %0, r0, #16\n\t"
                            ".syntax divided"
                            : "=r"(previous_other_candidate) : "r"(previous_other_candidate_signed) : "r0");
                    }
                }
            }
        }
        }
        {
        register u16 keys asm("r1") = *(u16 *)0x03006034;
        if ((((BATTLE_UNIT_SELECTION_KEY_UP & keys) && (CARRY_R3(side_byte) != 0)) || ((BATTLE_UNIT_SELECTION_KEY_DOWN & keys) && (CARRY_R0(side_byte) == 0))) && ((u32) ModuloUnsigned32((u32) selected_unit_slot, 3) <= 1U)) {
            register s32 current_group_limit asm("r4");
            next_candidate = selected_unit_slot + 1;
            asm volatile("" :: "r"(next_candidate));
            next_group_probe = selected_unit_slot + 3;
            current_group_limit = next_group_probe - ModuloSigned32(next_group_probe, 3);
            asm volatile(
                ".syntax unified\n\t"
                "lsls %0, %0, #16\n\t"
                "asrs %0, %0, #16\n\t"
                ".syntax divided"
                : "+r"(current_group_limit));
            while ((s32) (next_candidate_signed = next_candidate) < (s32) current_group_limit) {
                next_candidate_byte = (u8) next_candidate;
                if ((IsBattleUnitActive(side_byte, next_candidate_byte) << 0x18) != 0) {
                    selected_unit_slot = next_candidate_byte;
                    PlaySong(BATTLE_UNIT_SELECTION_MOVE_SOUND);
                    break;
                }
                next_candidate = (s16) (u16) (next_candidate_signed + 1);
            }
            if (selected_unit_slot == previous_selected_slot) {
                register s32 count asm("r0");
                register s32 next_other_candidate asm("r5");
                register s32 next_other_candidate_signed asm("r6");
                register s32 next_other_group_limit asm("r2");
                register s32 base asm("r0") = 0;
                if ((u32) selected_unit_slot <= 2U) {
                    base = 3;
                }
                asm volatile("" : "+r"(base));
                next_other_group_start = base;
                count = ModuloUnsigned32((u32) selected_unit_slot, 3);
                asm volatile(
                    ".syntax unified\n\t"
                    "adds %1, %1, #1\n\t"
                    "adds %0, %2, %1\n\t"
                    ".syntax divided"
                    : "=r"(next_other_candidate), "+r"(count) : "r"(next_other_group_start));
                next_other_group_limit = next_other_group_start + 3;
                while ((s32) ({
                    asm volatile(
                        ".syntax unified\n\t"
                        "lsls r0, %1, #16\n\t"
                        "asrs %0, r0, #16\n\t"
                        ".syntax divided"
                        : "=r"(next_other_candidate_signed) : "r"(next_other_candidate) : "r0");
                    next_other_candidate_signed;
                }) < next_other_group_limit) {
                    next_other_candidate_byte = (u8) next_other_candidate;
                    asm volatile("" :: "r"(next_other_candidate));
                    if ((s32) ({
                        register s32 active_result asm("r0") = side_byte;
                        register s32 candidate_unit_slot asm("r1") = next_other_candidate_byte;
                        asm volatile(
                            ".syntax unified\n\t"
                            "str %2, [sp, #20]\n\t"
                            "bl func_080E9D88\n\t"
                            ".syntax divided"
                            : "+r"(active_result), "+r"(candidate_unit_slot), "+r"(next_other_group_limit)
                            : : "r3", "lr", "cc", "memory");
                            active_result <<= 0x18;
                        next_other_group_limit = saved_group_boundary;
                        active_result;
                    }) != 0) {
                        selected_unit_slot = next_other_candidate_byte;
                        PlaySong(BATTLE_UNIT_SELECTION_MOVE_SOUND);
                        break;
                    }
                    asm volatile(
                        ".syntax unified\n\t"
                        "adds r0, %1, #1\n\t"
                        "lsls r0, r0, #16\n\t"
                        "lsrs %0, r0, #16\n\t"
                        ".syntax divided"
                        : "=r"(next_other_candidate) : "r"(next_other_candidate_signed) : "r0");
                }
            }
        }
        }
        {
        register u16 keys asm("r1") = *(u16 *)0x03006034;
        if ((((BATTLE_UNIT_SELECTION_KEY_LEFT & keys) && (CARRY_R2(side_byte) == 0)) || ((BATTLE_UNIT_SELECTION_KEY_RIGHT & keys) && (CARRY_R3(side_byte) != 0))) && ((DivideUnsigned32(selected_unit_slot, 3) << 0x18) != 0)) {
            first_group_paired_slot = selected_unit_slot - 3;
            if ((IsBattleUnitActive(side_byte, first_group_paired_slot) << 0x18) != 0) {
                selected_unit_slot = first_group_paired_slot;
                PlaySong(BATTLE_UNIT_SELECTION_MOVE_SOUND);
            } else {
                register s32 first_group_candidate asm("r5");
                register s32 first_group_candidate_signed asm("r6");
                first_group_candidate = 0;
                while ((s32) ({
                    asm volatile(
                        ".syntax unified\n\t"
                        "lsls r0, %1, #16\n\t"
                        "asrs %0, r0, #16\n\t"
                        ".syntax divided"
                        : "=r"(first_group_candidate_signed) : "r"(first_group_candidate) : "r0");
                    first_group_candidate_signed;
                }) <= 2) {
                    first_group_candidate_byte = (u8) first_group_candidate;
                    asm volatile("" :: "r"(first_group_candidate));
                    if ((IsBattleUnitActive(side_byte, first_group_candidate_byte) << 0x18) != 0) {
                        selected_unit_slot = first_group_candidate_byte;
                        PlaySong(BATTLE_UNIT_SELECTION_MOVE_SOUND);
                        break;
                    }
                    asm volatile(
                        ".syntax unified\n\t"
                        "adds r0, %1, #1\n\t"
                        "lsls r0, r0, #16\n\t"
                        "lsrs %0, r0, #16\n\t"
                        ".syntax divided"
                        : "=r"(first_group_candidate) : "r"(first_group_candidate_signed) : "r0");
                }
            }
        }
        }
        {
        register u16 keys asm("r1") = *(u16 *)0x03006034;
        if ((((BATTLE_UNIT_SELECTION_KEY_LEFT & keys) && (CARRY_R0(side_byte) != 0)) || ((BATTLE_UNIT_SELECTION_KEY_RIGHT & keys) && (CARRY_R1(side_byte) == 0))) && ((DivideUnsigned32(selected_unit_slot, 3) << 0x18) == 0)) {
            second_group_paired_slot = selected_unit_slot + 3;
            if ((IsBattleUnitActive(side_byte, second_group_paired_slot) << 0x18) != 0) {
                selected_unit_slot = second_group_paired_slot;
                PlaySong(BATTLE_UNIT_SELECTION_MOVE_SOUND);
            } else {
                register s32 second_group_candidate asm("r5");
                register s32 second_group_candidate_signed asm("r6");
                second_group_candidate = 3;
                while ((s32) ({
                    asm volatile(
                        ".syntax unified\n\t"
                        "lsls r0, %1, #16\n\t"
                        "asrs %0, r0, #16\n\t"
                        ".syntax divided"
                        : "=r"(second_group_candidate_signed) : "r"(second_group_candidate) : "r0");
                    second_group_candidate_signed;
                }) <= 5) {
                    second_group_candidate_byte = (u8) second_group_candidate;
                    asm volatile("" :: "r"(second_group_candidate));
                    if ((IsBattleUnitActive(side_byte, second_group_candidate_byte) << 0x18) != 0) {
                        selected_unit_slot = second_group_candidate_byte;
                        PlaySong(BATTLE_UNIT_SELECTION_MOVE_SOUND);
                        break;
                    }
                    asm volatile(
                        ".syntax unified\n\t"
                        "adds r0, %1, #1\n\t"
                        "lsls r0, r0, #16\n\t"
                        "lsrs %0, r0, #16\n\t"
                        ".syntax divided"
                        : "=r"(second_group_candidate) : "r"(second_group_candidate_signed) : "r0");
                }
            }
        }
        }
        {
        register u16 keys asm("r1") = *(u16 *)0x0300000E;
        if (BATTLE_UNIT_SELECTION_KEY_CONFIRM & keys) {
            PlaySong(BATTLE_UNIT_SELECTION_CONFIRM_SOUND);
        } else if (BATTLE_UNIT_SELECTION_KEY_CANCEL & keys) {
            selected_unit_slot = BATTLE_UNIT_NOT_SELECTED;
            PlaySong(BATTLE_UNIT_SELECTION_CANCEL_SOUND);
        } else {
            register u32 table asm("r2") = 0x02032E8C;
            register u32 slot asm("r1") = (u32) selected_unit_slot << 2;
            register u32 side asm("r3") = side_byte;
            register u32 offset asm("r0") = side << 1;
            asm volatile("" : "+r"(table));
            asm volatile("" : "+r"(slot));
            asm volatile("" : "+r"(side));
            asm volatile("" : "+r"(offset));
            offset += side_byte;
            offset <<= 3;
            slot += offset;
            slot += table;
            BATTLE_SPRITE_FIELD(selection_cursor, u16, x) = (u16) BATTLE_SPRITE_FIELD(M2C_FIELD(slot, void **, 0), u16, x);
            asm volatile("" ::: "memory");
            selected_unit_sprite = M2C_FIELD(slot, void **, 0);
            asm volatile(
                ".syntax unified\n\t"
                "movs r3, #12\n\t"
                "ldrsh %0, [%1, r3]\n\t"
                ".syntax divided"
                : "=l"(sprite_scale) : "l"(selected_unit_sprite) : "r3");
            if ((s32) sprite_scale < 0) {
                sprite_scale += 7;
            }
            BATTLE_SPRITE_FIELD(selection_cursor, s16, y) = (s16) (BATTLE_SPRITE_FIELD(selected_unit_sprite, u16, y) - (sprite_scale >> 3));
            YieldTaskForUpdates(1);
            goto selection_input_loop;
        }
        }
    DestroySprite(selection_cursor);
    {
        register u8 result asm("r0") = selected_unit_slot;
        asm volatile(".Lsub_080C05A8_return:" : : "r"(result));
        return result;
    }
}
