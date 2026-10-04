#include "m2c_prelude.h"
#include "combinations/battle_combination.h"

extern void StartBattleCameraTransition(s32, s32, s32, s32) asm("func_080BB224");
extern s32 IsBattleCameraTransitionComplete(void) asm("func_080BB654");
extern s32 IsBattleUnitActive(s32, s32) asm("func_080E9D88");
extern void CopyBytes(void *, const void *, u32) asm("func_080ED038");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");
extern u32 gBattleUnitSprites[] asm("D_02032E8C");
extern u32 gBattleUnitGaugeSprites[] asm("D_02032EBC");
extern u8 gBattleUnitSpriteMotionStates[] asm("D_02032EEC");
extern u8 gBattleUnitRecordSwapScratch[] asm("D_02032FE4");
extern u32 gBattleUnitSpriteSwapScratch asm("D_02033E84");
extern u32 gBattleGaugeSpriteSwapScratch asm("D_02033E9C");

struct BattleRearRowPromotionFrame {
    u32 remap_turn_order;
    u32 side_sprite_byte_offset;
    u32 rear_motion_byte_offset;
    u32 side_times_two;
    u32 next_side;
    u32 next_rear_unit_slot;
    union {
        u32 destination_sprite_byte_offset;
        u32 motion_flag_address;
    } scratch;
};

void PromoteBattleRearRow(u8 remap_turn_order) asm("func_080C577C");

void PromoteBattleRearRow(u8 remap_turn_order)
{
    volatile struct BattleRearRowPromotionFrame frame;
    register u32 side asm("sl");

    frame.remap_turn_order = remap_turn_order;
    side = 0;
    do {
        u32 unit_slot = 0;

        {
            register u32 next_side asm("r1") = side;

            next_side += 1;
            frame.next_side = next_side;
        }
        while (unit_slot <= BATTLE_ACTIVE_UNIT_COUNT / 2 - 1 && (IsBattleUnitActive(side, unit_slot) << 24) == 0) {
            register u32 next asm("r0") = unit_slot + 1;

            next <<= 24;
            unit_slot = next >> 24;
        }
        if (unit_slot == BATTLE_ACTIVE_UNIT_COUNT / 2) {
            StartBattleCameraTransition(2, side, 0, 0);
            {
                register u32 side_times_two asm("r2") = side;

                asm volatile("" : "+r"(side_times_two));
                side_times_two <<= 1;
                frame.side_times_two = side_times_two;
            }
            while ((IsBattleCameraTransitionComplete() << 24) == 0) {
                YieldTaskForUpdates(1);
            }

            unit_slot = BATTLE_ACTIVE_UNIT_COUNT / 2;
            {
                u32 value = frame.side_times_two + side;

                register u32 side_sprite_offset asm("r4") = value << 3;

                frame.side_sprite_byte_offset = side_sprite_offset;
                frame.rear_motion_byte_offset = (value << 1) - 3;
            }
promote_rear_unit:
            {
                register s32 rear_unit_active asm("r0") = IsBattleUnitActive(side, unit_slot);

                rear_unit_active <<= 24;
                {
                    register u32 next_index asm("r1") = unit_slot + 1;

                    frame.next_rear_unit_slot = next_index;
                }
                if (rear_unit_active == 0) {
                    goto next_rear_unit;
                }
                {
                    register u32 front_unit_slot asm("r9");
                    register u32 front_unit_slot_seed asm("r2");
                    register u32 player_side_check asm("r4");
                    register u32 side_unit_record_offset asm("r5");
                    register u32 rear_unit_record_address asm("r6");
                    register u32 rear_slot_times_four asm("r4");
                    register u32 rear_sprite_byte_offset asm("r8");

                    front_unit_slot_seed = unit_slot;
                    front_unit_slot_seed -= BATTLE_ACTIVE_UNIT_COUNT / 2;
                    asm volatile("" : "+r"(front_unit_slot_seed));
                    front_unit_slot = front_unit_slot_seed;
                    player_side_check = side;
                    asm volatile("" : "+r"(player_side_check));

                    if (player_side_check == 0) {
                        register u8 *base asm("r0") = (u8 *)(0x02034B4C + BATTLE_COMBINATION_OFFSET(current_unit_original_party_slots));
                        register u8 *source_byte asm("r2");
                        register u32 value asm("r0");

                        source_byte = (u8 *)(unit_slot + (u32)base);
                        asm volatile("ldrb %0, [%1]"
                                     : "=r"(value)
                                     : "r"(source_byte));
                        {
                            register u8 *temporary asm("r1") =
                                (u8 *)0x02033EB4;

                            *temporary = value;
                        }
                        {
                            register u8 *destination_byte asm("r1") =
                                (u8 *)(0x02034B4C + BATTLE_COMBINATION_OFFSET(current_unit_original_party_slots));

                            asm volatile("add %0, %1"
                                         : "+r"(destination_byte)
                                         : "r"(front_unit_slot));
                            value = *destination_byte;
                            *source_byte = value;
                            {
                                register u8 *temporary asm("r2") =
                                    (u8 *)0x02033EB4;

                                value = *temporary;
                            }
                            *destination_byte = value;
                        }
                    }

                    rear_slot_times_four = unit_slot << 2;
                    rear_sprite_byte_offset = rear_slot_times_four;
                    rear_unit_record_address = rear_slot_times_four + unit_slot;
                    rear_unit_record_address <<= 3;
                    rear_unit_record_address -= unit_slot;
                    rear_unit_record_address <<= 4;
                    {
                        register u32 current_side asm("r0") = side;

                        side_unit_record_offset = current_side << 2;
                        side_unit_record_offset += side;
                        side_unit_record_offset <<= 3;
                        side_unit_record_offset -= current_side;
                        side_unit_record_offset <<= 7;
                    }
                    rear_unit_record_address += side_unit_record_offset;
                    {
                        register u32 base asm("r1") = 0x02034B4C;

                        asm volatile("" : "+r"(base));
                        rear_unit_record_address += base;
                    }
                    {
                        register void *record_swap_destination asm("r0") = gBattleUnitRecordSwapScratch;
                        register const void *rear_record_source asm("r1") =
                            (const void *)rear_unit_record_address;
                        register u32 size asm("r2") = 156;

                        size <<= 2;
                        CopyBytes(record_swap_destination, rear_record_source, size);
                    }
                    {
                        register u32 front_slot_carrier asm("r2") = front_unit_slot;
                        register u32 front_sprite_byte_offset asm("r3") =
                            front_slot_carrier << 2;
                        register u32 front_unit_record_address asm("r4");

                        front_unit_record_address = front_sprite_byte_offset + front_slot_carrier;
                        front_unit_record_address <<= 3;
                        front_unit_record_address -= front_slot_carrier;
                        front_unit_record_address <<= 4;
                        front_unit_record_address += side_unit_record_offset;
                        {
                            register u32 base asm("r0") = 0x02034B4C;

                            front_unit_record_address += base;
                        }
                        {
                            register void *rear_record_destination asm("r0") =
                                (void *)rear_unit_record_address;
                            register const void *front_record_source asm("r1") =
                                (const void *)front_unit_record_address;
                            register u32 size asm("r2") = 156;

                            size <<= 2;
                            frame.scratch.destination_sprite_byte_offset = front_sprite_byte_offset;
                            CopyBytes(rear_record_destination, front_record_source, size);
                        }
                        {
                            register void *front_record_destination asm("r0") =
                                (void *)front_unit_record_address;
                            register const void *record_swap_source asm("r1") =
                                gBattleUnitRecordSwapScratch;
                            register u32 size asm("r2") = 156;

                            size <<= 2;
                            CopyBytes(front_record_destination, record_swap_source, size);
                        }

                        {
                            register u32 *base asm("r1") = gBattleUnitSprites;
                            register u32 slot_offset asm("r2") = frame.side_sprite_byte_offset;
                            register u32 *first asm("r2");
                            register u32 second_offset asm("r3");
                            register u32 *temporary asm("r4");

                            rear_sprite_byte_offset += slot_offset;
                            {
                                register u32 offset_view asm("r4") =
                                    rear_sprite_byte_offset;

                                asm volatile("" : "+r"(offset_view));
                                first = (u32 *)((u32)offset_view +
                                                (u32)base);
                            }
                            {
                                register u32 value asm("r0") = *first;

                                temporary = &gBattleUnitSpriteSwapScratch;
                                *temporary = value;
                            }
                            second_offset = frame.scratch.destination_sprite_byte_offset;
                            {
                                register u32 local_offset asm("r0") = frame.side_sprite_byte_offset;

                                second_offset += local_offset;
                            }
                            base = (u32 *)((u32)second_offset + (u32)base);
                            *first = *base;
                            *base = *temporary;

                            {
                                register u32 *temporary asm("r2") =
                                    &gBattleGaugeSpriteSwapScratch;
                                register u32 *second_base asm("r1") =
                                    gBattleUnitGaugeSprites;
                                register u32 *first_view asm("r4");
                                register u32 value asm("r0");

                                rear_sprite_byte_offset += (u32)second_base;
                                asm volatile("" : "+r"(rear_sprite_byte_offset));
                                first_view = (u32 *)rear_sprite_byte_offset;
                                value = *first_view;
                                *temporary = value;
                                second_offset += (u32)second_base;
                                value = *(u32 *)second_offset;
                                *first_view = value;
                                value = *temporary;
                                *(u32 *)second_offset = value;
                            }
                        }
                    }

                    if (frame.remap_turn_order != 0) {
                        register u32 scan asm("r4") = 0;
                        register u8 *first asm("r3") = (u8 *)(0x02034B4C + BATTLE_TURN_ORDER_OFFSET(entries[0].side));
                        register u8 *second asm("r5") = first + 1;
                        register u32 replacement asm("r1") = front_unit_slot;

                        do {
                            register u8 *offset asm("r2") =
                                (u8 *)(scan << 1);
                            register u8 *probe asm("r0") = offset;

                            probe += (u32)first;
                            if (*probe == side) {
                                offset += (u32)second;
                                if (*offset == unit_slot) {
                                    *offset = replacement;
                                }
                            }
                            {
                                register u32 next asm("r0") = scan + 1;

                                next <<= 24;
                                scan = next >> 24;
                            }
                        } while (scan <= BATTLE_TURN_ORDER_ENTRY_COUNT - 1);
                    }
                    {
                        register u8 *address asm("r1");

                        {
                            register u8 *base asm("r0") = gBattleUnitSpriteMotionStates;
                            register u32 offset asm("r2") = frame.rear_motion_byte_offset;

                            address = (u8 *)(unit_slot + offset);
                            address += (u32)base;
                        }
                        {
                            register u32 one asm("r0") = 1;

                            *address = one;
                        }
                    }
                }
next_rear_unit:
                {
                    register u32 saved_index asm("r4") = frame.next_rear_unit_slot;
                    register u32 normalized asm("r0") = saved_index << 24;

                    unit_slot = normalized >> 24;
                }
                if (unit_slot > BATTLE_ACTIVE_UNIT_COUNT - 1) {
                    goto wait_for_promoted_units;
                }
                goto promote_rear_unit;
            }
wait_for_promoted_units:

            {
                register u32 *states asm("r5");
                register u8 *flags asm("r4");
                register u32 *state asm("r6");
                register u8 *flag asm("r3");
                register u32 base_index asm("r1");

                states = gBattleUnitSprites;
                base_index = frame.side_times_two;
                base_index += side;
                flags = gBattleUnitSpriteMotionStates;
                {
                    register u32 flag_offset asm("r0") = base_index << 1;

                    flag = (u8 *)(flag_offset + (u32)flags);
                }
                base_index <<= 3;
                state = (u32 *)(base_index + (u32)states);

wait_for_row_motion:
                {
                    register u32 one asm("r0") = 1;

                    frame.scratch.motion_flag_address = (u32)flag;
                    YieldTaskForUpdates(one);
                }
                unit_slot = 0;
                {
                    register u32 value asm("r0") = *state;

                    flag = (u8 *)frame.scratch.motion_flag_address;
                    if (value == 0) {
                        goto find_moving_front_unit;
                    }
                    value = *flag;
                    if (value != 0) {
                        goto front_unit_motion_found;
                    }
                }

find_moving_front_unit:
                {
                    register u32 next asm("r0") = unit_slot + 1;

                    next <<= 24;
                    unit_slot = next >> 24;
                }
                if (unit_slot > BATTLE_ACTIVE_UNIT_COUNT / 2 - 1) {
                    goto next_battle_side;
                }
                {
                    register u32 state_offset asm("r0") = unit_slot << 2;
                    register u32 current_base asm("r2") = frame.side_times_two;
                    register u32 scaled_base asm("r1");
                    register u32 value asm("r0");

                    current_base += side;
                    scaled_base = current_base << 3;
                    state_offset += scaled_base;
                    state_offset += (u32)states;
                    value = *(u32 *)state_offset;
                    if (value == 0) {
                        goto find_moving_front_unit;
                    }
                    value = current_base << 1;
                    value = unit_slot + value;
                    value += (u32)flags;
                    value = *(u8 *)value;
                    if (value == 0) {
                        goto find_moving_front_unit;
                    }
                }

front_unit_motion_found:
                if (unit_slot <= BATTLE_ACTIVE_UNIT_COUNT / 2 - 1) {
                    goto wait_for_row_motion;
                }
            }
        }
next_battle_side:
        {
            register u32 next_side asm("r1") = frame.next_side;
            register u32 normalized asm("r0") = next_side << 24;

            side = normalized >> 24;
        }
    } while (side <= BATTLE_SIDE_COUNT - 1);
}
