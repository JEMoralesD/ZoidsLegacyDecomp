#include "m2c_prelude.h"
__attribute__((naked)) void CallFunctionR2(void) { asm("bx r2"); asm("nop"); }
