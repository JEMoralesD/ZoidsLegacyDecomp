#include "m2c_prelude.h"
__attribute__((naked)) void CallFunctionR1(void) { asm("bx r1"); asm("nop"); }
