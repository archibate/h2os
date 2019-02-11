#include <l4/generic/idspace.h>
#include <memory.h>

void ids_init(struct id_space *ids)
{
	memset(ids, 0, sizeof(*ids));
}

static int id_hash(l4id_t id)
{
	return ((id * 1103518245 + 1512314) >> 16) % ID_HASH_MAX;
}

void *ids_lookup(struct id_space *ids, l4id_t id)
{
	int h = id_hash(id);
	struct hlist_head *hh = &ids->ht[h];
	struct ids_entry *ide;
	hlist_for_each_entry2(ide, hh, hlist) {
		if (ide->id == id) {
			return ide->p;
		}
	}
	return NULL;
}

void ids_add(struct id_space *ids, struct ids_entry *ide)
{
	int h = id_hash(ide->id);
	struct hlist_head *hh = &ids->ht[h];
	hlist_add_head(&ide->hlist, hh);
}

void *ids_delete(struct id_space *ids, l4id_t id)
{
	int h = id_hash(id);
	struct hlist_head *hh = &ids->ht[h];
	struct ids_entry *ide;
	hlist_for_each_entry2(ide, hh, hlist) {
		if (ide->id == id) {
			hlist_del(&ide->hlist);
			return ide->p;
		}
	}
	return NULL;
}
