#pragma once

#include <dirent.h>

typedef struct dirent de_t;

#define DESIZE sizeof(de_t)

uint32_t egetclus(const de_t *e);
void esetclus(de_t *e, uint32_t clus);
int ecmpname(const de_t *e, const char *name);
void egetname(const de_t *e, char *buf);
