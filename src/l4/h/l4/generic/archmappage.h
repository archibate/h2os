#pragma once

#include <l4/object/mman.h>

int arch_mm_map_page(struct mm *mm, word_t addr, int succ, word_t page);
