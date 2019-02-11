#pragma once

typedef struct spinlock
{
	atomic_t a;
}
spinlock_t;

void spinlock_init(spinlock_t *s)
{
	atomic_set(&s->a, 0);
}

void spin_lock(spinlock_t *s)
{
	while (atomic_xchg(&s->a, 1));
}

void spin_unlock(spinlock_t *s)
{
	atomic_set(&s->a, 0);
}
