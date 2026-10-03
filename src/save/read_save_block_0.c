#include "m2c_prelude.h"
int TransferSaveBlock() asm("func_8093EC8");
u8 ReadSaveBlock0(void) { return TransferSaveBlock(1, 0, 0); }
