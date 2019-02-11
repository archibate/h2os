#include <l4/generic/space.h>

struct capability *cap_lookup(struct space *space, l4id_t capid)
{
	struct capability *cap;

	if (capid > space->cs_size)
		return NULL;

	cap = &space->cspace[capid];

	return cap;
}
