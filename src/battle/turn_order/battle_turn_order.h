#ifndef BATTLE_TURN_ORDER_H
#define BATTLE_TURN_ORDER_H

#include "../battle.h"

enum BattleTurnOrderLimits {
    BATTLE_TURN_ORDER_ENTRY_COUNT = 36,
    BATTLE_TURN_ORDER_EMPTY_ENTRY = 0xFF,
    BATTLE_TURN_ORDER_STATE_OFFSET = 0x270C,
    BATTLE_TURN_ORDER_PRIORITIES_FROM_SIDE = 0x49,
    BATTLE_TURN_ORDER_PRIORITIES_FROM_MODE = 0x4D
};

enum BattleTurnOrderMode {
    BATTLE_TURN_ORDER_DESCENDING_INITIATIVE = 0,
    BATTLE_TURN_ORDER_ASCENDING_INITIATIVE = 1,
    BATTLE_TURN_ORDER_FIRST_EMPTY_ENTRY = 2
};

struct BattleTurnOrderEntry {
    u8 side;
    u8 unit_slot;
} __attribute__((packed));

struct BattleTurnOrderView {
    u8 elapsed_rounds;
    u8 escape_roll;
    u8 data02;
    u8 mode;
    u8 current_entry;
    u8 entry_count;
    u8 tie_preferred_side;
    struct BattleTurnOrderEntry entries[BATTLE_TURN_ORDER_ENTRY_COUNT];
    u8 data4F;
    s16 priorities[BATTLE_TURN_ORDER_ENTRY_COUNT];
};

struct BattleTurnOrderStateView {
    u8 data0000[BATTLE_TURN_ORDER_STATE_OFFSET];
    struct BattleTurnOrderView turn_order;
};

#define BATTLE_TURN_ORDER_OFFSET(field) \
    ((s32)&((struct BattleTurnOrderStateView *)0)->turn_order.field)

#endif
