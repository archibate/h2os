#pragma once

#include <l4/types.h>

int l4Read(l4CPtr_t cptr, l4Word_t offset, void *data, l4Word_t length);
int l4Write(l4CPtr_t cptr, l4Word_t offset, const void *data, l4Word_t length);
