#pragma once

#include <l4/enum/rtype.h>

void *id_get_object(unsigned int rtype, l4id_t id);

#define id_get_thread(id)   ((struct ktcb     *)id_get_object(RTYPE_THREAD,   id))
#define id_get_endpoint(id) ((struct endpoint *)id_get_object(RTYPE_ENDPOINT, id))
