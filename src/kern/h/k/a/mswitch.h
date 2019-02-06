#pragma once

#include <mmu/types.h>

void init_pgdir(pa_t pd);
void switch_pgdir(pa_t oldPd, pa_t newPd);
