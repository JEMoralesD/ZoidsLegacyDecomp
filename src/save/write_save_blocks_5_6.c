#include "m2c_prelude.h"
int TransferSaveBlock() asm("func_8093EC8");
u8 WriteSaveBlock5(void) { return TransferSaveBlock(0, 5, 0); }
u8 WriteSaveBlock6(void) { return TransferSaveBlock(0, 6, 0); }
