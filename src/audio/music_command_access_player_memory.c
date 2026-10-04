#include "sound_engine.h"

M2C_UNK CallFunctionR2(void *, void *, s32) asm("func_080ECD64");

void MusicCommandAccessPlayerMemory(void *player, void *track) asm("func_080EC900");

void MusicCommandAccessPlayerMemory(void *player, void *track) {
    register void *track_state asm("r6");
    register u8 *memory_cell_or_value asm("r3");
    register s32 operand asm("r2");
    u8 operation;
    register u8 *cursor_or_memory_base asm("r0");
    register s32 cursor_or_memory_index asm("r1");
    register u8 *operand_cursor asm("r2");

    track_state = track;
    cursor_or_memory_index = (s32)MUSIC_TRACK_FIELD(track_state, u8 **, command);
    operation = *(u8 *)cursor_or_memory_index;
    operand_cursor = (u8 *)cursor_or_memory_index + 1;
    MUSIC_TRACK_FIELD(track_state, u8 **, command) = operand_cursor;
    cursor_or_memory_base = MUSIC_PLAYER_FIELD(player, u8 **, memory);
    cursor_or_memory_index = *(u8 *)(cursor_or_memory_index + 1);
    memory_cell_or_value = (u8 *)(cursor_or_memory_index + (s32)cursor_or_memory_base);
    cursor_or_memory_base = operand_cursor + 1;
    MUSIC_TRACK_FIELD(track_state, u8 **, command) = cursor_or_memory_base;
    operand = operand_cursor[1];
    MUSIC_TRACK_FIELD(track_state, u8 **, command) = cursor_or_memory_base + 1;
    switch ((u32)operation) {
    case MUSIC_MEMORY_SET_IMMEDIATE:
        *memory_cell_or_value = operand;
        return;
    case MUSIC_MEMORY_ADD_IMMEDIATE: {
        register s32 current asm("r1");
        register s32 result asm("r0");
        current = *memory_cell_or_value;
        result = current + operand;
        *memory_cell_or_value = result;
        return;
    }
    case MUSIC_MEMORY_SUBTRACT_IMMEDIATE: {
        register s32 current asm("r1");
        register s32 result asm("r0");
        current = *memory_cell_or_value;
        result = current - operand;
        *memory_cell_or_value = result;
        return;
    }
    case MUSIC_MEMORY_COPY_BYTE:
        *memory_cell_or_value = MUSIC_PLAYER_FIELD(player, u8 **, memory)[operand];
        return;
    case MUSIC_MEMORY_ADD_BYTE: {
        register s32 current asm("r1");
        register s32 other asm("r0");
        other = (s32)(MUSIC_PLAYER_FIELD(player, u8 **, memory) + operand);
        current = *memory_cell_or_value;
        other = *(u8 *)other;
        other = current + other;
        *memory_cell_or_value = other;
        return;
    }
    case MUSIC_MEMORY_SUBTRACT_BYTE: {
        register s32 current asm("r1");
        register s32 other asm("r0");
        other = (s32)(MUSIC_PLAYER_FIELD(player, u8 **, memory) + operand);
        current = *memory_cell_or_value;
        other = *(u8 *)other;
        other = current - other;
        *memory_cell_or_value = other;
        return;
    }
    case MUSIC_MEMORY_BRANCH_EQUAL_IMMEDIATE:
        memory_cell_or_value = (u8 *)(u32)*memory_cell_or_value;
        if ((u32)memory_cell_or_value == (u32)operand) {
            goto invoke;
        }
        goto skip;
    case MUSIC_MEMORY_BRANCH_NOT_EQUAL_IMMEDIATE:
        memory_cell_or_value = (u8 *)(u32)*memory_cell_or_value;
        if ((u32)memory_cell_or_value != (u32)operand) {
            goto invoke;
        }
        goto skip;
    case MUSIC_MEMORY_BRANCH_GREATER_IMMEDIATE:
        memory_cell_or_value = (u8 *)(u32)*memory_cell_or_value;
        if ((u32)memory_cell_or_value > (u32)operand) {
            goto invoke;
        }
        goto skip;
    case MUSIC_MEMORY_BRANCH_GREATER_EQUAL_IMMEDIATE:
        memory_cell_or_value = (u8 *)(u32)*memory_cell_or_value;
        if ((u32)memory_cell_or_value >= (u32)operand) {
            goto invoke;
        }
        goto skip;
    case MUSIC_MEMORY_BRANCH_LESS_EQUAL_IMMEDIATE:
        memory_cell_or_value = (u8 *)(u32)*memory_cell_or_value;
        if ((u32)memory_cell_or_value <= (u32)operand) {
            goto invoke;
        }
        goto skip;
    case MUSIC_MEMORY_BRANCH_LESS_IMMEDIATE:
        memory_cell_or_value = (u8 *)(u32)*memory_cell_or_value;
        if ((u32)memory_cell_or_value < (u32)operand) {
            goto invoke;
        }
        goto skip;
    case MUSIC_MEMORY_BRANCH_EQUAL_BYTE: {
        register u8 *other asm("r0");
        other = MUSIC_PLAYER_FIELD(player, u8 **, memory) + operand;
        memory_cell_or_value = (u8 *)(u32)*memory_cell_or_value;
        if ((u32)memory_cell_or_value == (u32)*other) {
            goto invoke;
        }
        goto skip;
    }
    case MUSIC_MEMORY_BRANCH_NOT_EQUAL_BYTE: {
        register u8 *other asm("r0");
        other = MUSIC_PLAYER_FIELD(player, u8 **, memory) + operand;
        memory_cell_or_value = (u8 *)(u32)*memory_cell_or_value;
        if ((u32)memory_cell_or_value != (u32)*other) {
            goto invoke;
        }
        goto skip;
    }
    case MUSIC_MEMORY_BRANCH_GREATER_BYTE: {
        register u8 *other asm("r0");
        other = MUSIC_PLAYER_FIELD(player, u8 **, memory) + operand;
        memory_cell_or_value = (u8 *)(u32)*memory_cell_or_value;
        if ((u32)memory_cell_or_value > (u32)*other) {
            goto invoke;
        }
        goto skip;
    }
    case MUSIC_MEMORY_BRANCH_GREATER_EQUAL_BYTE: {
        register u8 *other asm("r0");
        other = MUSIC_PLAYER_FIELD(player, u8 **, memory) + operand;
        memory_cell_or_value = (u8 *)(u32)*memory_cell_or_value;
        if ((u32)memory_cell_or_value >= (u32)*other) {
            goto invoke;
        }
        goto skip;
    }
    case MUSIC_MEMORY_BRANCH_LESS_EQUAL_BYTE: {
        register u8 *other asm("r0");
        other = MUSIC_PLAYER_FIELD(player, u8 **, memory) + operand;
        memory_cell_or_value = (u8 *)(u32)*memory_cell_or_value;
        if ((u32)memory_cell_or_value <= (u32)*other) {
            goto invoke;
        }
        goto skip;
    }
    case MUSIC_MEMORY_BRANCH_LESS_BYTE: {
        register u8 *other asm("r0");
        other = MUSIC_PLAYER_FIELD(player, u8 **, memory) + operand;
        memory_cell_or_value = (u8 *)(u32)*memory_cell_or_value;
        if ((u32)memory_cell_or_value < (u32)*other) {
            goto invoke;
        }
        goto skip;
    }
    }
    return;
invoke:
    CallFunctionR2(player, track_state, *(s32 *)(MUSIC_COMMAND_TABLE_RAM + 4));
    return;
skip:
    MUSIC_TRACK_FIELD(track_state, u8 **, command) += 4;
}
