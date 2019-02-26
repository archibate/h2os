#include <l4/generic/idget.h>
#include <l4/generic/idspace.h>
#include <l4/misc/bug.h>

static struct id_space idspaces[RTYPE_MAX];

void init_idg(void)
{
	unsigned int i;
	for (i = 0; i < RTYPE_MAX; i++) {
		ids_init(&idspaces[i]);
	}
}

struct ids_entry *id_get_object(unsigned int rtype, l4id_t id)
{
	BUG_ON(rtype >= RTYPE_MAX);
	return ids_lookup(&idspaces[rtype], id);
}

l4id_t idg_new_entry(struct ids_entry *ide, unsigned int rtype)
{
	BUG_ON(rtype >= RTYPE_MAX);
	return ids_new_entry(&idspaces[rtype], ide);
}

#if 0
int idg_new_entry_at(struct ids_entry *ide, unsigned int rtype, unsigned int flags)
{
	BUG_ON(rtype >= RTYPE_MAX);
	return ids_new_entry_at(&idspaces[rtype], ide);
}
#endif

bool idg_set_entry(struct ids_entry *ide, unsigned int rtype)
{
	BUG_ON(rtype >= RTYPE_MAX);
	return NULL == ids_add(&idspaces[rtype], ide);
}

bool idg_free_id(unsigned int rtype, l4id_t id)
{
	BUG_ON(rtype >= RTYPE_MAX);
	return ids_del(&idspaces[rtype], id);
}
