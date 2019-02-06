#pragma once

#include <mmu/types.h>

void Arch_switchPgdirAndUTCB(pde_t *oldPd, pde_t *newPd, pa_t utcb);
