#include "m2c_prelude.h"
struct CopyRequest {
    s32 flags;
    s32 source;
    s32 destination;
    s32 byte_count;
};

extern struct CopyRequest gTransferQueue[];

s32 QueueCopy(s32 source, s32 destination, u16 byte_count) {
    u8 slot_index;
    struct CopyRequest *request;

    slot_index = 0;
    request = gTransferQueue;
    if (gTransferQueue[0].flags & 1) {
        do {
            slot_index++;
            if (slot_index > 15) break;
            request = &gTransferQueue[slot_index];
        } while (request->flags & 1);
    }
    if (slot_index == 16) {
        return 0;
    }
    request->flags = 1;
    request->source = source;
    request->destination = destination;
    request->byte_count = byte_count;
    return 1;
}
