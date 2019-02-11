#pragma once

#include <stdarg.h>

void syslog(const char *s);
int vprintk(const char *fmt, va_list ap);
int printk(const char *fmt, ...);
