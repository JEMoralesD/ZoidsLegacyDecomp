#include "m2c_prelude.h"
struct DeferredCallback { s32 next; s32 callback; s32 argument; u8 delay_frames; u8 pad[3]; };

void InitDeferredCallbacks(void) {
    void *base;
    void *current_entry;
    void *next_entry;
    s32 remaining;

    base = (void *)0x030009F0;
    current_entry = base;
    remaining = 7;
    do {
        next_entry = (s8 *)current_entry + 0x10;
        *(void **)current_entry = next_entry;
        current_entry = next_entry;
        remaining -= 1;
    } while (remaining >= 0);
    *(void **)current_entry = 0;
    current_entry = base;
    {
        s32 free_callback = -1;
        remaining = 8;
        do {
            *(s32 *)((s8 *)current_entry + 4) = free_callback;
            current_entry = (s8 *)current_entry + 0x10;
            remaining -= 1;
        } while (remaining >= 0);
    }
}

void *QueueDeferredCallback(s32 callback, s32 argument) {
    struct DeferredCallback *entry = (struct DeferredCallback *)0x030009F0;
    if (entry->next != 0) {
        do {
            if (entry->callback == -1) {
                entry->callback = callback;
                entry->argument = argument;
                entry->delay_frames = 0;
                return entry;
            }
            entry = (struct DeferredCallback *)((s8 *)entry + 16);
        } while (entry->next != 0);
    }
    return (void *)-1;
}
