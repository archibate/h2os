#pragma once

#include <l4/system/kbase.h>

#define kPd          ((pde_t*) KernPgdirAddr)
#define kPtes           ((pte_t*) KernPtesBegin)
