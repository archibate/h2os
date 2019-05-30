#include <stdlib.h>
#include <stddef.h>
#include <compiler.h>

struct exiter {
	struct exiter *next;
	void (*proc)(void*);
	void *arg;
};

static struct exiter *head;

void atexit(void (*proc)(void))
{
	atexitarg((void*)proc, NULL);
}

void atexitarg(void (*proc)(void*), void *arg)
{
	struct exiter *ent = malloc(sizeof(struct exiter));
	ent->proc = proc;
	ent->next = head;
	ent->arg = arg;
	head = ent;
}

static void _DTOR atexit_dtor(void)
{
	while (head) {
		head->proc(head->arg);
		struct exiter *p = head;
		head = p->next;
		free(p);
	}
}
