#pragma once

#include <l4/object/mman.h>

void mm_init(struct mm *mm);
void mm_copy_curr_fds(struct mm *dst_mm, struct mm *src_mm);
struct mregion *mm_new_mreg(struct mm *mm, word_t start, word_t end, unsigned int prot);
struct mregion *mm_lookup_mreg(struct mm *mm, word_t addr);
void mm_mreg_del(struct mregion *mreg);
void mm_destroy(struct mm *mm);
