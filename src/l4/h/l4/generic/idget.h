#pragma once

#include <l4/enum/rtype.h>
#include <l4/object/identry.h>

void *id_get_object(unsigned int rtype, l4id_t id);

#define id_get_thread(id)   container_of(id_get_object(RTYPE_THREAD  , id), struct ktcb    , ide)
#define id_get_endpoint(id) container_of(id_get_object(RTYPE_ENDPOINT, id), struct endpoint, ide)
#define id_get_slab(id)     container_of(id_get_object(RTYPE_SLAB,     id), struct slab    , ide)
