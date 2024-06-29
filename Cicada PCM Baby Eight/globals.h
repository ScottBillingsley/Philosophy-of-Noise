#pragma once

#include "Arduino.h"

/************************* Defines ********************************/

/* Tell the compiler we are going to use our own main.. */
#undef main
/* Tell the compiler to optimize for speed instead of size */
#pragma GCC optimize("-Ofast")

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define NOP __asm__ __volatile__("nop\n\t")
