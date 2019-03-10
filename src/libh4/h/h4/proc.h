#pragma once

#include <compiler.h>

typedef int pid_t;

void _NORETURN _exit(int status);
int pause(void);
pid_t getpid(void);
