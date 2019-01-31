#pragma once

#include <l4/types.h>

int l4Slab_Retype(l4CPtr_t cptr, l4Byte_t retype);
int l4Slab_Allocate(l4CPtr_t cptr, l4Word_t num);
