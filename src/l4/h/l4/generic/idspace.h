#pragma once

#include <l4/object/idspace.h>
#include <l4/object/identry.h>

void ids_init(struct id_space *ids);
struct ids_entry *ids_lookup(struct id_space *ids, l4id_t id);
void __ids_add(struct id_space *ids, struct ids_entry *ide);
void *ids_add(struct id_space *ids, struct ids_entry *ide);
l4id_t ids_new_entry(struct id_space *ids, struct ids_entry *ide);
struct ids_entry *ids_del(struct id_space *ids, l4id_t id);

#define LID(space, name, id) container_of(ids_get((space)->lids_##name, id), struct name, lide)
#define LIDNEW(space, obj) ids_new((space)->lids_##name, &(obj)->lide)
#define LIDSET(space, obj, id) ({ (obj)->lide.id = id; ids_add((space)->lids_##name, &(obj)->lide); }
#define LIDDEL(obj) ids_del(&(obj)->lide)
