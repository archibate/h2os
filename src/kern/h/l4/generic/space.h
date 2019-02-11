#pragma once

#include <l4/generic/capability.h>

struct space
{
	size_t cs_size;
	struct capability *cspace;
};

struct capability *space_cap_lookup(struct space *space, l4id_t capid);
