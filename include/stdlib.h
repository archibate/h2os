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


#include <inttypes.h>
void *bsearch(const void *key, const void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*));


long strtol(const char *s, const char **endp, int base);
unsigned long strtoul(const char *s, const char **endp, int base);
long atol(const char *s);
int atoi(const char *s);


#define RAND_MAX 32767

void srand(unsigned int seed);
int rand(void);


#define EXIT_SUCCESS 0
#define EXIT_FAILURE (-1)

#include <compiler.h>
void _NORETURN exit(int status);
void atexit(void (*proc)(void));
#include <abort.h>


#include <malloc.h>


char *getenv(const char *name);
int system(const char *cmd);
