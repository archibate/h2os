#pragma once

#include <compiler.h>

void _NORETURN _exit(int status);
int pause(void);
pid_t getpid(void);
int wait(void);
