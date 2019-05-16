#pragma once

#include "vn.h"
#include "de.h"

int dir_addent(vn_t *dir, de_t *e);
int dir_lookup(vn_t *dir, const char *name, de_t *e);
vn_t *dir_getpv(vn_t *dir, const char *path, char *name);
vn_t *dir_vopen(vn_t *dir, const char *path, unsigned int flags);
