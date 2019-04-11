#pragma once

#include <l4/object/mman.h>

struct mregion *mm_new(struct mm *mm, word_t start, word_t end);
struct mregion *mm_lookup(struct mm *mm, word_t addr);
