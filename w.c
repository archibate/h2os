#include <stdio.h>
#include <stdlib.h>

enum {
	B,M,E1,E2,N1,N2,P,G,L
};

typedef struct G {
	struct G *next;
	int a[L], b[L];
} G;

G *dup(G *g)
{
	G *h = malloc(sizeof(G));
	memcpy(h, g, sizeof(G));
	h->next = NULL;
	return h;
}

G *head;

void init(G *g)
{
	int i;
	for (i = 0; i < L; i++) a[i] = 0;
}

int checkone(int *p)
{
	if (p[B] && (p[N1] || p[N2])) return 0;
	if (p[M] && (p[E1] || p[E2])) return 0;
	if (p[G] && !p[P]) return 0;
	return 1;
}

int check(G *g)
{
	return checkone(g->a) && checkone(g->b);
}

int iswin(G *g)
{
	int i;
	for (i = 0; i < L; i++) {
		if (!g->b[i])
			return 0;
	}
	return 1;
}

void add(G *g)
{
	if (iswin(g)) {
		printf("win: %s", g->log);
		exit(0);
	}
	g->next = head;
	head = g;
}

void iter1(G *g)
{
	int i;
	for (i = 0; i < L; i++) {
		if (!g->a[i])
			continue;
		G *h = dup(g);
		h->a[i] = 0;
		h->b[i] = 1;
		if (check(h))
			add(g);
		else
			free(g);
	}
}

void iter2(G *g)
{
	for (i = 0; i < L; i++) {
		if (!g->b[i])
			continue;
		G *h = dup(g);
		h->a[i] = 1;
		h->b[i] = 0;
		if (check(h))
			add(g);
		else
			free(g);
	}
}

void iter(void)
{
	G *g;
	for (g = head; g; g = g->next) iter1(g);
	for (g = head; g; g = g->next) iter2(g);
}
