#include "m2c_prelude.h"

struct SpriteGroup {
    u32 flags;
    u8 pad04[8];
    u32 sprite_slots[32];
    u8 pad8C[0x20];
    s32 init_callback;
    s32 update_callback;
};

struct SpriteGroup *CreateSpriteGroup(s32 flags, s32 init_callback, s32 update_callback) {
    s32 group_flags;
    s32 initial_callback;
    register s32 recurring_callback asm("r12");
    register u32 index asm("r3");
    struct SpriteGroup *groups;
    register struct SpriteGroup *first_group asm("r0");
    register s32 initial_flags asm("r1");
    register s32 initial_mask asm("r2");

    group_flags = flags;
    initial_callback = init_callback;
    recurring_callback = update_callback;
    index = 0;
    first_group = (struct SpriteGroup *)0x030034A4;
    initial_flags = first_group->flags;
    initial_mask = 1;
    initial_flags &= initial_mask;
    groups = first_group;
    if (initial_flags != 0) {
        register u8 *scan_groups asm("r4");
        register s32 stride asm("r2");
        register s32 mask asm("r1");

        scan_groups = (u8 *)groups;
        stride = 0xB4;
        mask = 1;
        do {
            register u32 next_index asm("r0");
            register s32 scan_offset asm("r0");
            register s32 scan_flags asm("r0");

            next_index = index + 1;
            next_index <<= 24;
            index = next_index >> 24;
            if (index > 15) {
                break;
            }
            scan_offset = index;
            scan_offset *= stride;
            scan_offset += (s32)scan_groups;
            scan_flags = *(u32 *)scan_offset;
            scan_flags &= mask;
            if (scan_flags == 0) {
                break;
            }
        } while (1);
    }
    if (index == 16) {
        return (struct SpriteGroup *)0;
    }
    {
        register s32 record_offset asm("r0");
        register struct SpriteGroup *group asm("r1");
        register u32 slot asm("r3");
        register u32 *clear_base asm("r2");
        register s32 clear_zero asm("r4");

        record_offset = 0xB4;
        record_offset *= index;
        group = (struct SpriteGroup *)(record_offset + (s32)groups);
        group->flags = group_flags | 1;
        slot = 0;
        clear_base = group->sprite_slots;
        clear_zero = 0;
        do {
            register u32 next_slot asm("r0");
            register s32 slot_offset asm("r0");
            register u32 *clear_address asm("r0");

            slot_offset = slot << 2;
            clear_address = (u32 *)((s32)clear_base + slot_offset);
            *clear_address = clear_zero;
            next_slot = slot + 1;
            next_slot <<= 24;
            slot = next_slot >> 24;
        } while (slot <= 31);
        {
            register u8 *tail asm("r0");
            register s32 final_callback asm("r2");

            tail = (u8 *)group;
            tail += 0xAC;
            *(s32 *)tail = initial_callback;
            asm volatile("" : "+r"(tail));
            tail += 4;
            asm volatile("" : "+r"(tail));
            final_callback = recurring_callback;
            *(s32 *)tail = final_callback;
        }
        return group;
    }
}
