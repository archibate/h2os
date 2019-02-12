#pragma once

#include <l4/object/slab.h>

void *slab_new(struct slab *slab);
void slab_free(struct slab *slab, void *p);
void slab_init_freg(struct slab *slab, void *begin, void *end, size_t rsize);
void __slab_rt_revoke(struct slab *slab, unsigned int rtype);
