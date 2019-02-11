#include <l4/generic/space.h>
#include <l4/object/capability.h>

struct capability *space_cap_lookup(struct space *space, l4id_t capid)
{
	struct capability *cap;

	if (capid > space->cs_size)
		return NULL;

	cap = &space->cspace[capid];

	return cap;
}