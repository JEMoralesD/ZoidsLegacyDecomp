#include "m2c_prelude.h"
struct TilemapCopyRequest {
    s32 flags;
    s32 source;
    s32 destination;
    u16 width_halfwords;
    u16 row_count;
};

extern struct TilemapCopyRequest gTransferQueue[];

s32 QueueTilemapCopy(s32 source, s32 destination, u16 width_halfwords, u16 row_count) {
    struct TilemapCopyRequest *request;
    u8 slot_index;
    slot_index = 0;
    request = &gTransferQueue[0];
    if (gTransferQueue[0].flags & 1) {
        do {
            slot_index = slot_index + 1;
            if (slot_index > 15) break;
            request = &gTransferQueue[slot_index];
        } while (request->flags & 1);
    }
    if (slot_index == 16) {
        return 0;
    }
    request->flags = 3;
    request->source = source;
    request->destination = destination;
    request->width_halfwords = width_halfwords;
    request->row_count = row_count;
    return 1;
}
