#pragma once

#include <l4/types.h>
#include <mmu/types.h>

typedef struct Memory
{
	pde_t *pgd;
}
mem_t;

size_t mwrite(mem_t *mem, word_t addr, byte_t const *data, size_t length);
size_t mread(mem_t *mem, word_t addr, byte_t *data, size_t length);
