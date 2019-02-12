#pragma once

#include <l4/object/idspace.h>
#include <l4/object/identry.h>

void ids_init(struct id_space *ids);
struct ids_entry *ids_lookup(struct id_space *ids, l4id_t id);
void ids_add(struct id_space *ids, struct ids_entry *ide);
struct ids_entry *ids_delete(struct id_space *ids, l4id_t id);
