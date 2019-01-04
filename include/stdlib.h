#pragma once

typedef struct _div_t {
	unsigned int quot, rem;
} div_t;

typedef struct _ldiv_t {
	unsigned long quot, rem;
} ldiv_t;

div_t div(unsigned int a, unsigned int b);
ldiv_t ldiv(unsigned long a, unsigned long b);
unsigned long strtol(const char *s, const char **endp, int base);
int atoi(const char *s);
