#ifndef EVENT_SCRIPT_H
#define EVENT_SCRIPT_H

enum EventOpcode {
    EVENT_END = 0x00,
    EVENT_ENTER_FIELD = 0x01,
    EVENT_ENTER_BATTLE = 0x02,
    EVENT_ENTER_BATTLE_SCENE = 0x03,
    EVENT_START_BATTLE = 0x04,
    EVENT_SPAWN_ACTOR = 0x05,
    EVENT_SPAWN_ACTOR_RELATIVE = 0x06,
    EVENT_REMOVE_ACTOR = 0x08,
    EVENT_START_SCRIPTS = 0x09,
    EVENT_JUMP = 0x0A,
    EVENT_RESTART = 0x0B,
    EVENT_WAIT_FRAMES = 0x0D,
    EVENT_YES_NO = 0x0E,
    EVENT_BRANCH_TRUE = 0x0F,
    EVENT_BRANCH_FALSE = 0x10,
    EVENT_CHOICE = 0x11,
    EVENT_CHOICE_CASE = 0x12,
    EVENT_IF_INTERACTED_ACTOR = 0x13,
    EVENT_IF_PLAYER_IN_RECT = 0x14,
    EVENT_IF_PLAYER_INTERACTION = 0x15,
    EVENT_IF_FLAG_SET = 0x16,
    EVENT_IF_FLAG_CLEAR = 0x17,
    EVENT_ALTERNATIVE = 0x18,
    EVENT_IF_PARTY_RESTRICTION = 0x19,
    EVENT_SET_FLAG = 0x1E,
    EVENT_CLEAR_FLAG = 0x1F,
    EVENT_TEXT = 0x20,
    EVENT_CHANGE_MAP = 0x42,
    EVENT_ITEM_SHOP = 0x46,
    EVENT_WEAPON_SHOP = 0x47,
    EVENT_ARMOR_SHOP = 0x48,
    EVENT_RETURN_TO_TITLE = 0x83
};

enum EventScanSelector {
    EVENT_SCAN_NEXT = -1
};

enum EventScriptLimits {
    EVENT_SCRIPT_SLOT_COUNT = 70
};

enum EventHandlerResult {
    EVENT_CONTINUE = 0,
    EVENT_YIELD = 1
};

struct EventFlagCommand {
    u8 opcode;
    u8 flag_id;
};

struct EventActorConditionCommand {
    u8 opcode;
    u8 actor_id;
};

struct EventBattleCommand {
    u8 opcode;
    u8 encounter_id;
    u8 formation_id;
    u8 terrain_id;
    u8 options;
};

#endif
