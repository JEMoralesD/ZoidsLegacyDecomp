#ifndef BATTLE_COMBINATION_H
#define BATTLE_COMBINATION_H

#include "../battle_display.h"
#include "../turn_order/battle_turn_order.h"
#include "../../game/player_state.h"
#include "../../graphics/camera.h"

enum BattleCombinationRecipeId {
    BATTLE_COMBINATION_TWO_ARM_LIZARD = 0,
    BATTLE_COMBINATION_FUZOR_DRAGON = 1,
    BATTLE_COMBINATION_CHIMERA_DRAGON = 2,
    BATTLE_COMBINATION_GOJULOX = 3,
    BATTLE_COMBINATION_KILLER_SPINER = 4,
    BATTLE_COMBINATION_GOJULAS_GIGA_CANNON = 5,
    BATTLE_COMBINATION_GRIFFIN = 6,
    BATTLE_COMBINATION_LORD_GALE = 7
};

enum BattleCombinationFormationMode {
    BATTLE_COMBINATION_CONSECUTIVE_COMPONENTS = 0,
    BATTLE_COMBINATION_FRONT_REAR_PAIR = 1,
    BATTLE_COMBINATION_TWO_BY_TWO_COMPONENTS = 2,
    BATTLE_COMBINATION_HOST_AND_REAR_COMPONENTS = 3,
    BATTLE_COMBINATION_ALL_SIX_COMPONENTS = 4
};

enum BattleCombinationLimits {
    BATTLE_COMBINATION_RECIPE_COUNT = 8,
    BATTLE_COMBINATION_MODEL_COUNT = 6,
    BATTLE_COMBINATION_NO_OWNER = 0xFF,
    BATTLE_COMBINATION_FORMATION_TABLE_ROM = 0x087A2854,
    BATTLE_COMBINATION_COMPONENTS_OFFSET = 1
};

enum BattleCombinationPresentation {
    BATTLE_COMBINATION_UNCHANGED_UNIT = 0x80,
    BATTLE_COMBINATION_CHANGED_MODEL = 0xFF,
    BATTLE_COMBINATION_MODEL_FLAG = 0x20,
    BATTLE_COMBINATION_BLEND_CONTROL = 0x540,
    BATTLE_COMBINATION_ANIMATION_UPDATES = 16,
    BATTLE_COMBINATION_PROJECTED_SPRITE_CALLBACK = 0x080BADD5
};

struct BattleCombinationFormationRule {
    u8 mode;
    u8 model_ids[BATTLE_COMBINATION_MODEL_COUNT];
} __attribute__((packed));

struct BattleCombinationStateView {
    struct BattleSide sides[BATTLE_SIDE_COUNT];
    u8 current_unit_original_party_slots[BATTLE_ACTIVE_UNIT_COUNT];
    u8 combination_owner_original_party_slots[BATTLE_ACTIVE_UNIT_COUNT];
    struct BattleTurnOrderView turn_order;
    u8 active_side;
    u8 active_unit_slot;
    u8 data27A6[2];
};

#define BATTLE_COMBINATION_OFFSET(field) \
    ((s32)&((struct BattleCombinationStateView *)0)->field)

#endif
