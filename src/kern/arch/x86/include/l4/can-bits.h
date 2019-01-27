#pragma once

#include <mmu/pte-bits.h>
// `Can' Permissions:
#define CanRead  PG_P
#define CanWrite PG_W
#define CanGrant PG_U

#define CanBits  3
