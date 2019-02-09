#pragma once

#include <l4/capability.h>

int do_Page_RetypeToSlab(cap_t *page, byte_t toType, byte_t objType);
int do_Slab_Allocate(cap_t *slab, cap_t *capDest, word_t num);
