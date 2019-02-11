#pragma once

#include <mmu/types.h>

void Arch_InitPgdir(pde_t *pd);
void Arch_switchPgdirAndUTCB(pde_t *newPd, pa_t utcb);
