#include "m2c_prelude.h"
int TransferSaveBlock() asm("func_8093EC8");
u8 WriteSaveBlock1(void) { return TransferSaveBlock(0, 1, 0); }
