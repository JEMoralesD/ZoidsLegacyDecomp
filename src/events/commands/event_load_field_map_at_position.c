#include "m2c_prelude.h"
#include "../event_script.h"

extern u8 gFieldEventActive asm("D_02030664");
extern struct EventFieldMapStateView gEventFieldMapState asm("D_0202ECF4");
extern u8 gEventMapId;
extern u8 gFieldMapModeFlags asm("D_020324B0");
extern struct EventFieldMapGroupView gEventFieldMapGroups[] asm("D_087C4434");

void StopTask(s32) asm("func_08092E0C");
s32 TestEventFlag(s32) asm("func_0809F818");
void StartTaskWithArgument(s32, s32, u8) asm("func_08092D9C");
void ClearSpritePools(void) asm("func_08094330");
void InitializeEventSpritePool(void) asm("func_0809F850");
void ResetFieldActors(void) asm("func_080A9888");
void InitializeFieldMapGraphics(u8, s32, s32) asm("func_0809D938");
void SeekEventCommand(s32, s32, s32) asm("func_080A016C");

s32 EventLoadFieldMapAtPosition(s32 script_slot, struct EventFieldMapPositionCommand **cursor) asm("func_080A1F68");

s32 EventLoadFieldMapAtPosition(s32 script_slot, struct EventFieldMapPositionCommand **cursor) {
    struct EventFieldMapPositionCommand **command_slot;
    register s32 saved_script_slot asm("r9");
    register s32 cell_size_pixels asm("r8");
    volatile struct EventFieldMapPositionCommand *command;
    register s32 map_id asm("r1");

    command_slot = cursor;
    script_slot <<= 24;
    saved_script_slot = (u32)script_slot >> 24;
    StopTask(FIELD_MAP_TRANSITION_TASK);
    gFieldEventActive = 1;
    command = *command_slot;
    map_id = command->map_id;
    cell_size_pixels = FIELD_MAP_CELL_PIXELS;
    if (map_id == 0) {
        cell_size_pixels = FIELD_PACKED_MAP_CELL_PIXELS;
    }

    if (gEventMapId != map_id) {
        u32 map_group_id;

        map_group_id = gEventFieldMapGroups[command->map_id].map_group_id;
        if (map_group_id != 0) {
            u8 previous_map_group_id;

            previous_map_group_id = gEventFieldMapGroups[gEventFieldMapState.map_id].map_group_id;
            if (previous_map_group_id != map_group_id || map_group_id == 0x1D ||
                (u8)(map_group_id - 0x21) <= 9) {
                register s32 map_group_index asm("r6");
                register s32 test_result asm("r0");

                test_result = TestEventFlag(0);
                test_result <<= 24;
                map_group_index = map_group_id - 1;
                asm volatile("" : "+r"(map_group_id));
                if (test_result != 0) {
                    test_result = TestEventFlag(0x8A);
                    test_result <<= 24;
                    if (test_result == 0) {
                        register s32 command_address_or_map_id asm("r3");
                        struct EventFieldMapStateView *map_state;
                        struct EventFieldMapStateView *map_state_source;
                        s32 current_map_id;
                        register s32 map_transition_task_thumb asm("r12");

                        map_state_source = &gEventFieldMapState;
                        current_map_id = map_state_source->map_id;
                        command_address_or_map_id = (s32)*command_slot;
                        map_state = map_state_source;
                        asm volatile("" : : "r"(map_state));
                        if (current_map_id != 0x40 ||
                            ((struct EventFieldMapPositionCommand *)command_address_or_map_id)->map_id !=
                                0x3F) {
                            register s32 word_offset asm("r2");
                            register s32 field_offset asm("r1");
                            register s32 address asm("r0");
                            register u32 shift asm("r0");
                            register u32 bit asm("r1");
                            register u32 visited_group_word asm("r0");

                            word_offset = map_group_id >> 5;
                            word_offset <<= 2;
                            field_offset = EVENT_COMMAND_OFFSET(EventFieldMapStateView, visited_map_group_bits) / 2;
                            field_offset <<= 1;
                            address = (s32)map_state + field_offset;
                            word_offset += address;
                            shift = 0x1F;
                            shift &= map_group_id;
                            bit = 1 << shift;
                            visited_group_word = *(u32 *)word_offset;
                            visited_group_word |= bit;
                            *(u32 *)word_offset = visited_group_word;
                        }

                        {
                            register s32 return_map_index asm("r2");
                            register struct EventFieldReturnMapEntry *return_map_table_source asm("r0");
                            register struct EventFieldReturnMapEntry *return_map_table asm("r5");
                            register struct EventFieldReturnMapEntry *return_map_scan asm("r4");
                            register u16 return_map_id asm("r1");

                            if ((u16)(map_state->map_id - 0x3A) <= 6) {
                                goto setup_return_map24;
                            }
                            return_map_index = 0;
                            return_map_table_source = (struct EventFieldReturnMapEntry *)FIELD_RETURN_MAP_TABLE_ROM;
                            return_map_id = M2C_FIELD(return_map_table_source, u16 *, EVENT_COMMAND_OFFSET(EventFieldReturnMapEntry, map_id));
                            return_map_table = return_map_table_source;
                            map_transition_task_thumb = FIELD_MAP_TRANSITION_TASK_THUMB;
                            map_group_index = map_group_id - 1;
                            if (return_map_id == 0) {
                                goto return_map_scan_done;
                            }
                            return_map_scan = return_map_table;
                            command_address_or_map_id =
                                ((struct EventFieldMapPositionCommand *)command_address_or_map_id)->map_id;
scan_return_map22:
                            {
                                register s32 offset asm("r0");

                                offset = return_map_index << 2;
                                offset += return_map_index;
                                offset <<= 1;
                                return_map_id = *(u16 *)(offset + (s32)return_map_scan);
                            }
                            if (return_map_id == command_address_or_map_id) {
                                goto store_return_map22;
                            }
                            {
                                register u8 next asm("r0");

                                next = return_map_index + 1;
                                return_map_index = next;
                            }
                            {
                                register s32 offset asm("r0");

                                offset = return_map_index << 2;
                                offset += return_map_index;
                                offset <<= 1;
                                if (*(u16 *)(offset + (s32)return_map_table) != 0) {
                                    goto scan_return_map22;
                                }
                            }
                            goto return_map_scan_done;

store_return_map22:
                            gEventFieldMapState.return_map_id22 = return_map_id;
                            goto return_map_scan_done;

store_return_map24:
                            gEventFieldMapState.return_map_id24 = return_map_id;
                            goto return_map_scan_done;

setup_return_map24:
                            return_map_index = 0;
                            return_map_table_source = (struct EventFieldReturnMapEntry *)FIELD_RETURN_MAP_TABLE24_ROM;
                            return_map_id = M2C_FIELD(return_map_table_source, u16 *, EVENT_COMMAND_OFFSET(EventFieldReturnMapEntry, map_id));
                            return_map_table = return_map_table_source;
                            map_transition_task_thumb = FIELD_MAP_TRANSITION_TASK_THUMB;
                            map_group_index = map_group_id - 1;
                            if (return_map_id == 0) {
                                goto return_map_scan_done;
                            }
                            return_map_scan = return_map_table;
                            command_address_or_map_id =
                                ((struct EventFieldMapPositionCommand *)command_address_or_map_id)->map_id;
scan_return_map24:
                            {
                                register s32 offset asm("r0");

                                offset = return_map_index << 2;
                                offset += return_map_index;
                                offset <<= 1;
                                return_map_id = *(u16 *)(offset + (s32)return_map_scan);
                            }
                            if (return_map_id == command_address_or_map_id) {
                                goto store_return_map24;
                            }
                            {
                                register u8 next asm("r0");

                                next = return_map_index + 1;
                                return_map_index = next;
                            }
                            {
                                register s32 offset asm("r0");

                                offset = return_map_index << 2;
                                offset += return_map_index;
                                offset <<= 1;
                                if (*(u16 *)(offset + (s32)return_map_table) != 0) {
                                    goto scan_return_map24;
                                }
                            }
                        }
return_map_scan_done:
                        StartTaskWithArgument(FIELD_MAP_TRANSITION_TASK, map_transition_task_thumb,
                                      (*command_slot)->map_id);
                    }
                }
                {
                    register struct EventFieldMapStateView *world_map_save_state asm("r3");
                    register u8 *group_world_position_bytes asm("r2");
                    register s32 offset asm("r1");
                    register s32 address asm("r0");
                    register s32 world_position_fixed8 asm("r0");

                    world_map_save_state = &gEventFieldMapState;
                    group_world_position_bytes = (u8 *)FIELD_MAP_GROUP_WORLD_POSITIONS_ROM;
                    asm volatile("" : "+r"(group_world_position_bytes));
                    offset = map_group_index << 1;
                    offset += map_group_index;
                    offset <<= 1;
                    address = (s32)group_world_position_bytes + 2;
                    address = offset + address;
                    world_position_fixed8 = *(s16 *)address;
                    world_position_fixed8 <<= 11;
                    world_map_save_state->saved_world_map_x_fixed8 = world_position_fixed8;
                    group_world_position_bytes += 4;
                    offset += (s32)group_world_position_bytes;
                    world_position_fixed8 = *(s16 *)offset;
                    world_position_fixed8 <<= 11;
                    world_map_save_state->saved_world_map_y_fixed8 = world_position_fixed8;
                }
            }
        }
        if (gEventMapId != (*command_slot)->map_id) {
            ClearSpritePools();
            InitializeEventSpritePool();
            ResetFieldActors();
        }
    }

    gFieldMapModeFlags = 1;
    {
        u32 destination_map_id;

        asm volatile("" : : "r"(&gEventFieldMapState), "r"(&gEventMapId));
        destination_map_id = (*command_slot)->map_id;
        gEventMapId = destination_map_id;
        asm volatile("" : "+r"(destination_map_id));
        destination_map_id <<= 24;
        destination_map_id >>= 24;
        gEventFieldMapState.map_id = destination_map_id;
    }
    command = *command_slot;
    InitializeFieldMapGraphics(command->map_id,
                  (cell_size_pixels * command->map_cell_x) << 8,
                  (cell_size_pixels * command->map_cell_y) << 8);
    {
        register s32 next_command_selector asm("r1");

        next_command_selector = EVENT_SCAN_NEXT;
        SeekEventCommand(saved_script_slot, next_command_selector, 0);
    }
    return EVENT_CONTINUE;
}
