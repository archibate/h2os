#pragma once

#include "vn.h"
#include "de.h"

int dir_gete(vn_t *dir, const char *path, de_t *e);
vn_t *dir_getpv(vn_t *dir, const char *path, char **endp);
vn_t *dir_vopen(vn_t *dir, const char *path, unsigned int flags);
