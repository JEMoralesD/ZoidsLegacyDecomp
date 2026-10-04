#include "m2c_prelude.h"
int TransferSaveBlock() asm("func_8093EC8");
u8 WriteSaveBlock3(void) { return TransferSaveBlock(0, 3, 0); }
