#include <l4/generic/idspace.h>
#include <l4/misc/bug.h>
#include <memory.h>

void ids_init(struct id_space *ids)
{
	memset(ids, 0, sizeof(*ids));
	ids->idmax = 1;
}

static int id_hash(l4id_t id)
{
	return ((id * 1103518245 + 1512314) >> 16) % ID_HASH_MAX;
}

struct ids_entry *ids_get(struct id_space *ids, l4id_t id)
{
	int h = id_hash(id);
	struct hlist_head *hh = &ids->ht[h];
	struct ids_entry *ide;
	hlist_for_each_entry2(ide, hh, hlist) {
		if (ide->id == id) {
			return ide;
		}
	}
	return NULL;
}

void __ids_add(struct id_space *ids, struct ids_entry *ide)
{
	int h = id_hash(ide->id);
	struct hlist_head *hh = &ids->ht[h];
	hlist_add_head(&ide->hlist, hh);
}

void *ids_add(struct id_space *ids, struct ids_entry *ide)
{
	int h = id_hash(ide->id);
	struct hlist_head *hh = &ids->ht[h];
	struct ids_entry *idse;
	hlist_for_each_entry2(idse, hh, hlist) {
		if (idse->id == ide->id) {
			return idse;
		}
	}
	hlist_add_head(&ide->hlist, hh);
	return NULL;
}

#if 0
struct ids_entry *ids_unset(struct id_space *ids, l4id_t id)
{
	int h = id_hash(id);
	struct hlist_head *hh = &ids->ht[h];
	struct ids_entry *ide;
	hlist_for_each_entry2(ide, hh, hlist) {
		if (ide->id == id) {
			hlist_del(&ide->hlist);
			return ide;
		}
	}
	return NULL;
}
#endif

void ide_del(struct ids_entry *ide)
{
	hlist_del(&ide->hlist);
}

l4id_t ids_new(struct id_space *ids, struct ids_entry *ide)
{
	l4id_t id = ids->idmax++; // T: use idtop大法 instread
	ide->id = id;
	BUG_ON(ids_add(ids, ide) != NULL);
	return id;
}
