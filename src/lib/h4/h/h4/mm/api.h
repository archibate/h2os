#pragma once

#include <inttypes.h>

void *sbrk(ptrdiff_t incptr);
void *brk(void *p);
int execve(const char *path, char *const *argv, char *const *envp);
