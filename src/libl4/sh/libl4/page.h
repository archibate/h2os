#pragma once

#include <l4/types.h>

int l4Pgdir_MapPage(l4CPtr_t cptr, l4CPtr_t page, l4Word_t vaddr);
int l4Pgdir_MapPgtab(l4CPtr_t cptr, l4CPtr_t pgtab, l4Word_t vaddr);
int l4Pgdir_UnmapPage(l4CPtr_t cptr, l4Word_t vaddr);
int l4Pgdir_UnmapPgtab(l4CPtr_t cptr, l4Word_t vaddr);
