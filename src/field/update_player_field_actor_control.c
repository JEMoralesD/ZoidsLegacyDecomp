#include "field_actor.h"

extern volatile u16 gHeldKeys;
extern volatile u16 gPressedKeys;
extern s32 gFieldMovementCounter asm("D_020324A8");
extern s32 gPreviousFieldMovementCounter asm("D_020324AC");
extern struct FieldActor gFieldActors[] asm("D_020325A0");
extern u8 gInteractedFieldActorId asm("D_02032998");
extern u8 gCurrentMapId asm("D_0202ECF4");

void SetSpriteAnimation(s32) asm("func_08094564");
void RequestFieldActorPositionSwap(struct FieldActor *) asm("func_080AAE60");

void UpdatePlayerFieldActorControl(struct FieldActor *actor) asm("func_080AA1E4");

void UpdatePlayerFieldActorControl(struct FieldActor *actor)
{
    register s32 slot_index asm("r5");
    register s32 interaction_found asm("r8");
    register s32 object_base asm("ip");
    s32 index8;
    register volatile u16 *buttons_ptr asm("r9");
    register u32 input_lock_or_b_button asm("r4");
    register u32 initial_flags asm("r2");

    initial_flags = actor->flags;
    input_lock_or_b_button = FIELD_ACTOR_INPUT_LOCKED;
    asm volatile("" : "+r"(input_lock_or_b_button), "+r"(initial_flags));
    initial_flags &= input_lock_or_b_button;
    if (initial_flags) {
        actor->movement_mode = 0;
        return;
    }

    {
        register u8 *direction_table asm("r1") = (u8 *)0x087A1BD8;
        register volatile u16 *input_ptr asm("r0") = &gHeldKeys;
        register s32 input_value asm("r3");
        u8 requested_direction;

        asm volatile("" : "+r"(direction_table), "+r"(input_ptr));
        input_value = *input_ptr;
        requested_direction = *(u8 *)((s32)((u32)(input_value & 0xF0) >> 4)
            + (s32)direction_table);
        if (requested_direction != 0xFF) {
            actor->requested_direction = requested_direction;
            {
                register s32 input_mask asm("r0") = input_lock_or_b_button;
                asm volatile("" : "+r"(input_mask));
                input_mask &= input_value;
                if (!input_mask) {
                    actor->movement_mode = FIELD_ACTOR_NORMAL_SPEED;
                    gPreviousFieldMovementCounter = gFieldMovementCounter;
                    gFieldMovementCounter++;
                } else {
                    actor->movement_mode = FIELD_ACTOR_DOUBLE_SPEED;
                    gPreviousFieldMovementCounter = gFieldMovementCounter;
                    gFieldMovementCounter += 2;
                }
            }
        } else {
            actor->movement_mode = initial_flags;
        }
    }

    {
        register volatile u16 *initial_buttons asm("r2") = &gPressedKeys;
        register u32 masked_buttons asm("r0");
        register u16 buttons_value asm("r1");
        asm volatile("" : "+r"(initial_buttons));
        buttons_value = *initial_buttons;
        masked_buttons = 3;
        masked_buttons &= buttons_value;
        buttons_ptr = initial_buttons;
        if (!masked_buttons) {
            return;
        }
    }

    {
        register s32 zero asm("r0") = 0;
        asm volatile("" : "+r"(zero));
        interaction_found = zero;
    }
    slot_index = 0;
    {
        register s32 base_value asm("r2") = (s32)gFieldActors;
        asm volatile("" : "+r"(base_value));
        object_base = base_value;
    }
scan:
    {
        register s32 initial_index8 asm("r2");
        register s32 object_index_sum asm("r0");
        register s32 object_offset asm("r4");
        register s32 address_base asm("r7");
        register struct FieldActor *other asm("r3");
        register u32 object_flags asm("r1");
        register u32 one asm("r0");
        register s32 x_distance asm("r3");
        register s32 y_distance asm("r2");

        initial_index8 = slot_index << 3;
        object_index_sum = initial_index8 + slot_index;
        object_offset = object_index_sum << 3;
        address_base = object_base;
        asm volatile("" : "+r"(address_base));
        other = (struct FieldActor *)(object_offset + address_base);
        object_flags = other->flags;
        one = 1;
        object_flags &= one;
        index8 = initial_index8;
        if (!object_flags || actor == other || other->actor_id == 0xD) {
            goto next;
        }

        {
            register s32 field_base asm("r0") = object_base + 8;
            register s32 *field_address asm("r0");
            register s32 other_coordinate asm("r1");
            register s32 own_coordinate asm("r0");
            asm volatile("" : "+r"(field_base));
            field_address = (s32 *)(object_offset + field_base);
            other_coordinate = *field_address;
            own_coordinate = actor->world_x_fixed8;
            x_distance = other_coordinate - own_coordinate;
        }
        {
            register s32 field_base asm("r0") = object_base + 12;
            register s32 *field_address asm("r0");
            register s32 other_coordinate asm("r1");
            register s32 own_coordinate asm("r0");
            asm volatile("" : "+r"(field_base));
            field_address = (s32 *)(object_offset + field_base);
            other_coordinate = *field_address;
            own_coordinate = actor->world_y_fixed8;
            y_distance = other_coordinate - own_coordinate;
        }
        switch (actor->requested_direction) {
        case 0: {
            register s32 x_addend asm("r0") = 0x800;
            register u32 adjusted_x asm("r1") = x_distance + x_addend;
            asm volatile("" : "+r"(adjusted_x));
            if (adjusted_x > 0x1000U) goto next;
            if (y_distance < -0x1800) goto next;
            if (y_distance > 0) goto next;
            goto interact_with_actor;
        }
        case 2:
            if ((u32)x_distance > 0x1800U) goto next;
            if (y_distance < -0x800) goto next;
            if (y_distance > 0x800) goto next;
            goto interact_with_actor;
        case 4: {
            register s32 x_addend asm("r0") = 0x800;
            register u32 adjusted_x asm("r1") = x_distance + x_addend;
            asm volatile("" : "+r"(adjusted_x));
            if (adjusted_x > 0x1000U) goto next;
            if (y_distance < 0) goto next;
            if (y_distance > 0x1800) goto next;
            goto interact_with_actor;
        }
        case 6: {
            register s32 x_addend asm("r1") = 0x1800;
            register u32 adjusted_x asm("r0") = x_distance + x_addend;
            asm volatile("" : "+r"(adjusted_x));
            if (adjusted_x > 0x1800U) goto next;
            if (y_distance < -0x800) goto next;
            if (y_distance > 0x800) goto next;
            goto interact_with_actor;
        }
        case 1: {
            register s32 x_addend asm("r4") = 0x800;
            register u32 adjusted_x asm("r1") = x_distance + x_addend;
            asm volatile("" : "+r"(adjusted_x));
            if (adjusted_x > 0x2000U) goto next;
            if (y_distance < -0x1800) goto next;
            if (y_distance > x_addend) goto next;
            goto interact_with_actor;
        }
        case 3: {
            register s32 x_addend asm("r0") = 0x800;
            register u32 adjusted_x asm("r1") = x_distance + x_addend;
            asm volatile("" : "+r"(adjusted_x));
            if (adjusted_x > 0x2000U) goto next;
            if (y_distance < -0x800) goto next;
            if (y_distance > 0x1800) goto next;
            goto interact_with_actor;
        }
        case 5: {
            register s32 x_addend asm("r4") = 0x1800;
            register u32 adjusted_x asm("r1") = x_distance + x_addend;
            asm volatile("" : "+r"(adjusted_x));
            if (adjusted_x > 0x2000U) goto next;
            if (y_distance < -0x800) goto next;
            if (y_distance > x_addend) goto next;
            goto interact_with_actor;
        }
        case 7: {
            register s32 x_addend asm("r0") = 0x1800;
            register u32 adjusted_x asm("r1") = x_distance + x_addend;
            asm volatile("" : "+r"(adjusted_x));
            if (adjusted_x <= 0x2000U && y_distance >= -0x1800 &&
                y_distance <= 0x800) {
                register s32 found asm("r2") = 1;
                asm volatile("" : "+r"(found));
                interaction_found = found;
            }
            break;
        }
        }

next:
        {
            register s32 interaction_found_test asm("r0") = interaction_found;
            asm volatile("" : "+r"(interaction_found_test));
            if (!interaction_found_test) {
                goto increment;
            }
        }
    }

interact_with_actor:
    {
        register u32 one asm("r9");
        register s32 one_seed asm("r0");
        register s32 interaction_buttons asm("r2");

        interaction_buttons = *buttons_ptr;
        one_seed = 1;
        one = one_seed;
        if (interaction_buttons & one_seed) {
            register s32 interaction_index_sum asm("r0") = index8 + slot_index;
            register s32 interaction_actor_offset asm("r3");
            register s32 interaction_actor_base asm("r2");
            register struct FieldActor *other asm("r4");
            register s32 behavior asm("r1");

            interaction_actor_offset = interaction_index_sum << 3;
            asm volatile("" : "=r"(interaction_actor_base) : "0"(object_base));
            other = (struct FieldActor *)(interaction_actor_offset + interaction_actor_base);
            behavior = other->behavior;
            if ((u16)(behavior - 1) <= 3) {
                gInteractedFieldActorId = other->actor_id;
                actor->flags |= FIELD_ACTOR_INPUT_LOCKED;
                if (other->behavior != FIELD_ACTOR_STATIONARY && other->behavior != FIELD_ACTOR_DIRECTION_OFFSET) {
                    register s32 direction_value asm("r1") = actor->requested_direction;
                    register s32 direction_sum asm("r3") = direction_value + 4;
                    register s32 reduced_direction asm("r0") = direction_sum;
                    asm volatile("" : "+r"(reduced_direction));
                    reduced_direction >>= 3;
                    asm volatile("" : "+r"(reduced_direction));
                    reduced_direction <<= 3;
                    asm volatile("" : "+r"(reduced_direction));
                    reduced_direction = direction_sum - reduced_direction;
                    other->requested_direction = reduced_direction;
                }
                {
                    register s32 rebuilt_sum asm("r0");
                    register s32 rebuilt_offset asm("r0");
                    register struct FieldActor *rebuilt_other asm("r0");

                    asm volatile("" : "+r"(index8), "+r"(slot_index) : : "memory");
                    rebuilt_sum = index8 + slot_index;
                    rebuilt_offset = rebuilt_sum << 3;
                    rebuilt_other = (struct FieldActor *)(rebuilt_offset + object_base);
                    rebuilt_other->flags |= FIELD_ACTOR_INPUT_LOCKED;
                }
                return;
            }
            {
                register s32 behavior_test asm("r0") = (u16)behavior;
                asm volatile("" : "+r"(behavior_test));
                if (behavior_test == FIELD_ACTOR_MAP_INTERACTION) {
                    register s32 scene_base asm("r1") = (s32)&gCurrentMapId;
                    register u32 *bitset_base asm("r8");
                    register s32 bit_index_field_base asm("r0");
                    register u32 *bit_index_ptr asm("r5");
                    register u32 bit_index asm("r2");
                    register u32 bitset_address asm("r7");
                    register u32 mask_limit asm("r7");
                    register u32 *word asm("r0");
                    register u32 mask asm("r1");

                    asm volatile("" : "+r"(scene_base));
                    bit_index_field_base = object_base + 0x28;
                    bit_index_ptr = (u32 *)(interaction_actor_offset + bit_index_field_base);
                    bit_index = *bit_index_ptr;
                    word = (u32 *)((bit_index >> 5) << 2);
                    asm volatile("" : "+r"(word));
                    bitset_address = 0x170;
                    bitset_address = bitset_address - (0 - scene_base);
                    bitset_base = (u32 *)bitset_address;
                    word = (u32 *)((s32)word + (s32)bitset_base);
                    mask_limit = 0x1F;
                    mask = one << (bit_index & mask_limit);
                    if (!(*word & mask)) {
                        register s32 sprite_field_base asm("r0");
                        register s32 sprite_address asm("r0");

                        gInteractedFieldActorId = other->actor_id;
                        actor->flags |= FIELD_ACTOR_INPUT_LOCKED;
                        sprite_field_base = object_base + 0x20;
                        sprite_address = *(s32 *)(interaction_actor_offset + sprite_field_base);
                        /* R1 retains animation index 2 for this native call. */
                        SetSpriteAnimation(sprite_address);
                        {
                            register u32 current_index asm("r0") = *bit_index_ptr;
                            register u32 *current_word asm("r2");
                            register u32 current_mask asm("r1");

                            current_word = &bitset_base[current_index >> 5];
                            current_mask = one << (current_index & mask_limit);
                            *current_word |= current_mask;
                        }
                        return;
                    }
                }
            }
        } else if (interaction_buttons & 2) {
            register s32 other_sum asm("r0") = index8 + slot_index;
            register s32 other_offset asm("r0");
            register s32 other_base asm("r2") = object_base;
            register struct FieldActor *button_other asm("r1");

            other_offset = other_sum << 3;
            button_other = (struct FieldActor *)(other_offset + other_base);
            if (button_other->behavior == FIELD_ACTOR_WANDERING) {
                /* R1 retains the other actor for the native swap call. */
                RequestFieldActorPositionSwap(actor);
                return;
            }
        }
    }
    goto done;

increment:
    {
        register s32 next_slot asm("r0") = slot_index + 1;
        asm volatile("" : "+r"(next_slot));
        slot_index = (u8)next_slot;
    }
    if ((u32)slot_index <= FIELD_ACTOR_MAX_SLOT) {
        goto scan;
    }
done:
    return;
}
