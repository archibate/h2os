#pragma once

#include <stdarg.h>

int cputchar(int ch);
int cputs(const char *s);
int vcprintf(const char *fmt, va_list ap);
int cprintf(const char *fmt, ...);
