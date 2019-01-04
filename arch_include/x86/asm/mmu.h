#pragma once

#include <asm/pte-bits.h>
#include <asm/mmu-ops.h>
#include <inttypes.h>
#include <ptrdeco.h>

typedef ulong va_t, __phys pa_t;
typedef pa_t pde_t, pte_t;

#define PTE(pa, attr) ((pa) | (attr))
#define PDE(pa, attr) ((pa) | (attr))
