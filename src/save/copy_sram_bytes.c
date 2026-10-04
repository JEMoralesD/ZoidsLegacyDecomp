#include "m2c_prelude.h"
void CopySramBytes(const u8 *source, u8 *destination, u32 size) asm("func_080ECB7C");

#define REG_WAITCNT (*(volatile u16 *)0x04000204)
#define WAITCNT_SRAM_MASK 3
#define WAITCNT_SRAM_8 3

void CopySramBytes(const u8 *source, u8 *destination, u32 size) {
    REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_8;
    while (--size != -1)
        *destination++ = *source++;
}
