#pragma once

#include <k/kbase.h>

#define kern_pd         ((pde_t*) KernPgdirAddr)
#define kern_ptes       ((pte_t*) KernPtesBegin)
#define myWindowPgtab   ((pte_t*) KernWinPtAddr)
#define myWindowAddr    ((va_t)   KernWindowAddr)
