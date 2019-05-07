#pragma once

#include <l4/object/mman.h>

struct mregion *mm_new(struct mm *mm, word_t start, word_t end, unsigned int prot);
struct mregion *mm_lookup(struct mm *mm, word_t addr);
void mm_del(struct mregion *mreg);
void mm_destroy(struct mm *mm);
struct mm *mm_fork(struct mm *mm);
