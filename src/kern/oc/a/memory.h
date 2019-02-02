#pragma once

#include <mmu/types.h>

typedef struct memory
{
	pde_t *pgd;
}
mem_t;
