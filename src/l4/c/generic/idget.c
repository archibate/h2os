#include <l4/generic/idget.h>
#include <l4/generic/idspace.h>
#include <l4/generic/thread.h>

struct ids_entry *id_get_object(unsigned int rtype, l4id_t id)
{
	return ids_lookup(current->idspaces[rtype], id);
}

l4id_t idg_new_entry(struct ids_entry *ide, unsigned int rtype)
{
	return ids_new_entry(current->idspaces[rtype], ide);
}

bool idg_free_id(unsigned int rtype, l4id_t id)
{
	return ids_del(current->idspaces[rtype], id);
}
