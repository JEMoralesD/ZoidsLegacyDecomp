#include "m2c_prelude.h"
#include "link_transfer.h"
extern u16 gLinkControlFlags asm("D_030009EC");
M2C_UNK StopTask(s32) asm("func_08092E0C");
M2C_UNK ResetLinkTransferState() asm("func_0809AC98");

void StopLinkConnection(void) asm("func_0809AEA0");

void StopLinkConnection(void) {
    ResetLinkTransferState();
    *(u16 *)&gLinkControlFlags |= LINK_CONTROL_REQUEST_STOP;
    StopTask(LINK_CONNECTION_MONITOR_TASK_SLOT);
}
