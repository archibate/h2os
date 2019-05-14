#pragma once

#include <l4/object/kcache.h>

void kcache_init(struct kcache *kc, size_t rsize);
void kcache_free(struct kcache *kc, void *p);
void *kcache_alloc(struct kcache *kc);

#define KCACHE(name) struct kcache kc_##name; THIS_INIT { kcache_init(&kc_##name, sizeof(struct name)); }
#define KCNEW(name) ({ extern struct kcache kc_##name; (struct name *)kcache_alloc(&kc_##name); })
#define KCDEL(name, obj) ({ extern struct kcache kc_##name; struct name *__obj = (obj); kcache_free(&kc_##name, __obj); })
