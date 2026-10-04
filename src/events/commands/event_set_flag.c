#include "m2c_prelude.h"
#include "../event_script.h"


struct EventFlagUnlockEntry {
    u8 flag_id;
    u8 unlock_bit;
} __attribute__((packed));

struct EventFieldPosition {
    u8 pad00[20];
    s32 x;
    s32 y;
    u8 pad1C;
    u8 facing_direction;
};

void SetEventFlag(s32) asm("func_0809F7C8");
void *CreateFieldActor(s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080A9D78");
void SeekEventCommand(s32, s32, s32) asm("func_080A016C");

extern u8 gEventFlagConditionEnabled[];
extern struct EventFieldPosition D_0202ECF4;

s32 EventSetFlag(s32 script_slot, struct EventFlagCommand **cursor)
{
    register struct EventFlagCommand **source asm("r5") = cursor;
    register u32 saved_script_slot asm("r9");
    register u32 i asm("r4");
    struct EventFlagCommand *input;

    script_slot <<= 24;
    saved_script_slot = (u32)script_slot >> 24;
    SetEventFlag((*source)->flag_id);

    i = 0;
    do {
        register u32 next asm("r0");

        gEventFlagConditionEnabled[i] = 1;
        next = i + 1;
        asm volatile("" : "+r"(next));
        i = (u8)next;
    } while (i <= 0x45U);

    i = 0;
    {
        register struct EventFlagUnlockEntry *table_init asm("r0") =
            (struct EventFlagUnlockEntry *)0x087A1894;
        register u32 first_type asm("r1") = table_init->flag_id;
        register struct EventFlagCommand *retained asm("ip");
        register struct EventFlagUnlockEntry *table asm("r6");

        asm volatile("" : "+r"(table_init), "+r"(first_type));
        input = *source;
        retained = input;
        table = table_init;

        if (first_type != 0) {
            struct EventFlagCommand *check = retained;
            register struct EventFlagUnlockEntry *scan asm("r5") = table;
            register u32 *bits asm("r8") = (u32 *)0x020217C8;

            do {
                register u32 entry_offset asm("r2") = i << 1;
                register u8 *entry asm("r1") =
                    (u8 *)(entry_offset + (u32)scan);

                if (check->flag_id == entry[0]) {
                    register u8 *bit_address asm("r0") =
                        (u8 *)table + 1;
                    register u32 unlock_bit asm("r3");
                    register u32 *word asm("r2");
                    register u32 mask asm("r1");
                    register u32 value asm("r0");

                    bit_address = (u8 *)(entry_offset + (u32)bit_address);
                    unlock_bit = *bit_address;
                    word = (u32 *)((u32)bits + ((unlock_bit >> 5) << 2));
                    unlock_bit &= 0x1F;
                    mask = 1;
                    mask <<= unlock_bit;
                    value = *word;
                    value |= mask;
                    *word = value;
                }
                {
                    register u32 next asm("r0") = i + 1;

                    asm volatile("" : "+r"(next));
                    i = (u8)next;
                }
            } while (scan[i].flag_id != 0);
        }

        {
            register struct EventFlagCommand *check asm("r1") = retained;

            if (check->flag_id == 0x88) {
                D_0202ECF4.x = 0x81000;
                D_0202ECF4.y = 0xBE000;
                CreateFieldActor(0x6C, 0xD, 0x81000, 0xBE000,
                    D_0202ECF4.facing_direction, 2, 0xFF, 0);
            }
        }
    }

    SeekEventCommand(saved_script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
