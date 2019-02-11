#pragma once

#include <l4/object/space.h>

struct capability *space_cap_lookup(struct space *space, l4id_t capid);
