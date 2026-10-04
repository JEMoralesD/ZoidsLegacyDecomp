#include "m2c_prelude.h"
int TransferSaveBlock() asm("func_8093EC8");
u8 ReadSaveBlock2(void) { return TransferSaveBlock(1, 2, 0); }
