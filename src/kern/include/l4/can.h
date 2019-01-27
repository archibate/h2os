#pragma once

#include <l4/can-bits.h>
#define CanRW    (CanRead | CanWrite)
#define CanRO    (CanRead)

#include <inttypes.h>
typedef uchar can_t;
typedef uint wcan_t;
