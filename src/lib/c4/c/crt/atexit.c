#include <stdlib.h>
#include <compiler.h>

struct exiter {
	struct exiter *next;
	void (*proc)(void);
};

static struct exiter *head;

void atexit(void (*proc)(void))
{
	struct exiter *ent = malloc(sizeof(struct exiter));
	ent->proc = proc;
	ent->next = head;
	head = ent;
}

static void _DTOR atexit_dtor(void)
{
	while (head) {
		head->proc();
		struct exiter *p = head;
		head = p->next;
		free(p);
	}
}
