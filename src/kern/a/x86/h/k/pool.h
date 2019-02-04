#pragma once

#include <inttypes.h>
#include <mmu/types.h>
#include <mmu/page.h>

void *calloc(size_t nmemb, size_t size);
#define pool_alloc_frame() ((pa_t)calloc(1, PageSize))
