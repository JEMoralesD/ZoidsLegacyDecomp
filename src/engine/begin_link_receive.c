#include "m2c_prelude.h"
#include "link_transfer.h"
extern s8 gLinkOutgoingReceiveSequence asm("D_03000998");
extern s32 gLinkReceiveBuffer asm("D_0300604C");
extern s32 gLinkReceiveBytesRemaining asm("D_03006054");
extern s8 gLinkReceiveSequence asm("D_03006059");
extern s32 gLinkReceiveTag asm("D_03006044");

void BeginLinkReceive(s32 destination_address, s32 length, s32 tag_address) asm("func_0809B00C");

void BeginLinkReceive(s32 destination_address, s32 length, s32 tag_address) {
    gLinkOutgoingReceiveSequence = LINK_TRANSFER_TAG_SEQUENCE;
    gLinkReceiveBuffer = destination_address;
    gLinkReceiveBytesRemaining = length;
    gLinkReceiveSequence = LINK_TRANSFER_TAG_SEQUENCE;
    gLinkReceiveTag = tag_address;
}
