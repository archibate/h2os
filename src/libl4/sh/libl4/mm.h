#pragma once

#include <l4/types.h>

int l4Segment_Split(l4CPtr_t cptr, l4Byte_t point);
int l4Segment_AllocPage(l4CPtr_t cptr, l4Word_t count);
int l4Page_RetypeToSlab(l4CPtr_t cptr, l4Byte_t type);
int l4Slab_Allocate(l4CPtr_t cptr, l4Word_t count);
