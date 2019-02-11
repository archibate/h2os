#pragma once

#include <l4/system/kbase.h>

#define kPgdir          ((pde_t*) KernPgdirAddr)
#define kPtes           ((pte_t*) KernPtesBegin)
