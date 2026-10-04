#include "m2c_prelude.h"
__attribute__((naked)) void CallFunctionR0(void) { asm("bx r0"); asm("nop"); }
