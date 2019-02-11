#include <l4/generic/idget.h>
#include <l4/generic/idspace.h>
#include <l4/generic/thread.h>

void *id_get_object(unsigned int rtype, l4id_t id)
{
	return ids_lookup(current->idspaces[rtype], id);
}
