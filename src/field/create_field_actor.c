#include "field_actor.h"

extern struct FieldActor gFieldActors[] asm("D_020325A0");
extern s32 gFieldActorDirectionVectors[] asm("D_087A1B98");
extern s32 gFieldActorPositionHistoryWords[] asm("D_020329AC");
extern u8 gFieldActorDirectionHistory[] asm("D_02032A6C");
void InitializeFieldActorSprites(struct FieldActor *) asm("func_080A9AFC");
void UpdateFieldActorMapCell(struct FieldActor *) asm("func_080A98C4");

struct FieldActor *CreateFieldActor(u8 model_id, u8 actor_id, s32 world_x_fixed8, s32 world_y_fixed8, u8 direction, s32 initial_flags, u16 behavior, s32 interaction_flag_id) asm("func_080A9D78");

struct FieldActor *CreateFieldActor(u8 model_id, u8 actor_id, s32 world_x_fixed8, s32 world_y_fixed8, u8 direction, s32 initial_flags, u16 behavior, s32 interaction_flag_id)
{
    u8 free_slot;
    u8 slot_index;
    struct FieldActor *slot;
    u32 zero;
    u8 zero_byte;

    if (actor_id > FIELD_ACTOR_MAX_ID) {
        goto fail;
    }
    {
        register u32 base asm("r10");
        register u32 one asm("r9");

        free_slot = FIELD_ACTOR_SLOT_NOT_FOUND;
        slot_index = 0;
        base = FIELD_ACTORS_RAM;
        one = FIELD_ACTOR_ACTIVE;
        goto cond;
inc:
        slot_index++;
cond:
        if (slot_index <= FIELD_ACTOR_MAX_SLOT) {
            u32 offset = slot_index << 3;
            struct FieldActor *candidate;

            offset += slot_index;
            offset <<= 3;
            candidate = (struct FieldActor *)(offset + base);

            if ((candidate->flags & one) == 0) {
                if (free_slot != FIELD_ACTOR_SLOT_NOT_FOUND) {
                    goto inc;
                }
                free_slot = slot_index;
                goto inc;
            }
            if (candidate->actor_id != actor_id) {
                goto inc;
            }
            goto fail;
        }
        asm volatile("" : "=r"(base));
        if (free_slot == FIELD_ACTOR_SLOT_NOT_FOUND) {
fail:
            return 0;
        }
        slot = &gFieldActors[free_slot];
        slot->flags = initial_flags | FIELD_ACTOR_ACTIVE;
        zero = 0;
        slot->model_id = model_id;
        slot->actor_id = actor_id;
        zero_byte = 0;
        slot->behavior = behavior;
        slot->world_x_fixed8 = world_x_fixed8;
        slot->world_y_fixed8 = world_y_fixed8;
        if (behavior == FIELD_ACTOR_DIRECTION_OFFSET) {
            s32 direction_table = (s32)gFieldActorDirectionVectors;
            s32 direction_offset = direction * 8;

            slot->world_x_fixed8 = (*(s32 *)(direction_offset + direction_table) << 4) + world_x_fixed8;
            direction_table += 4;
            slot->world_y_fixed8 = (*(s32 *)(direction_offset + direction_table) << 4) + world_y_fixed8;
        }
        slot->velocity_y_fixed8 = zero;
        slot->velocity_x_fixed8 = zero;
        slot->movement_mode = zero_byte;
        slot->current_direction = direction;
        slot->requested_direction = direction;
        slot->turn_timer = zero_byte;
        if (behavior != FIELD_ACTOR_MAP_INTERACTION) {
            u8 state_index;

            state_index = 0;
            {
                register s32 *state_words asm("r1") = slot->behavior_state.words;

            asm volatile("" : "+r"(state_words));
            do {
                u32 offset = (u32)state_words;

                offset += state_index << 2;
                *(s32 *)offset = 0;
                state_index++;
            } while (state_index < FIELD_ACTOR_STATE_WORD_COUNT);
            }
            if (behavior == FIELD_ACTOR_FOLLOWER) {
                s32 *words;
                s32 *words1;
                u8 *bytes;

                state_index = 0;
                words = gFieldActorPositionHistoryWords;
                words1 = words + 1;
                bytes = gFieldActorDirectionHistory;
                do {
                    u32 offset = state_index << 3;

                    *(s32 *)(offset + (u32)words) = world_x_fixed8;
                    *(s32 *)(offset + (u32)words1) = world_y_fixed8;
                    {
                        u32 byte_offset = state_index;

                        byte_offset += (u32)bytes;
                        *(u8 *)byte_offset = direction;
                    }
                    state_index++;
                } while (state_index <= FIELD_ACTOR_HISTORY_LAST);
            }
        } else {
            FIELD_ACTOR_FIELD(slot, s32 *, behavior_state.map_interaction.flag_id) = interaction_flag_id;
        }
        InitializeFieldActorSprites(slot);
        UpdateFieldActorMapCell(slot);
        slot->map_cell_change = 0;
        return slot;
    }
    return 0;
}
