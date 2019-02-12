#pragma once

#include <l4/object/slab.h>

void *slab_alloc(struct slab *slab);
void slab_init(struct slab *slab);
int slab_retype(struct slab *slab, struct mem *mem, unsigned int rtype);
