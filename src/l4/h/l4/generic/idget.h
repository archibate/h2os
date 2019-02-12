#pragma once

#include <l4/enum/rtype.h>
#include <l4/object/identry.h>

struct ids_entry *id_get_object(unsigned int rtype, l4id_t id);
l4id_t idg_new_entry(struct ids_entry *ide, unsigned int rtype);
bool idg_free_id(unsigned int rtype, l4id_t id);

#define id_get_thread(id)   container_of(id_get_object(RTYPE_THREAD  , id), struct ktcb    , ide)
#define id_get_endpoint(id) container_of(id_get_object(RTYPE_ENDPOINT, id), struct endpoint, ide)
#define id_get_slab_ctl(id) container_of(id_get_object(RTYPE_SLAB_CTL, id), struct slab_ctl, ide)
