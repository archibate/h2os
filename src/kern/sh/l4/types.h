#pragma once

#include <inttypes.h>

typedef uintptr_t l4Word_t, l4CPtr_t;
typedef uint8_t l4Byte_t, l4Bits_t;
typedef uint16_t l4Half_t;
typedef intptr_t l4SWord_t, l4Ret_t;
typedef int8_t l4SByte_t, l4SBits_t;
typedef int16_t l4SHalf_t;
#ifdef _KERNEL
#define word_t l4Word_t
#define half_t l4Half_t
#define byte_t l4Byte_t
#define sword_t l4SWord_t
#define shalf_t l4SHalf_t
#define sbyte_t l4SByte_t
#define sbits_t l4SBits_t
#define bits_t l4Bits_t
#define cptr_t l4CPtr_t
#endif
