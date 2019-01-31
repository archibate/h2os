#pragma once

#include <inttypes.h>

typedef uintptr_t l4Word_t, l4CPtr_t;
typedef uint8_t l4Byte_t, l4Bits_t;
typedef uint16_t l4Half_t;
#ifdef _KERNEL
#define word_t l4Word_t
#define half_t l4Half_t
#define byte_t l4Byte_t
#define bits_t l4Bits_t
#define cptr_t l4CPtr_t
#endif
