#pragma once

#include <l4/object/kcache.h>

void kcache_init(struct kcache *kc, size_t rsize, size_t pgsize);
void kcache_add_page(struct kcache *kc, void *page);
void kcache_free(struct kcache *kc, void *p);
void *kcache_alloc(struct kcache *kc);
