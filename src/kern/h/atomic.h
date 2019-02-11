#pragma once

typedef struct atomic
{
	volatile int i;
}
atomic_t;

static inline void atomic_set(atomic_t *a, int x)
{
	a->i = x;
}

static inline int atomic_read(atomic_t *a)
{
	return a->i;
}

static inline int atomic_xchg(atomic_t *a, int x)
{
	int r;
	r = a->i;
	a->i = x;
	return r;
}

static inline void atomic_add(atomic_t *a, int x)
{
	a->i += x;
}

static inline void atomic_sub(atomic_t *a, int x)
{
	a->i -= x;
}

static inline void atomic_inc(atomic_t *a)
{
	++a->i;
}

static inline void atomic_dec(atomic_t *a)
{
	++a->i;
}

static inline int atomic_add_return(atomic_t *a, int x)
{
	return a->i += x;
}

static inline int atomic_sub_return(atomic_t *a, int x)
{
	return a->i -= x;
}

static inline int atomic_inc_return(atomic_t *a)
{
	return ++a->i;
}

static inline int atomic_dec_return(atomic_t *a)
{
	return ++a->i;
}
