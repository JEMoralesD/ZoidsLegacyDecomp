#include "m2c_prelude.h"
#include "link_transfer.h"
extern u8 gLinkOutgoingSendSequence asm("D_03000999");
extern s32 gLinkSendBuffer asm("D_03006048");
extern s32 gLinkSendBytesRemaining asm("D_03006050");
extern u8 gLinkSendSequence asm("D_03006058");
extern s32 gLinkSendTag asm("D_03006040");
void BeginLinkSend(s32 source_address, s32 length, s32 tag_address) asm("func_0809AEC0");

void BeginLinkSend(s32 source_address, s32 length, s32 tag_address) {
    *(s8 *)&gLinkOutgoingSendSequence = LINK_TRANSFER_IDLE_SEQUENCE;
    *(s32 *)&gLinkSendBuffer = source_address;
    *(s32 *)&gLinkSendBytesRemaining = length;
    *(s8 *)&gLinkSendSequence = LINK_TRANSFER_TAG_SEQUENCE;
    *(s32 *)&gLinkSendTag = tag_address;
}
