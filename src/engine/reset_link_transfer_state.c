#include "m2c_prelude.h"
#include "link_transfer.h"
extern s8 gLinkDisconnectTimeoutCounter asm("D_0300605A");
extern s8 gLinkConnectionRequested asm("D_0300605B");
extern s8 gLinkSendSequence asm("D_03006058");
extern s8 gLinkReceiveSequence asm("D_03006059");
extern s8 gLinkSavedFrameUpdateCount asm("D_0300603D");

void ResetLinkTransferState(void) asm("func_0809AC98");

void ResetLinkTransferState(void) {
    register s8 idle_sequence asm("r2");
    register s8 *timeout_counter asm("r1") = &gLinkDisconnectTimeoutCounter;
    idle_sequence = LINK_TRANSFER_IDLE_SEQUENCE;
    *timeout_counter = idle_sequence;
    {
        register s8 *connection_requested asm("r1") = &gLinkConnectionRequested;
        *connection_requested = idle_sequence;
    }
    {
        register s8 *send_sequence asm("r3") = &gLinkSendSequence;
        register s8 *receive_sequence asm("r1") = &gLinkReceiveSequence;
        *receive_sequence = idle_sequence;
        *send_sequence = idle_sequence;
    }
    {
        register s8 *saved_frame_update_count asm("r1") = &gLinkSavedFrameUpdateCount;
        *saved_frame_update_count = idle_sequence;
    }
}
