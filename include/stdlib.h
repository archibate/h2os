#pragma once


typedef struct _div_t {
	int quot, rem;
} div_t;

typedef struct _ldiv_t {
	long quot, rem;
} ldiv_t;

div_t div(int a, int b);
ldiv_t ldiv(long a, long b);
int abs(int x);
long labs(long x);


long strtol(const char *s, const char **endp, int base);
unsigned long strtoul(const char *s, const char **endp, int base);
long atol(const char *s);
int atoi(const char *s);


#define RAND_MAX 32767

void srand(unsigned int seed);
int rand(void);
