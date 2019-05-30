#pragma once

#include <inttypes.h>

void *malloc(size_t size);
void *zalloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *amalloc(size_t len, size_t align);
void *realloc(void *p, size_t size);
void free(void *p);
#define free(p) (p) // TODO: Nothing to do, due to free BUG
