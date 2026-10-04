#include "m2c_prelude.h"
int ModuloSigned32() asm("func_080ECE30");
int DivideSigned32() asm("func_080ECD98");

int DivideFixed12(int numerator, int denominator) asm("func_08092A14");

int DivideFixed12(int numerator, int denominator)
{
    int remainder;
    int denominator_magnitude;
    int quotient;
    int nibble_count;

    remainder = numerator;
    if (remainder < 0)
        remainder = -remainder;
    denominator_magnitude = denominator;
    if (denominator_magnitude < 0)
        denominator_magnitude = -denominator_magnitude;
    if (remainder == 0 || denominator_magnitude == 0)
        return 0;
    asm volatile("" :: "r"(remainder));
    quotient = 0;
    asm volatile("" : "+r"(quotient));
    nibble_count = quotient;
    do {
        quotient <<= 4;
        quotient += DivideSigned32(remainder, denominator_magnitude);
        remainder = ModuloSigned32(remainder, denominator_magnitude);
        remainder <<= 4;
        nibble_count++;
    } while (nibble_count <= 3 && remainder != 0);
    while (nibble_count <= 3) {
        quotient <<= 4;
        nibble_count++;
    }
    if (numerator < 0) {
        if (denominator >= 0)
            quotient = -quotient;
    } else if (denominator < 0) {
        quotient = -quotient;
    }
    return quotient;
}
