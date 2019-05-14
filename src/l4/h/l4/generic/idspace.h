#pragma once

#include <l4/object/idspace.h>
#include <l4/object/identry.h>

void ids_init(struct id_space *ids);
struct ids_entry *ids_get(struct id_space *ids, l4id_t id);
void __ids_add(struct id_space *ids, struct ids_entry *ide);
void *ids_add(struct id_space *ids, struct ids_entry *ide);
l4id_t ids_new(struct id_space *ids, struct ids_entry *ide);
//struct ids_entry *ids_unset(struct id_space *ids, l4id_t id);
void ide_del(struct ids_entry *ide);

#define IDSPACE(name) struct id_space ids_##name; THIS_INIT { ids_init(&ids_##name); }
#define ID(name, id) ({ extern struct id_space ids_##name; struct ids_entry *__ide = ids_get(&ids_##name, id); !__ide ? NULL : container_of(__ide, struct name, ide); })
#define IDNEW(name, obj) ({ extern struct id_space ids_##name; ids_new(&ids_##name, &(obj)->ide); })
#define IDSET(name, obj, _id) ({ extern struct id_space ids_##name; (obj)->ide.id = (_id); ids_add(&ids_##name, &(obj)->ide); })
#define IDDEL(obj) ide_del(&(obj)->ide)

#define LID(space, name, id) ({ struct ids_entry *__ide = ids_get(&(space)->lids_##name, id); !__ide ? NULL : container_of(__ide, struct name, lide); })
#define LIDNEW(space, name, obj) ids_new(&(space)->lids_##name, &(obj)->lide)
#define LIDSET(space, name, obj, _id) ({ (obj)->lide.id = (_id); ids_add(&(space)->lids_##name, &(obj)->lide); }
#define LIDDEL(obj) ide_del(&(obj)->lide)
