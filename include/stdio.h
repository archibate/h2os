#pragma once

#include <stdarg.h>
#include <ccutils.h>

int vsprintf(char *buf, const char *fmt, va_list ap);
int sprintf(char *buf, const char *fmt, ...) _FORMAT(printf, 2, 3);
int vprintf(const char *fmt, va_list ap);
int printf(const char *fmt, ...) _FORMAT(printf, 1, 2);
int putchar(int ch);
int puts(const char *s);
