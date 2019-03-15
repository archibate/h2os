#pragma once

#include <list.h>

#ifndef ID_HASH_MAX
#define ID_HASH_MAX    29
#endif

#ifndef ID_KEY
#define ID_KEY int
#endif

struct idmap
{
	struct hlist_head ht[ID_HASH_MAX];
};

#ifndef ID_HASH
#define ID_HASH __idm_hash
static int __idm_hash(ID_KEY id)
{
	return ((id * 1103518245 + 1512314) >> 16) % ID_HASH_MAX;
}
#endif

#ifndef ID_OBJECT
#define ID_OBJECT struct idobj
struct idobj {
	struct hlist_node hlist;
	ID_KEY id;
#ifndef ID_OBJECT_CONTENTS
#ifdef ID_VALUE_TYPE
#define ID_OBJECT_CONTENTS ID_VALUE_TYPE val;
#else
#define ID_OBJECT_CONTENTS union { int val; void *ptr; };
#endif
#endif
	ID_OBJECT_CONTENTS
};
#endif

#ifndef ID_EQUAL
#define ID_EQUAL(x,y) ((x)==(y))
#endif

ID_OBJECT *idmap_get(struct idmap *m, ID_KEY id)
{
	int h = ID_HASH(id);
	struct hlist_head *hh = &m->ht[h];
	ID_OBJECT *e;
	hlist_for_each_entry2(e, hh, hlist) {
		if (ID_EQUAL(e->id, id)) {
			return e;
		}
	}
	return NULL;
}

void __idmap_add(struct idmap *m, ID_OBJECT *obj)
{
	int h = ID_HASH(obj->id);
	hlist_add_head(&obj->hlist, &m->ht[h]);
}

void *idmap_add(struct idmap *m, ID_OBJECT *obj)
{
	int h = ID_HASH(obj->id);
	struct hlist_head *hh = &m->ht[h];
	ID_OBJECT *e;
	hlist_for_each_entry2(e, hh, hlist) {
		if (ID_EQUAL(e->id, obj->id)) {
			return e;
		}
	}
	hlist_add_head(&obj->hlist, hh);
	return NULL;
}

ID_OBJECT *idmap_del(struct idmap *m, ID_KEY id)
{
	int h = ID_HASH(id);
	struct hlist_head *hh = &m->ht[h];
	ID_OBJECT *e;
	hlist_for_each_entry2(e, hh, hlist) {
		if (ID_EQUAL(e->id, id)) {
			hlist_del(&e->hlist);
			return e;
		}
	}
	return NULL;
}
